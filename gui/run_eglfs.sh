#!/bin/bash

# Run the GUI App on Raspberry Pi using EGLFS mode
#
# It assumes the project is already built on folder "build"
# and it should ran without have the graphical interface (X11)
# started on the rPi.

# Provides Rendering debug information
export QT_LOGGING_RULES=qt.qpa.*=true

# Tells Qt which display to be used.
export DISPLAY=:0

# Please note, the configuration variable for specific HDMI mode is removed
# because it changes according to the display you plug your raspberry. To
# force a specific mode, uncomment the line below and update kms.json to
# use the desired mode
#export QT_QPA_EGLFS_KMS_CONFIG=kms.json
export QT_QPA_EGLFS_INTEGRATION=eglfs_kms

# Run GUI APP forcing to use eglfs
build/ProjectVentilatorGUI -platform eglfs
