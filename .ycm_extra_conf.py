# This file originally comes from YCM:
#
#   https://raw.githubusercontent.com/ycm-core/ycmd/66030cd94299114ae316796f3cad181cac8a007c/.ycm_extra_conf.py
#
# It has been modified to fit your screen (and work with the
# RespiraWorks/VentilatorSoftware repository).  Original license follows.

# This file is NOT licensed under the GPLv3, which is the license for the rest
# of YouCompleteMe.
#
# Here's the license text for this file:
#
# This is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.
#
# In jurisdictions that recognize copyright laws, the author or authors
# of this software dedicate any and all copyright interest in the
# software to the public domain. We make this dedication for the benefit
# of the public at large and to the detriment of our heirs and
# successors. We intend this dedication to be an overt act of
# relinquishment in perpetuity of all present and future rights to this
# software under copyright law.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
# For more information, please refer to <http://unlicense.org/>

# This config requires that YCM was built with clangd support, which is the
# recommended configuration today.  See
# https://github.com/ycm-core/YouCompleteMe#c-family-semantic-completion

from distutils.sysconfig import get_python_inc
import glob
import os
import platform
import subprocess
import ycm_core

DIR_OF_THIS_SCRIPT = os.path.abspath(os.path.dirname(__file__))
SOURCE_EXTENSIONS = [".cpp", ".cxx", ".cc", ".c", ".m", ".mm"]

# I am unclear on what is the working directory used when this script is
# invoked; it seems to change?  For our purposes, the working directory should
# be the location of the script itself.
os.chdir(DIR_OF_THIS_SCRIPT)


def pio_pkg_dir(pkg):
    return os.path.join(os.environ["HOME"], ".platformio/packages", pkg)


# We manually specify compile flags, which is a bummer.  It's theoretically
# possible to use compile_commands.json; platformio can generate it,
# https://docs.platformio.org/en/latest/integration/compile_commands.html, and
# that file is how we came up with most of these flags.  But there are a few
# problems.
#
#  - platformio does not -I libraries that are not "currently used".  This
#    means that when you add a new header to a file, ycm won't know how to use
#    that header until you manually regenerate compile_commands.json!
#
#  - platformio uses arm-none-eabi-g++ to compile for our chip, and that
#    compiler implicitly adds a bunch of necessary flags that ycm/clangd are
#    not aware of.  So we'd have to add them here *anyway*.
#
#  - Most files get compiled with ARM flags, but some (tests) must be
#    compiled with native flags.  So even if we used a compilation_database,
#    we'd have to intelligently choose which set of flags to use here.
#
# Flags are defined below.  Feel free to modify them if necessary!

common_flags = [
    "-x",
    "c++",
    "-std=gnu++17",
    "-fno-exceptions",
    "-Wall",
    "-Werror=conversion",
    "-Wno-sign-conversion",
] + [
    "-I" + d for d in {os.path.dirname(h) for h in glob.glob("**/*.h", recursive=True)}
]

stm32_flags = ["-nostdlib", "-DBARE_STM32", "-DF_CPU=80000000L",] + [
    "-I" + d for d in glob.glob(".pio/libdeps/stm32/*")
]

native_flags = [
    "-DTEST_MODE",
    "-I.pio/libdeps/native/googletest_ID5976/googlemock/include",
    "-I.pio/libdeps/native/googletest_ID5976/googletest/include",
] + [
    # Sigh, gtest/gmock require different treatment.  Its headers aren't under
    # googletest/* directly, but are instead under googlemock/include and
    # googletest/include.
    "-I" + d
    for d in glob.glob(".pio/libdeps/native/*")
    if "googletest" not in d
]

# Set this to the absolute path to the folder (NOT the file!) containing the
# compile_commands.json file to use that instead of 'flags'. See here for
# more details: http://clang.llvm.org/docs/JSONCompilationDatabase.html
#
# You can get CMake to generate this file for you by adding:
#   set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )
# to your CMakeLists.txt file.
#
# Most projects will NOT need to set this to anything; you can just change the
# 'flags' list of compilation flags. Notice that YCM itself uses that approach.
compilation_database_folder = ""

if os.path.exists(compilation_database_folder):
    database = ycm_core.CompilationDatabase(compilation_database_folder)
else:
    database = None


def IsHeaderFile(filename):
    extension = os.path.splitext(filename)[1]
    return extension in [".h", ".hxx", ".hpp", ".hh"]


def FindCorrespondingSourceFile(filename):
    if IsHeaderFile(filename):
        basename = os.path.splitext(filename)[0]
        for extension in SOURCE_EXTENSIONS:
            replacement_file = basename + extension
            if os.path.exists(replacement_file):
                return replacement_file
    return filename


def Settings(**kwargs):
    if kwargs["language"] == "cfamily":
        # If the file is a header, try to find the corresponding source file and
        # retrieve its flags from the compilation database if using one. This is
        # necessary since compilation databases don't have entries for header files.
        # In addition, use this source file as the translation unit. This makes it
        # possible to jump from a declaration in the header file to its definition
        # in the corresponding source file.
        filename = FindCorrespondingSourceFile(kwargs["filename"])

        if not database:
            # Tests get compiled with native flags; other files get compiled with
            # Arduino flags.
            #
            # TODO(jlebar): This is not a perfect heuristic.  In particular, hal.h
            # mixes modes.
            if "/test/" not in filename:
                flags = common_flags + stm32_flags
            else:
                flags = common_flags + native_flags

            return {
                "flags": flags,
                "include_paths_relative_to_dir": DIR_OF_THIS_SCRIPT,
                "override_filename": filename,
            }

        compilation_info = database.GetCompilationInfoForFile(filename)
        if not compilation_info.compiler_flags_:
            return {}

        # Bear in mind that compilation_info.compiler_flags_ does NOT return a
        # python list, but a "list-like" StringVec object.
        final_flags = list(compilation_info.compiler_flags_)

        # NOTE: This is just for YouCompleteMe; it's highly likely that your project
        # does NOT need to remove the stdlib flag. DO NOT USE THIS IN YOUR
        # ycm_extra_conf IF YOU'RE NOT 100% SURE YOU NEED IT.
        try:
            final_flags.remove("-stdlib=libc++")
        except ValueError:
            pass

        return {
            "flags": final_flags,
            "include_paths_relative_to_dir": compilation_info.compiler_working_dir_,
            "override_filename": filename,
        }
    return {}
