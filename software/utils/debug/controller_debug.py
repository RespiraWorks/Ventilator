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
import var_info
import fnmatch
from lib.error import Error
import test_scenario
from test_data import *
from pathlib import Path
from lib.colors import red, orange

# TODO: Import constants from proto instead!

# Command codes.  See debug_types.h in the controller debug library
OP_MODE = 0x00
OP_PEEK = 0x01
OP_POKE = 0x02
OP_PBREAD = 0x03
OP_VAR = 0x04
OP_TRACE = 0x05
OP_EEPROM = 0x06

# Some commands take a sub-command as their first byte of data
SUBCMD_VAR_INFO = 0x00
SUBCMD_VAR_GET = 0x01
SUBCMD_VAR_SET = 0x02
SUBCMD_VAR_GET_COUNT = 0x03

SUBCMD_TRACE_FLUSH = 0x00
SUBCMD_TRACE_GETDATA = 0x01
SUBCMD_TRACE_START = 0x02
SUBCMD_TRACE_STOP = 0x03
SUBCMD_TRACE_GET_VARID = 0x04
SUBCMD_TRACE_SET_VARID = 0x05
SUBCMD_TRACE_GET_PERIOD = 0x06
SUBCMD_TRACE_SET_PERIOD = 0x07
SUBCMD_TRACE_GET_NUM_SAMPLES = 0x08

SUBCMD_EEPROM_READ = 0x00
SUBCMD_EEPROM_WRITE = 0x01

# Can trace this many variables at once.  Keep this in sync with
# kMaxTraceVars in the controller.
TRACE_VAR_CT = 4

MODE_NORMAL = 0
MODE_BOOT = 1

