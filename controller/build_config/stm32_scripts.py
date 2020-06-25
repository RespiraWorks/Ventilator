# This script is called as part of the platformio build sequence for
# the stm32 controller firmware.
#
# Its purpose is to add some additional linker options which can't be
# directly added to the platformio.ini line for some reason.
#
#
Import("env")

# Flags that can't be in platformio.ini because gcc raises an error if they're
# provided to C code.
env.Append(
    CXXFLAGS=[
        # Threadsafe-statics ends up calling malloc.  But we don't need them to
        # be thread-safe because we don't have threads!
        "-fno-threadsafe-statics",
        # Disable rtti per this guide to disabling exceptions in embedded C++:
        # http://elegantinvention.com/blog/information/smaller-binary-size-with-c-on-baremetal-g/
        "-fno-rtti",
    ]
)

# These link flags tell the linker that the processor we're using has
# hardware floating point, so the software floating point libraries aren't
# needed.
env.Append(LINKFLAGS=["-mfpu=fpv4-sp-d16", "-mfloat-abi=hard"])
