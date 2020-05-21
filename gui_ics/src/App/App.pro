include (../../globalflags.pri)
include (../../global.pri)
include (../../lib/GreenHouseLight/GreenHouseLight.pri)
include (../../lib/AppCore/AppCore.pri)

include (VentilatorUI/VentilatorUI.App.pri)

TARGET = VentilatorUI
TEMPLATE = app
CONFIG += c++11

QT += qml quick

SOURCES += \
main.cpp \
integration.cpp

HEADERS += \
integration.h

RESOURCES += qml.qrc

DESTDIR = $$builddir/bin

# Additional import path used to resolve QML modules in Qt Creator's code model
# QML_IMPORT_PATH += ""

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include("../../deploy.pri")
qtcAddDeployment()
