#!/bin/bash

Xvfb :1 &
DISPLAY=:1 ./gui/build/ProjectVentilatorGUI -h
