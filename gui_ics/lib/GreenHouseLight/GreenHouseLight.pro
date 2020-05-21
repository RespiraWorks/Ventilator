include (../../globalflags.pri)
include (../../global.pri)
include (GreenHouseLight-local.pri)

TARGET = GreenHouseLight
TEMPLATE = lib

VERSION = $$GREENHOUSE_VERSION

DEFINES += GREENHOUSE_LIBRARY

QT += qml quick

SOURCES += \
    greenhousetheme.cpp \
    greenhouse_log.cpp \
    greenhousecontext.cpp \
    greenhousemodel.cpp \
    greenhouseviewmanager.cpp

HEADERS += \
    greenhousetheme.h \
    greenhouse_global.h \
    greenhouse_log.h \
    greenhousecontext.h \
    greenhousemodel.h \
    greenhouseviewmanager.h

RESOURCES += \
    builtinitems.qrc

DESTDIR = $$builddir/lib

unix {
    target.path = /usr/lib
    INSTALLS += target
}
