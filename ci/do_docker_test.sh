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
cd "$(dirname "$0")"

test_name=$1

image_name="rwv_${test_name}_image"
container_name="rwv_${test_name}_instance"
container_script="${test_name}.sh"

if [ $test_name == "precommit" ]
then
  dockerfile="Dockerfile_precommit"
else
  dockerfile="../software/${test_name}/Dockerfile"
fi

cleanup() {
  docker stop ${container_name}
  docker rm -f ${container_name}
  docker rmi ${image_name}
}

#pre-cleanup without failing
cleanup

# Fail if any command fails
set -e
set -o pipefail

# Build image
docker build -t ${image_name} -f ${dockerfile} ../

# Start instance
docker run -t -d --name ${container_name} ${image_name}

# PWD
docker exec ${container_name} bash -e -x -c "pwd"

# CD & LS
docker exec ${container_name} bash -e -x -c "cd Ventilator && ls -l"

# Run test
#docker exec ${container_name} bash -e -x -c "cd Ventilator && ./ci/${container_script}"

#post-cleanup
cleanup
