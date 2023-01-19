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

image_name="rw_ci_test_image"
container_name="rw_ci_test_instance"

# Build image
docker build . -t ${image_name}

# Start instance
docker run -t -d --name ${container_name} ${image_name}

# Copy repo into container
docker cp ../../ventilator ${container_name}:/home/jenkins/ventilator

# Run test
docker exec ${container_name} bash -e -x -c "cd ventilator && ./jenkins/test.sh"

# Cleanup
docker stop ${container_name}
docker rm -f ${container_name}
