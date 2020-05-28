# This script is called as part of the platformio build sequence for
# the stm32 controller firmware.
#
# Its purpose is to add some additional linker options which can't be
# directly added to the platformio.ini line for some reason.
#
#
Import("env")

# Threadsafe-statics ends up calling malloc.  But we don't need them to be
# thread-safe because we don't have threads!  This flag can't be in
# platformio.ini because gcc raises an error if it's provided to C code.
env.Append(CXXFLAGS=["-fno-threadsafe-statics"])

# These link flags tell the linker that the processor we're using has
# hardware floating point, so the software floating point libraries aren't
# needed.
env.Append(LINKFLAGS=["-mfpu=fpv4-sp-d16", "-mfloat-abi=hard"])
