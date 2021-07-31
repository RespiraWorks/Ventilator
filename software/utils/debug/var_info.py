import debug_types

# TODO: Import constants from proto instead!

# Variable types (see vars.h)
VAR_INT32 = 1
VAR_UINT32 = 2
VAR_FLOAT = 3


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

    def from_bytes(self, data):
        if self.type == VAR_INT32:
            return debug_types.bytes_to_int32s(data, signed=True)[0]
        elif self.type == VAR_UINT32:
            return debug_types.bytes_to_int32s(data)[0]
        elif self.type == VAR_FLOAT:
            return debug_types.bytes_to_float32s(data)[0]
        else:
            raise Error(f"Sorry, I don't know how to handle variable type {self.type}")

    def to_bytes(self, value):
        if self.type == VAR_INT32:
            if not isinstance(value, int):
                value = int(value, 0)
            return debug_types.int32s_to_bytes(value)
        elif self.type == VAR_UINT32:
            if not isinstance(value, int):
                value = int(value, 0)
            return debug_types.int32s_to_bytes(value)
        elif self.type == VAR_FLOAT:
            return debug_types.float32s_to_bytes(float(value))
        else:
            raise Error(f"Sorry, I don't know how to handle variable type {self.type}")
