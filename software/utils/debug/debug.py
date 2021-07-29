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

# Variable types (see vars.h)
VAR_INT32 = 1
VAR_UINT32 = 2
VAR_FLOAT = 3

# Can trace this many variables at once.  Keep this in sync with
# kMaxTraceVars in the controller.
TRACE_VAR_CT = 4


class Error(Exception):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return str(self.value)


class VarInfo:

    # Initialize the variable info from the data returned
    # by the controller.  Set var.cpp in the controller for
    # details on this formatting
    def __init__(self, id, dat):
        self.id = id

        if len(dat) < 8:
            raise Error("Invalid VarInfo data returned")

        self.type = dat[0]
        name_length = dat[4]
        fmt_length = dat[5]
        help_length = dat[6]

        if len(dat) < 8 + name_length + fmt_length + help_length:
            raise Error("Invalid VarInfo data returned")

        n = 8
        self.name = "".join([chr(x) for x in dat[n: n + name_length]])
        n += name_length
        self.fmt = "".join([chr(x) for x in dat[n: n + fmt_length]])
        n += fmt_length
        self.help = "".join([chr(x) for x in dat[n: n + help_length]])

    # Convert an unsigned 32-bit value into the correct type for
    # this variable
    def convert_int(self, d):
        if self.type == VAR_FLOAT:
            return debug_types.i_to_f(d)
        if self.type == VAR_INT32:
            if d & 0x80000000:
                return d - (1 << 32)
            return d
        return d


