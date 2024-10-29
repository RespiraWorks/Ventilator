#!/bin/bash

# Copyright 2024, RespiraWorks
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


# \todo: keep PIO_VERSION updated, test thoroughly whenever you do, leave this "todo" here
PIO_VERSION=6.1.16

# \todo: keep CONAN_VERSION updated, test thoroughly whenever you do, leave this "todo" here
CONAN_VERSION=2.2.3

EXIT_FAILURE=1
EXIT_SUCCESS=0

PLATFORM="$(uname -s)"

# Fail if any command fails
set -e
set -o pipefail

# Print each command as it executes
if [ -n "$VERBOSE" ]; then
  set -o xtrace
fi

function exit_fail() {
  exit "$EXIT_FAILURE"
}

function exit_good() {
  exit "$EXIT_SUCCESS"
}

function ensure_linux() {
  # Check if Linux
  if [ "$PLATFORM" != "Linux" ]; then
    echo "Error: This script only supports 'Linux'. You have $PLATFORM."
    exit_fail
  fi
}

# Silent pushd
function pushd() {
  command pushd "$@" > /dev/null
}

# Silent popd
function popd() {
  command popd > /dev/null
}

function ensure_not_root() {
  if [ "$EUID" -eq 0 ] && [ -z "$FORCED_ROOT" ]; then
    echo "Please do not run update with root privileges!"
    exit_fail
  fi
}

function clean_dir() {
  dir_name=$1
  if [ -d "$dir_name" ]; then
    echo "Removing $dir_name"
    rm -rf "$dir_name"
    return "$EXIT_SUCCESS"
  elif [ -f "$dir_name" ]; then
    echo "File with this name already exists, not a directory."
    return "$EXIT_FAILURE"
  fi
}

function install_py() {
  sudo apt update
  sudo apt install -y \
           python-is-python3 \
           python3-pip \
           pipx
  pipx ensurepath
  pipx install --force poetry
  source "${HOME}/.profile"
}

function install_common_tooling() {
  install_py
  sudo apt update
  sudo apt install -y \
           build-essential \
           clang-tidy \
           cppcheck \
           curl \
           gcovr \
           git \
           lcov \
           libtinfo6 \
           protobuf-compiler
  pipx install --force nanopb
  pipx install --force platformio==${PIO_VERSION}
}

function launch_browser() {
  python3 -m webbrowser "${1}"
}
