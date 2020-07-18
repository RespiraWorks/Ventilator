CONFIG += c++1z

DEFINES += QT_DEPRECATED_WARNINGS


top_srcdir=$$PWD
# top_builddir=$$shadowed($$PWD)

SOURCES += \
    $$top_srcdir/../common/generated_libs/network_protocol/network_protocol.pb.c \
    $$top_srcdir/../common/third_party/nanopb/pb_common.c \
    $$top_srcdir/../common/third_party/nanopb/pb_decode.c \
    $$top_srcdir/../common/third_party/nanopb/pb_encode.c \
    $$top_srcdir/../common/libs/units/units.cpp \
    $$files("$$top_srcdir//../common/**/*.c")

HEADERS += \
    $$top_srcdir/../common/generated_libs/network_protocol/network_protocol.pb.h \
    $$top_srcdir/../common/third_party/nanopb/pb.h \
    $$top_srcdir/../common/third_party/nanopb/pb_common.h \
    $$top_srcdir/../common/third_party/nanopb/pb_decode.h \
    $$top_srcdir/../common/third_party/nanopb/pb_encode.h \
    $$top_srcdir/../common/libs/units/units.h

HEADERS += $$files("$$top_srcdir/../common/**/*.h")

INCLUDEPATH += \
    $$top_srcdir/../common/generated_libs/network_protocol \
    $$top_srcdir/../common/third_party/nanopb \
    $$top_srcdir/../common/libs/units
