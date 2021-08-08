#!/usr/bin/env python3

# Ventilator debug interface: simple command line interface
# For a list of available commands, enter 'help'

__copyright__ = "Copyright 2021 RespiraWorks"

__license__ = """

    Copyright 2021 RespiraWorks

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

"""

import debug
import argparse
import cmd
import glob
import json
import matplotlib.pyplot as plt
import os
import shlex
import subprocess
import sys
import traceback
import colors
import error
import test_data


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
        self.test_definitions_dir = "test_scenarios/"

    def autoload(self):
        for x in glob.glob(self.test_definitions_dir + "*.json"):
            interface.tests_import(x)

    def update_prompt(self, mode=None):
        if not interface.connected():
            self.prompt = colors.red("OFFLINE] ")
        else:
            if mode is None:
                mode = interface.mode_get()
            if mode == 1:
                self.prompt = colors.orange("BOOT] ")
            else:
                self.prompt = colors.green("] ")

    def cli_loop(self):
        self.autoload()
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
        params = shlex.split(line)
        if os.path.exists(params[0]):
            file_name = params[0]
        elif os.path.exists(self.scripts_directory + params[0]):
            file_name = self.scripts_directory + params[0]
        else:
            print("Unknown file " + params[0])
            return
        gbl = globals().copy()
        gbl["cmdline"] = line
        gbl["parser"] = self
        exec(open(file_name).read(), gbl)

    def complete_run(self, text, line, begidx, endidx):
        return glob.glob(text + "*.py") + [
            x[len(self.scripts_directory) :]
            for x in glob.glob(self.scripts_directory + text + "*.py")
        ]

    def do_test(self, line):
        """This command is for working with structured test scenarios and saved test data.

A test scenario defines independent variables in question, whether they be ventilator settings
or manual settings applicable to external test equipment. It may also specify test criteria
which can be confirmed by human examination or other scripts. A scenario also defines which
variables to capture, how often to sample them and length of test run.

Running a test scenario will capture traced variables as well as a snapshot of all ventilator
variables and other available metadata concerning the testing environment.

The unique test identifier (as well as the file name) is defined as
    <UTC date-time>_<tester>_<scenario_name>

test load <file>
  loads test scenarios from specified .csv or .json <file>

test autoload
  loads all test scenarios found in `test_scenarios` subdirectory

test clear
  clears all test scenarios loaded in this session

test list [--verbose/-v]
  lists all test scenarios loaded in this session

test show <scenario>
  prints out the full definition of named <scenario>

test apply <scenario>
  applies all ventilator settings for named <scenario>

test run <scenario> [--verbose/-v] [--plot/-p]
  runs named <scenario> and saves data to .json
    --verbose/-v  will also print out full trace data in columns
    --plot/-p will also plot traces and save plots as .png

test read <file> [--verbose/-v] [--plot/-p]
  reads test data from <file> and prints it out,
    --verbose/-v  will also print out full trace data in columns
    --plot/-p will also plot traces and save plots as .png
        """
        params = shlex.split(line)
        if len(params) < 1:
            print(colors.red("Not enough args for `test`\n"))
            return
        subcommand = params[0]

        if subcommand == "load":
            if len(params) < 2:
                print(colors.red("File name not provided for `test load`\n"))
                return
            interface.tests_import(params[1])

        elif subcommand == "autoload":
            for x in glob.glob(self.test_definitions_dir + "*.json"):
                interface.tests_import(x)

        elif subcommand == "clear":
            interface.scenarios.clear()

        elif subcommand == "list":
            verbose = len(params) > 1 and (
                params[1] == "-v" or params[1] == "--verbose"
            )
            interface.tests_list(verbose)

        elif subcommand == "show":
            if len(params) < 2:
                print(colors.red("Test name not provided for `test show`\n"))
            if params[1] not in interface.scenarios.keys():
                print(colors.red(f"Test `{params[1]}` does not exist\n"))
            print(interface.scenarios[params[1]].long_description())

        elif subcommand == "apply":
            if len(params) < 2:
                print(colors.red("Test name not provided for `test apply`\n"))
                return
            if params[1] not in interface.scenarios.keys():
                print(colors.red(f"Test `{params[1]}` does not exist\n"))
            interface.test_apply(params[1])

        elif subcommand == "run":
            if len(params) < 2:
                print(colors.red("Test name not provided for `test run`\n"))
                return
            if params[1] not in interface.scenarios.keys():
                print(colors.red(f"Test `{params[1]}` does not exist\n"))
            test = interface.test_run(params[1])
            test.save_json(print_self=True)
            if len(params) > 2:
                parser = CmdArgumentParser("test")
                parser.add_argument(
                    "--verbose", "-v", default=False, action="store_true"
                )
                parser.add_argument("--plot", "-p", default=False, action="store_true")
                args2 = parser.parse_args(params[2:])
                if args2.verbose:
                    print(test.print_trace())
                if args2.plot:
                    test.plot(save=True, show=True)

        elif subcommand == "read":
            if len(params) < 2:
                print(colors.red("File name not provided for `test read`\n"))
                return
            test = test_data.TestData.from_json(params[1])
            print(test)
            if len(params) > 2:
                parser = CmdArgumentParser("test")
                parser.add_argument(
                    "--verbose", "-v", default=False, action="store_true"
                )
                parser.add_argument("--plot", "-p", default=False, action="store_true")
                args2 = parser.parse_args(params[2:])
                if args2.verbose:
                    print(test.print_trace())
                if args2.plot:
                    test.plot(save=False, show=True)

        else:
            print("Invalid test args: {}", params)

    def do_exec(self, line):
        """exec()'s a string.  Good luck!"""
        exec(line)

    def do_peek(self, line):
        """Peek at a memory location.

ex: peek <address> <ct> <fmt> <file>

   address - the starting address passed as an integer value
   ct      - Number of bytes to read (default 1)
   fmt     - An optional formatting string.
   file    - An optional file to save the data to

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
        file_name = None
        if len(param) > 3:
            file_name = param[3]
        if len(param) > 2:
            fmt = param[2]
        if len(param) > 1:
            ct = int(param[1], 0)
        address = int(param[0], 0)
        interface.peek(address, ct, fmt, file_name)

    def do_poke(self, line):
        """Write data to a memory address

