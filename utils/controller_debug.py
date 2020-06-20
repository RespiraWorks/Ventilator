#!/usr/bin/env python3

# Very simple command line interface for development
#
# At the moment this program is just a command line utility.  You run
# it and get a command prompt from which you can enter commands to
# observe and control the system.
#
# For a list of available commands, enter 'help'

from datetime import datetime
from typing import List, Dict, Union
import argparse
import cmd
import dataclasses
import glob
import json
import math
import matplotlib.pyplot as plt
import os
import serial
import shlex
import struct
import subprocess
import sys
import textwrap
import threading
import time
import traceback

try:
    import readline
except ImportError:
    # readline package isn't available on Windows.
    pass

# Command codes.  See debug.h in the controller debug library
OP_MODE = 0x00
OP_PEEK = 0x01
OP_POKE = 0x02
OP_PBREAD = 0x03
OP_VAR = 0x04
OP_TRACE = 0x05

# Some commands take a sub-command as their first byte of data
SUBCMD_VAR_INFO = 0
SUBCMD_VAR_GET = 1
SUBCMD_VAR_SET = 2

SUBCMD_TRACE_FLUSH = 0
SUBCMD_TRACE_GETDATA = 1

# Special characters used to frame commands
ESC = 0xF1
TERM = 0xF2

# Variable types (see vars.h)
VAR_INT32 = 1
VAR_UINT32 = 2
VAR_FLOAT = 3

# Can trace this many variables at once.  Keep this in sync with TRACE_VAR_CT
# in the controller.
TRACE_VAR_CT = 4

# If true, the raw bytes of the serial data will be printed.
# This is handy for debugging the low level serial interface
# It can be toggled with the 'debug' command
showSerial = False

# Copied from network_protocol.proto.
# TODO: Import the proto instead!
VentMode_OFF = 0
VentMode_PRESSURE_CONTROL = 1
VentMode_PRESSURE_ASSIST = 2


@dataclasses.dataclass
class Preset:
    """A named list of DebugVars + values."""

    name: str
    desc: str
    vars: Dict[str, Union[int, float]]

    def ShortDesc(self):
        return f"{self.name} - {self.vars}"

    def LongDesc(self):
        return (
            self.desc
            + "\n"
            + "\n".join(f"  - {var} = {val}" for var, val in self.vars.items())
        )


def CoventPCPreset(
    test_num,
    intended_tv,
    lung_compliance,
    lung_resistance,
    rr,
    inspiratory_time,
    delta_inspiratory_pressure,
    fio2,
    bap,
):
    """Constructs a Preset object for a particular covent pressure-control test.

    Copied from CoVent-19 Ventilator Testing Procedure, table 201.105:
    https://drive.google.com/file/d/1FJOs6pdwHqV-Ygm5gMwIRBAmqH6Xxby8
    """
    desc = f"""\
CoVent-19 pressure-control test #{test_num}

If you have a calibrated test lung, configure it as follows:

 - Compliance: {lung_compliance} ml/hPa +/- 10%
 - Linear resistance: {lung_resistance} hPa/l/s +/- 10%

Intended TV: {intended_tv} ml
"""

    sec_per_breath = 60 / rr
    ie_ratio = inspiratory_time / (sec_per_breath - inspiratory_time)
    # TODO: For now fio2 is intentionally ignored.  Most of us don't have
    # access to pressurized gas, so all tests have to run with the blower.
    # run_covent_tests.py is able to respect the fio2 settings if you have
    # pressurized gas available.
    vars = {
        "gui_bpm": rr,
        "gui_ie_ratio": round(ie_ratio, 2),
        "gui_pip": bap + delta_inspiratory_pressure,
        "gui_peep": bap,
        # It's important to set gui_mode last.  Otherwise we'll start breathing
        # immediately, with whatever the old parameters happen to be.  (Python3
        # dicts have consistent ordering.)
        "gui_mode": VentMode_PRESSURE_CONTROL,
    }
    return Preset(f"covent_pc_{test_num}", desc, vars)


# DebugVar presets recognized by the `preset` command.
PRESETS = [
    CoventPCPreset(1, 500, 50, 5, 20, 1, 10, 30, 5),
    CoventPCPreset(2, 500, 50, 20, 12, 1, 15, 90, 10),
    CoventPCPreset(3, 500, 20, 5, 20, 1, 25, 90, 5),
    CoventPCPreset(4, 500, 20, 20, 20, 1, 25, 30, 10),
    CoventPCPreset(5, 300, 20, 20, 20, 1, 15, 30, 5),
    CoventPCPreset(6, 300, 20, 50, 12, 1, 25, 90, 10),
    CoventPCPreset(7, 300, 10, 50, 20, 1, 30, 90, 5),
    CoventPCPreset(8, 200, 10, 10, 20, 1, 25, 30, 10),
]


class Error(Exception):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return str(self.value)


class ArgparseShowHelpError(Exception):
    """Exception raised when CmdArgumentParser encounters --help.

    Canonical way of handling this is to catch it in the main command loop and
    then ignore it.  Argparse will print out the help message, and that's all
    the user needs to understand what happened.
    """

    pass


