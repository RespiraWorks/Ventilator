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
  install      One-time installation of build toolchain and dependencies
  clean        Clean build directories
  build        Generate documentation
  wire <path>  Render wiring harness diagrams for specified path, which may be a file or a directory
  view         Open generated documentation in browser
  check        Check for broken links
  help/-h      Display this help info
EOF
}

clean_all() {
  clean_dir ${OUTPUT_DIR}
  # \todo clean intermediate files from wiring and parts
}

install_linux() {
  install_py
  sudo apt update
  sudo apt --yes install \
       make \
       doxygen \
       graphviz
  sudo apt --yes install \
       apt-transport-https \
       ca-certificates \
       curl \
       software-properties-common
  curl -fsSL https://download.docker.com/linux/ubuntu/gpg \
    | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
  echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" \
    | sudo tee /etc/apt/sources.list.d/docker.list >/dev/null

  if [ "$(getent group docker)" ]; then
    echo "group docker already exists."
  else
    sudo addgroup docker
  fi

  sudo usermod -aG docker "${USER}"

  sudo apt update
  apt-cache policy docker-ce
  sudo apt --yes install docker-ce

  docker pull lycheeverse/lychee
}

process_wiring() {
  source "${HOME}/.profile"
  poetry install --no-root --quiet
  poetry run wiring "$1"
}

build_all() {
  source "${HOME}/.profile"
  poetry install --no-root --quiet
  poetry run wiring ./wiring
  poetry run parts ./purchasing/parts.json
  poetry run make html
}

launch_browser() {
  python3 -m webbrowser "${OUTPUT_DIR}/html/index.html"
}

check_links() {
  touch ./_build/html/.nojekyll
  docker run --init --rm -w /input -v "$(pwd)/..":/input lycheeverse/lychee -c ./docs/lychee.toml --no-ignore .
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

#########
# BUILD #
#########
elif [ "$1" == "wire" ]; then
  ensure_not_root
  process_wiring "$2"
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
