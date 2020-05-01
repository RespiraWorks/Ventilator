QT += core quick charts
CONFIG += c++17
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += $$files("*.cpp")
SOURCES += $$files("$$PWD/../common/**/*.c")
HEADERS += $$files("*.h")
HEADERS += $$files("$$PWD/../common/**/*.h")
INCLUDEPATH += $$PWD/../common/third_party/nanopb
RESOURCES += qml.qrc Logo.png
DISTFILES += Logo.png
TRANSLATIONS += ProjectVentilatorGUI_es_GT.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
