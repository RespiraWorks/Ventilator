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
import controller_types

# TODO: Import the proto instead!

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
            return controller_types.I2F(d)
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
    def GetVarInfo(self):
        self.varDict.clear()
        try:
            for vid in range(256):
                dat = self.SendCmd(OP_VAR, [SUBCMD_VAR_INFO] + controller_types.Split16(vid))
                V = VarInfo(vid, dat)
                self.varDict[V.name] = V
        except Error as e:
            pass

    def FindVarByID(self, vid):
        for name in self.varDict:
            if self.varDict[name].id == vid:
                return self.varDict[name]
        return None

    def GetVar(self, name, raw=False, fmt=None):
        if not name in self.varDict:
            raise Error("Unknown variable %s" % name)

        V = self.varDict[name]
        dat = self.SendCmd(OP_VAR, [SUBCMD_VAR_GET] + controller_types.Split16(V.id))

        if V.type == VAR_INT32:
            val = controller_types.Build32(dat, signed=True)[0]

        elif V.type == VAR_UINT32:
            val = controller_types.Build32(dat)[0]

        elif V.type == VAR_FLOAT:
            val = controller_types.BuildFlt(dat)[0]

        else:
            raise Error("Sorry, I don't know how to handle that variable type yet")

        if raw:
            return val

        # If a format wasn't passed, use the default for this var
        if fmt == None:
            fmt = V.fmt

        return fmt % val

    def SetVar(self, name, value):
        if not name in self.varDict:
            raise Error("Unknown variable %s" % name)

        V = self.varDict[name]

        if V.type == VAR_INT32:
            if not isinstance(value, int):
                value = int(value, 0)
            dat = controller_types.Split32(value)

        elif V.type == VAR_UINT32:
            if not isinstance(value, int):
                value = int(value, 0)
            dat = controller_types.Split32(value)

        elif V.type == VAR_FLOAT:
            dat = controller_types.SplitFlt(float(value))

        self.SendCmd(OP_VAR, [SUBCMD_VAR_SET] + controller_types.Split16(V.id) + dat)
        return

    def TraceActiveVars(self):
        """Return a list of active trace variables"""
        ret = []
        for i in range(TRACE_VAR_CT):
            dat = self.SendCmd(OP_TRACE, [SUBCMD_TRACE_GET_VARID, i])
            var_id = controller_types.Build16(dat)[0]
            var = self.FindVarByID(var_id)
            if var:
                ret.append(var)
        return ret

    def TraceDownload(self):
        """Fetches a trace from the controller.

        Returns a list of N+1 lists where N is the number of active trace variables.
        The first list gives the time in seconds of each sample relative to the
        start of the trace, and the remaining N lists each holds the trace data
        for one variable.
        """
        traceVars = self.TraceActiveVars()
        if len(traceVars) < 1:
            return None

        # get samples count
        dat = self.SendCmd(OP_TRACE, [SUBCMD_TRACE_GET_NUM_SAMPLES])
        ct = controller_types.Build32(dat)[0] * len(traceVars)

        data = []
        while len(data) < 4 * ct:
            dat = self.SendCmd(OP_TRACE, [SUBCMD_TRACE_GETDATA])
            if len(dat) < 1:
                break
            data += dat

        # Convert the bytes into an array of unsigned 32-bit values
        data = controller_types.Build32(data)

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

        # get trace period
        dat = self.SendCmd(OP_TRACE, [SUBCMD_TRACE_GET_PERIOD])
        per = controller_types.Build32(dat)[0]
        if per < 1:
            per = 1

        # Scale this by the loop period which is an integer in microseconds.
        # I multiply by 1e-6 (i.e. 1/1,000,000) to convert it to seconds.
        per *= self.GetVar("loop_period", raw=True) * 1e-6

        time = [x * per for x in range(len(ret[0]))]
        ret.insert(0, time)

        return ret


    def Peek(self, addr, ct=1, fmt="+XXXX", fname=None, raw=False):
        addr = controller_types.DecodeAddr(addr)
        if addr == None:
            print("Unknown symbol")
            return

        out = []

        A = addr

        while ct:
            n = min(ct, 256)
            dat = self.SendCmd(OP_PEEK, controller_types.Split32(A) + controller_types.Split16(n))
            out += dat
            ct -= len(dat)
            A += len(dat)

        if raw:
            return out

        s = controller_types.FmtPeek(out, fmt, addr)
        if fname == None:
            print(s)
        else:
            fp = open(fname, "w")
            fp.write(s)
            fp.write("\n")
            fp.close()

    def Peek16(self, addr, ct=None, le=True, signed=False):
        addr = controller_types.DecodeAddr(addr)
        if addr == None:
            print("Unknown symbol")
            return

        if ct == None:
            ct = 1
        out = self.Peek(addr, 2 * ct, raw=True)
        return controller_types.Build16(out, le, signed)

    def Peek32(self, addr, ct=None, le=True, signed=False):
        addr = controller_types.DecodeAddr(addr)
        if addr == None:
            print("Unknown symbol")
            return

        if ct == None:
            ct = 1
        out = self.Peek(addr, 4 * ct, raw=True)
        return controller_types.Build32(out, le, signed)


    def Poke(self, addr, dat, ptype):
        addr = controller_types.DecodeAddr(addr)
        if addr == None:
            print("Unknown symbol")
            return
        if isinstance(dat, int):
            dat = [dat]

        if ptype == "long":
            dat = controller_types.Split32(dat)
        if ptype == "short":
            dat = controller_types.Split16(dat)
        if ptype == "float":
            dat = [controller_types.F2I(x) for x in dat]
            dat = controller_types.Split32(dat)

        self.SendCmd(OP_POKE, controller_types.Split32(addr) + dat)

    def Poke32(self, addr, dat):
        self.Poke(addr, dat, "long")

    def Poke16(self, addr, dat):
        self.Poke(addr, dat, "short")

    # Wait for a response from the controller to the last command
    # The binary format uses two special characters to frame a
    # command or response.  This function removes those characters
    # before returning.
    # See debug.cpp in the controller source for more detail on
    # command framing.
    def GetResp(self):
        dat = []
        esc = False
        self.DbgPrint("Getting resp: ", end="")
        while True:
            x = self.ser.read(1)
            if len(x) < 1:
                self.DbgPrint("timeout")
                return dat
            x = ord(x)
            self.DbgPrint("0x%02x" % x, end=" ")

            if esc:
                esc = False
                dat.append(x)
                continue

            if x == controller_types.ESC:
                esc = True
                continue

            if x == controller_types.TERM:
                self.DbgPrint()
                return dat
            dat.append(x)


    def DbgPrint(self, *args, **kwargs):
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
    def SendCmd(self, op, data=[], timeout=None):
        buff = [op] + data

        crc = controller_types.CRC16().calc(buff)
        buff += controller_types.Split16(crc)

        S = "CMD: "
        for b in buff:
            S += "0x%02x " % b
        self.DbgPrint(S)

        buff = controller_types.EscCmd(buff)
        S = "ESC: "
        for b in buff:
            S += "0x%02x " % b
        self.DbgPrint(S)

        with self.cmdLock:
            self.ser.write(bytearray(buff))
            if timeout != None:
                self.DbgPrint("Setting timeout to %.1f" % timeout)
                oldto = self.ser.timeout
                self.ser.timeout = timeout

            rsp = self.GetResp()
            if timeout != None:
                self.ser.timeout = oldto

            if len(rsp) < 3:
                raise Error("Invalid response, too short")

            crc = controller_types.CRC16().calc(rsp[:-2])
            rcrc = controller_types.Build16(rsp[-2:])[0]
            if crc != rcrc:
                raise Error(
                    "CRC error on response, calculated 0x%04x received 0x%04x" % (crc, rcrc)
                )

            if rsp[0]:
                raise Error("Error %d (0x%02x)" % (rsp[0], rsp[0]))
                return []
            return rsp[1:-2]

    def ReSync(self):
        with self.cmdLock:
            cmd = [controller_types.TERM, controller_types.TERM]
            self.ser.write(bytearray(cmd))
            time.sleep(0.1)
            self.ser.reset_input_buffer()