ex: poke [type] <address> <data>

   type    - Optional type, can be byte, short, long or float
             determines how the data will be interpreted.

   address - Address at which to write data

   data    - One or more data items to write.
"""
        param = shlex.split(line)
        if len(param) < 2:
            print("Please pass the address and at least one value to write")
            return

        poke_type = "byte"
        if param[0] in ["long", "short", "float"]:
            poke_type = param[0]
            param = param[1:]

        address = param[0]

        if poke_type == "float":
            data = [float(x) for x in param[1:]]
        else:
            data = [int(x, 0) for x in param[1:]]
        interface.poke(address, data, poke_type)

    def do_EOF(self, line):
        return True

    def do_exit(self, line):
        return True

    def do_get(self, line):
        cl = line.split()
        if len(cl) < 1:
            print("Please give the variable name to read")
            return

        if cl[0] == "all":
            all_vars = interface.variables_get_all()
            for name, val in all_vars.items():
                print(f"{name:25} = {val}")
            return

        if len(cl) > 1:
            fmt = cl[1]
        else:
            fmt = None

        print(interface.variable_get(cl[0], fmt=fmt))

    def complete_get(self, text, line, begidx, endidx):
        return interface.variables_starting_with(text)

    def help_get(self):
        print("Read the value of a ventilator debug variable and display it.")
        print(
            "In addition to those listed below 'get all' will retrieve all variables.\n"
        )
        print(interface.variables_list())

    def do_set(self, line):
        cl = line.split()
        if len(cl) < 2:
            print("Please give the variable name and value")
            return
        interface.variable_set(cl[0], cl[1])

    def complete_set(self, text, line, begidx, endidx):
        return interface.variables_starting_with(text)

    def help_set(self):
        print("Sets one of the ventilator debug variables listed below:\n")
        print(interface.variables_list())

    def do_trace(self, line):
        """The `trace` command controls/reads the controller's trace buffer.

