#!/bin/bash

# Use this script during development to have a good approximation
# of whether your PR will pass tests on CircleCI:
#
# $ ./test.sh
#
# CircleCI runs this script (via .travis.yml), but might have some
# environment differences, so the approximation is not perfect. For
# a more exact test run, please build the tests_Dockerfile.
#
# Feel free to add more checks here, but keep in mind that:
# - They have to pass on CircleCI
# - They have to have a very good chance of passing for other
#   developers if they run via ./test.sh

# Fail if any command fails
set -e
set -o pipefail

# Print each command as it executes
set -o xtrace

# This script should work no matter where you call it from.
cd "$(dirname "$0")"

# Controller unit tests on native.
pio test -e native

# Make sure controller builds for target platform.
pio run -e stm32

# Code style / bug-prone pattern checks (eg. clang-tidy)
# WARNING: This might sometimes give different results for different people,
# and different results on CI:
# See https://community.platformio.org/t/no-version-of-tool-clangtidy-works-on-all-os/13219
# Feel free to edit .clang_tidy to blacklist problematic checks.
# TODO(jkff) Currently this fails with a bunch of errors. Need to uncomment and fix errors.
# pio check -e native --fail-on-defect=high