class CmdArgumentParser(argparse.ArgumentParser):
    """An ArgumentParser that doesn't call sys.exit() on error.

    https://docs.python.org/3/library/argparse.html#argparse.ArgumentParser.exit
    """

    def exit(self, status=0, message=None):
        if status:
            raise Error(f"Encountered a parse error: {message}")
        else:
            raise ArgparseShowHelpError()


# This class creates a simple command line interface using the standard
# Python cmd module.
#
# Member functions named do_something will implement a command called
# 'something'.  See the Python documentation for the cmd module for
# more details.
class CmdLine(cmd.Cmd):
    def __init__(self):
        super().__init__()
        self.scriptsDir = "scripts/"
        self.GetVarInfo()

    def UpdatePrompt(self, mode=None):
        if mode == None:
            mode = SendCmd(OP_MODE)[0]
        if mode == 1:
            self.prompt = "BOOT] "
        else:
            self.prompt = "] "

    def CmdLoop(self):
        self.UpdatePrompt()
        ReSync()
        self.GetVarInfo()
        while True:
            try:
                return cmd.Cmd.cmdloop(self)
            except ArgparseShowHelpError:
                pass
            except Error as e:
                print(e)
            except:
                traceback.print_exc()

    def emptyline(self):
        ReSync()
        self.UpdatePrompt()
        self.GetVarInfo()

    def do_debug(self, line):
        """Sets display of low level serial data on/off.

        Usage:

          debug on
          debug off
        """
        global showSerial
        line = line.strip().lower()
        if line == "on":
            showSerial = True
        elif line == "off":
            showSerial = False
        else:
            print("Unknown command; pass 'on' or 'off'.")

    def help_run(self):
        print(
            f"""\
Run an external Python script which can send commands, set variables, etc.

If no explicit path is given then the current directory and a sub-directory
named {self.scriptsDir} will be searched for the python script.
        """
        )

    def do_run(self, line):
        p = shlex.split(line)
        if os.path.exists(p[0]):
            fname = p[0]
        elif os.path.exists(self.scriptsDir + p[0]):
            fname = self.scriptsDir + p[0]
        else:
            print("Unknown file " + p[0])
            return
        gbl = globals().copy()
        gbl["cmdline"] = line
        gbl["parser"] = self
        exec(open(fname).read(), gbl)

    def complete_run(self, text, line, begidx, endidx):
        return glob.glob(text + "*.py") + [
            x[len(self.scriptsDir) :]
            for x in glob.glob(self.scriptsDir + text + "*.py")
        ]

    def do_exec(self, line):
        """exec()'s a string.  Good luck!"""
        exec(line)

    def do_peek(self, line):
        """Peek at a memory location.

ex: peek <addr> <ct> <fmt> <file>

   addr - the starting address passed as an integer value
   ct   - Number of bytes to read (default 1)
   fmt  - An optional formatting string.
   file - An optional file to save the data to

   The formatting string determines how the data is interpreted and displayed.
   Its a string made up of the following characters:
     +  current address
     x  16-bit integer displayed in hex
     i  16-bit signed integer displayed in decimal
     u  16-bit unsigned integer displayed in decimal
     X  32-bit integer displayed in hex
     I  32-bit signed integer displayed in decimal
     U  32-bit unsigned integer displayed in decimal
     f  32-bit float
     e  32-bit float in exponential format
     c  Single byte displayed as an ASCII character
     b  Single byte displayed in hex

   The data is extracted from what's returned and formatted as described in the string.
   If there's more data left over at the end of the string, a new line starts and the
   string starts over.

   The default formatting string if none is supplied is +XXXX
   i.e. Data is displayed as a series of 4 32-bit hex values / line
"""
        param = shlex.split(line)
        if len(param) < 1:
            print("Please specify the address at which to peek at a minimum")
            return
        ct = 1
        fmt = "+XXXX"
        fname = None
        if len(param) > 3:
            fname = param[3]
        if len(param) > 2:
            fmt = param[2]
        if len(param) > 1:
            ct = int(param[1], 0)
        addr = int(param[0], 0)
        Peek(addr, ct, fmt, fname)

    def do_poke(self, line):
        """Write data to a memory address

ex: poke [type] <addr> <data>

   type - Optional type, can be byte, short, long or float
          determines how the data will be interpreted.

   addr - Address at which to write data

   data - One or more data items to write.
"""
        param = shlex.split(line)
        if len(param) < 2:
            print("Please pass the address and at least one value to write")
            return

        ptype = "byte"
        if param[0] in ["long", "short", "float"]:
            ptype = param[0]
            param = param[1:]

        addr = param[0]

        if ptype == "float":
            data = [float(x) for x in param[1:]]
        else:
            data = [int(x, 0) for x in param[1:]]
        Poke(addr, data, ptype)

    def do_console(self, line):
        """Switch from command mode to a simple console display which
continuously reads debug print statements from the controller and displays the
data received to the screen.

When firmware in the controller calls the debug.Print() function it formats a
string which is written to a circular buffer.  When this program is running in
console mode it constantly reads this data and displays it.

Enter <ctrl>C to exit this mode

A couple optional parameters can be passed as arguments to this command:

--flush
  Flush the print buffer on the controller before we start displaying the data,
  removing any old data that had previously been added to the print buffer.

--dest <filename>
  Save received data to this file.
"""
        parser = CmdArgumentParser(prog="console")
        parser.add_argument(
            "--flush",
            action="store_true",
            help="flush print buffer on device before displaying data",
        )
        parser.add_argument(
            "--dest", help="write output to this file", default=os.devnull
        )
        args = parser.parse_args(line.split())

        if args.flush:
            while len(SendCmd(OP_PBREAD)) > 0:
                pass

        with open(args.dest, "w") as fp:
            try:
                while True:
                    dat = SendCmd(OP_PBREAD)
                    if len(dat) < 1:
                        continue

                    s = "".join(chr(x) for x in dat)
                    sys.stdout.write(s)
                    sys.stdout.flush()
                    fp.write(s)
            except KeyboardInterrupt:
                print()
                ReSync()
                pass

    def do_EOF(self, line):
        return True

    def do_exit(self, line):
        return True

    def do_get(self, line):
        cl = line.split()
        if len(cl) < 1:
            print("Please give the variable name to read")
            return

        if len(cl) > 1:
            fmt = cl[1]
        else:
            fmt = None

        print(GetVar(cl[0], fmt=fmt))

    def complete_get(self, text, line, begidx, endidx):
        var = text

        out = []
        for i in varDict.keys():
            if i.startswith(var):
                out.append(i)

        return out

    def help_get(self):
        print("Read the value of a debug variable and display it\n")
        print("Variables currently defined:")
        for k in varDict.keys():
            print("   %-10s - %s" % (k, varDict[k].help))

    def do_preset(self, line):
        parser = CmdArgumentParser("preset")
        parser.add_argument("preset", metavar="PRESET", type=str)
        args = parser.parse_args(line.split())

        candidates = [p for p in PRESETS if p.name.lower() == args.preset.lower()]
        if not candidates:
            print(f"No preset named {args.preset}")
            return
        if len(candidates) > 1:
            print(
                f"Two or more presets named {args.preset} (case-insensitive)!  "
                "Fix the PRESETS variable in the code."
            )
            return

        preset = candidates[0]
        print(f"Applying preset {preset.name}:\n")
        print(textwrap.indent(preset.LongDesc(), "    "))

        for var, val in preset.vars.items():
            SetVar(var, val)

        print(f"\nPreset {preset.name} successfully applied!")

    def help_preset(self):
        print(
            """\
Apply a preset list of settings, all at once.

Usage:

preset PRESET
  Apply the given preset.

Available PRESETs:
"""
        )
        for p in PRESETS:
            print(f"  - {p.ShortDesc()}")

    def complete_preset(self, text, line, begidx, endidx):
        return [k for k in PRESETS.keys() if k.lower().startswith(text.lower())]

    def do_set(self, line):
        cl = line.split()
        if len(cl) < 2:
            print("Please give the variable name and value")
            return
        SetVar(cl[0], cl[1])

    def complete_set(self, text, line, begidx, endidx):
        var = text

        out = []
        for i in varDict.keys():
            if i.startswith(var):
                out.append(i)

        return out

    def help_set(self):
        print("You can very easily add debug variables to the C++ code.")
        print("You give these variables names and a pointer to a value to")
        print("access.  Then using the get/set commands you can read the")
        print("current state of that C++ location and modify it.\n")
        print("This command allows you to modify such a debug variable\n")
        print("Variables currently defined:")
        for k in varDict.keys():
            print("   %-10s - %s" % (k, varDict[k].help))

    def do_trace(self, line):
        """The `trace` command controls/reads the controller's trace buffer.

Tracing lets you sample debug variables in real time.  Their values are saved
to a large internal memory buffer in the device, which you can then download
and/or display as a graph.

A sub-command must be passed as an option:

trace flush
  Stops the trace if one was on-going and flushes the trace buffer

trace start [--period p] [var1 ... ]
  Starts collecting trace data.

  You can specify the names of up to TRACE_VAR_CT debug variables to trace.  If
  you don't specify any, we use the values in trace_var1, ....

  --period controls the sample period in units of one trip through the
  controller's high-priority loop.  If you don't specify a period, we use
  whatever is in the trace_period debug variable.

trace graph [--dest=<filename>] [--title=<title>] [--nointeractive]
            [--scale=<field>/scale --scale=<field>*scale]
  Downloads the data and displays it graphically.

  The plain-text graph data and the rendered figure are saved to your local
  machine as last_graph.dat/png; --dest configures this.

  --scale lets you multiply or divide a field by a given scaling factor.  This
  makes it feasible to fit values with different magnitudes on the same Y axis.

  You can add a title to the rendered graph with --title.

  By default the figure is also shown interactively.  --nointeractive
  configures this.

trace download [--separator=<str>] [--dest=<filename>]
  This will download the data and save it to a file with the given name.  If no
  file name is given, then trace.dat will be used If the --separator=<str>
  option is given, then the specified string will separate each column of data.
  The default separator is a few spaces.

The tracing API also exposes the following debug vars, controlled via the
standard `get` and `set` commands.  You probably don't need to touch these.

trace_var1, ..., trace_var{TRACE_VAR_CT}
  Names of the variables to trace.  Can be set explicitly, or via `trace
  start`.

trace_period
  Interval between two samples in the trace.  A value of N means we sample once
  every N times through the controller's high priority loop.

trace_ctrl
  This variable is used to start the trace.  Just set it to 1 to start.
  It will be cleared when the trace buffer is full.

trace_samples
  This variable gives the number of samples stored in the buffer currently.
"""
        cl = shlex.split(line)
        if len(cl) < 1:
            print("Error, please specify the trace command to run.")
            print(self.do_trace.__doc__)
            return

        if cl[0] == "flush":
            SendCmd(OP_TRACE, [SUBCMD_TRACE_FLUSH])

        elif cl[0] == "start":
            parser = CmdArgumentParser("trace start")
            parser.add_argument("--period", type=int)
            parser.add_argument("var", nargs="*")
            args = parser.parse_args(cl[1:])

            if len(args.var) > TRACE_VAR_CT:
                print(f"Can't trace more than {TRACE_VAR_CT} variables at once.")
                return
            if args.period:
                SetVar("trace_period", args.period)
            if args.var:
                # Unset existing trace vars, so we only get what was asked for.
                var = args.var + [""] * (TRACE_VAR_CT - len(args.var))
                for (i, var) in enumerate(var):
                    SetVar(f"trace_var{i + 1}", var)

            SendCmd(OP_TRACE, [SUBCMD_TRACE_FLUSH])

            # The transition from 0 to 1 resets and starts the trace
            SetVar("trace_ctrl", 0)
            SetVar("trace_ctrl", 1)

        elif cl[0] == "download":
            parser = CmdArgumentParser(prog="trace download")
            parser.add_argument(
                "--separator", type=str, default="  ", help="field separator in file"
            )
            parser.add_argument(
                "--dest",
                type=str,
                default="trace.dat",
                help="filename to save the trace to",
            )
            args = parser.parse_args(cl[1:])

            tv = TraceActiveVars()
            if len(tv) < 1:
                print("No active trace variables")
                return

            dat = TraceDownload()
            TraceSaveDat(dat, fname=args.dest, separator=args.separator)

        elif cl[0] == "graph":
            TraceGraph(cl[1:])

        else:
            print("Unknown trace sub-command %s" % cl[0])
            return

    # Read info about all the supported variables and load
    # them in a map
    def GetVarInfo(self):
        varDict.clear()
        try:
            for vid in range(256):
                dat = SendCmd(OP_VAR, [SUBCMD_VAR_INFO] + Split16(vid))
                V = VarInfo(vid, dat)
                varDict[V.name] = V
        except Error as e:
            pass


