# Makes PlatformIO link this environment with sanitizers.
Import("env")

import sys

# Sanitizers have to be enabled both in CCFLAGS and LINKFLAGS.
sanitizers = [
    "-fsanitize=undefined",
    "-fsanitize=address",
]

# TODO: Add -fsanitize=memory when supported.
# Caveats: It's only supported in clang and only on Linux.
# And PlatformIO may choose gcc even if you have clang too.
# If it didn't, the code below would probably work:
#
# if sys.platform == "linux" and env["CC"] == "clang":
#    sanitizers.append('-fsanitize=memory')

env.Append(CCFLAGS=sanitizers)
env.Append(LINKFLAGS=sanitizers)
