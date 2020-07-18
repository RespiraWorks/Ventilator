#!/bin/bash

# Run the GUI App on Raspberry Pi using EGLFS mode
#
# It assumes the project is already built on folder "build"
# and it should ran without have the graphical interface (X11)
# started on the rPi.

# Provides Rendering debug information
QT_LOGGING_RULES=qt.qpa.*=true

# Tells Qt which display to be used.
DISPLAY=:0

# Run GUI APP forcing to use eglfs with a particular configuration
# defined in kms.json
QT_QPA_EGLFS_KMS_CONFIG=kms.json QT_QPA_EGLFS_INTEGRATION=eglfs_kms build/app/ProjectVentilatorGUI -platform eglfs
