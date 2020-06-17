include( ../defaults.pri )
! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}
TEMPLATE = app

QT += testlib gui
CONFIG += qt warn_on depend_includepath testcase

SOURCES += tst_main.cpp
HEADERS += \
  breath_signals_test.h \
  latching_alarm_test.h

LIBS += -L../src -leverything
