include( ../defaults.pri )
! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}
TEMPLATE = app

QT += testlib gui
CONFIG += qt warn_on depend_includepath testcase

SOURCES += tst_main.cpp
HEADERS += max_pressure_alarm_test.h

LIBS += -L../src -leverything
