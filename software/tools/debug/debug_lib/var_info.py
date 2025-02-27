# Debug variable info abstraction for ventilator debug interface

__copyright__ = "Copyright 2020-2022 RespiraWorks"

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

import debug.debug_lib.debug_types as debug_types
from debug.util.error import Error

# TODO: Import constants from proto instead!

VAR_INVALID_ID = 100

# Variable types (see vars.h)
VAR_INT32 = 1
VAR_UINT32 = 2
VAR_FLOAT = 3
VAR_FLOAT_ARRAY = 4
VAR_STRING = 5

VAR_TYPE_REPRESENTATION = ["?", "i", "u", "f", "A", "S"]

VAR_ACCESS_READ_ONLY = 0
VAR_ACCESS_WRITE = 1

VAR_VENT_MODE = [
    "off",
    "pressure_control",
    "pressure_assist",
    "high_flow_nasal_cannula",
    "invalid",
]


class VarInfo:
    """Abstraction for debug variable metadata, utilities for automatic type conversion"""

    id: int
    type: int
    write_access: bool
    byte_size: int
    name: str
    units: str = ""
    format: str
    help: str

    def size(self):
        if self.type == VAR_STRING:
            return self.byte_size
        else:
            return int(self.byte_size / 4)

    # Initialize the variable info from the data returned
    # by the controller.  Set var.cpp in the controller for
    # details on this formatting
    def __init__(self, id, data):
        self.id = id

        if len(data) < 8:
            raise Error("Invalid VarInfo data returned: bad header length")

        self.type = data[0]
        self.write_access = data[1] == VAR_ACCESS_WRITE
        self.byte_size = data[2]
        name_length = data[4]
        fmt_length = data[5]
        help_length = data[6]
        units_length = data[7]

        if len(data) < 8 + name_length + fmt_length + help_length + units_length:
            raise Error("Invalid VarInfo data returned: bad total length")

        n = 8
        self.name = "".join([chr(x) for x in data[n : n + name_length]])
        n += name_length
        self.format = "".join([chr(x) for x in data[n : n + fmt_length]])
        n += fmt_length
        self.help = "".join([chr(x) for x in data[n : n + help_length]])
        n += help_length
        self.units = "".join([chr(x) for x in data[n : n + units_length]])

    def verbose(self, show_access=True, show_format=True):
        # \todo show possible enum values
        type_str = "?"
        if self.type < len(VAR_TYPE_REPRESENTATION):
            type_str = VAR_TYPE_REPRESENTATION[self.type]
        ret = f"[{self.id:>02}] {self.size():>2}{type_str} "
        if show_access:
            ret += "w+ " if self.write_access else "w- "
        ret += f"{self.name:30} {self.units:>13} "
        if show_format:
            format_string = "[" + self.format + "]"
            ret += f"{format_string:>8} "
        ret += f" {self.help}"
        return ret

    def format_value(self, value, raw=False, fmt=None):
        if raw:
            return value

        # If a format wasn't passed, use the default for this var
        if fmt is None:
            fmt = self.format

        if self.name == "forced_mode":
            if value < len(VAR_VENT_MODE):
                return VAR_VENT_MODE[value]
            else:
                raise Error(f"Do not know how to interpret forced_mode={value}")
        if self.type == VAR_FLOAT_ARRAY:
            return "[" + " ".join(fmt % k for k in value) + "]"
        elif self.type == VAR_STRING:
            # TODO: may not need this once we have generated protocol and no trailing zeros
            return value.rstrip("\00")
        else:
            return fmt % value

    def print_value(self, value, show_access=True):
        write = ""
        if show_access:
            write = "w+ " if self.write_access else "w- "
        return f"{write}{self.name:30} = {str(value):>25} {self.units}"

    # Convert an unsigned 32-bit value into the correct type for
    # this variable
    def convert_int(self, data):
        if self.type == VAR_FLOAT:
            return debug_types.i_to_f(data)
        if self.type == VAR_INT32:
            if data & 0x80000000:
                return data - (1 << 32)
            return data
        return data

    def from_bytes(self, data):
        if self.type == VAR_INT32:
            return debug_types.bytes_to_int32s(data, signed=True)[0]
        elif self.type == VAR_UINT32:
            return debug_types.bytes_to_int32s(data)[0]
        elif self.type == VAR_FLOAT:
            return debug_types.bytes_to_float32s(data)[0]
        elif self.type == VAR_FLOAT_ARRAY:
            return debug_types.bytes_to_float32s(data)
        elif self.type == VAR_STRING:
            return bytes(data).decode("ascii")
        else:
            raise Error(f"Sorry, I don't know how to handle variable type {self.type}")

    def to_bytes(self, value):
        val_type = type(value).__name__
        if (self.name == "forced_mode") and isinstance(value, str):
            # \todo: move this enum conversion to some higher layer
            if value not in VAR_VENT_MODE:
                raise Error(f"Do not know how to encode forced_mode={value}")
            idx = VAR_VENT_MODE.index(value)
            return debug_types.int32s_to_bytes(idx)
        elif self.type == VAR_INT32:
            if not isinstance(value, int):
                raise TypeError(f"Expected type INT32, got {val_type}")
            return debug_types.int32s_to_bytes(value)
        elif self.type == VAR_UINT32:
            if not isinstance(value, int):
                raise TypeError(f"Expected type UINT32, got {val_type}")
            if value < 0:
                raise ValueError(f"Expected type UINT32, got negative integer")
            return debug_types.int32s_to_bytes(value)
        elif self.type == VAR_FLOAT:
            if not isinstance(value, float) and not isinstance(value, int):
                raise TypeError(f"Expected type FLOAT, got {val_type}")
            return debug_types.float32s_to_bytes(float(value))
        elif self.type == VAR_FLOAT_ARRAY:
            if not isinstance(value, list):
                raise TypeError(f"Expected type FLOAT_ARRAY, got {val_type}")
            float_array = [float(k) for k in value]
            if len(float_array) != self.size():
                raise Error(
                    f"FloatArray size mismatch. Should be {self.size()}, was {len(float_array)}"
                )
            return debug_types.float32s_to_bytes(float_array)
        elif self.type == VAR_STRING:
            if not isinstance(value, str):
                raise TypeError(f"Expected type STRING, got {val_type}")
            if len(value) > self.size():
                raise Error(
                    f"String size exceeds limit. Should be {self.size()}, was {len(value)}"
                )
            return list(value.ljust(self.size(), "\0").encode("ascii", "replace"))
        else:
            raise Error(f"Sorry, I don't know how to handle variable type {self.type}")