class ControllerDebugInterface:
    # If true, the raw bytes of the serial data will be printed.
    # This is handy for debugging the low level serial interface
    # It can be toggled with the 'debug' command
    showSerial = False

    ser = serial.Serial()

    varDict = {}

    # This lock is used to make the command interface thread safe.
    # The main debug program doesn't currently use threads, but scripts
    # run from it may and those scripts use these same functions to
    # send commands
    cmdLock = threading.Lock()

    def __init__(self, port):
        self.ser = serial.Serial(port=port, baudrate=115200)
        self.ser.timeout = 0.8

    # Read info about all the supported variables and load
    # them in a map
    def update_variable_info(self):
        self.varDict.clear()
        try:
            for vid in range(256):
                dat = self.send_command(OP_VAR, [SUBCMD_VAR_INFO] + debug_types.int16s_to_bytes(vid))
                V = VarInfo(vid, dat)
                self.varDict[V.name] = V
        except Error as e:
            pass

    def get_variable_metadata(self, vid):
        for name in self.varDict:
            if self.varDict[name].id == vid:
                return self.varDict[name]
        return None

    def get_variable(self, name, raw=False, fmt=None):
        if not (name in self.varDict):
            raise Error("Unknown variable %s" % name)

        V = self.varDict[name]
        dat = self.send_command(OP_VAR, [SUBCMD_VAR_GET] + debug_types.int16s_to_bytes(V.id))

        if V.type == VAR_INT32:
            val = debug_types.bytes_to_int32s(dat, signed=True)[0]

        elif V.type == VAR_UINT32:
            val = debug_types.bytes_to_int32s(dat)[0]

        elif V.type == VAR_FLOAT:
            val = debug_types.bytes_to_float32s(dat)[0]

        else:
            raise Error("Sorry, I don't know how to handle that variable type yet")

        if raw:
            return val

        # If a format wasn't passed, use the default for this var
        if fmt is None:
            fmt = V.fmt

        return fmt % val

    def set_variable(self, name, value):
        if not (name in self.varDict):
            raise Error("Unknown variable %s" % name)

        V = self.varDict[name]

        if V.type == VAR_INT32:
            if not isinstance(value, int):
                value = int(value, 0)
            dat = debug_types.int32s_to_bytes(value)

        elif V.type == VAR_UINT32:
            if not isinstance(value, int):
                value = int(value, 0)
            dat = debug_types.int32s_to_bytes(value)

        elif V.type == VAR_FLOAT:
            dat = debug_types.float32s_to_bytes(float(value))

        else:
            raise Error("Sorry, I don't know how to handle that variable type yet")

        self.send_command(OP_VAR, [SUBCMD_VAR_SET] + debug_types.int16s_to_bytes(V.id) + dat)
        return

    def trace_active_variables_list(self):
        """Return a list of active trace variables"""
        ret = []
        for i in range(TRACE_VAR_CT):
            dat = self.send_command(OP_TRACE, [SUBCMD_TRACE_GET_VARID, i])
            var_id = debug_types.bytes_to_int16s(dat)[0]
            var = self.get_variable_metadata(var_id)
            if var:
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
                ret[i].append(trace_vars[i].ConvertInt(val))

        # get trace period
        dat = self.send_command(OP_TRACE, [SUBCMD_TRACE_GET_PERIOD])
        per = debug_types.bytes_to_int32s(dat)[0]
        if per < 1:
            per = 1

        # Scale this by the loop period which is an integer in microseconds.
        # I multiply by 1e-6 (i.e. 1/1,000,000) to convert it to seconds.
        per *= self.get_variable("loop_period", raw=True) * 1e-6

        time = [x * per for x in range(len(ret[0]))]
        ret.insert(0, time)

        return ret

    def peek(self, address, ct=1, fmt="+XXXX", fname=None, raw=False):
        address = debug_types.decode_address(address)
        if address is None:
            print("Unknown symbol")
            return

        out = []

        A = address

        while ct:
            n = min(ct, 256)
            dat = self.send_command(OP_PEEK, debug_types.int32s_to_bytes(A) + debug_types.int16s_to_bytes(n))
            out += dat
            ct -= len(dat)
            A += len(dat)

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

    def debug_print(self, *args, **kwargs):
        if self.showSerial:
            print(*args, **kwargs)

    # This formats a binary command and sends it to the system.
    # It then waits for and returns a response.
    #
    #  op - The command code to send.  See the list at the top
    #       of the file.

    #  data - Zero or more bytes of data to be sent with the command

    #  timeout - How long (seconds) to wait for the response.  If
    #            not specified then a reasonable system default is used
    def send_command(self, op, data=[], timeout=None):
        buff = [op] + data

        crc = debug_types.CRC16().calc(buff)
        buff += debug_types.int16s_to_bytes(crc)

        S = "CMD: "
        for b in buff:
            S += "0x%02x " % b
        self.debug_print(S)

        buff = debug_types.frame_command(buff)
        S = "ESC: "
        for b in buff:
            S += "0x%02x " % b
        self.debug_print(S)

        with self.cmdLock:
            self.ser.write(bytearray(buff))
            if timeout is not None:
                self.debug_print("Setting timeout to %.1f" % timeout)
                oldto = self.ser.timeout
                self.ser.timeout = timeout

            rsp = self.get_response()
            if timeout is not None:
                self.ser.timeout = oldto

            if len(rsp) < 3:
                raise Error("Invalid response, too short")

            crc = debug_types.CRC16().calc(rsp[:-2])
            rcrc = debug_types.bytes_to_int16s(rsp[-2:])[0]
            if crc != rcrc:
                raise Error(
                    "CRC error on response, calculated 0x%04x received 0x%04x" % (crc, rcrc)
                )

            if rsp[0]:
                raise Error("Error %d (0x%02x)" % (rsp[0], rsp[0]))
            return rsp[1:-2]

    def resynchronize(self):
        with self.cmdLock:
            cmd = [debug_types.TERM, debug_types.TERM]
            self.ser.write(bytearray(cmd))
            time.sleep(0.1)
            self.ser.reset_input_buffer()

    def trace_flush(self):
        self.send_command(OP_TRACE, [SUBCMD_TRACE_FLUSH])

    def trace_period_set(self, period):
        self.send_command(OP_TRACE, [SUBCMD_TRACE_SET_PERIOD] + debug_types.int32s_to_bytes(period))

    def trace_period_get(self):
        dat = self.send_command(OP_TRACE, [SUBCMD_TRACE_GET_PERIOD])
        return debug_types.bytes_to_int32s(dat)[0]

    def trace_select(self, var_names):
        if len(var_names) > TRACE_VAR_CT:
            raise Error(f"Can't trace more than {TRACE_VAR_CT} variables at once.")
        var_names += [""] * (TRACE_VAR_CT - len(var_names))
        for (i, var_name) in enumerate(var_names):
            var_id = -1
            if var_name in self.varDict:
                var_id = self.varDict[var_name].id
            var = debug_types.int16s_to_bytes(var_id)
            self.send_command(OP_TRACE, [SUBCMD_TRACE_SET_VARID, i] + var)
        self.send_command(OP_TRACE, [SUBCMD_TRACE_FLUSH])

    def trace_start(self):
        self.send_command(OP_TRACE, [SUBCMD_TRACE_START])

    def trace_status(self):
        dat = self.send_command(OP_TRACE, [SUBCMD_TRACE_GET_NUM_SAMPLES])
        return debug_types.bytes_to_int32s(dat)[0]

    def eeprom_read(self, address, length):
        dat = self.send_command(OP_EEPROM, [SUBCMD_EEPROM_READ]
                                + debug_types.int16s_to_bytes(int(address, 0))
                                + debug_types.int16s_to_bytes(int(length, 0)))
        return debug_types.format_peek(dat, "+XXXX", int(address, 0))

    def eeprom_write(self, address, data):
        self.send_command(OP_EEPROM, [SUBCMD_EEPROM_WRITE] + debug_types.int16s_to_bytes(int(address, 0)) + data)

    def variable_list(self):
        ret = "Variables currently defined:\n"
        for k in self.varDict.keys():
            ret += "   %-10s - %s" % (k, self.varDict[k].print_help) + "\n"
        return ret

    def variables_starting_with(self, text):
        out = []
        for i in self.varDict.keys():
            if i.startswith(text):
                out.append(i)
        return out

    def mode_get(self):
        return self.send_command(OP_MODE)[0]