varDict = {}


class VarInfo:

    # Initialize the variable info from the data returned
    # by the controller.  Set var.cpp in the controller for
    # details on this formatting
    def __init__(self, id, dat):
        self.id = id

        if len(dat) < 8:
            raise Error("Invalid VarInfo data returned")

        self.type = dat[0]
        nameLen = dat[4]
        fmtLen = dat[5]
        helpLen = dat[6]

        if len(dat) < 8 + nameLen + fmtLen + helpLen:
            raise Error("Invalid VarInfo data returned")

        n = 8
        self.name = "".join([chr(x) for x in dat[n : n + nameLen]])
        n += nameLen
        self.fmt = "".join([chr(x) for x in dat[n : n + fmtLen]])
        n += fmtLen
        self.help = "".join([chr(x) for x in dat[n : n + helpLen]])

    # Convert an unsigned 32-bit value into the correct type for
    # this variable
    def ConvertInt(self, d):
        if self.type == VAR_FLOAT:
            return I2F(d)
        if self.type == VAR_INT32:
            if d & 0x80000000:
                return d - (1 << 32)
            return d
        return d


def FindVarByID(vid):
    for name in varDict:
        if varDict[name].id == vid:
            return varDict[name]
    return None


def GetVar(name, raw=False, fmt=None):
    if not name in varDict:
        raise Error("Unknown variable %s" % name)

    V = varDict[name]
    dat = SendCmd(OP_VAR, [SUBCMD_VAR_GET] + Split16(V.id))

    if V.type == VAR_INT32:
        val = Build32(dat, signed=True)[0]

    elif V.type == VAR_UINT32:
        val = Build32(dat)[0]

    elif V.type == VAR_FLOAT:
        val = BuildFlt(dat)[0]

    else:
        raise Error("Sorry, I don't know how to handle that variable type yet")

    if raw:
        return val

    # If a format wasn't passed, use the default for this var
    if fmt == None:
        fmt = V.fmt

    # I'll convert trace variable values to variable names
    if name.startswith("trace_var"):
        if val < 0:
            return "none"
        tv = FindVarByID(val)
        if tv != None:
            return tv.name

    return fmt % val


