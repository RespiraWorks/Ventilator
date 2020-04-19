# Makes PlatformIO link this environment with sanitizers.
Import("env")

import sys

# Sanitizers have to be enabled both in CCFLAGS and LINKFLAGS.
sanitizers = [
      "-fsanitize=undefined",
      "-fsanitize=address",
]
if sys.platform == "linux":
    # https://releases.llvm.org/3.7.0/tools/clang/docs/MemorySanitizer.html#supported-platforms
    # Currently only works on Linux but e.g. not Mac OS.
    sanitizers.append('-fsanitize=memory')

env.Append(CCFLAGS=sanitizers)
env.Append(LINKFLAGS=sanitizers)

