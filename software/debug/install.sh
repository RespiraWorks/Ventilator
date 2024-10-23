#!/bin/bash

# Copyright 2020-2022, RespiraWorks
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

# Fail if any command fails
set -e
set -o pipefail

# Print each command as it executes
if [ -n "$VERBOSE" ]; then
  set -o xtrace
fi

# Silent pushd
pushd () {
    command pushd "$@" > /dev/null
}

# Silent popd
popd () {
    command popd > /dev/null
}

install_debugger_deps() {
#  \todo deprecated!
  sudo apt update
  sudo apt install -y \
           pipx \
           python-is-python3 \
           python3-pip

  pipx ensurepath
  pipx install --force pyserial
  pipx install --force matplotlib
  pipx install --force pandas
  pipx install --force gitpython
  pipx install --force numpy
  pipx install --force pytest
  pipx install --force platformio==${PIO_VERSION}
  source ${HOME}/.profile
}

# This script should work no matter where you call it from.
MY_PATH="$(dirname "$0")"

pushd $MY_PATH

########
# HELP #
########

if [ "$1" == "help" ] || [ "$1" == "-h" ]; then
  print_help
  exit $EXIT_SUCCESS

###########
# INSTALL #
###########
elif [ "$1" == "install" ]; then
  ../common/common.sh install
  if [ "$EUID" -eq 0 ] && [ -z "$FORCED_ROOT" ]; then
    echo "Please do not run install with root privileges!"
    exit $EXIT_FAILURE
  fi
  install_debugger_deps
  exit $EXIT_SUCCESS

popd
