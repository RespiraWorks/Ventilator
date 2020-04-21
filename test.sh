#!/bin/bash

# Use this script during development to have a good approximation
# of whether your PR will pass tests on Travis:
#
# $ ./test.sh
#
# Travis runs this script (via .travis.yml), but might have some
# environment differences, so the approximation is not perfect.
#
# Feel free to add more checks here, but keep in mind that:
# - They have to pass on Travis
# - They have to have a very good chance of passing for other
#   developers if they run via ./test.sh

# Fail if any command fails
set -e
set -o pipefail

# Print each command as it executes
set -o xtrace

# Controller unit tests on native.
pio test -e native
# Make sure controller builds for target platform.
pio run

# Code style / bug-prone pattern checks (eg. clang-tidy)
# WARNING: This might sometimes give different results for different people,
# and different results on CI:
# See https://community.platformio.org/t/no-version-of-tool-clangtidy-works-on-all-os/13219
# Feel free to edit .clang_tidy to blacklist problematic checks.
#
# Disabled due to failures in nanoproto-generated code,
# https://github.com/RespiraWorks/VentilatorSoftware/issues/146
# pio check --pattern="*" --fail-on-defect=high
# pio check -e native --pattern="*" --fail-on-defect=high
