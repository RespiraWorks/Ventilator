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

# This script should work no matter where you call it from.
pushd "$(dirname "$0")" > /dev/null || exit_fail

. ../software/common/base.sh

ensure_linux

OUTPUT_DIR=_build

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

install_linux() {
  install_py
  sudo apt update
  sudo apt install -y \
           make \
           docker.io \
           doxygen \
           graphviz
  docker pull lycheeverse/lychee
  echo "If you wish to use \`./docs.sh check\` to check validity of links locally, please follow installation instructions at https://github.com/lycheeverse/lychee"
}

build_all() {
  source "${HOME}/.profile"
  poetry install
  poetry run wiring -d ./wiring
  poetry run parts -f ./purchasing/parts.json
  poetry run make html
  touch ./_build/html/.nojekyll
}

launch_browser() {
  python3 -m webbrowser "${OUTPUT_DIR}/html/index.html"
}

check_links() {
  echo "If you wish to use \`./docs.sh check\` to check validity of links locally, please follow installation instructions at https://github.com/lycheeverse/lychee"
  docker run --init -it --rm -w /input -v $(pwd)/..:/input lycheeverse/lychee -c ./docs/lychee.toml .
#  lychee ..
}

########
# HELP #
########

if [ "$1" == "help" ] || [ "$1" == "-h" ]; then
  print_help
  exit_good

###########
# INSTALL #
###########
elif [ "$1" == "install" ]; then
  ensure_not_root
  install_linux
  exit_good

#########
# CLEAN #
#########
elif [ "$1" == "clean" ]; then
  clean_all
  exit_good

#########
# BUILD #
#########
elif [ "$1" == "build" ]; then
  ensure_not_root
  build_all
  exit_good

########
# VIEW #
########
elif [ "$1" == "view" ]; then
  ensure_not_root
  launch_browser
  exit_good

#########
# CHECK #
#########
elif [ "$1" == "check" ]; then
  ensure_not_root
  check_links
  exit_good

################
# ERROR & HELP #
################
else
  echo "ERROR: Bad command or insufficient parameters!"
  echo
  print_help
  exit_fail
fi
