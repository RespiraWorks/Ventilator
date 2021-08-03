#!/usr/bin/env python3

# Very simple command line interface for development
#
# At the moment this program is just a command line utility.  You run
# it and get a command prompt from which you can enter commands to
# observe and control the system.
#
# For a list of available commands, enter 'help'

import debug
from datetime import datetime
from typing import List
import argparse
import cmd
import glob
import json
import matplotlib.pyplot as plt
import os
import shlex
import subprocess
import sys
import textwrap
import traceback
import colors
import error


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
            raise error.Error(f"Encountered a parse error: {message}")
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
        self.scripts_directory = "scripts/"

    def update_prompt(self, mode=None):
        if not interface.connected():
            self.prompt = colors.Colors.RED + "OFFLINE] " + colors.Colors.ENDC
        else:
            if mode is None:
                mode = interface.mode_get()
            if mode == 1:
                self.prompt = colors.Colors.ORANGE + "BOOT] " + colors.Colors.ENDC
            else:
                self.prompt = colors.Colors.GREEN + "] " + colors.Colors.ENDC

    def cli_loop(self):
        if interface.connected():
            interface.resynchronize()
            interface.variables_update_info()
        self.update_prompt()

        while True:
            try:
                return cmd.Cmd.cmdloop(self)
            except ArgparseShowHelpError:
                pass
            except error.Error as e:
                print(e)
            except:
                traceback.print_exc()

    def emptyline(self):
        interface.resynchronize()
        self.update_prompt()
        interface.variables_update_info()

    def do_debug(self, line):
        """Sets display of low level serial data on/off.

        Usage:
          debug on
          debug off

        """
        line = line.strip().lower()
        if line == "on":
            interface.showSerial = True
        elif line == "off":
            interface.showSerial = False
        else:
            print("Unknown command; pass 'on' or 'off'.")

    def help_run(self):
        print(
            f"""\
Run an external Python script which can send commands, set variables, etc.

If no explicit path is given then the current directory and a sub-directory
named {self.scripts_directory} will be searched for the python script.
        """
        )

    def do_run(self, line):
        p = shlex.split(line)
        if os.path.exists(p[0]):
            fname = p[0]
        elif os.path.exists(self.scripts_directory + p[0]):
            fname = self.scripts_directory + p[0]
        else:
            print("Unknown file " + p[0])
            return
        gbl = globals().copy()
        gbl["cmdline"] = line
        gbl["parser"] = self
        exec(open(fname).read(), gbl)

    def complete_run(self, text, line, begidx, endidx):
        return glob.glob(text + "*.py") + [
            x[len(self.scripts_directory):]
            for x in glob.glob(self.scripts_directory + text + "*.py")
        ]

    def do_test(self, line):
        params = shlex.split(line)
        if len(params) < 1:
            print(colors.Colors.RED + "Not enough args for `test`\n" + colors.Colors.ENDC)
            return

        if params[0] == "load":
            if len(params) < 2:
                print(colors.Colors.RED + "Not enough args for `test load`\n" + colors.Colors.ENDC)
                return
            interface.tests_import_csv(params[1])
        elif params[0] == "list":
            interface.tests_list(bool(len(params) > 1
                                      and (params[1] == "-v" or params[1] == "--verbose")))
        elif params[0] == "run":
            if len(params) < 2:
                print(colors.Colors.RED + "Not enough args for `test run`\n" + colors.Colors.ENDC)
                return
            interface.test_apply(params[1])
        else:
            print("Invalid test args: {}", params)

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
        interface.peek(addr, ct, fmt, fname)

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
        interface.poke(addr, data, ptype)

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

        print(interface.variable_get(cl[0], fmt=fmt))

    def complete_get(self, text, line, begidx, endidx):
        return interface.variables_starting_with(text);

    def help_get(self):
        print("Read the value of a debug variable and display it\n")
        print(interface.variables_list())

    def do_set(self, line):
        cl = line.split()
        if len(cl) < 2:
            print("Please give the variable name and value")
            return
        interface.variable_set(cl[0], cl[1])

    def complete_set(self, text, line, begidx, endidx):
        return interface.variables_starting_with(text);

    def help_set(self):
        print("You can very easily add debug variables to the C++ code.")
        print("You give these variables names and a pointer to a value to")
        print("access.  Then using the get/set commands you can read the")
        print("current state of that C++ location and modify it.\n")
        print("This command allows you to modify such a debug variable\n")
        print(interface.variables_list())

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
  you don't specify any, we use the last known values.

  --period controls the sample period in units of one trip through the
  controller's high-priority loop.  If you don't specify a period, we use 1.

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

trace current
  This returns the current state of the trace:
    - traced variables
    - trace period
    - number of samples in the trace buffer

"""
        cl = shlex.split(line)
        if len(cl) < 1:
            print("Error, please specify the trace command to run.")
            print(self.do_trace.__doc__)
            return

        if cl[0] == "flush":
            interface.trace_start()

        elif cl[0] == "start":
            parser = CmdArgumentParser("trace start")
            parser.add_argument("--period", type=int)
            parser.add_argument("var", nargs="*")
            args = parser.parse_args(cl[1:])

            #TODO: check validity of all variables

            if args.period:
                interface.trace_period_set(args.period)
            else:
                interface.trace_period_set(1)

            if args.var:
                interface.trace_select(args.var)

            interface.trace_flush()
            interface.trace_start()

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

            tv = interface.trace_active_variables_list()
            if len(tv) < 1:
                print("No active trace variables")
                return

            dat = interface.trace_download()
            trace_save_data(dat, fname=args.dest, separator=args.separator)

        elif cl[0] == "graph":
            trace_graph(cl[1:])

        elif cl[0] == "current":
            print("Traced variables:")
            for var in interface.trace_active_variables_list():
                print(" - %s" % var.name)
            print("Trace period: %i" % interface.trace_period_get())
            print("Samples in buffer: %i" % interface.trace_status())

        else:
            print("Unknown trace sub-command %s" % cl[0])
            return

    def do_eeprom(self, line):
        """The `eeprom` command allows you to read/write to the controller's
