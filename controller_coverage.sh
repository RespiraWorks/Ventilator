#!/bin/bash

# Usage: ./controller_coverage.sh
# Then run: firefox controller_coverage/index.html

set -e
set -x

cd "$(dirname "$0")"
ENVIRONMENT=native
OUTPUT_DIR=controller_coverage
SRC_DIR=".pio/build/$ENVIRONMENT"

# delete coverage files
#find .pio -name "*.gcno" -delete

# run tests
pio test -e native


# delete the old report as "safely" as possible
find "$OUTPUT_DIR" -name "*.html" -delete
mkdir -p "$OUTPUT_DIR"
lcov --directory "$SRC_DIR" --capture --output-file "$OUTPUT_DIR/$ENVIRONMENT.info"

# the file "output_export.cpp" causes an lcov error, but it doesn't appear to be part of our source, so I'm excluding it
lcov -r "$OUTPUT_DIR/$ENVIRONMENT.info" --output-file "$OUTPUT_DIR/${ENVIRONMENT}_trimmed.info" \
    "*output_export.cpp" \
    "*.pio/libdeps/*" \
    "/usr/include*"
genhtml "$OUTPUT_DIR/${ENVIRONMENT}_trimmed.info" --output-directory "$OUTPUT_DIR"

echo "Coverage report generated. Open '$OUTPUT_DIR/index.html' in a browser to view it."
