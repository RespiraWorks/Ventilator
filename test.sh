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

# This script should work no matter where you call it from.
cd "$(dirname "$0")"

# Pre-commit checks are only runing on changed files.
changed_files=$(git diff --name-only $TRAVIS_BRANCH...HEAD)
echo "Changed files: " $changed_files
# Make git think these changes are uncommitted
git reset --soft $TRAVIS_BRANCH
# Run precommit only on the files changed in our branch since merge-base
pre-commit run --show-diff-on-failure --files $changed_files

# Controller unit tests on native.
pio test -e native
# Make sure controller builds for target platform.
pio run

# Code style / bug-prone pattern checks (eg. clang-tidy)
# WARNING: This might sometimes give different results for different people,
# and different results on CI:
# See https://community.platformio.org/t/no-version-of-tool-clangtidy-works-on-all-os/13219
# Feel free to edit .clang_tidy to blacklist problematic checks.
pio check -e native --fail-on-defect=high