ERROR_NONE = 0
ERROR_CODES = [
    "None",
    "CRC error on command",
    "Unknown command code received",
    "Not enough data passed with command",
    "Insufficient memory",
    "Some type of internal error (aka bug)",
    "The requested variable ID is invalid",
    "Invalid data in request",
    "Response timeout",
]


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
        return self.send_command(OP_MODE)[0]

    def resynchronize(self):
        with self.command_lock:
            try:
                cmd = [debug_types.TERM, debug_types.TERM]
                self.serial_port.write(bytearray(cmd))
                time.sleep(0.1)
                self.serial_port.reset_input_buffer()
            except serial.serialutil.SerialException:
                self.serial_port.close()
                self.variable_metadata.clear()
                raise Error(
                    "Could not resynchronize. Serial Exception encountered. Closing port."
                )

    def sanity_checks(self):
        # TODO: make sure git is configured and we have identity for tester
        # we do this just for the git info
        test = TestData(test_scenario.TestScenario())

        ctrl_version = self.variable_get("0_controller_version")
        ctrl_dirty = bool(self.variable_get("0_controller_git_dirty", raw=True))
        ctrl_branch = self.variable_get("0_controller_branch")
        version_mismatch = test.git_version != ctrl_version
        branch_mismatch = test.git_branch != ctrl_branch
        problem = version_mismatch or branch_mismatch or test.git_dirty or ctrl_dirty

        if not problem:
            return True

        print(
            red(
                "---------------================= WARNING =================---------------"
            )
        )
        print(
            red(
                " Potential controller version incompatibility or uncertain provenance "
            )
        )
        if version_mismatch:
            print(
                red(
                    f"    * version mismatch: ctrl=({ctrl_version}) != dbg=({test.git_version})"
                )
            )
        if branch_mismatch:
            print(
                red(
                    f"    * branch mismatch: ctrl=({ctrl_branch}) != dbg=({test.git_branch})"
                )
            )
        if ctrl_dirty:
            print(red(f"    * controller built from dirty (uncommitted) code"))
        if test.git_dirty:
            print(red(f"    * debugger running with dirty (uncommitted) code"))
        print(
            red(
                "---------------================= WARNING =================---------------"
            )
        )
        print(
            "The above might be a sign of problems. Test results may not be meaningful or"
            " reproducible."
        )

        value = input("Are you sure you want to continue (y/Y)? ")
        return value == "y" or value == "Y"

    # Read info about all the supported variables and load
    # them in a map
    def variables_update_info(self):
        self.variable_metadata.clear()
        data = self.send_command(OP_VAR, [SUBCMD_VAR_GET_COUNT])
        var_count = debug_types.bytes_to_int32s(data)[0]
        for vid in range(var_count):
            data = self.send_command(
                OP_VAR, [SUBCMD_VAR_INFO] + debug_types.int16s_to_bytes(vid)
            )
            if data is None:
                raise Error(f"bad variable info retrieved for vid={vid}")
            variable = var_info.VarInfo(vid, data)
            if variable.name in self.variable_metadata.keys():
                raise Error(
                    f"variable name clash  \n"
                    f" retrieved: {variable.verbose()}\n"
                    f" existing:  {self.variable_metadata[variable.name].verbose()}"
                )
            self.variable_metadata[variable.name] = variable

    def variables_find(self, pattern="", access_filter=None):
        out = []
        for name, metadata in self.variable_metadata.items():
            if access_filter is not None and metadata.write_access != access_filter:
                continue
            if len(pattern) and not fnmatch.fnmatchcase(name, pattern):
                continue
            out.append(name)
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
        # forced_mode because if we unforced it while we were still in mode 0
        # (i.e. "ventilator off"), the fan would momentarily spin down.
        self.variable_set("forced_exhale_valve_pos", -1)
        self.variable_set("forced_blower_valve_pos", -1)
        self.variable_set("forced_blower_power", -1)
        # todo unforce o2 psol?

    def variables_set(self, pairs):
        for var, val in pairs.items():
            self.variable_set(var, val, verbose=True)

    def variables_get(self, names, raw=False):
        ret = {}
        for name in names:
            ret[name] = self.variable_get(name, raw=raw)
        return ret

    def variable_get(self, name, raw=False, fmt=None):
        if not (name in self.variable_metadata):
            raise Error(f"Cannot get unknown variable {name}")

        variable = self.variable_metadata[name]
        data = self.send_command(
            OP_VAR, [SUBCMD_VAR_GET] + debug_types.int16s_to_bytes(variable.id)
        )
        return variable.format_value(variable.from_bytes(data), raw, fmt)

    def variable_set(self, name, value, verbose=False):
        if not (name in self.variable_metadata):
            raise Error(f"Cannot set unknown variable {name}")

        variable = self.variable_metadata[name]
        # \TODO this will not work for FloatArray
        if verbose:
            text = variable.print_value(value, show_access=False)
            print(f"  applying {text}")

        data = variable.to_bytes(value)
        if self.print_raw:
            print(f"  data converted as {data}")

        self.send_command(
            OP_VAR, [SUBCMD_VAR_SET] + debug_types.int16s_to_bytes(variable.id) + data
        )

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

        test = TestData(self.scenarios[name])

        if not self.sanity_checks():
            return

        self.variables_force_open()
        self.variable_set("forced_mode", "off")
        print(f"\nExecuting test scenario:\n {test.scenario.long_description(True)}")

        # Give the user a chance to adjust the test lung.
        if len(test.scenario.manual_settings):
            input(
                orange(
                    "\nAdjust manual settings per above, then press enter to start test.\n"
                )
            )

        # Apply all vent settings
        print("\n")
        self.variables_set(test.scenario.ventilator_settings)
        self.variables_force_off()
        self.trace_set_period(test.scenario.trace_period)
        self.trace_select(test.scenario.trace_variable_names)
        time.sleep(test.scenario.capture_ignore_secs)

        print(
            f"\nStarting data capture for {test.scenario.capture_duration_secs} seconds"
        )
        self.trace_start()
        time.sleep(test.scenario.capture_duration_secs)
        self.trace_stop()

        print("\nRetrieving data and halting ventilation")
        # get data and halt ventilation
        self.save_test_data(test)
        self.variable_set("forced_mode", "off")
        return test

    def trace_save(self, scenario_name="manual_trace"):
        test = TestData(test_scenario.TestScenario())

        # get data
        self.save_test_data(test)

        # Label null scenario
        test.scenario.name = scenario_name
        test.scenario.description = (
            "Manually triggered trace -- undefined test scenario"
        )

        # Infer scenario parameters
        time_series = test.traces[0].data
        test.scenario.capture_duration_secs = (
            time_series[len(time_series) - 1] - time_series[0]
        )
        test.scenario.capture_ignore_secs = 0
        test.scenario.trace_period = self.trace_get_period()
        test.scenario.trace_variable_names = [
            x.name for x in self.trace_active_variables_list()
        ]

        return test

    def save_test_data(self, test):
        test.traces = self.trace_download()
        test.ventilator_settings = self.variables_get(
            self.variables_find(access_filter=var_info.VAR_ACCESS_WRITE), raw=True
        )
        test.ventilator_readings = self.variables_get(
            self.variables_find(access_filter=var_info.VAR_ACCESS_READ_ONLY), raw=True
        )

    def peek(self, address, ct=1, fmt="+XXXX", fname=None, raw=False):
        decoded_address = debug_types.decode_address(address)
        if decoded_address is None:
            raise Error(f"Could not successfully decode address: {address}")

        out = []

        address_iterator = decoded_address

        while ct:
            n = min(ct, 256)
            data = self.send_command(
                OP_PEEK,
                debug_types.int32s_to_bytes(address_iterator)
                + debug_types.int16s_to_bytes(n),
            )
            out += data
            ct -= len(data)
            address_iterator += len(data)

        if raw:
            return out

        s = debug_types.format_peek(out, fmt, decoded_address)
        if fname is None:
            print(s)
        else:
            fp = open(fname, "w")
            fp.write(s)
            fp.write("\n")
            fp.close()

    def peek16(self, address, ct=None, le=True, signed=False):
        decoded_address = debug_types.decode_address(address)
        if decoded_address is None:
            raise Error(f"Could not successfully decode address: {address}")

        if ct is None:
            ct = 1
        out = self.peek(decoded_address, 2 * ct, raw=True)
        return debug_types.bytes_to_int16s(out, le, signed)

    def peek32(self, address, ct=None, le=True, signed=False):
        decoded_address = debug_types.decode_address(address)
        if decoded_address is None:
            raise Error(f"Could not successfully decode address: {address}")

        if ct is None:
            ct = 1
        out = self.peek(decoded_address, 4 * ct, raw=True)
        return debug_types.bytes_to_int32s(out, le, signed)

    def poke(self, address, dat, ptype):
        decoded_address = debug_types.decode_address(address)
        if decoded_address is None:
            raise Error(f"Could not successfully decode address: {address}")

        if isinstance(dat, int):
            dat = [dat]

        if ptype == "long":
            dat = debug_types.int32s_to_bytes(dat)
        if ptype == "short":
            dat = debug_types.int16s_to_bytes(dat)
        if ptype == "float":
            dat = [debug_types.f_to_i(x) for x in dat]
            dat = debug_types.int32s_to_bytes(dat)

        self.send_command(OP_POKE, debug_types.int32s_to_bytes(decoded_address) + dat)

    def poke32(self, address, dat):
        self.poke(address, dat, "long")

    def poke16(self, address, dat):
        self.poke(address, dat, "short")

    def trace_flush(self):
        self.send_command(OP_TRACE, [SUBCMD_TRACE_FLUSH])

    def trace_set_period(self, period):
        self.send_command(
            OP_TRACE, [SUBCMD_TRACE_SET_PERIOD] + debug_types.int32s_to_bytes(period)
        )

    def trace_get_period(self):
        dat = self.send_command(OP_TRACE, [SUBCMD_TRACE_GET_PERIOD])
        return debug_types.bytes_to_int32s(dat)[0]

    def trace_get_period_us(self):
        # get trace period as number of loops
        period = self.trace_get_period()
        if period < 1:
            period = 1

        # Scale this by the loop period which is an integer in microseconds.
        return period * self.variable_get("loop_period", raw=True)

    def trace_select(self, var_names):
        if len(var_names) > TRACE_VAR_CT:
            raise Error(f"Can't trace more than {TRACE_VAR_CT} variables at once.")
        for name in var_names:
            if name not in self.variable_metadata.keys():
                raise Error(f"Cannot select trace. Variable `{name}` does not exist.")
        var_names += [""] * (TRACE_VAR_CT - len(var_names))
        for (i, var_name) in enumerate(var_names):
            var_id = var_info.VAR_INVALID_ID
            if var_name in self.variable_metadata:
                var_id = self.variable_metadata[var_name].id
            var = debug_types.int16s_to_bytes(var_id)
            self.send_command(OP_TRACE, [SUBCMD_TRACE_SET_VARID, i] + var)

    def trace_start(self):
        self.send_command(OP_TRACE, [SUBCMD_TRACE_START])

    def trace_stop(self):
        self.send_command(OP_TRACE, [SUBCMD_TRACE_STOP])

    def trace_num_samples(self):
        dat = self.send_command(OP_TRACE, [SUBCMD_TRACE_GET_NUM_SAMPLES])
        return debug_types.bytes_to_int32s(dat)[0]

    def trace_active_variables_list(self):
        """Return a list of active trace variables"""
        ret = []
        for i in range(TRACE_VAR_CT):
            data = self.send_command(OP_TRACE, [SUBCMD_TRACE_GET_VARID, i])
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
        num_samples = self.trace_num_samples()
        total_num_samples = num_samples * var_count
        bytes_per_int32 = 4

        data = []
        while len(data) < bytes_per_int32 * total_num_samples:
            byte = self.send_command(OP_TRACE, [SUBCMD_TRACE_GETDATA])
            if len(byte) < 1:
                break
            data += byte

        # Convert the bytes into an array of unsigned 32-bit values
        data = debug_types.bytes_to_int32s(data)

        # The data comes as a list of of samples, where each sample contains
        # len(trace_vars) uint32s: [a1, b1, c1, a2, b2, c2, ...].  Parse this into
        # sublists [[a1', a2', ...], [b1', b2', ...], [c1', c2', ...]], where each
        # of the variables is converted to the correct type.
        ret = [Trace("time", "s")]
        for v in trace_vars:
            ret.append(Trace(v.name, v.units))

        # The `zip` expression groups data into sublists of var_count elems.  See the
        # "grouper" recipe:
        # https://docs.python.org/3/library/itertools.html#itertools-recipes
        iters = [iter(data)] * var_count
        for sample in zip(*iters):
            for i, val in enumerate(sample):
                ret[i + 1].data.append(trace_vars[i].convert_int(val))

        # this may be higher than the initially reported number because the buffer
        # may have filled with more while retrieving
        actual_number_of_samples = len(ret[1].data)

        # get trace period, multiply by 1e-6 (i.e. 1/1,000,000) to convert it to seconds.
        # todo: record actual clock ticks in controller instead of assuming?
        period = self.trace_get_period_us() * 1e-6
        ret[0].data = [x * period for x in range(actual_number_of_samples)]

        return ret

    def eeprom_read(self, address, length):
        data = self.send_command(
            OP_EEPROM,
            [SUBCMD_EEPROM_READ]
            + debug_types.int16s_to_bytes(int(address, 0))
            + debug_types.int16s_to_bytes(int(length, 0)),
        )
        return debug_types.format_peek(data, "+XXXX", int(address, 0))

    def eeprom_write(self, address, data):
        self.send_command(
            OP_EEPROM,
            [SUBCMD_EEPROM_WRITE] + debug_types.int16s_to_bytes(int(address, 0)) + data,
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

            if x == debug_types.ESC:
                esc = True
                continue

            if x == debug_types.TERM:
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

            if response[0] != ERROR_NONE:
                if response[0] < len(ERROR_CODES):
                    raise Error(
                        f'Error received from controller: "{ERROR_CODES[response[0]]}"'
                    )
                else:
                    raise Error(
                        f"Unknown error received from controller: {response[0]:d} (0x{response[0]:02x})"
                    )

            return response[1:-2]