def SetVar(name, value):
    if not name in varDict:
        raise Error("Unknown variable %s" % name)

    V = varDict[name]

    # If this is a trace variable, the passed value
    # should be a variable name
    if name.startswith("trace_var"):
        if value == "" or value == "none":
            value = "-1"
        elif value in varDict:
            value = "%d" % varDict[value].id

    if V.type == VAR_INT32:
        if not isinstance(value, int):
            value = int(value, 0)
        dat = Split32(value)

    elif V.type == VAR_UINT32:
        if not isinstance(value, int):
            value = int(value, 0)
        dat = Split32(value)

    elif V.type == VAR_FLOAT:
        dat = SplitFlt(float(value))

    SendCmd(OP_VAR, [SUBCMD_VAR_SET] + Split16(V.id) + dat)
    return


def TraceActiveVars():
    """Return a list of active trace variables"""
    ret = []
    for i in range(10):
        name = "trace_var%d" % (i + 1)
        if not name in varDict:
            break
        vid = GetVar(name, raw=True)
        V = FindVarByID(vid)
        if V:
            ret.append(V)
    return ret


def TraceDownload():
    """Fetches a trace from the controller.

    Returns a list of N+1 lists where N is the number of active trace variables.
    The first list gives the time in seconds of each sample relative to the
    start of the trace, and the remaining N lists each holds the trace data
    for one variable.
    """
    traceVars = TraceActiveVars()
    if len(traceVars) < 1:
        return None

    ct = GetVar("trace_samples", raw=True) * len(traceVars)

    data = []
    while len(data) < 4 * ct:
        dat = SendCmd(OP_TRACE, [SUBCMD_TRACE_GETDATA])
        if len(dat) < 1:
            break
        data += dat

    # Convert the bytes into an array of unsigned 32-bit values
    data = Build32(data)

    # The data comes as a list of of samples, where each sample contains
    # len(traceVars) uint32s: [a1, b1, c1, a2, b2, c2, ...].  Parse this into
    # sublists [[a1', a2', ...], [b1', b2', ...], [c1', c2', ...]], where each
    # of the variables is converted to the correct type.
    nvars = len(traceVars)
    ret = [[] for _ in range(nvars)]

    # The `zip` expression groups data into sublists of nvars elems.  See the
    # "grouper" recipe:
    # https://docs.python.org/3/library/itertools.html#itertools-recipes
    iters = [iter(data)] * nvars
    for sample in zip(*iters):
        for i, val in enumerate(sample):
            ret[i].append(traceVars[i].ConvertInt(val))

    per = GetVar("trace_period", raw=True)
    if per < 1:
        per = 1

    # Scale this by the loop period which is an integer in microseconds.
    # I multiply by 1e-6 (i.e. 1/1,000,000) to convert it to seconds.
    per *= GetVar("loop_period", raw=True) * 1e-6

    time = [x * per for x in range(len(ret[0]))]
    ret.insert(0, time)

    return ret


