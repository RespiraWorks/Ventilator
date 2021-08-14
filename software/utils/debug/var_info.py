# Debug variable info abstraction for ventilator debug interface

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

import debug_types
from lib.error import Error
import debug_protocol_pb2 as debug_proto


class VarInfo:
    """Abstraction for debug variable metadata, utilities for automatic type conversion"""

    id: int
    type: int
    name: str
    format: str
    help: str

    # Initialize the variable info from the data returned
    # by the controller.  Set var.cpp in the controller for
    # details on this formatting
    def __init__(self, id, data):
        self.id = id

        if len(data) < 8:
            raise Error("Invalid VarInfo data returned")

        self.type = data[0]
        name_length = data[4]
        fmt_length = data[5]
        help_length = data[6]

        if len(data) < 8 + name_length + fmt_length + help_length:
            raise Error("Invalid VarInfo data returned")

        n = 8
        self.name = "".join([chr(x) for x in data[n : n + name_length]])
        n += name_length
        self.format = "".join([chr(x) for x in data[n : n + fmt_length]])
        n += fmt_length
        self.help = "".join([chr(x) for x in data[n : n + help_length]])

    # Convert an unsigned 32-bit value into the correct type for
    # this variable
    def convert_int(self, data):
        if self.type == debug_proto.Var.Type.Float:
            return debug_types.i_to_f(data)
        if self.type == debug_proto.Var.Type.Int32:
            if data & 0x80000000:
                return data - (1 << 32)
            return data
        return data

    def from_bytes(self, data):
        if self.type == debug_proto.Var.Type.Int32:
            return debug_types.bytes_to_int32s(data, signed=True)[0]
        elif self.type == debug_proto.Var.Type.UInt32:
            return debug_types.bytes_to_int32s(data)[0]
        elif self.type == debug_proto.Var.Type.Float:
            return debug_types.bytes_to_float32s(data)[0]
        else:
            raise Error(f"Sorry, I don't know how to handle variable type {self.type}")

    def to_bytes(self, value):
        if self.type == debug_proto.Var.Type.Int32:
            if not isinstance(value, int):
                value = int(value, 0)
            return debug_types.int32s_to_bytes(value)
        elif self.type == debug_proto.Var.Type.UInt32:
            if not isinstance(value, int):
                value = int(value, 0)
            return debug_types.int32s_to_bytes(value)
        elif self.type == debug_proto.Var.Type.Float:
            return debug_types.float32s_to_bytes(float(value))
        else:
            raise Error(f"Sorry, I don't know how to handle variable type {self.type}")
