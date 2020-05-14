HEADERS += \
    $$PWD/surfacecontroller.hpp\
    $$PWD/ventilatorparametersinterface.hpp\
    $$PWD/setup.hpp

SOURCES += \
    $$PWD/surfacecontroller.cpp\
    $$PWD/ventilatorparametersinterface.cpp\
    $$PWD/setup.cpp

RESOURCES += $$PWD/VentilatorUI_private.qrc
include (MedicalPlotting.pri)
