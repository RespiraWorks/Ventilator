include( ../defaults.pri )
! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}
TEMPLATE = app

QT += testlib gui multimedia
CONFIG += qt warn_on depend_includepath testcase

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

SOURCES += tst_main.cpp
HEADERS += \
  breath_signals_test.h \
  latching_alarm_test.h \
  patient_detached_alarm_test.h

LIBS += -L../src -leverything
