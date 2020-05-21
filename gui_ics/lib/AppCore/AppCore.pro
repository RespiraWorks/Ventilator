include (../../globalflags.pri)
include (../../global.pri)
include (../../lib/GreenHouseLight/GreenHouseLight.pri)
include (AppCore-local.pri)
include (VentilatorUI/VentilatorUI.Core.pri)

TARGET = AppCore
TEMPLATE = lib

VERSION = $$APPCORE_VERSION

DEFINES += APPCORE_LIBRARY

QT += qml quick gui

SOURCES += \
    appcore_log.cpp \
    appcommandlineparser.cpp

HEADERS += \
    appcore_global.h \
    appcore_log.h \
    appcommandlineparser.h

RESOURCES += imports.qrc

DESTDIR = $$builddir/lib

unix {
    target.path = /usr/lib
    INSTALLS += target
}
