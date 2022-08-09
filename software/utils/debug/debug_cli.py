#!/usr/bin/env python3

# Ventilator debug self.interface: simple command line self.interface
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

import argparse
import cmd
import glob
import os
import shlex
import traceback
from lib.colors import *
from lib.error import Error
from lib.serial_detect import detect_stm32_ports, print_detected_ports
from controller_debug import ControllerDebugInterface, MODE_BOOT
from debug_funcs import SUPPORTED_FUNCTIONS, DebugFunctions
from var_info import VAR_ACCESS_READ_ONLY, VAR_ACCESS_WRITE, VAR_FLOAT_ARRAY
import numpy as np
import matplotlib.pyplot as plt
import test_data
from pathlib import Path


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

    interface: ControllerDebugInterface
    scripts_directory: str
    test_scenarios_dir: Path
    test_data_dir: Path

    def __init__(self, port):
        super(CmdLine, self).__init__()
        self.scripts_directory = "scripts"
        self.test_scenarios_dir = Path("test_scenarios").absolute().resolve()
        self.test_data_dir = Path("../../../test_data").absolute().resolve()
        self.interface = ControllerDebugInterface()
        if not port:
            port = auto_select_port()
        if port:
            self.interface.connect(port)

        # We must do this so that autocomplete will work with dash `-` in filenames
        try:
            import readline

            delims = readline.get_completer_delims()
            readline.set_completer_delims(delims.replace("-", ""))
        except ImportError:
            pass

    def autoload(self):
        for x in glob.glob(str(self.test_scenarios_dir / "*.json")):
            self.interface.tests_import(x)

    def update_prompt(self):
        if not self.interface.connected():
            self.prompt = purple("[OFFLINE] ")
            return

        self.prompt = purple("[ERROR] ")
        try:
            mode = self.interface.mode_get()
        except Error as e:
            print(red(str(e)))
            return
        except:
            traceback.print_exc()
            return

        if mode == MODE_BOOT:
            self.prompt = orange(f"[{self.interface.serial_port.port}:boot] ")
        else:
            sn = self.interface.variable_get("0_ventilator_serial_number", raw=True)
            if sn > 0:
                self.prompt = green(f"[sn:{sn}] ")
            else:
                self.prompt = green(f"[port:{self.interface.serial_port.port}] ")

    def cli_loop(self):
        self.autoload()
        if self.interface.connected():
            self.interface.resynchronize()
            self.interface.variables_update_info()
        self.update_prompt()
        if self.interface.connected() and not self.interface.sanity_checks():
            return

        while True:
            try:
                return cmd.Cmd.cmdloop(self)
            except ArgparseShowHelpError:
                pass
            except Error as e:
                print(red(str(e)))
            except:
                traceback.print_exc()
            self.update_prompt()

    def emptyline(self):
        self.interface.resynchronize()
        self.update_prompt()

    def do_EOF(self, line):
        return True

    def do_exit(self, line):
        return True

    def do_quit(self, line):
        return True

    def do_env(self, line):
        """Prints this command interface's environmental variables"""
        print(f"scripts_directory = {self.scripts_directory}")
        print(f"test_scenarios_dir = {self.test_scenarios_dir}")
        print(f"self.test_data_dir = {self.test_data_dir}")

    def do_debug(self, line):
        """Sets display of low level serial data on/off.

        Usage:
          debug on
          debug off

        """
        line = line.strip().lower()
        if line == "on":
            self.interface.print_raw = True
        elif line == "off":
            self.interface.print_raw = False
        else:
            print("Unknown command; pass 'on' or 'off'.")

    def do_connect(self, line):
        """This command manages the connection of the debug interface to the ventilator controller

        If ventilator USB cable is unplugged, the CLI prompt should update to reflect this on the next loop
        (blank line or failed command). You may also start the debugger without a plugged in device.
        Use one of the following commands to reconnect.

        connect list
          searches and lists available STM32 devices on the serial bus

        connect auto
          attempt to connect to a plugged in controller automatically

        connect <port>
          connect to controller on a specific port

        """

        params = shlex.split(line)
        if len(params) < 1:
            print(red("Not enough args for `connect`\n"))
            return
        subcommand = params[0]

        if subcommand == "list":
            print_detected_ports()

        elif subcommand == "auto":
            self.interface.connect(auto_select_port())
            self.interface.resynchronize()
            self.interface.variables_update_info()
            self.update_prompt()

        else:
            self.interface.connect(params[0])
            self.interface.resynchronize()
            self.interface.variables_update_info()
            self.update_prompt()

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

        if len(params) < 1:
            red("Must specify script name for `run` command")
            return

        if not os.path.exists(self.scripts_directory + "/" + params[0] + ".py"):
            print("Unknown script " + params[0])
            return

        import_command = f"import {self.scripts_directory}.{params[0]}"
        exec(import_command)

        passed_params = ""
        if len(params) > 1:
            passed_params = ', "' + " ".join(params[1:]) + '"'
        run_command = f"{self.scripts_directory}.{params[0]}.{params[0]}(self.interface{passed_params})"
        exec(run_command)

    def complete_run(self, text, line, begidx, endidx):
        wildcard_path = Path(self.scripts_directory) / (text + "*.py")
        return [Path(x).stem for x in glob.glob(str(wildcard_path))]

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

        test run <scenario> [--verbose/-v] [--plot/-p] [--csv/-c]
          runs named <scenario> and saves data to .json
            --verbose/-v  - also print out full trace data in columns
            --plot/-p     - also plot traces and save plots as .png
            --csv/-c      - also save traces as .csv

        test read <file> [--verbose/-v] [--plot/-p] [--csv/-c]
          reads test data from <file> and prints it out,
            --verbose/-v  - also print out full trace data in columns
            --plot/-p     - also plot traces and save plots as .png
            --csv/-c      - also save traces as .csv
        """
        params = shlex.split(line)
        if len(params) < 1:
            print(red("Not enough args for `test`\n"))
            return
        subcommand = params[0]

        if subcommand == "load":
            if len(params) < 2:
                print(red("File name not provided for `test load`\n"))
                return
            file_name = self.test_scenarios_dir / params[1]
            self.interface.tests_import(str(file_name))

        elif subcommand == "autoload":
            for x in glob.glob(str(self.test_scenarios_dir / "*.json")):
                self.interface.tests_import(x)

        elif subcommand == "clear":
            self.interface.scenarios.clear()

        elif subcommand == "list":
            verbose = len(params) > 1 and (
                params[1] == "-v" or params[1] == "--verbose"
            )
            self.interface.tests_list(verbose)

        elif subcommand == "show":
            if len(params) < 2:
                print(red("Test name not provided for `test show`\n"))
            if params[1] not in self.interface.scenarios.keys():
                print(red(f"Test `{params[1]}` does not exist\n"))
            print(self.interface.scenarios[params[1]].long_description())

        elif subcommand == "apply":
            if len(params) < 2:
                print(red("Test name not provided for `test apply`\n"))
                return
            if params[1] not in self.interface.scenarios.keys():
                print(red(f"Test `{params[1]}` does not exist\n"))
            self.interface.test_apply(params[1])

        elif subcommand == "run":
            if len(params) < 2:
                print(red("Test name not provided for `test run`\n"))
                return
            if params[1] not in self.interface.scenarios.keys():
                print(red(f"Test `{params[1]}` does not exist\n"))
            test = self.interface.test_run(params[1])
            if test is None:
                print("Aborted")
                return
            test.save_json(str(self.test_data_dir), print_self=True)
            if len(params) > 2:
                parser = CmdArgumentParser("test")
                parser.add_argument(
                    "--verbose", "-v", default=False, action="store_true"
                )
                parser.add_argument("--plot", "-p", default=False, action="store_true")
                parser.add_argument("--csv", "-c", default=False, action="store_true")
                args2 = parser.parse_args(params[2:])
                if args2.verbose:
                    print(test.print_traces())
                if args2.plot:
                    test.plot(str(self.test_data_dir), save=True, show=True)
                if args2.csv:
                    test.save_csv(str(self.test_data_dir))

        elif subcommand == "read":
            if len(params) < 2:
                print(red("File name not provided for `test read`\n"))
                return
            file_name = self.test_data_dir / (params[1] + ".json")
            test = test_data.TestData.from_json(str(file_name))
            print(test)
            if len(params) > 2:
                parser = CmdArgumentParser("test")
                parser.add_argument(
                    "--verbose", "-v", default=False, action="store_true"
                )
                parser.add_argument("--plot", "-p", default=False, action="store_true")
                parser.add_argument("--csv", "-c", default=False, action="store_true")
                args2 = parser.parse_args(params[2:])
                if args2.verbose:
                    print(test.print_traces())
                if args2.plot:
                    test.plot(str(self.test_data_dir), save=True, show=True)
                if args2.csv:
                    test.save_csv(str(self.test_data_dir))

        else:
            print("Invalid test args: {}", params)

    def get_dataset_names(self, text=""):
        wildcard_path = self.test_data_dir / (text + "*.json")
        return [str(Path(x).stem) for x in glob.glob(str(wildcard_path))]

    def get_scenario_file_names(self, text=""):
        wildcard_path = self.test_scenarios_dir / (text + "*")
        return [str(Path(x).name) for x in glob.glob(str(wildcard_path))]

    def complete_test(self, text, line, begidx, endidx):
        sub_commands = [
            "load",
            "autoload",
            "clear",
            "list",
            "show",
            "apply",
            "run",
            "read",
        ]
        tokens = line.split()
        if len(tokens) == 3 and tokens[1] == "read":
            return self.get_dataset_names(text)
        elif len(tokens) == 3 and tokens[1] == "load":
            return self.get_scenario_file_names(text)
        elif len(tokens) == 3 and tokens[1] in ["run", "apply", "show"]:
            return [x for x in self.interface.scenarios.keys() if x.startswith(text)]
        elif len(tokens) == 2 and text == "read":
            return ["read "]
        elif len(tokens) == 2 and tokens[1] == "read" and text == "":
            return self.get_dataset_names("")
        elif len(tokens) == 2 and text == "load":
            return ["load "]
        elif len(tokens) == 2 and tokens[1] == "load" and text == "":
            return self.get_scenario_file_names("")
        elif len(tokens) == 2 and text in ["run", "apply", "show"]:
            return [text + " "]
        elif len(tokens) == 2 and text == "" and tokens[1] in ["run", "apply", "show"]:
            return [x for x in self.interface.scenarios.keys()]
        elif len(tokens) == 2 and any(s.startswith(text) for s in sub_commands):
            return [s for s in sub_commands if s.startswith(text)]
        elif len(tokens) == 1:
            return sub_commands

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
           i.e. Data is displayed as a series of 4 32-bit hex values / line"""
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
        self.interface.peek(address, ct, fmt, file_name)

    def do_poke(self, line):
        """Write data to a memory address

        ex: poke [type] <address> <data>

           type    - Optional type, can be byte, short, long or float
                     determines how the data will be interpreted.

           address - Address at which to write data

           data    - One or more data items to write."""
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
        self.interface.poke(address, data, poke_type)

    def do_get(self, line):
        # TODO: plotting (-p/--plot)?
        cl = line.split()
        if len(cl) < 1:
            print("Please give the variable name to read")
            return

        var_name = cl[0]
        raw = False
        fmt = None
        if len(cl) > 1:
            if cl[1] == "--raw":
                raw = True
            else:
                fmt = cl[1]

        if var_name == "all":
            found = self.interface.variables_find()
            all_vars = self.interface.variables_get(found, raw=raw)
            self.print_variable_values(all_vars, show_access=True)
            return
        elif var_name == "set":
            found = self.interface.variables_find(access_filter=VAR_ACCESS_WRITE)
            all_vars = self.interface.variables_get(found, raw=raw)
            self.print_variable_values(all_vars, show_access=False)
            return
        elif var_name == "read":
            found = self.interface.variables_find(access_filter=VAR_ACCESS_READ_ONLY)
            all_vars = self.interface.variables_get(found, raw=raw)
            self.print_variable_values(all_vars, show_access=False)
            return
        elif "*" in var_name or "?" in var_name:
            found = self.interface.variables_find(pattern=var_name)
            all_vars = self.interface.variables_get(found, raw=raw)
            self.print_variable_values(all_vars, show_access=False)
            return
        else:
            variable_md = self.interface.variable_metadata[var_name]
            val = self.interface.variable_get(var_name, raw=raw, fmt=fmt)
            print(variable_md.print_value(val))
            # TODO: should analyze only be done when an addition flag is passed?
            self.analyze_var(variable_md, val)

    def analyze_var(self, variable_md, val):
        data_analysis_checks = dict()
        # TODO: usefeul checks for non-float array types
        if variable_md.type == VAR_FLOAT_ARRAY:
            float_array = np.array(val[1:-1].split()).astype(float)

            # TODO: other data analysis checks
            data_analysis_checks.update(
                {
                    "linear": False,
                }
            )

            # Check for linearity
            lin_tolerance = 0.0001  # floats are rounded to three decimal places
            diff = np.diff(float_array)
            data_analysis_checks["linear"] = np.all(diff - diff[0] < lin_tolerance)

            print("properties:")
            for k, v in data_analysis_checks.items():
                print(f"    {k} : {v}")

    def print_variable_values(self, names_values, show_access):
        for count, name in enumerate(sorted(names_values.keys())):
            variable_md = self.interface.variable_metadata[name]
            text = variable_md.print_value(names_values[name], show_access=show_access)
            if (count % 2) == 0:
                print(white(text))
            else:
                print(dark_orange(text))

    def complete_get(self, text, line, begidx, endidx):
        return self.interface.variables_find(pattern=(text + "*")) + [
            x for x in ["all", "set", "read"] if x.startswith(text)
        ]

    def help_get(self):
        print("Read the value of a ventilator debug variable and display it.")
        print("  get all [--raw]          -  retrieves all variables")
        print(
            "  get set [--raw]          -  retrieves all settings (writable variables)"
        )
        print("  get read [--raw]         -  retrieves all read-only variables")
        print("  get <var> [fmt] [--raw]  -  retrieves a specific variable(s)")
        print("       <var> - can contain wildcards * or ?")
        print("Available variables:")
        for k in sorted(self.interface.variable_metadata.keys()):
            print(f" {self.interface.variable_metadata[k].verbose()}")

    def do_set(self, line):
        set_parser = CmdArgumentParser(description="set value(s) to debug variables")
        set_parser.add_argument("var", help="variable to set")
        set_parser.add_argument("data", nargs="+", help="data to assign to variable")

        cl = line.split()
        args = set_parser.parse_args(cl)

        if args.var == "force_off":
            self.interface.variables_force_off()
            return
        elif args.var == "force_open":
            self.interface.variables_force_open()
            return

        debug_func_dict = {k: getattr(DebugFunctions, k) for k in dir(DebugFunctions)}
        try:
            data = eval("".join(args.data), globals(), debug_func_dict)
        except:
            print(
                "You may incorrectly used a supported function or called an unsupported function."
            )
            print("Run 'help set' to see available functions and usage.")
            raise

        if isinstance(data, np.ndarray):
            data = data.tolist()

        self.interface.variable_set(args.var, data)

    def complete_set(self, text, line, begidx, endidx):
        return self.interface.variables_find(
            pattern=(text + "*"), access_filter=VAR_ACCESS_WRITE
        ) + [x for x in ["force_off", "force_open"] if x.startswith(text)]

    def help_set(self):
        print("usage: set [var] [value(s)]")
        print("Sets value for a ventilator debug variable (or convenience macro):")
        print("  force_off           - resets all forced actuator variables")
        print("  force_open          - forces all valves open and blower to maximum")
        print()
        print("Available variables:")
        for k in sorted(self.interface.variables_find(access_filter=VAR_ACCESS_WRITE)):
            print(
                f" {self.interface.variable_metadata[k].verbose(show_access=False, show_format=False)}"
            )
        print()
        print("If a variable points to an array, then there are two ways to set it.")
        print(
            "  manually               - list the values in the order they should be set in the array"
        )
        print("  automatically          - populate the array values using a function")
        print("Available shortcut functions:")
        for key, value in SUPPORTED_FUNCTIONS.items():
            print("  {0:20} - {1}".format(key, value))
        print(
            "All native Python functionality and numpy can be used for function composition as well"
        )
        print("e.g.: set var np.flip(lin(1,5) + 5)")

    def do_trace(self, line):
        """The `trace` command controls/reads the controller's trace buffer.

        Tracing lets you sample debug variables in real time.  Their values are saved
        to a large internal memory buffer in the device, which you can then download
        and/or display as a graph.

        For meaningful performance testing, it is recommended that you use the higher
        level `test` self.interface for a more structured experiment control experience. See
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

        trace save [--verbose/-v] [--plot/-p] [--csv/-c]
          Downloads trace data and saves it as an "unplanned test". File will be named as
          <date-time>_<user>_manual_trace.json with a blank test scenario definition.
          See more about tests and test scenarios with `help test`.
            --verbose/-v  - also print out full trace data
            --plot/-p     - also plot traces and save as .png
            --csv/-c      - also save traces as .csv"""
        cl = shlex.split(line)
        if len(cl) < 1:
            print("Error, please specify the trace command to run.")
            print(self.do_trace.__doc__)
            return

        if cl[0] == "flush":
            self.interface.trace_flush()

        elif cl[0] == "start":
            parser = CmdArgumentParser("trace start")
            parser.add_argument("--period", type=int)
            parser.add_argument("var", nargs="*")
            args = parser.parse_args(cl[1:])

            # TODO: check validity of all variables

            if args.period:
                self.interface.trace_set_period(args.period)
            else:
                self.interface.trace_set_period(1)

            if args.var:
                self.interface.trace_select(args.var)

            self.interface.trace_start()

        elif cl[0] == "stop":
            self.interface.trace_stop()

        elif cl[0] == "save":
            test = self.interface.trace_save()
            test.save_json(self.test_data_dir, print_self=True)

            if len(cl) > 1:
                parser = CmdArgumentParser("test")
                parser.add_argument(
                    "--verbose", "-v", default=False, action="store_true"
                )
                parser.add_argument("--plot", "-p", default=False, action="store_true")
                parser.add_argument("--csv", "-c", default=False, action="store_true")
                args2 = parser.parse_args(cl[1:])
                if args2.verbose:
                    print(test.print_traces())
                if args2.plot:
                    test.plot(self.test_data_dir, save=True, show=True)
                if args2.csv:
                    test.save_csv(self.test_data_dir)

        elif cl[0] == "status":
            print("Traced variables:")
            for var in self.interface.trace_active_variables_list():
                print(f" - {var.name}")
            print(f"Trace period: {self.interface.trace_get_period_us()} \u03BCs")
            print(f"Samples in buffer: {self.interface.trace_num_samples()}")

        else:
            print(f"Unknown trace sub-command {cl[0]}")
            return

    def complete_trace(self, text, line, begidx, endidx):
        sub_commands = ["start", "flush", "stop", "status", "save"]
        tokens = shlex.split(line)
        if len(tokens) > 2 and tokens[1] == "start":
            return self.interface.variables_find(
                pattern=(text + "*"), access_filter=VAR_ACCESS_READ_ONLY
            )
        elif len(tokens) == 2 and text == "start":
            return ["start "]
        elif len(tokens) == 2 and text == "" and tokens[1] == "start":
            return self.interface.variables_find(access_filter=VAR_ACCESS_READ_ONLY)
        elif len(tokens) == 2 and any(s.startswith(text) for s in sub_commands):
            return [s for s in sub_commands if s.startswith(text)]
        elif len(tokens) == 1:
            return sub_commands

    def do_eeprom(self, line):
        """The `eeprom` command allows you to read/write to the controller's
        non-volatile memory.

        A sub-command must be passed as an option:

        eeprom read <address> <length>
          Reads length bytes in the EEPROM starting at given address.

        eeprom write <address> <data>
          Writes data (provided as a series of bytes) to the EEPROM."""
        cl = shlex.split(line)
        if len(cl) < 1:
            print("Error, please specify the operation to perform.")
            return
        if cl[0] == "read":
            if len(cl) < 3:
                print("Error, please provide address and length.")
                return
            print(self.interface.eeprom_read(cl[1], cl[2]))
        elif cl[0] == "write":
            if len(cl) < 3:
                print("Error, please provide address and at least 1 byte of data.")
                return
            self.interface.eeprom_write(cl[1], list(map(int, cl[2:])))
        else:
            print("Error: Unknown subcommand %s" % cl[0])
            return


def auto_select_port():
    ports = detect_stm32_ports()
    if not ports:
        red(
            "Could not auto-detect serial port; platformio device list did not "
            "yield any STM32 devices."
        )
        return None
    if len(ports) > 1:
        red(
            "Could not auto-detect serial port; platformio device list "
            f"yielded multiple STM32 devices: {', '.join(ports)}.  "
            "Choose port explicitly with --port."
        )
        return None
    return ports[0]


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

    terminal_args = terminal_parser.parse_args()

    interpreter = CmdLine(terminal_args.port)
    path = Path(interpreter.test_data_dir)
    path.mkdir(parents=True, exist_ok=True)

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
