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

pushd () {
    command pushd "$@" > /dev/null
}

popd () {
    command popd > /dev/null
}

# This script should work no matter where you call it from.
MY_PATH="$(dirname "$0")"

pushd $MY_PATH
PYTHONPATH=$MY_PATH ./debug_cli.py "${@:1}"
popd
