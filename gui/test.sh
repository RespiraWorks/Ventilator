#!/bin/bash

Xvfb :1 &
DISPLAY=:1 ./build/ProjectVentilatorGUI -d
DISPLAY=:1 ./build/ProjectVentilatorGUI -h
