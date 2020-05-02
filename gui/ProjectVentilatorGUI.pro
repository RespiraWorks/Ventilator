QT += core quick charts serialport
CONFIG += c++17
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += $$files("*.cpp") \
    ../common/generated_libs/network_protocol/network_protocol.pb.c \
    ../common/third_party/nanopb/pb_common.c \
    ../common/third_party/nanopb/pb_decode.c \
    ../common/third_party/nanopb/pb_encode.c
SOURCES += $$files("$$PWD/../common/**/*.c")
HEADERS += $$files("*.h") \
    ../common/generated_libs/network_protocol/network_protocol.pb.h \
    ../common/third_party/nanopb/pb.h \
    ../common/third_party/nanopb/pb_common.h \
    ../common/third_party/nanopb/pb_decode.h \
    ../common/third_party/nanopb/pb_encode.h
HEADERS += $$files("$$PWD/../common/**/*.h")
INCLUDEPATH += $$PWD/../common/third_party/nanopb
RESOURCES += qml.qrc Logo.png
DISTFILES += Logo.png
TRANSLATIONS += ProjectVentilatorGUI_es_GT.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
