#!/bin/bash

# Fail if any command fails
set -e
set -o pipefail

# Print each command as it executes
set -o xtrace

# This script should work no matter where you call it from.
cd "$(dirname "$0")"

create_clean_directory(){
  dir_name=$1
  if [ -d "$dir_name" ]; then
    echo "Removing $dir_name"
    rm -rf "$dir_name"
  elif [ -f "$dir_name" ]; then
    echo "File with this name already exists, not a directory."
    exit
  fi
  if mkdir "$dir_name"; then
    echo "Clean directory created: $dir_name"
    return 0
  else
    echo "Creating directory failed: $dir_name"
    return 1
  fi
}

create_clean_directory build
qmake -unset QMAKEFEATURES
cd build
qmake ..
make -j
