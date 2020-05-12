# This script is called as part of the platformio build sequence for
# the stm32 controller firmware.
#
# It's purpose is to add some additional linker options which can't be
# directly added to the platformio.ini line for some reason.
#
# These link flags tell the linker that the processor we're using has
# hardware floating point, so the software floating point libraries aren't
# needed.
#
Import("env")

env.Append(LINKFLAGS=["-mfpu=fpv4-sp-d16", "-mfloat-abi=hard"])
