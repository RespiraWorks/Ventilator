#!/bin/bash

display_usage() {
  echo
  echo "This script deploys firmware to a specified device."
  echo
  echo "usage: deploy.sh <target_name> [device_id]"
  echo "    <target_name> is the platformio 'environment' name to build an deploy"
  echo "    [device_id] is optional id of nucleo device, as defined in device_lookup_table.txt"
  echo
  echo "To specify additional devices and for troubleshooting, README.md"
  echo
}

#if less than one argument supplied, display usage
if [  $# -lt 1 ]
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

#ensure that we are in same dir as platformio.ini
pushd "$(dirname "$0")"/..

env_name="$1"
echo "Deploying: $env_name"

if [  $# -gt 1 ]
then
  device_id="$2"
  device_sn=$(awk '$1 == LOOKUPVAL { print $2 }' "LOOKUPVAL=$2" platformio/device_lookup_table.txt)
  echo "  on device: $device_id ($device_sn)"
  SERIAL=$device_sn pio run -t upload -e $env_name
else
  pio run -t upload -e $env_name
fi
