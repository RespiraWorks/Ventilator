QT += core quick charts serialport

! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

TARGET = ProjectVentilatorGUI


SOURCES += $$files("*.cpp")

HEADERS += $$files("*.h")

RESOURCES += qml.qrc images/Logo.png

DISTFILES += images/Logo.png

TRANSLATIONS += ProjectVentilatorGUI_es_GT.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