def GitRevInfo():
    """Returns a description of the current repository."""
    try:
        rev = (
            subprocess.check_output(["git", "rev-parse", "--short", "HEAD"])
            .decode("utf-8")
            .strip()
        )
        is_dirty = len(subprocess.check_output(["git", "status", "-s"])) > 0
        return f"{rev} ({'dirty' if is_dirty else 'clean'})"
    except subprocess.CalledProcessError:
        return "(unknown)"


def TraceMetadataStr(title=""):
    """Gets a human-readable metadata string for a trace we just captured."""
    if not title:
        title = ""

    ret = title
    if ret and not ret.endswith("\n"):
        ret += "\n"

    ret += f"Captured {datetime.now()} by {os.environ.get('USER', 'unknown user')}\n"
    ret += f"Built at revision {GitRevInfo()}\n"
    return ret


def TraceGraph(raw_args: List[str]):
    parser = CmdArgumentParser("trace graph")
    parser.add_argument(
        "--dest",
        default="last_graph.dat",
        metavar="FILE",
        help="Filename to save trace data to.  The rendered graph will be "
        "saved to this destination with the extension (if present) replaced "
        "by '.png'.",
    )
    parser.add_argument(
        "--title", default=None, metavar="TITLE", help="Graph title",
    )
    parser.add_argument(
        "--nointeractive",
        default=False,
        action="store_true",
        help="Don't show the graph interactively.  Useful for scripts.",
    )
    parser.add_argument(
        "--scale",
        default=[],
        action="append",
        metavar="FIELD / SCLAE or FIELD * SCALE",
        help="Scale a var by the given amount, e.g. "
        "--scale=volume/100 or --scale=pressure*10",
    )

    args = parser.parse_args(raw_args)

    scalings = {}
    for s in args.scale:
        if "/" in s:
            var, n = (x.strip() for x in s.split("/"))
            scalings[var] = ("/", float(n))
        elif "*" in s:
            var, n = s.split("*")
            scalings[var] = ("*", float(n))
        else:
            raise Error(f"Invalid scaling parameter {s}")

    (base, _) = os.path.splitext(args.dest)
    graph_img_filename = base + ".png"

    traceVars = TraceActiveVars()
    unrecognized_scale_vars = set(scalings.keys()) - set(v.name for v in traceVars)
    if unrecognized_scale_vars:
        raise Error(
            f"Can't scale by vars that aren't being traced: {unrecognized_scale_vars}"
        )

    dat = TraceDownload()
    TraceSaveDat(dat, args.dest, title=args.title)

    timestamps_sec = dat[0]
    dat = dat[1:]
    plt.figure()
    for i, d in enumerate(dat):
        var = traceVars[i]

        scale_kind, scale = scalings.get(var.name, (None, None))
        label = var.help
        if scale_kind:
            label += f", scaled {scale_kind} {scale}"

        if scale_kind == "*":
            multiplier = scale
        elif scale_kind == "/":
            multiplier = 1 / scale
        else:
            multiplier = 1

        plt.plot(timestamps_sec, [v * multiplier for v in d], label=label)

    # Draw a black gridline at y=0 to highlight the x-axis.
    plt.axhline(linewidth=1, color="black")

    plt.xlabel("Seconds")
    plt.grid()
    plt.legend()
    if args.title:
        plt.title(args.title)

    fig_md = {"Description": TraceMetadataStr(args.title)}
    if args.title:
        fig_md["Title"] = args.title
    plt.savefig(graph_img_filename, format="png", metadata=fig_md)

    if not args.nointeractive:
        plt.show()


