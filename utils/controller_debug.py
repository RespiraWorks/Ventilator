#!/usr/bin/python3

# Very simple command line interface for development
#
# At the moment this program is just a command line utility.  You run
# it and get a command prompt from which you can enter commands to
# observe and control the system.
#
# For a list of available commands, enter 'help'

import sys
import math
import cmd
import glob
import os
import serial
import struct
import time
import traceback

# Command codes.  See debug.h in the controller debug library
OP_MODE = 0x00
OP_PEEK = 0x01
OP_POKE = 0x02
OP_PBREAD = 0x03
OP_VAR = 0x04

# Special characters used to frame commands
ESC = 0xF1
TERM = 0xF2

# Variable types (see vars.h)
VAR_INT32 = 1
VAR_FLOAT = 2

port = "/dev/ttyACM0"
if len(sys.argv) > 1:
    port = sys.argv[1]

ser = serial.Serial(port=port, baudrate=115200)
ser.timeout = 0.8

# If true, the raw bytes of the serial data will be printed.
# This is handy for debugging the low level serial interface
# It can be toggled with the 'debug' command
showSerial = False


class Error(Exception):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return self.value


class CmdLine(cmd.Cmd):
    def __init__(self):
        cmd.Cmd.__init__(self)
        self.UpdatePrompt()
        ReSync()
        self.GetVarInfo()
        self.CmdLoop()

    def UpdatePrompt(self, mode=None):
        if mode == None:
            mode = SendCmd(OP_MODE)[0]
        if mode == 1:
            self.prompt = "BOOT] "
        else:
            self.prompt = "] "

    def CmdLoop(self):
        while True:

            try:
                return cmd.Cmd.cmdloop(self)

            except Error as e:
                print(e)

            except:
                traceback.print_exc()
                # print sys.exc_info()
                pass

    def emptyline(self):
        ReSync()
        self.UpdatePrompt()
        self.GetVarInfo()

    def do_debug(self, line):
        """Toggles display of low level serial data on/off"""
        global showSerial
        showSerial = not showSerial

    def do_run(self, line):
        "Run a Python script located in the scripts directory"
        p = str.split(line)
        if os.path.exists(p[0]):
            fname = p[0]
        elif os.path.exists("scripts/" + p[0]):
            fname = "scripts/" + p[0]
        else:
            print("Unknown file " + p[0])
            return
        gbl = globals().copy()
        gbl["cmdline"] = line
        gbl["parser"] = self
        execfile(fname, gbl)

    def complete_run(self, text, line, begidx, endidx):
        return glob.glob(text + "*.py") + [
            x[8:] for x in glob.glob("scripts/" + text + "*.py")
        ]

    def do_exec(self, line):
        exec(line)

    def do_peek(self, line):
        """
Peek at a memory location.

ex: peek <addr> <ct> <fmt> <file>

   addr - the starting address
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
        param = str.split(line)
        addr = 0
        ct = 1
        fmt = "+XXXX"
        fname = None
        if len(param) > 3:
            fname = param[3]
        if len(param) > 2:
            fmt = param[2]
        if len(param) > 1:
            ct = int(param[1], 0)
        if len(param) > 0:
            addr = param[0]
        Peek(addr, ct, fmt, fname)

    def do_poke(self, line):
        """
Write data to a memory address

ex: poke [type] <addr> <data>

   type - Optional type, can be byte, short, long or float
          determines how the data will be interpreted.

   addr - Address at which to write data

   data - One or more data items to write.
"""
        param = str.split(line)
        if len(param) < 2:
            return
        ldat = False

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
        """
Switch from command mode to a simple console display which
continuously readings debug print statements from the controller
and displays the data received to the screen.

Enter <ctrl>C to exit this mode

A couple optional parameters can be passed on the command line:

--flush     If given, the console will be flushed before we start
            to display data.  That way any old (possibly incomplete
            data) will be removed before the display starts

<filename>  If a file name is given, the data received will be
            written to the file as well as displayed.
