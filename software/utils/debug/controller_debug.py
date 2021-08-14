# Ventilator debug interface: fundamental implementation and API

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

import serial
import threading
import time
import debug_types
import debug_protocol_pb2 as debug_proto
import var_info
from lib.error import Error
import test_scenario
import test_data
from pathlib import Path
from lib.colors import red


class ControllerDebugInterface:
    # If true, the raw bytes of the serial data will be printed.
    # This is handy for debugging the low level serial interface
    # It can be toggled with the 'debug' command
    print_raw = False

    serial_port = serial.Serial()

    variable_metadata = {}

    scenarios = {}

    # This lock is used to make the command interface thread safe.
    # The main debug program doesn't currently use threads, but scripts
    # run from it may and those scripts use these same functions to
    # send commands
    command_lock = threading.Lock()

    def connect(self, port):
        self.serial_port = serial.Serial(port=port, baudrate=115200)
        self.serial_port.timeout = 0.8
        print(f"Debug interface connected to device at {port}")

    def connected(self):
        return self.serial_port.isOpen()

    def debug_print(self, *args, **kwargs):
        if self.print_raw:
            print(*args, **kwargs)

    def mode_get(self):
        return self.send_command(debug_proto.Cmd.Code.Mode)[0]

    def resynchronize(self):
        with self.command_lock:
            try:
                cmd = [
                    debug_proto.Cmd.SpecialChar.Term,
                    debug_proto.Cmd.SpecialChar.Term,
                ]
                self.serial_port.write(bytearray(cmd))
                time.sleep(0.1)
                self.serial_port.reset_input_buffer()
            except serial.serialutil.SerialException:
                self.serial_port.close()
                self.variable_metadata.clear()
                raise Error(
                    "Could not resynchronize. Serial Exception encountered. Closing port."
                )

    # Read info about all the supported variables and load
    # them in a map
    def variables_update_info(self):
        self.variable_metadata.clear()
        try:
            for vid in range(256):
                data = self.send_command(
                    debug_proto.Cmd.Code.Variable,
                    [debug_proto.VarAccess.Subcmd.GetInfo]
                    + debug_types.int16s_to_bytes(vid),
                )
                if data is None:
                    break
                variable = var_info.VarInfo(vid, data)
                self.variable_metadata[variable.name] = variable
        # todo maybe not wait for an exception to terminate this loop?
        except Error as e:
            pass

    def variables_list(self):
        ret = "Variables currently defined:\n"
        for k in self.variable_metadata.keys():
            ret += " {:25} - {}\n".format(k, self.variable_metadata[k].help)
        return ret

    def variables_starting_with(self, text):
        out = []
        for i in self.variable_metadata.keys():
            if i.startswith(text):
                out.append(i)
        return out

    def variable_by_id(self, vid):
        for name in self.variable_metadata:
            if self.variable_metadata[name].id == vid:
                return self.variable_metadata[name]
        return None

    def variables_force_open(self):
        # Ensure the ventilator fan is on, and disconnect the fan from the system
        # so it's not inflating the test lung.  Edwin's request is that the fan
        # doesn't have to spin up during these tests.
        self.variable_set("forced_exhale_valve_pos", 1)
        self.variable_set("forced_blower_valve_pos", 1)
        self.variable_set("forced_blower_power", 1)
        # todo force o2 psol open or closed?

    def variables_force_off(self):
        # Unforce parameters so they can be controlled by the
        # controller.  Note that you should unforce the blower power *after* setting the
        # gui_mode because if we unforced it while we were still in mode 0
        # (i.e. "ventilator off"), the fan would momentarily spin down.
        self.variable_set("forced_exhale_valve_pos", -1)
        self.variable_set("forced_blower_valve_pos", -1)
        self.variable_set("forced_blower_power", -1)
        # todo unforce o2 psol?

    def variables_set(self, pairs):
        for var, val in pairs.items():
            print(f"  applying {var:25} = {val}")
            if var == "gui_mode":
                # todo replace these with enums, preferably from proto
                if val == "pressure_control":
                    self.variable_set(var, 1)
                elif val == "pressure_assist":
                    self.variable_set(var, 2)
                else:
                    print(f"WARNING: Do not know how to set gui_mode={val}")
            else:
                try:
                    self.variable_set(var, val)
                except:
                    print(f"WARNING: failed to set {var}={val}({type(val)})")

    def variables_get_all(self):
        ret = {}
        for name in self.variable_metadata:
            ret[name] = self.variable_get(name)
        return ret

    def variable_get(self, name, raw=False, fmt=None):
        if not (name in self.variable_metadata):
            raise Error("Unknown variable %s" % name)

        variable = self.variable_metadata[name]
        data = self.send_command(
            debug_proto.Cmd.Code.Variable,
            [debug_proto.VarAccess.Subcmd.GetInfo]
            + debug_types.int16s_to_bytes(variable.id),
        )
        value = variable.from_bytes(data)

        if raw:
            return value

        # If a format wasn't passed, use the default for this var
        if fmt is None:
            fmt = variable.format

        return fmt % value

    def variable_set(self, name, value):
        if not (name in self.variable_metadata):
            raise Error("Unknown variable %s" % name)

        variable = self.variable_metadata[name]
        data = variable.to_bytes(value)

        self.send_command(
            debug_proto.Cmd.Code.Variable,
            [debug_proto.VarAccess.Subcmd.Set]
            + debug_types.int16s_to_bytes(variable.id)
            + data,
        )
        return

    def tests_import(self, file_name):
        in_file = Path(file_name)
        if not in_file.is_file():
            raise Error(f"Input file does not exist {file_name}")
        elif in_file.suffix == ".csv":
            imported_scenarios = test_scenario.TestScenario.from_csv(
                in_file, self.variable_metadata.keys()
            )
        elif in_file.suffix == ".json":
            imported_scenarios = test_scenario.TestScenario.from_json(in_file)
        else:
            raise Error(f"Unknown file format `{in_file.suffix}`")
        if bool(set(imported_scenarios.keys()) & set(self.scenarios.keys())):
            raise Error(
                "Cannot import test scenarios, id's clash with already loaded ones"
            )
        print(
            "Imported {} new test scenarios from {}".format(
                len(imported_scenarios.keys()), file_name
            )
        )
        # this just appends one dictionary to the other, weird syntax
        self.scenarios = {**self.scenarios, **imported_scenarios}

    def tests_list(self, verbose=False):
        for key in self.scenarios:
            if verbose:
                print(self.scenarios[key].long_description())
            else:
                print(self.scenarios[key].short_description())

    def test_apply(self, name):
        if name not in self.scenarios.keys():
            raise Error(f"No such test scenario: {name}")
        scenario = self.scenarios[name]
        print(f"Applying test scenario: {scenario.short_description()}")
        self.variables_set(scenario.ventilator_settings)

    def test_run(self, name):
        if name not in self.scenarios.keys():
            raise Error(f"No such test scenario: {name}")

        test = test_data.TestData(self.scenarios[name])

        if test.git_dirty:
            print(
                red(
                    "There are unstaged or uncommitted changes to the code. Saving this"
                    " data with reference to most recent commit might be misleading."
                )
            )
            value = input("Are you sure you want to continue with this test run? ")
            if value != "y" and value != "Y":
                return

        self.variables_force_open()
        self.variable_set("gui_mode", 0)
        print(f"\nExecuting test scenario:\n {test.scenario.long_description(True)}")

        # Give the user a chance to adjust the test lung.
        if len(test.scenario.manual_settings):
            input("\nAdjust manual settings per above, then press enter.\n")

        # Apply all vent settings
        print("\n")
        self.variables_set(test.scenario.ventilator_settings)
        self.variables_force_off()
        time.sleep(test.scenario.capture_ignore_secs)

        print(
            f"\nStarting data capture for {test.scenario.capture_duration_secs} seconds"
        )
        self.trace_flush()
        self.trace_set_period(test.scenario.trace_period)
        self.trace_select(test.scenario.trace_variable_names)
        self.trace_start()
        time.sleep(test.scenario.capture_duration_secs)

        print("\nRetrieving data and halting ventilation")
        # get data and halt ventilation
        test.traces = self.trace_download()
        self.variable_set("gui_mode", 0)
        self.trace_stop()
        test.ventilator_settings = self.variables_get_all()
        return test

    def trace_save(self, scenario_name="manual_trace"):
        test = test_data.TestData(test_scenario.TestScenario())
        test.traces = self.trace_download()
        time_series = test.traces[0]
        test.scenario.capture_duration_secs = (
            time_series[len(time_series) - 1] - time_series[0]
        )
        test.scenario.name = scenario_name
        test.scenario.description = (
            "Manually triggered trace -- undefined test scenario"
        )
        test.scenario.capture_ignore_secs = 0
        test.scenario.trace_period = self.trace_get_period()
        test.scenario.trace_variable_names = [
            x.name for x in self.trace_active_variables_list()
        ]
        test.ventilator_settings = self.variables_get_all()
        return test

    def peek(self, address, ct=1, fmt="+XXXX", fname=None, raw=False):
        address = debug_types.decode_address(address)
        if address is None:
            print("Unknown symbol")
            return

        out = []

        address_iterator = address

        while ct:
            n = min(ct, 256)
            data = self.send_command(
                debug_proto.Cmd.Code.Peek,
                debug_types.int32s_to_bytes(address_iterator)
                + debug_types.int16s_to_bytes(n),
            )
            out += data
            ct -= len(data)
            address_iterator += len(data)

        if raw:
            return out

        s = debug_types.format_peek(out, fmt, address)
        if fname is None:
            print(s)
        else:
            fp = open(fname, "w")
            fp.write(s)
            fp.write("\n")
            fp.close()

    def peek16(self, address, ct=None, le=True, signed=False):
        address = debug_types.decode_address(address)
        if address is None:
            print("Unknown symbol")
            return

        if ct is None:
            ct = 1
        out = self.peek(address, 2 * ct, raw=True)
        return debug_types.bytes_to_int16s(out, le, signed)

    def peek32(self, address, ct=None, le=True, signed=False):
        address = debug_types.decode_address(address)
        if address is None:
            print("Unknown symbol")
            return

        if ct is None:
            ct = 1
        out = self.peek(address, 4 * ct, raw=True)
        return debug_types.bytes_to_int32s(out, le, signed)

    def poke(self, address, dat, ptype):
        address = debug_types.decode_address(address)
        if address is None:
            print("Unknown symbol")
            return
        if isinstance(dat, int):
            dat = [dat]

        if ptype == "long":
            dat = debug_types.int32s_to_bytes(dat)
        if ptype == "short":
            dat = debug_types.int16s_to_bytes(dat)
        if ptype == "float":
            dat = [debug_types.f_to_i(x) for x in dat]
            dat = debug_types.int32s_to_bytes(dat)

        self.send_command(
            debug_proto.Cmd.Code.Poke, debug_types.int32s_to_bytes(address) + dat,
        )

    def poke32(self, address, dat):
        self.poke(address, dat, "long")

    def poke16(self, address, dat):
        self.poke(address, dat, "short")

    def trace_flush(self):
        self.send_command(
            debug_proto.Cmd.Code.Trace, [debug_proto.TraceCmd.Subcmd.Flush],
        )

    def trace_set_period(self, period):
        self.send_command(
            debug_proto.Cmd.Code.Trace,
            [debug_proto.TraceCmd.Subcmd.SetPeriod]
            + debug_types.int32s_to_bytes(period),
        )

    def trace_get_period(self):
        dat = self.send_command(
            debug_proto.Cmd.Code.Trace, [debug_proto.TraceCmd.Subcmd.GetPeriod],
        )
        return debug_types.bytes_to_int32s(dat)[0]

    def trace_get_period_us(self):
        # get trace period as number of loops
        period = self.trace_get_period()
        if period < 1:
            period = 1

        # Scale this by the loop period which is an integer in microseconds.
        return period * self.variable_get("loop_period", raw=True)

    def trace_select(self, var_names):
        if len(var_names) > debug_proto.TraceCmd.MaxTraceVars:
            raise Error(
                f"Can't trace more than {debug_proto.TraceCmd.MaxTraceVars} variables at once."
            )
        for name in var_names:
            if name not in self.variable_metadata.keys():
                raise Error(f"Cannot select trace. Variable `{name}` does not exist.")
        var_names += [""] * (debug_proto.TraceCmd.MaxTraceVars - len(var_names))
        for (i, var_name) in enumerate(var_names):
            var_id = -1
            if var_name in self.variable_metadata:
                var_id = self.variable_metadata[var_name].id
            var = debug_types.int16s_to_bytes(var_id)
            self.send_command(
                debug_proto.Cmd.Code.Trace,
                [debug_proto.TraceCmd.Subcmd.SetVarId, i] + var,
            )

    def trace_start(self):
        self.send_command(
            debug_proto.Cmd.Code.Trace, [debug_proto.TraceCmd.Subcmd.Start],
        )

    def trace_stop(self):
        self.trace_select([])
        self.trace_flush()

    def trace_num_samples(self):
        dat = self.send_command(
            debug_proto.Cmd.Code.Trace, [debug_proto.TraceCmd.Subcmd.CountSamples],
        )
        return debug_types.bytes_to_int32s(dat)[0]

    def trace_active_variables_list(self):
        """Return a list of active trace variables"""
        ret = []
        for i in range(debug_proto.TraceCmd.MaxTraceVars):
            data = self.send_command(
                debug_proto.Cmd.Code.Trace, [debug_proto.TraceCmd.Subcmd.GetVarId, i],
            )
            var_id = debug_types.bytes_to_int16s(data)[0]
            var = self.variable_by_id(var_id)
            if var is not None:
                ret.append(var)
        return ret

    def trace_download(self):
        """Fetches a trace from the controller.

        Returns a list of N+1 lists where N is the number of active trace variables.
        The first list gives the time in seconds of each sample relative to the
        start of the trace, and the remaining N lists each holds the trace data
        for one variable.
        """
        trace_vars = self.trace_active_variables_list()
        if len(trace_vars) < 1:
            raise Error("No active traces to download")
        var_count = len(trace_vars)

        # get samples count
        total_num_samples = self.trace_num_samples() * var_count
        bytes_per_int32 = 4

        data = []
        while len(data) < bytes_per_int32 * total_num_samples:
            byte = self.send_command(
                debug_proto.Cmd.Code.Trace, [debug_proto.TraceCmd.Subcmd.Download]
            )
            if len(byte) < 1:
                break
            data += byte

        # Convert the bytes into an array of unsigned 32-bit values
        data = debug_types.bytes_to_int32s(data)

        # The data comes as a list of of samples, where each sample contains
        # len(trace_vars) uint32s: [a1, b1, c1, a2, b2, c2, ...].  Parse this into
        # sublists [[a1', a2', ...], [b1', b2', ...], [c1', c2', ...]], where each
        # of the variables is converted to the correct type.
        ret = [[] for _ in range(var_count)]

        # The `zip` expression groups data into sublists of var_count elems.  See the
        # "grouper" recipe:
        # https://docs.python.org/3/library/itertools.html#itertools-recipes
        iters = [iter(data)] * var_count
        for sample in zip(*iters):
            for i, val in enumerate(sample):
                ret[i].append(trace_vars[i].convert_int(val))

        # get trace period, multiply by 1e-6 (i.e. 1/1,000,000) to convert it to seconds.

        period = self.trace_get_period_us() * 1e-6

        timestamp = [x * period for x in range(len(ret[0]))]
        ret.insert(0, timestamp)

        return ret

    def eeprom_read(self, address, length):
        data = self.send_command(
            debug_proto.Cmd.Code.EepromAccess,
            [debug_proto.EepromCmd.Subcmd.Read]
            + debug_types.int16s_to_bytes(int(address, 0))
            + debug_types.int16s_to_bytes(int(length, 0)),
        )
        return debug_types.format_peek(data, "+XXXX", int(address, 0))

    def eeprom_write(self, address, data):
        self.send_command(
            debug_proto.Cmd.Code.EepromAccess,
            [debug_proto.EepromCmd.Subcmd.Write]
            + debug_types.int16s_to_bytes(int(address, 0))
            + data,
        )

    # Wait for a response from the controller to the last command
    # The binary format uses two special characters to frame a
    # command or response.  This function removes those characters
    # before returning.
    # See debug.cpp in the controller source for more detail on
    # command framing.
    def get_response(self):
        data = []
        esc = False
        self.debug_print("Getting response: ", end="")
        while True:
            try:
                x = self.serial_port.read(1)
            except serial.serialutil.SerialException:
                self.serial_port.close()
                self.variable_metadata.clear()
                raise Error(
                    "Could not read response. Serial Exception encountered. Closing port."
                )

            if len(x) < 1:
                self.debug_print("timeout")
                return data
            x = ord(x)
            self.debug_print(f"0x{x:02x}", end=" ")

            if esc:
                esc = False
                data.append(x)
                continue

            if x == debug_proto.Cmd.SpecialChar.Esc:
                esc = True
                continue

            if x == debug_proto.Cmd.SpecialChar.Term:
                self.debug_print()
                return data
            data.append(x)

    # This formats a binary command and sends it to the system.
    # It then waits for and returns a response.
    #
    #  op - The command code to send.  See the list at the top
    #       of the file.

    #  data - Zero or more bytes of data to be sent with the command

    #  timeout - How long (seconds) to wait for the response.  If
    #            not specified then a reasonable system default is used
    def send_command(self, op, data=None, timeout=None):

        if not self.serial_port.isOpen():
            raise Error("Cannot send command. Serial port not open.")

        if data is None:
            data = []
        buff = [op] + data

        buff += debug_types.int16s_to_bytes(debug_types.CRC16().calc(buff))
        buff = debug_types.frame_command(buff)

        debug_string = "Sending command: " + "".join([f"0x{b:02x} " for b in buff])
        self.debug_print(debug_string)

        with self.command_lock:
            try:
                self.serial_port.write(bytearray(buff))
            except serial.serialutil.SerialException:
                self.serial_port.close()
                self.variable_metadata.clear()
                raise Error(
                    "Could not send command. Serial Exception encountered. Closing port."
                )

            if timeout is not None:
                self.debug_print(f"Setting timeout to {timeout:.1f}")
                old_timeout = self.serial_port.timeout
                self.serial_port.timeout = timeout

            response = self.get_response()
            if timeout is not None:
                self.serial_port.timeout = old_timeout

            if len(response) < 3:
                raise Error("Invalid response, too short")

            crc = debug_types.CRC16().calc(response[:-2])
            rcrc = debug_types.bytes_to_int16s(response[-2:])[0]
            if crc != rcrc:
                raise Error(
                    f"CRC error on response, calculated 0x{crc:04x} received 0x{rcrc:04x}"
                )

            if response[0]:
                raise Error(f"Error {response[0]:d} (0x{response[0]:02x})")

            return response[1:-2]
