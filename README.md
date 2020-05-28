# RespiraWorks Ventilator Software

[![CircleCI Build Status](https://circleci.com/gh/RespiraWorks/VentilatorSoftware.svg?style=shield)](https://circleci.com/gh/RespiraWorks/VentilatorSoftware/tree/master)
[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit&logoColor=white)](https://github.com/pre-commit/pre-commit)
[![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black)
[![Code style: clang-format](https://img.shields.io/badge/code%20style-clang--format-blue)](https://clang.llvm.org/docs/ClangFormat.html)
[![Built with: platformio](https://img.shields.io/badge/built%20with-platformio-orange)](https://platformio.org/)
[![GitHub commit activity](https://img.shields.io/github/commit-activity/m/RespiraWorks/VentilatorSoftware)](https://github.com/RespiraWorks/VentilatorSoftware/pulse)

This is a common repository for all software components of the [RespiraWorks](http://respira.works) open source ventilator project.

##Repository structure

* [boards](boards) - files for platformio to build for the stm32 chip
* [common](common) - code common to both controller and GUI executables
* [controller](controller) - code for pneumatic system controller (stm32)
* [gui](gui) - code for the ventilator graphical interface (Qt)
* [sample-data](sample-data) - some sample data for something **TODO: explain**
* [scripts](scripts) - for testing something **TODO: explain**
* [utils](utils) - python scripts for debugging controller

To contribute, please check the [onboarding wiki](https://github.com/RespiraWorks/VentilatorSoftware/wiki).
