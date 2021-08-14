# Data type manipulation functions for ventilator debug interface

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

import struct
import debug_protocol_pb2 as debug_proto


def format_peek(data, fmt="+XXXX", address=0):

    peek_format_info = {
        "+": ("0x%08x: ", 0, lambda dat: address),
        "x": ("0x%04x ", 2, pop_uint16),
        "i": ("%5d ", 2, pop_int16),
        "u": ("%5u ", 2, pop_uint16),
        "X": ("0x%08x ", 4, pop_uint32),
        "I": ("%9d ", 4, pop_int32),
        "U": ("%9u ", 4, pop_uint32),
        "n": ("\n", 0, lambda dat: None),
        "f": ("%8.4f ", 4, pop_float),
        "e": ("%12.4e ", 4, pop_float),
        "c": ("%s", 1, lambda dat: chr(pop_uint8(dat))),
        "b": ("0x%02x ", 1, pop_uint8),
    }

    ret = ""
    ndx = -1
    while len(data) > 0:
        ndx += 1
        if ndx == len(fmt):
            ret += "\n"
            ndx = 0

        ch = fmt[ndx]

        if ch in peek_format_info:
            (fmtString, byteCt, func) = peek_format_info[ch]
            val = func(data)

            if val is None:
                ret += fmtString
            else:
                ret += fmtString % val
            address += byteCt
        else:
            ret += ch

    return ret


def decode_address(address, fw=None):

    if isinstance(address, int):
        return address

    return int(address, 0)


# This adds the escape characters to the serial data stream
# to implement the framing used by that format.
# See debug.cpp in the controller code for details
def frame_command(buff):
    ret = []
    for i in buff:
        if (
            i == debug_proto.Cmd.SpecialChar.Esc
            or i == debug_proto.Cmd.SpecialChar.Term
        ):
            ret.append(debug_proto.Cmd.SpecialChar.Esc)
        ret.append(i)
    ret.append(debug_proto.Cmd.SpecialChar.Term)
    return ret


def i_to_f(ival):
    s = struct.pack("I", ival & 0xFFFFFFFF)
    return struct.unpack("f", s)[0]


def f_to_i(fval):
    s = struct.pack("f", fval)
    return struct.unpack("I", s)[0]


# Takes as input a list of integer values and
# splits them into a list of bytes which is returned.
#
# int_list - The input list of ints
# size     - Byte size of input int type
# le  -      Little endian format if true
def ints_to_bytes(int_list, size=4, le=True):
    ret = []

    if isinstance(int_list, list):
        for v in int_list:
            ret += ints_to_bytes(v, size, le)
    else:
        for i in range(size):
            ret.append(int(int_list & 0x00FF))
            int_list >>= 8

        if not le:
            ret = ret[::-1]
    return ret


# Splits a list of 16-bit ints into a list of bytes
def int16s_to_bytes(x, le=True):
    if isinstance(x, int):
        x = [x]
    return ints_to_bytes(x, 2, le)


# Splits a list of 32-bit ints into a list of bytes
def int32s_to_bytes(x, le=True):
    if isinstance(x, int):
        x = [x]
    return ints_to_bytes(x, 4, le)


# Splits a list of 32-bit floats into a list of bytes
def float32s_to_bytes(x):
    if isinstance(x, float):
        x = [x]
    return int32s_to_bytes([f_to_i(i) for i in x])


# Utility function to convert bytes to ints
#   byte_set - Array of byte values to convert
#              All input bytes will be combined into one integer
#   signed - If true, the output will be signed
#   le - If true, use little endian format
def bytes_to_ints(byte_set, signed=True, le=True):
    if not le:
        byte_set = byte_set[::-1]
    val = 0
    shift = 0
    for b in byte_set:
        val |= b << shift
        shift += 8

    if signed and byte_set[-1] & 0x80:
        return val - (1 << shift)
    return val


# Convert a list of bytes into a list of 32-bit integers
def bytes_to_int32s(data, le=True, signed=False):
    ret = []
    for i in range(int(len(data) / 4)):
        ret.append(bytes_to_ints(data[(4 * i) : (4 * i + 4)], signed=signed, le=le))
    return ret


# Convert a list of bytes into a list of 16-bit integers
def bytes_to_int16s(data, le=True, signed=False):
    ret = []
    for i in range(int(len(data) / 2)):
        ret.append(bytes_to_ints(data[(2 * i) : (2 * i + 2)], signed=signed, le=le))
    return ret


# Convert a list of bytes into a list of 32-bit floats integers
def bytes_to_float32s(data):
    tmp = bytes_to_int32s(data, le=True, signed=False)
    return [i_to_f(x) for x in tmp]


# Utility function which removes the first count elements from a list and returns them
def pop_n_elements(data, count):
    if len(data) < count:
        data.extend((count - len(data)) * [0])
    ret = data[:count]
    del data[0:count]
    return ret


# These functions all pop off the first bytes of the input list and convert
# them into another type before returning them.
def pop_int8(data, le=True):
    return bytes_to_ints(pop_n_elements(data, 1), signed=True, le=le)


def pop_uint8(data, le=True):
    return bytes_to_ints(pop_n_elements(data, 1), signed=False, le=le)


def pop_int16(data, le=True):
    return bytes_to_ints(pop_n_elements(data, 2), signed=True, le=le)


def pop_uint16(data, le=True):
    return bytes_to_ints(pop_n_elements(data, 2), signed=False, le=le)


def pop_int32(data, le=True):
    return bytes_to_ints(pop_n_elements(data, 4), signed=True, le=le)


def pop_uint32(data, le=True):
    return bytes_to_ints(pop_n_elements(data, 4), signed=False, le=le)


def pop_int64(data, le=True):
    return bytes_to_ints(pop_n_elements(data, 8), signed=True, le=le)


def pop_uint64(data, le=True):
    return bytes_to_ints(pop_n_elements(data, 8), signed=False, le=le)


def pop_float(data, le=True):
    return i_to_f(pop_uint32(data, le=le))


def make_crc_table(poly):
    ret = []
    for i in range(256):
        crc = i
        for j in range(8):
            lsb_set = crc & 1
            crc >>= 1
            if lsb_set:
                crc ^= poly
        ret.append(crc)
    return ret


class CRC16:
    """Simple utility class for calculating CRC values."""

    tbl = make_crc_table(0xA001)

    def __init__(self):
        self.init = 0

    def calc(self, data, init=0):
        crc = init ^ self.init
        for d in data:
            a = 0x00FF & (d ^ crc)
            crc = CRC16.tbl[a] ^ (crc >> 8)
        return crc ^ self.init
