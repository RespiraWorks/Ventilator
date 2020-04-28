QT += core quick charts
CONFIG += c++17
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += *.cpp
HEADERS += *.h
RESOURCES += qml.qrc Logo.png
DISTFILES += Logo.png
TRANSLATIONS += ProjectVentilatorGUI_es_GT.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
