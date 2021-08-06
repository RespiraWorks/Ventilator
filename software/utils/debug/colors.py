class Color:
    ENDC = "\033[m"
    GRAY = "\033[90m"
    RED = "\033[91m"
    GREEN = "\033[92m"
    ORANGE = "\033[93m"
    BLUE = "\033[94m"
    PURPLE = "\033[95m"
    YELLOW = "\033[96m"
    WHITE = "\033[97m"


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
