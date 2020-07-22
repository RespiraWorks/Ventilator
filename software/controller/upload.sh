#!/bin/bash

display_usage() {
  echo
  echo "This script uploads the default release firmware (to a specified device)."
  echo
  echo "usage: deploy.sh [device_id]"
  echo "    [device_id] is optional id of nucleo device, as defined in platformio/device_lookup_table.txt"
  echo
  echo "To specify additional devices and for troubleshooting, see platformio/README.md"
  echo
}

#check whether user had supplied -h or --help . If yes display usage
if [[ ( $# == "--help") ||  $# == "-h" ]]
then
  display_usage
  exit 0
fi

#ensure that we are in same dir as platformio.ini
pushd "$(dirname "$0")"

platformio/deploy.sh stm32 $@
