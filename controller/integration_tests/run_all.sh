#!/bin/bash

# This script runs a sequence of integration tests on a specified prototype. Can be run from any path.
#
# Run from main repo directory with platformio.ini file as follows:
# Syntax:
#   ./controller/integration_tests/run_all.sh DEVICED_NAME
# where DEVICE_NAME if one of predefined ID for devices available on the system.
#

device_name="$1"
wait_time=15

# This is a work in progress, honing in on deploying this in CI

# This script runs a few of the integration tests in order, with slight pauses in between
# Ends with putting controller in idle loop.

#ensure that we are in script dir
pushd "$(dirname "$0")"

./deploy_test.sh $device_name blower 0.0f 1.0f
sleep $wait_time

./deploy_test.sh $device_name buzzer 0.0f 1.0f
sleep $wait_time

./deploy_test.sh $device_name pinch_valve 0
sleep $wait_time

./deploy_test.sh $device_name pinch_valve 1
sleep $wait_time

./deploy_test.sh $device_name idle
