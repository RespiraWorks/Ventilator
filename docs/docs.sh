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

OUTPUT_DIR=_build

# Fail if any command fails
set -e
set -o pipefail

# Print each command as it executes
if [ -n "$VERBOSE" ]; then
  set -o xtrace
fi

# This script should work no matter where you call it from.
cd "$(dirname "$0")"

EXIT_FAILURE=1
EXIT_SUCCESS=0

# Check if Linux
PLATFORM="$(uname -s)"
if [ $PLATFORM != "Linux" ]; then
  echo "Error: This script only supports 'Linux'. You have $PLATFORM."
  exit $EXIT_FAILURE
fi

print_help() {
    cat <<EOF
Utility script for generating Ventilator documentation.

The following options are available:
  install     One-time installation of build toolchain and dependencies
  clean       Clean build directories
  build       Generate documentation
  view        Open generated documentation in browser
  check       Check for broken links
  help/-h     Display this help info
EOF
}

clean_all() {
  clean_dir ${OUTPUT_DIR}
  # \todo clean intermediate files from wiring and parts
}

clean_dir() {
  dir_name=$1
  if [ -d "$dir_name" ]; then
    echo "Removing $dir_name"
    rm -rf "$dir_name"
    return $EXIT_SUCCESS
  elif [ -f "$dir_name" ]; then
    echo "File with this name already exists, not a directory."
    return $EXIT_FAILURE
  fi
}

install_linux() {
  sudo apt-get update
  sudo apt-get install -y \
               make \
               doxygen \
               graphviz \
               python3-pip
  pip3 install -U pip
  pip3 install sphinx-rtd-theme breathe sphinx-sitemap mlx.traceability wireviz pandas
  source ${HOME}/.profile
  echo "If you wish to use \`./docs.sh check\` to check validity of links locally, please follow installation instructions at https://github.com/lycheeverse/lychee"
}

build_all() {
  source ${HOME}/.profile
  ./wiring/make_wiring.sh
  ./purchasing/make_parts.sh
  make html
}

launch_browser() {
  python3 -m webbrowser "${OUTPUT_DIR}/html/index.html"
}

check_links() {
  echo "If you wish to use \`./docs.sh check\` to check validity of links locally, please follow installation instructions at https://github.com/lycheeverse/lychee"
  lychee ..
}

########
# HELP #
########

if [ "$1" == "help" ] || [ "$1" == "-h" ]; then
  print_help
  exit $EXIT_SUCCESS

###########
# INSTALL #
###########
elif [ "$1" == "install" ]; then
  if [ "$EUID" -eq 0 ] && [ -z "$FORCED_ROOT" ]; then
    echo "Please do not run install with root privileges!"
    exit $EXIT_FAILURE
  fi
  install_linux
  exit $EXIT_SUCCESS

#########
# CLEAN #
#########
elif [ "$1" == "clean" ]; then
  clean_all
  exit $EXIT_SUCCESS

########
# MAKE #
########
elif [ "$1" == "build" ]; then
  if [ "$EUID" -eq 0 ] && [ -z "$FORCED_ROOT" ]; then
    echo "Please do not run build with root privileges!"
    exit $EXIT_FAILURE
  fi
  build_all
  exit $EXIT_SUCCESS

########
# VIEW #
########
elif [ "$1" == "view" ]; then
  if [ "$EUID" -eq 0 ] && [ -z "$FORCED_ROOT" ]; then
    echo "Please do not run view with root privileges!"
    exit $EXIT_FAILURE
  fi
  launch_browser
  exit $EXIT_SUCCESS

#########
# CHECK #
#########
elif [ "$1" == "check" ]; then
  if [ "$EUID" -eq 0 ] && [ -z "$FORCED_ROOT" ]; then
    echo "Please do not run view with root privileges!"
    exit $EXIT_FAILURE
  fi
  check_links
  exit $EXIT_SUCCESS

################
# ERROR & HELP #
################
else
  echo "ERROR: Bad command or insufficient parameters!"
  echo
  print_help
  exit $EXIT_FAILURE
fi