def TraceSaveDat(dat, fname, separator=" ", title=""):
    tv = TraceActiveVars()

    with open(fname, "w") as fp:
        fp.write(textwrap.indent(TraceMetadataStr(title), "# "))

        line = ["time(sec)"]
        for v in tv:
            line.append(v.name)
        fp.write(separator.join(line) + "\n")

        for i in range(len(dat[0])):
            # First column is time in seconds
            line = ["%.3f" % dat[0][i]]
            for j in range(len(tv)):
                line.append(tv[j].fmt % dat[j + 1][i])
            fp.write(separator.join(line) + "\n")


def FmtPeek(dat, fmt="+XXXX", addr=0):

    fmtInfo = {
        "+": ("0x%08x: ", 0, lambda dat: addr),
        "x": ("0x%04x ", 2, GrabU16),
        "i": ("%5d ", 2, GrabI16),
        "u": ("%5u ", 2, GrabU16),
        "X": ("0x%08x ", 4, GrabU32),
        "I": ("%9d ", 4, GrabI32),
        "U": ("%9u ", 4, GrabU32),
        "n": ("\n", 0, lambda dat: None),
        "f": ("%8.4f ", 4, GrabFlt),
        "e": ("%12.4e ", 4, GrabFlt),
        "c": ("%s", 1, lambda dat: chr(GrabU8(dat))),
        "b": ("0x%02x ", 1, GrabU8),
    }

    ret = ""
    ndx = -1
    while len(dat) > 0:
        ndx += 1
        if ndx == len(fmt):
            ret += "\n"
            ndx = 0

        ch = fmt[ndx]

        if ch in fmtInfo:
            (fmtString, byteCt, func) = fmtInfo[ch]
            val = func(dat)

            if val == None:
                ret += fmtString
            else:
                ret += fmtString % val
            addr += byteCt
        else:
            ret += ch

    return ret


def DecodeAddr(addr, fw=None):

    if isinstance(addr, int):
        return addr

    return int(addr, 0)


def Peek(addr, ct=1, fmt="+XXXX", fname=None, raw=False):
    addr = DecodeAddr(addr)
    if addr == None:
        print("Unknown symbol")
        return

    out = []

    A = addr

    while ct:
        n = min(ct, 256)
        dat = SendCmd(OP_PEEK, Split32(A) + Split16(n))
        out += dat
        ct -= len(dat)
        A += len(dat)

    if raw:
        return out

    s = FmtPeek(out, fmt, addr)
    if fname == None:
        print(s)
    else:
        fp = open(fname, "w")
        fp.write(s)
        fp.write("\n")
        fp.close()


def Peek16(addr, ct=None, le=True, signed=False):
    addr = DecodeAddr(addr)
    if addr == None:
        print("Unknown symbol")
        return

    if ct == None:
        ct = 1
    out = Peek(addr, 2 * ct, raw=True)
    return Build16(out, le, signed)


def Peek32(addr, ct=None, le=True, signed=False):
    addr = DecodeAddr(addr)
    if addr == None:
        print("Unknown symbol")
        return

    if ct == None:
        ct = 1
    out = Peek(addr, 4 * ct, raw=True)
    return Build32(out, le, signed)