non-volatile memory.

A sub-command must be passed as an option:

eeprom read <address> <length>
  Reads length bytes in the EEPROM starting at given address.

eeprom write <address> <data>
  Writes data (provided as a series of bytes) to the EEPROM.
"""
        cl = shlex.split(line)
        if len(cl) < 1:
            print("Error, please specify the operation to perform.")
            return
        if cl[0] == "read":
            if len(cl) < 3:
                print("Error, please provide address and length.")
                return
            print(interface.eeprom_read(cl[1], cl[2]))
        elif cl[0] == "write":
            if len(cl) < 3:
                print("Error, please provide address and at least 1 byte of data.")
                return
            interface.eeprom_write(cl[1], list(map(int, cl[2:])))
        else:
            print("Error: Unknown subcommand %s" % cl[0])
            return


def git_rev_info():
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


def trace_metadata_string(title=""):
    """Gets a human-readable metadata string for a trace we just captured."""
    if not title:
        title = ""

    ret = title
    if ret and not ret.endswith("\n"):
        ret += "\n"

    ret += f"Captured {datetime.now()} by {os.environ.get('USER', 'unknown user')}\n"
    ret += f"Built at revision {git_rev_info()}\n"
    return ret


def trace_graph(raw_args: List[str]):
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
            raise error.Error(f"Invalid scaling parameter {s}")

    (base, _) = os.path.splitext(args.dest)
    graph_img_filename = base + ".png"

    trace_vars = interface.trace_active_variables_list()
    unrecognized_scale_vars = set(scalings.keys()) - set(v.name for v in trace_vars)
    if unrecognized_scale_vars:
        raise error.Error(
            f"Can't scale by vars that aren't being traced: {unrecognized_scale_vars}"
        )

    dat = interface.trace_download()
    trace_save_data(dat, args.dest, title=args.title)

    timestamps_sec = dat[0]
    dat = dat[1:]
    plt.figure()
    for i, d in enumerate(dat):
        var = trace_vars[i]

        scale_kind, scale = scalings.get(var.name, (None, None))
        label = var.print_help
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

    fig_md = {"Description": trace_metadata_string(args.title)}
    if args.title:
        fig_md["Title"] = args.title
    plt.savefig(graph_img_filename, format="png", metadata=fig_md)

    if not args.nointeractive:
        plt.show()


def trace_save_data(dat, fname, separator=" ", title=""):
    tv = interface.trace_active_variables_list()

    with open(fname, "w") as fp:
        fp.write(textwrap.indent(trace_metadata_string(title), "# "))

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


def detect_serial_port():
    j = json.loads(
        subprocess.check_output(["platformio", "device", "list", "--json-output"])
    )
    ports = [d["port"] for d in j if "STM32" in d.get("description", "")]
    if not ports:
        raise error.Error(
            "Could not auto-detect serial port; platformio device list did not "
            "yield any STM32 devices.  Choose port explicitly with --port."
        )
    if len(ports) > 1:
        raise error.Error(
            "Could not auto-detect serial port; platformio device list "
            f"yielded multiple STM32 devices: {', '.join(ports)}.  "
            "Choose port explicitly with --port."
        )
    print(f"Detected device connected to {ports[0]}", file=sys.stderr)
    return ports[0]


interface: debug.ControllerDebugInterface
interpreter: CmdLine


def main():
    terminal_parser = argparse.ArgumentParser()
    terminal_parser.add_argument(
        "--port",
        type=str,
        help="Serial port device is connected to, e.g. /dev/ttyACM0.  If "
        "unspecified, we try to auto-detect the port.",
    )
    terminal_parser.add_argument(
        "--command", "-c", type=str, help="Run the given command and exit."
    )
    terminal_parser.add_argument("--offline", "-o", default=False, action='store_true',
                                 help="Run interpreter without connecting to device")
    terminal_parser.add_argument(
        "--detect-port-and-quit",
        action="store_true",
        help="""Detect the port that the device is connected to and then
immediately exit.  This is useful for scripts that invoke this program multiple
times.  Port detection is the slowest part of startup, so this option lets you
do it once upfront.""",
    )

    terminal_args = terminal_parser.parse_args()

    if terminal_args.offline:
        print("Starting in offline mode")
    elif not terminal_args.port:
        terminal_args.port = detect_serial_port()

    if terminal_args.detect_port_and_quit:
        print(terminal_args.port)
        sys.exit(0)

    global interface
    interface = debug.ControllerDebugInterface(port=terminal_args.port)

    global interpreter
    interpreter = CmdLine()

    if terminal_args.command:
        # Set matplotlib to noninteractive mode.  This causes show() to block,
        # so if you do `controller_debug.py -c "trace graph"` the program won't
        # exit until you close the displayed graph.
        plt.ioff()
        interpreter.onecmd(terminal_args.command)
    else:
        # Turn on interactive mode for matplotlib, so show() doesn't block.
        plt.ion()
        interpreter.cli_loop()


if __name__ == "__main__":
    main()
