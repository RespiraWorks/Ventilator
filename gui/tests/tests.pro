QT += testlib gui

CONFIG += qt warn_on depend_includepath testcase

! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}


TEMPLATE = app

SOURCES +=  \
    goodbyetest.cpp \
    hellotest.cpp \
    tst_main.cpp

HEADERS += \
    goodbyetest.h \
    hellotest.h
