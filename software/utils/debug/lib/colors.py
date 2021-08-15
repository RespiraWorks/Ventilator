# Terminal output colorization functions

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


class Color:
    ENDC = "\033[m"
    BLACK = "\033[30m"
    DARK_RED = "\033[31m"
    DARK_GREEN = "\033[32m"
    DARK_ORANGE = "\033[33m"
    DARK_BLUE = "\033[34m"
    DARK_PURPLE = "\033[35m"
    DARK_YELLOW = "\033[36m"
    LIGHT_GRAY = "\033[37m"
    GRAY = "\033[90m"
    RED = "\033[91m"
    GREEN = "\033[92m"
    ORANGE = "\033[93m"
    BLUE = "\033[94m"
    PURPLE = "\033[95m"
    YELLOW = "\033[96m"
    WHITE = "\033[97m"


def dark_orange(text):
    return Color.DARK_ORANGE + text + Color.ENDC


def gray(text):
    return Color.GRAY + text + Color.ENDC


def red(text):
    return Color.RED + text + Color.ENDC


def green(text):
    return Color.GREEN + text + Color.ENDC


def orange(text):
    return Color.ORANGE + text + Color.ENDC


def blue(text):
    return Color.BLUE + text + Color.ENDC


def purple(text):
    return Color.PURPLE + text + Color.ENDC


def yellow(text):
    return Color.YELLOW + text + Color.ENDC


def white(text):
    return Color.WHITE + text + Color.ENDC
