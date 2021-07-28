#!/usr/bin/env python3

# Very simple command line interface for development
#
# At the moment this program is just a command line utility.  You run
# it and get a command prompt from which you can enter commands to
# observe and control the system.
#
# For a list of available commands, enter 'help'

import struct

# Special characters used to frame commands
ESC = 0xF1
TERM = 0xF2


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
