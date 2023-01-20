#!/bin/bash

# Copyright 2023, RespiraWorks
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

# Print each command as it executes
set -o xtrace

# This script should work no matter where you call it from.
cd "$(dirname "$0")"/..

merge_base=$(git merge-base -a HEAD origin/master)
changed_files=$(git diff --name-only $merge_base...HEAD)
echo "Changed files since branched from origin/master: " $changed_files
git reset --soft $merge_base
pre-commit run --show-diff-on-failure --files $changed_files
