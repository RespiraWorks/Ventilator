#!/bin/bash

# This script is designed for local unix usage.

# Fail if any command fails
set -e
set -o pipefail

# This script should work no matter where you call it from.
pushd "$(dirname "$0")"/debug

./debug_cli.py

popd