Tracing lets you sample debug variables in real time.  Their values are saved
to a large internal memory buffer in the device, which you can then download
and/or display as a graph.

For meaningful performance testing, it is recommended that you use the higher
level `test` interface for a more structured experiment control experience. See
`help test`.

A sub-command must be passed as an option:

trace start [--period p] [var1 ... ]
  Starts collecting trace data.

  You can specify the names of up to TRACE_VAR_CT debug variables to trace.  If
  you don't specify any, we use the last known values.

  --period controls the sample period in units of one trip through the
  controller's high-priority loop.  If you don't specify a period, we use 1.

trace flush
  Flushes the trace buffer. If trace is ongoing, buffer will be filled with new data.

trace stop
  Stops trace

trace status
  Returns the current state of the trace:
    - traced variables
    - trace period
    - number of samples in the trace buffer

trace save [--verbose/-v] [--plot/-p]
  Downloads trace data and saves it as an "unplanned test". File will be named as
  <date-time>_<user>_manual_trace.json with a blank test scenario definition.
  See more about tests and test scenarios with `help test`.
    --verbose/-v  will also print out full trace data
    --plot/-p will also plot traces and save as .png
"""
        cl = shlex.split(line)
        if len(cl) < 1:
            print("Error, please specify the trace command to run.")
            print(self.do_trace.__doc__)
            return

        if cl[0] == "flush":
            interface.trace_flush()

        elif cl[0] == "start":
            parser = CmdArgumentParser("trace start")
            parser.add_argument("--period", type=int)
            parser.add_argument("var", nargs="*")
            args = parser.parse_args(cl[1:])

            # TODO: check validity of all variables

            if args.period:
                interface.trace_set_period(args.period)
            else:
                interface.trace_set_period(1)

            if args.var:
                interface.trace_select(args.var)

            interface.trace_flush()
            interface.trace_start()

        elif cl[0] == "stop":
            interface.trace_stop()

        elif cl[0] == "save":
            test = interface.trace_save()
            test.save_json(print_self=True)

            if len(cl) > 1:
                parser = CmdArgumentParser("test")
                parser.add_argument(
                    "--verbose", "-v", default=False, action="store_true"
                )
                parser.add_argument("--plot", "-p", default=False, action="store_true")
                args2 = parser.parse_args(cl[1:])
                if args2.verbose:
                    print(test.print_trace())
                if args2.plot:
                    test.plot(save=True, show=True)

        elif cl[0] == "status":
            print("Traced variables:")
            for var in interface.trace_active_variables_list():
                print(f" - {var.name}")
            print(f"Trace period: {interface.trace_get_period_us()} \u03BCs")
            print(f"Samples in buffer: {interface.trace_num_samples()}")

        else:
            print(f"Unknown trace sub-command {cl[0]}")
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
        "-p",
        type=str,
        help="Serial port device is connected to, e.g. /dev/ttyACM0."
        " If unspecified, we try to auto-detect the port.",
    )
    terminal_parser.add_argument(
        "--command", "-c", type=str, help="Run the given command and exit."
    )
    terminal_parser.add_argument(
        "--offline",
        "-o",
        default=False,
        action="store_true",
        help="Run interpreter without connecting to device",
    )
    terminal_parser.add_argument(
        "--detect-only",
        action="store_true",
        help="Detect the port that the device is connected to and then immediately exit."
        "This is useful for scripts that invoke this program multiple times.  Port detection is"
        "the slowest part of startup, so this option lets you do it once upfront.",
    )

    terminal_args = terminal_parser.parse_args()

    if terminal_args.offline:
        print("Starting in offline mode")
    elif not terminal_args.port:
        terminal_args.port = detect_serial_port()

    if terminal_args.detect_only:
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
