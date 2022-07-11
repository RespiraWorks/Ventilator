# RespiraWorks Open Source Ventilator

[![Open Source](https://badges.frapsoft.com/os/v1/open-source.png?v=103)](open_source.md)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
[![CircleCI Build Status](https://circleci.com/gh/RespiraWorks/Ventilator.svg?style=shield)](https://circleci.com/gh/RespiraWorks/Ventilator/tree/master)
[![codecov](https://codecov.io/gh/RespiraWorks/Ventilator/branch/master/graph/badge.svg)](https://codecov.io/gh/RespiraWorks/Ventilator)
[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit&logoColor=white)](https://github.com/pre-commit/pre-commit)
[![Code style: clang-format](https://img.shields.io/badge/code%20style-clang--format-blue)](https://clang.llvm.org/docs/ClangFormat.html)
[![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black)
[![Built with: platformio](https://img.shields.io/badge/built%20with-platformio-orange)](https://platformio.org/)
[![GitHub commit activity](https://img.shields.io/github/commit-activity/m/RespiraWorks/Ventilator)](https://github.com/RespiraWorks/Ventilator/pulse)
[![Donate](https://img.shields.io/badge/donate-gofundme-blueviolet)](https://www.gofundme.com/f/RespiraWorks)

**If you intend to contribute, please read our [wiki](https://github.com/RespiraWorks/Ventilator/wiki).**

[RespiraWorks](https://respira.works/) is a 501(c)(3) non-profit organization with over 200 contributors worldwide working to design a low-cost and [open-source](open_source.md) ventilator for communities underserved by the global supply chain. Our objective is to provide the know-how and technology to enable local enterprises to manufacture and service these systems.


 MacOS:

    $ brew install nanopb-generator
    $ pip3 install protobuf
    $ NANOPB_PATH=$(brew --cellar nanopb-generator)/$(brew list --versions \
          nanopb-generator | awk '{print $2}')
    $ protoc \
        --plugin=$NANOPB_PATH/bin/protoc-gen-nanopb \
        -I $NANOPB_PATH/libexec/proto \
        -Icommon/generated_libs/network_protocol \
        --nanopb_out=common/generated_libs/network_protocol \
        --python_out=utils \
        common/generated_libs/network_protocol/network_protocol.proto



 Via Git:

    $ git clone https://github.com/nanopb/nanopb.git
    $ pip3 install protobuf

    $ protoc \
        --plugin=nanopb/generator       
        -I nanopb/generator/proto \
        -Icommon/generated_libs/network_protocol \
        --nanopb_out=common/generated_libs/network_protocol \
        --python_out=utils \
        common/generated_libs/network_protocol/network_protocol.proto

