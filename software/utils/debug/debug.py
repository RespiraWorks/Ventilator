#!/usr/bin/env python3

# Very simple command line interface for development
#
# At the moment this program is just a command line utility.  You run
# it and get a command prompt from which you can enter commands to
# observe and control the system.
#
# For a list of available commands, enter 'help'

import serial
import threading
import time
import debug_types
import var_info
import error
import test_scenario
from pathlib import Path


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
SUBCMD_VAR_INFO = 0
SUBCMD_VAR_GET = 1
SUBCMD_VAR_SET = 2

SUBCMD_TRACE_FLUSH = 0
SUBCMD_TRACE_GETDATA = 1
SUBCMD_TRACE_START = 2
SUBCMD_TRACE_GET_VARID = 3
SUBCMD_TRACE_SET_VARID = 4
SUBCMD_TRACE_GET_PERIOD = 5
SUBCMD_TRACE_SET_PERIOD = 6
SUBCMD_TRACE_GET_NUM_SAMPLES = 7

SUBCMD_EEPROM_READ = 0
SUBCMD_EEPROM_WRITE = 1

# Can trace this many variables at once.  Keep this in sync with
# kMaxTraceVars in the controller.
TRACE_VAR_CT = 4


class ControllerDebugInterface:
    # If true, the raw bytes of the serial data will be printed.
    # This is handy for debugging the low level serial interface
    # It can be toggled with the 'debug' command
    showSerial = False

    ser = serial.Serial()

    variables = {}

    scenarios = {}

    # This lock is used to make the command interface thread safe.
    # The main debug program doesn't currently use threads, but scripts
    # run from it may and those scripts use these same functions to
    # send commands
    cmdLock = threading.Lock()

    def __init__(self, port):
        self.ser = serial.Serial(port=port, baudrate=115200)
        self.ser.timeout = 0.8

    def connected(self):
        return self.ser.isOpen()

    def debug_print(self, *args, **kwargs):
        if self.showSerial:
            print(*args, **kwargs)

    def mode_get(self):
        return self.send_command(OP_MODE)[0]

    def resynchronize(self):
        with self.cmdLock:
            cmd = [debug_types.TERM, debug_types.TERM]
            self.ser.write(bytearray(cmd))
            time.sleep(0.1)
            self.ser.reset_input_buffer()

    # Read info about all the supported variables and load
    # them in a map
    def variables_update_info(self):
        self.variables.clear()
        try:
            for vid in range(256):
                data = self.send_command(OP_VAR, [SUBCMD_VAR_INFO] + debug_types.int16s_to_bytes(vid))
                variable = var_info.VarInfo(vid, data)
                self.variables[variable.name] = variable
        # todo maybe not wait for an exception to terminate this loop?
        except error.Error as e:
            pass

    def variables_list(self):
        ret = "Variables currently defined:\n"
        for k in self.variables.keys():
            ret += " {:25} - {}\n".format(k, self.variables[k].help)
        return ret

    def variables_starting_with(self, text):
        out = []
        for i in self.variables.keys():
            if i.startswith(text):
                out.append(i)
        return out

    def variable_by_id(self, vid):
        for name in self.variables:
            if self.variables[name].name == vid:
                return self.variables[name]
        return None

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
        for name in self.variables:
            ret[name] = self.variable_get(name)
        return ret

    def variable_get(self, name, raw=False, fmt=None):
        if not (name in self.variables):
            raise error.Error("Unknown variable %s" % name)

        variable = self.variables[name]
        data = self.send_command(OP_VAR, [SUBCMD_VAR_GET] + debug_types.int16s_to_bytes(variable.id))
        value = variable.from_bytes(data)

        if raw:
            return value

        # If a format wasn't passed, use the default for this var
        if fmt is None:
            fmt = variable.fmt

        return fmt % value

    def variable_set(self, name, value):
        if not (name in self.variables):
            raise error.Error("Unknown variable %s" % name)

        variable = self.variables[name]
        data = variable.to_bytes(value)

        self.send_command(OP_VAR, [SUBCMD_VAR_SET] + debug_types.int16s_to_bytes(variable.id) + data)
        return

    def tests_import(self, file_name):
        in_file = Path(file_name)
        if not in_file.is_file():
            raise error.Error(f"Input file does not exist {file_name}")
        elif in_file.suffix == ".csv":
            imported_scenarios = test_scenario.TestScenario.from_csv(in_file, self.variables.keys())
        elif in_file.suffix == ".json":
            imported_scenarios = test_scenario.TestScenario.from_json(in_file)
        else:
            raise error.Error(f"Unknown file format `{in_file.suffix}`")
        if bool(set(imported_scenarios.keys()) & set(self.scenarios.keys())):
            raise error.Error("Cannot import test scenarios, id's clash with already loaded ones")
        print("Imported {} new test scenarios from {}".format(len(imported_scenarios.keys()),
                                                              file_name))
        self.scenarios = {**self.scenarios, **imported_scenarios}

    def tests_list(self, verbose=False):
        for key in self.scenarios:
            if verbose:
                print(self.scenarios[key].long_description())
            else:
                print(self.scenarios[key].short_description())

    def test_apply(self, name):
        if name not in self.scenarios.keys():
            raise error.Error(f"No such test scenario: {name}")
        scenario = self.scenarios[name]
        print(f"Applying test scenario: {scenario.short_description()}")
        self.variables_set(scenario.ventilator_settings)

    def test_run(self, name):
        if name not in self.scenarios.keys():
            raise error.Error(f"No such test scenario: {name}")
        scenario = self.scenarios[name]

        # Ensure the ventilator fan is on, and disconnect the fan from the system
        # so it's not inflating the test lung.  Edwin's request is that the fan
        # doesn't have to spin up during these tests.
        self.variable_set("forced_exhale_valve_pos", 1)
        self.variable_set("forced_blower_valve_pos", 0)
        self.variable_set("forced_blower_power", 1)
        self.variable_set("gui_mode", 0)

        # Give the user a chance to adjust the test lung.
        print(f"\nExecuting test scenario:\n {scenario.long_description(True)}")
        input("\nAdjust manual settings per above, then press enter.\n")

        # Apply all vent settings
        self.variables_set(scenario.ventilator_settings)

        # Unforce parameters we set above so they can be controlled by the
        # controller.  Note that we unforce the blower power *after* setting the
        # gui_mode because if we unforced it while we were still in mode 0
        # (i.e. "ventilator off"), the fan would momentarily spin down.
        self.variable_set("forced_exhale_valve_pos", -1)
        self.variable_set("forced_blower_valve_pos", -1)
        self.variable_set("forced_blower_power", -1)
        time.sleep(scenario.capture_ignore_secs)

        print("\nStarting data capture\n")
        # todo parametrize this
        # self.trace_period_set(1)
        self.trace_select(["pc_setpoint","pressure","volume","net_flow"])

        time.sleep(scenario.capture_duration_secs)

        # self.trace_flush()
        # self.trace_start()

        print("\nFinished data capture\n")
        # data = self.trace_download()

        #time.sleep(scenario.capture_ignore_secs)
        self.variable_set("gui_mode", 0)

        # print("Results:\n{}".format(data))

    def peek(self, address, ct=1, fmt="+XXXX", fname=None, raw=False):
        address = debug_types.decode_address(address)
        if address is None:
            print("Unknown symbol")
            return

        out = []

        address_iterator = address

        while ct:
            n = min(ct, 256)
            data = self.send_command(OP_PEEK, debug_types.int32s_to_bytes(address_iterator)
                                     + debug_types.int16s_to_bytes(n))
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

        self.send_command(OP_POKE, debug_types.int32s_to_bytes(address) + dat)

    def poke32(self, address, dat):
        self.poke(address, dat, "long")

    def poke16(self, address, dat):
        self.poke(address, dat, "short")

    def trace_flush(self):
        self.send_command(OP_TRACE, [SUBCMD_TRACE_FLUSH])

    def trace_period_set(self, period):
        self.send_command(OP_TRACE, [SUBCMD_TRACE_SET_PERIOD] + debug_types.int32s_to_bytes(period))

    def trace_period_get(self):
        dat = self.send_command(OP_TRACE, [SUBCMD_TRACE_GET_PERIOD])
        return debug_types.bytes_to_int32s(dat)[0]

    def trace_select(self, var_names):
        if len(var_names) > TRACE_VAR_CT:
            raise error.Error(f"Can't trace more than {TRACE_VAR_CT} variables at once.")
        var_names += [""] * (TRACE_VAR_CT - len(var_names))
        for (i, var_name) in enumerate(var_names):
            var_id = -1
            if var_name in self.variables:
                var_id = self.variables[var_name].id
            var = debug_types.int16s_to_bytes(var_id)
            self.send_command(OP_TRACE, [SUBCMD_TRACE_SET_VARID, i] + var)

    def trace_start(self):
        self.send_command(OP_TRACE, [SUBCMD_TRACE_START])

    def trace_status(self):
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
            return None

        # get samples count
        dat = self.send_command(OP_TRACE, [SUBCMD_TRACE_GET_NUM_SAMPLES])
        ct = debug_types.bytes_to_int32s(dat)[0] * len(trace_vars)

        data = []
        while len(data) < 4 * ct:
            dat = self.send_command(OP_TRACE, [SUBCMD_TRACE_GETDATA])
            if len(dat) < 1:
                break
            data += dat

        # Convert the bytes into an array of unsigned 32-bit values
        data = debug_types.bytes_to_int32s(data)

        # The data comes as a list of of samples, where each sample contains
        # len(trace_vars) uint32s: [a1, b1, c1, a2, b2, c2, ...].  Parse this into
        # sublists [[a1', a2', ...], [b1', b2', ...], [c1', c2', ...]], where each
        # of the variables is converted to the correct type.
        var_count = len(trace_vars)
        ret = [[] for _ in range(var_count)]

        # The `zip` expression groups data into sublists of var_count elems.  See the
        # "grouper" recipe:
        # https://docs.python.org/3/library/itertools.html#itertools-recipes
        iters = [iter(data)] * var_count
        for sample in zip(*iters):
            for i, val in enumerate(sample):
                ret[i].append(trace_vars[i].convert_int(val))

        # get trace period
        dat = self.send_command(OP_TRACE, [SUBCMD_TRACE_GET_PERIOD])
        per = debug_types.bytes_to_int32s(dat)[0]
        if per < 1:
            per = 1

        # Scale this by the loop period which is an integer in microseconds.
        # I multiply by 1e-6 (i.e. 1/1,000,000) to convert it to seconds.
        per *= self.variable_get("loop_period", raw=True) * 1e-6

        timestamp = [x * per for x in range(len(ret[0]))]
        ret.insert(0, timestamp)

        return ret

    def eeprom_read(self, address, length):
        dat = self.send_command(OP_EEPROM, [SUBCMD_EEPROM_READ]
                                + debug_types.int16s_to_bytes(int(address, 0))
                                + debug_types.int16s_to_bytes(int(length, 0)))
        return debug_types.format_peek(dat, "+XXXX", int(address, 0))

    def eeprom_write(self, address, data):
        self.send_command(OP_EEPROM, [SUBCMD_EEPROM_WRITE] + debug_types.int16s_to_bytes(int(address, 0)) + data)

    # Wait for a response from the controller to the last command
    # The binary format uses two special characters to frame a
    # command or response.  This function removes those characters
    # before returning.
    # See debug.cpp in the controller source for more detail on
    # command framing.
    def get_response(self):
        dat = []
        esc = False
        self.debug_print("Getting resp: ", end="")
        while True:
            x = self.ser.read(1)
            if len(x) < 1:
                self.debug_print("timeout")
                return dat
            x = ord(x)
            self.debug_print("0x%02x" % x, end=" ")

            if esc:
                esc = False
                dat.append(x)
                continue

            if x == debug_types.ESC:
                esc = True
                continue

            if x == debug_types.TERM:
                self.debug_print()
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
    def send_command(self, op, data=None, timeout=None):
        if data is None:
            data = []
        buff = [op] + data

        crc = debug_types.CRC16().calc(buff)
        buff += debug_types.int16s_to_bytes(crc)

        debug_string = "CMD: "
        for b in buff:
            debug_string += "0x%02x " % b
        self.debug_print(debug_string)

        buff = debug_types.frame_command(buff)
        debug_string = "ESC: "
        for b in buff:
            debug_string += "0x%02x " % b
        self.debug_print(debug_string)

        if not self.ser.isOpen():
            return

        with self.cmdLock:
            self.ser.write(bytearray(buff))
            if timeout is not None:
                self.debug_print("Setting timeout to %.1f" % timeout)
                old_timeout = self.ser.timeout
                self.ser.timeout = timeout

            rsp = self.get_response()
            if timeout is not None:
                self.ser.timeout = old_timeout

            if len(rsp) < 3:
                raise error.Error("Invalid response, too short")

            crc = debug_types.CRC16().calc(rsp[:-2])
            rcrc = debug_types.bytes_to_int16s(rsp[-2:])[0]
            if crc != rcrc:
                raise error.Error(
                    "CRC error on response, calculated 0x%04x received 0x%04x" % (crc, rcrc)
                )

            if rsp[0]:
                raise error.Error("Error %d (0x%02x)" % (rsp[0], rsp[0]))
            return rsp[1:-2]
