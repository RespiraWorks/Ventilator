#!/usr/bin/env python3

# Averages the last few lines of each fanXXX file, and prints it in a way that
# could be copy-pasted into C++ code.

import glob
import sys
import os

dir = os.path.dirname(os.path.realpath(__file__))

# Read last NUM_LINES lines of each file.
NUM_LINES = 32

# (filename, power level) tuples of all fanXXX files, sorted by power level.
files = sorted(
    [
        (fname, int(os.path.basename(fname)[3:]) / 100)
        for fname in glob.glob(os.path.join(dir, "fan*"))
    ],
    key=lambda tup: tup[1],
)

for fname, power_level in files:
    with open(fname, "r") as f:
        lines = f.readlines()[-NUM_LINES:]
    vals = [float(l.split()[1]) for l in lines]
    avg = sum(vals) / len(vals)
    print(f"{{{power_level:.2f}f, cmH2O({avg:.2f}f)}},")
