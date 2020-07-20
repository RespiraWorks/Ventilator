#!/bin/bash

display_usage() {
  echo
  echo "This script deploys an integration test to a specified device."
  echo
  echo "usage: deploy_test.sh <device> <test_name> [param1] [param2] [param3] [param3] [param4] [param5]"
  echo
  echo "<device> is one of the following prototypes on the BigCheese server:"
  echo "    p3 -- pizza build v0.2 SN 3"
  echo "    p4 -- pizza build v0.2 SN 4"
  echo "    p5 -- pizza build v0.2 SN 5"
  echo
  echo "<test_name> if one of the following + additional parameters"
  echo "    idle"
  echo "    blower <min_speed> <max_speed>"
  echo "    buzzer <min_volume> <max_volume>"
  echo "    stepper <motor_index> <step_degrees>"
  echo "    pinch_valve <motor_index>"
  echo
  echo "To specify additional devices and for troubleshooting info, see /controller/integration_tests/platformio/README.md"
  echo
}

#if less than two arguments supplied, display usage
if [  $# -le 1 ]
then
  echo "Not enough parameters"
  display_usage
  exit 1
fi

#check whether user had supplied -h or --help . If yes display usage
if [[ ( $# == "--help") ||  $# == "-h" ]]
then
  display_usage
  exit 0
fi

device_name="$1"
test_name="$2"
test_param_1="$3"
test_param_2="$4"
test_param_3="$5"
test_param_4="$6"
test_param_5="$7"

device_sn=""
case "$device_name" in
  p3 ) device_sn="0668FF303435554157105440";;
  p4 ) device_sn="066BFF303435554157104916";;
  p5 ) device_sn="066FFF303435554157105014";;
  *  ) echo "Bad device id: $device_name" && display_usage && exit 1;; # catchall, matches anything not matched above
esac

test_name="${test_name}_test.h"

echo "Deploying integration test: $test_name"
echo "    on device SN: $device_sn"
[ -n "$test_param_1" ] && echo "    test_param_1: $test_param_1"
[ -n "$test_param_2" ] && echo "    test_param_2: $test_param_2"
[ -n "$test_param_3" ] && echo "    test_param_3: $test_param_3"
[ -n "$test_param_4" ] && echo "    test_param_4: $test_param_4"
[ -n "$test_param_5" ] && echo "    test_param_5: $test_param_5"

#ensure that we are in same dir as platformio.ini
pushd "$(dirname "$0")"/..

SERIAL=$device_sn \
INTEGRATION_TEST_H=$test_name \
TEST_PARAM_1=$test_param_1 \
TEST_PARAM_2=$test_param_2 \
TEST_PARAM_3=$test_param_3 \
TEST_PARAM_4=$test_param_4 \
TEST_PARAM_5=$test_param_5 \
pio run -e integration-test -t upload