def Peekf(addr, ct=None):
    dat = Peekl(addr, ct, le=True, signed=False)
    if ct == None:
        return Util.I2F(dat)
    ret = []
    for d in dat:
        ret.append(Util.I2F(d))
    return ret


def Poke(addr, dat, ptype):
    addr = DecodeAddr(addr)
    if addr == None:
        print("Unknown symbol")
        return
    if isinstance(dat, int):
        dat = [dat]

    if ptype == "long":
        dat = Split32(dat)
    if ptype == "short":
        dat = Split16(dat)
    if ptype == "float":
        dat = [F2I(x) for x in dat]
        dat = Split32(dat)

    SendCmd(OP_POKE, Split32(addr) + dat)


def Poke32(addr, dat):
    Poke(addr, dat, "long")


def Poke16(addr, dat):
    Poke(addr, dat, "short")


# This adds the escape characters to the serial data stream
# to implement the framing used by that format.
# See debug.cpp in the controller code for details
def EscCmd(buff):
    ret = []
    for i in buff:
        if i == ESC or i == TERM:
            ret.append(ESC)
        ret.append(i)
    ret.append(TERM)
    return ret


# Wait for a response from the controller to the last command
# The binary format uses two special characters to frame a
# command or response.  This function removes those characters
# before returning.
# See debug.cpp in the controller source for more detail on
# command framing.
def GetResp():
    dat = []
    esc = False
    DbgPrint("Getting resp: ", end="")
    while True:
        x = ser.read(1)
        if len(x) < 1:
            DbgPrint("timeout")
            return dat
        x = ord(x)
        DbgPrint("0x%02x" % x, end=" ")

        if esc:
            esc = False
            dat.append(x)
            continue

        if x == ESC:
            esc = True
            continue

        if x == TERM:
            DbgPrint()
            return dat
        dat.append(x)


# This formats a binary command and sends it to the system.
# It then waits for and returns a response.
#
#  op - The command code to send.  See the list at the top
#       of the file.

#  data - Zero or more bytes of data to be sent with the command

#  timeout - How long (seconds) to wait for the response.  If
#            not specified then a reasonable system default is used


# This lock is used to make the command interface thread safe.
# The main debug program doesn't currently use threads, but scripts
# run from it may and those scripts use these same functions to
# send commands
cmdLock = threading.Lock()


def DbgPrint(*args, **kwargs):
    if showSerial:
        print(*args, **kwargs)


def SendCmd(op, data=[], timeout=None):
    buff = [op] + data

    crc = CRC16().calc(buff)
    buff += Split16(crc)

    S = "CMD: "
    for b in buff:
        S += "0x%02x " % b
    DbgPrint(S)

    buff = EscCmd(buff)
    S = "ESC: "
    for b in buff:
        S += "0x%02x " % b
    DbgPrint(S)

    with cmdLock:
        ser.write(bytearray(buff))
        if timeout != None:
            DbgPrint("Setting timeout to %.1f" % timeout)
            oldto = ser.timeout
            ser.timeout = timeout

        rsp = GetResp()
        if timeout != None:
            ser.timeout = oldto

        if len(rsp) < 3:
            raise Error("Invalid response, too short")

        crc = CRC16().calc(rsp[:-2])
        rcrc = Build16(rsp[-2:])[0]
        if crc != rcrc:
            raise Error(
                "CRC error on response, calculated 0x%04x received 0x%04x" % (crc, rcrc)
            )

        if rsp[0]:
            raise Error("Error %d (0x%02x)" % (rsp[0], rsp[0]))
            return []
        return rsp[1:-2]


def ReSync():
    with cmdLock:
        cmd = [TERM, TERM]
        ser.write(bytearray(cmd))
        time.sleep(0.1)
        ser.reset_input_buffer()


def I2F(ival):
    s = struct.pack("I", ival & 0xFFFFFFFF)
    return struct.unpack("f", s)[0]


def F2I(fval):
    s = struct.pack("f", fval)
    return struct.unpack("I", s)[0]


# Takes as input a list of integer values and
# splits them into a list of bytes which is returned.
#
# val - The input list of ints
# len - Length (in bytes) of the input ints.
# le  - Little endian format if true
def SplitInt(val, len=4, le=True):
    ret = []

    if isinstance(val, list):
        for v in val:
            ret += SplitInt(v, len, le)
    else:
        for i in range(len):
            ret.append(int(val & 0x00FF))
            val >>= 8

        if not le:
            ret = ret[::-1]
    return ret


# Splits a list of 16-bit ints into a list of bytes
def Split16(x, le=True):
    if isinstance(x, int):
        x = [x]
    return SplitInt(x, 2, le)


# Splits a list of 32-bit ints into a list of bytes
def Split32(x, le=True):
    if isinstance(x, int):
        x = [x]
    return SplitInt(x, 4, le)


# Splits a list of 32-bit floats into a list of bytes
def SplitFlt(x):
    if isinstance(x, float):
        x = [x]
    return Split32([F2I(i) for i in x])


