#!/bin/bash

# This script runs a sequence of integration tests on a specified prototype.
#
# Run from main repo directory with platformio.ini file as follows:
# Syntax:
#   ./controller/integration_tests/run_all.sh DEVICED_NAME
# whwere DEVICE_NAME if one of predefined ID for devices available on the system.
#
# The following prototypes are made available on the BigCheese server:
# p3 -- pizza build v0.2 SN 3
# p4 -- pizza build v0.2 SN 4
# p5 -- pizza build v0.2 SN 5

device_name="$1"
device_sn=""

if [ "$device_name" = "" ]; then
    echo "No device name provided."
    exit 1
else
  case "$device_name" in
        p3             ) device_sn="0668FF303435554157105440";;
        p4             ) device_sn="066BFF303435554157104916";;
        p5             ) device_sn="066FFF303435554157105014";;
        *              ) echo "bad id" && exit 1;; # catchall, matches anything not matched above
    esac
fi

# This is a work in progress, honing in on deploying this in CI

# This script runs a few of the integration tests in order, with slight pauses in between
# Ends with putting controller in idle loop.

SERIAL=$device_sn TEST=TEST_BLOWER pio run -e integration-test -t upload
sleep 10

SERIAL=$device_sn TEST=TEST_BUZZER pio run -e integration-test -t upload
sleep 10

SERIAL=$device_sn TEST=TEST_PINCH_VALVE pio run -e integration-test -t upload
sleep 10

SERIAL=$device_sn TEST=TEST_IDLE pio run -e integration-test -t upload