"""

        cl = line.split()

        # Optionally flush the buffer before we start displaying
        if "--flush" in cl:
            while len(SendCmd(OP_PBREAD)) > 0:
                pass
            cl.remove("--flush")

        if len(cl) > 0:
            fp = open(cl[0], "w")
        else:
            fp = None

        try:
            while True:
                dat = SendCmd(OP_PBREAD)
                if len(dat) < 1:
                    time.sleep(0.05)
                    continue

                S = "".join([chr(x) for x in dat])

                sys.stdout.write(S)
                sys.stdout.flush()

                if fp != None:
                    fp.write(S)

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

        print(GetVar(cl[0]))

    def help_get(self):
        global varDict
        print("Read the value of a debug variable and display it\n")
        print("Variables currently defined:")
        for k in varDict.keys():
            print("   %-10s - %s" % (k, varDict[k].help))

    def do_set(self, line):
        cl = line.split()
        if len(cl) < 2:
            print("Please give the variable name and value")
            return
        SetVar(cl[0], cl[1])

    def help_set(self):
        global varDict
        print("Set the value of a debug variable\n")
        print("Variables currently defined:")
        for k in varDict.keys():
            print("   %-10s - %s" % (k, varDict[k].help))

    # Read info about all the supported variables and load
    # them in a map
    def GetVarInfo(self):
        global varDict
        varDict = {}

        try:
            for vid in range(256):
                dat = SendCmd(OP_VAR, [0] + Split16(vid))
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
        nLen = dat[4]
        fLen = dat[5]
        hLen = dat[6]

        if len(dat) < 8 + nLen + fLen + hLen:
            raise Error("Invalid VarInfo data returned")

        n = 8
        self.name = "".join([chr(x) for x in dat[n : n + nLen]])
        n += nLen
        self.fmt = "".join([chr(x) for x in dat[n : n + fLen]])
        n += fLen
        self.help = "".join([chr(x) for x in dat[n : n + hLen]])


def GetVar(name, raw=False):
    global varDict
    if not name in varDict:
        raise Error("Unknown variable %s" % name)

    V = varDict[name]
    dat = SendCmd(OP_VAR, [1] + Split16(V.id))

    if V.type == VAR_INT32:
        val = Build32(dat)[0]

    elif V.type == VAR_FLOAT:
        val = BuildFlt(dat)[0]

    else:
        raise Error("Sorry, I don't know how to handle that variable type yet")

    if raw:
        return val

    return V.fmt % val


def SetVar(name, value):
    global varDict
    if not name in varDict:
        raise Error("Unknown variable %s" % name)

    V = varDict[name]

    if V.type == VAR_INT32:
        dat = Split32(int(value, 0))

    elif V.type == VAR_FLOAT:
        dat = SplitFlt(float(value))

    SendCmd(OP_VAR, [2] + Split16(V.id) + dat)
    return


def FmtPeek(dat, fmt="+XXXX", addr=0):

    fmtInfo = {
        "+": ("0x%08x: ", 0, "addr"),
        "x": ("0x%04x ", 2, "GrabU16(dat)"),
        "i": ("%5d ", 2, "GrabI16(dat)"),
        "u": ("%5u ", 2, "GrabU16(dat)"),
        "X": ("0x%08x ", 4, "GrabU32(dat)"),
        "I": ("%9d ", 4, "GrabI32(dat)"),
        "U": ("%9u ", 4, "GrabU32(dat)"),
        "n": ("\n", 0, "None"),
        "f": ("%8.4f ", 4, "GrabFlt(dat)"),
        "e": ("%12.4e ", 4, "GrabFlt(dat)"),
        "c": ("%s", 1, "chr(GrabU8(dat))"),
        "b": ("0x%02x ", 1, "GrabU8(dat)"),
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
            val = eval(fmtInfo[ch][2])

            if val == None:
                ret += fmtInfo[ch][0]
            else:
                ret += fmtInfo[ch][0] % val
            addr += fmtInfo[ch][1]
        else:
            ret += ch

    return ret


# elfFiles = {}
# def loadElf( fname ):
#   global elfFiles
#
#   ct = os.stat( fname ).st_ctime
#
#   if( not fname in elfFiles ):
#      pass
#   elif( elfFiles[fname]['ctime'] != ct ):
#      pass
#   else:
#      return elfFiles[fname]['elf']
#
#   e = {}
#   e['ctime'] = ct
#   e['elf'] = elffile.ElfFile( fname )
#   elfFiles[fname] = e
#   return e['elf']


def DecodeAddr(addr, fw=None):

    #   global fwname
    #   if( fw == None ):
    #      fw = fwname
    #
    if isinstance(addr, int):
        return addr

    #   if( addr[0] == '&' ):
    #      elf = loadElf( fw )
    #
    #      a = elf.getSymbol( addr[1:] )
    #      if( a != None ):
    #         a = a.value;
    #      return a
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
    return Build16(out, le=True, signed=False)


def Peek32(addr, ct=None, le=True, signed=False):
    addr = DecodeAddr(addr)
    if addr == None:
        print("Unknown symbol")
        return

    if ct == None:
        ct = 1
    out = Peek(addr, 4 * ct, raw=True)
    return Build32(out, le=True, signed=False)


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


def EscCmd(buff):
    ret = []
    for i in buff:
        if i == ESC or i == TERM:
            ret.append(ESC)
        ret.append(i)
    ret.append(TERM)
    return ret


def GetResp(DbgPrint):
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
def SendCmd(op, data=[], timeout=None):
    global showSerial, ser

    if showSerial:
        DbgPrint = print
    else:

        def DbgPrint(S="", end="\n"):
            pass

    buff = [op] + data

    crc = crc16.calc(buff)
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

    ser.write(bytearray(buff))
    if timeout != None:
        DbgPrint("Setting timeout to %.1f" % timeout)
        oldto = ser.timeout
        ser.timeout = timeout

    rsp = GetResp(DbgPrint)
    if timeout != None:
        ser.timeout = oldto

    if len(rsp) < 3:
        raise Error("Invalid response, too short")

    crc = crc16.calc(rsp[:-2])
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


def GrabI24(dat, le=True):
    return MakeInt(GrabElems(dat, 3), signed=True, le=le)


def GrabU24(dat, le=True):
    return MakeInt(GrabElems(dat, 3), signed=False, le=le)


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


# Simple utility class for calculating CRC values.
class CRCutil:
    def __init__(self, poly=0xEDB88320, init=0xFFFFFFFF):
        self.init = init
        self.InitTable(poly)

    def calc(self, dat, init=0):
        crc = init ^ self.init
        for d in dat:
            a = 0x00FF & (d ^ crc)
            crc = self.tbl[a] ^ (crc >> 8)
        return crc ^ self.init

    def InitTable(self, poly):
        self.tbl = []

        for i in range(256):
            crc = i

            for j in range(8):
                lsbSet = crc & 1
                crc >>= 1
                if lsbSet:
                    crc ^= poly
            self.tbl.append(crc)


# Standard 16-bit CRC calculations
crc16 = CRCutil(poly=0xA001, init=0)


def CRC16_Calc(dat):
    global crc16
    return crc16.calc(dat)


CmdLine()