# Utility function to convert bytes to ints
#   bytes - Array of byte values to convert
#          All input bytes will be compined into one integer
#   signed - If true, the output will be signed
#   le - If true, use little endian format
def MakeInt(bytes, signed=True, le=True):
    if not le:
        bytes = bytes[::-1]
    val = 0
    shift = 0
    for b in bytes:
        val |= b << shift
        shift += 8

    if signed and bytes[-1] & 0x80:
        return val - (1 << shift)
    return val


# Convert a list of bytes into a list of 32-bit integers
def Build32(dat, le=True, signed=False):
    ret = []
    for i in range(int(len(dat) / 4)):
        ret.append(MakeInt(dat[(4 * i) : (4 * i + 4)], signed=signed, le=le))
    return ret


# Convert a list of bytes into a list of 16-bit integers
def Build16(dat, le=True, signed=False):
    ret = []
    for i in range(int(len(dat) / 2)):
        ret.append(MakeInt(dat[(2 * i) : (2 * i + 2)], signed=signed, le=le))
    return ret


# Convert a list of bytes into a list of 32-bit floats integers
def BuildFlt(dat):
    tmp = Build32(dat, le=True, signed=False)
    return [I2F(x) for x in tmp]


# Utility function which removes the first ct elements from a list and returns them
def GrabElems(dat, ct):
    if len(dat) < ct:
        dat.extend((ct - len(dat)) * [0])
    ret = dat[:ct]
    del dat[0:ct]
    return ret


# These functions all pull the first bytes of the input list and convert
# them into another type before returning them.
def GrabI8(dat, le=True):
    return MakeInt(GrabElems(dat, 1), signed=True, le=le)


def GrabU8(dat, le=True):
    return MakeInt(GrabElems(dat, 1), signed=False, le=le)


def GrabI16(dat, le=True):
    return MakeInt(GrabElems(dat, 2), signed=True, le=le)


def GrabU16(dat, le=True):
    return MakeInt(GrabElems(dat, 2), signed=False, le=le)


def GrabI32(dat, le=True):
    return MakeInt(GrabElems(dat, 4), signed=True, le=le)


def GrabU32(dat, le=True):
    return MakeInt(GrabElems(dat, 4), signed=False, le=le)


def GrabI64(dat, le=True):
    return MakeInt(GrabElems(dat, 8), signed=True, le=le)


def GrabU64(dat, le=True):
    return MakeInt(GrabElems(dat, 8), signed=False, le=le)


def GrabFlt(dat, le=True):
    return I2F(GrabU32(dat, le=le))


def MakeCrcTable(poly):
    tbl = []
    for i in range(256):
        crc = i
        for j in range(8):
            lsbSet = crc & 1
            crc >>= 1
            if lsbSet:
                crc ^= poly
        tbl.append(crc)
    return tbl


class CRC16:
    """Simple utility class for calculating CRC values."""

    tbl = MakeCrcTable(0xA001)

    def __init__(self):
        self.init = 0

    def calc(self, dat, init=0):
        crc = init ^ self.init
        for d in dat:
            a = 0x00FF & (d ^ crc)
            crc = CRC16.tbl[a] ^ (crc >> 8)
        return crc ^ self.init


def DetectSerialPort():
    j = json.loads(
        subprocess.check_output(["platformio", "device", "list", "--json-output"])
    )
    ports = [d["port"] for d in j if "STM32" in d.get("description", "")]
    if not ports:
        raise Error(
            "Could not auto-detect serial port; platformio device list did not "
            "yield any STM32 devices.  Choose port explicitly with --port."
        )
    if len(ports) > 1:
        raise Error(
            "Could not auto-detect serial port; platformio device list "
            f"yielded multiple STM32 devices: {', '.join(ports)}.  "
            "Choose port explicitly with --port."
        )
    print(f"Detected device connected to {ports[0]}", file=sys.stderr)
    return ports[0]


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--port",
        type=str,
        help="Serial port device is connected to, e.g. /dev/ttyACM0.  If "
        "unspecified, we try to auto-detect the port.",
    )
    parser.add_argument(
        "--command", "-c", type=str, help="Run the given command and exit."
    )
    parser.add_argument(
        "--detect-port-and-quit",
        action="store_true",
        help="""Detect the port that the device is connected to and then
immediately exit.  This is useful for scripts that invoke this program multiple
times.  Port detection is the slowest part of startup, so this option lets you
do it once upfront.""",
    )

    args = parser.parse_args()

    if not args.port:
        args.port = DetectSerialPort()

    if args.detect_port_and_quit:
        print(args.port)
        sys.exit(0)

    global ser
    ser = serial.Serial(port=args.port, baudrate=115200)
    ser.timeout = 0.8

    cmdline = CmdLine()

    if args.command:
        # Set matplotlib to noninteractive mode.  This causes show() to block,
        # so if you do `controller_debug.py -c "trace graph"` the program won't
        # exit until you close the displayed graph.
        plt.ioff()
        cmdline.onecmd(args.command)
    else:
        # Turn on interactive mode for matplotlib, so show() doesn't block.
        plt.ion()
        cmdline.CmdLoop()
