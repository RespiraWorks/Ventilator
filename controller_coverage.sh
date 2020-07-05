#!/bin/bash

# Usage: bash ./controller_coverage.sh
# Then run: firefox controller_coverage/index.html

set -e
set -x

# delete coverage files
#find .pio -name "*.gcno" -delete

# run tests
pio test -e native

ENVIRONMENT=native
OUTPUT_DIR=controller_coverage
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR
lcov --directory .pio/build/$ENVIRONMENT --capture --output-file $OUTPUT_DIR/$ENVIRONMENT.info

# the file "output_export.cpp" causes an lcov error, but it doesn't appear to be part of our source, so I'm excluding it
lcov -r $OUTPUT_DIR/$ENVIRONMENT.info --output-file $OUTPUT_DIR/${ENVIRONMENT}_trimmed.info \
    "*output_export.cpp" \
    "*.pio/libdeps/*" \
    "/usr/include*"
genhtml $OUTPUT_DIR/${ENVIRONMENT}_trimmed.info --output-directory $OUTPUT_DIR
