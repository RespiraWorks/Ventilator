# TODO see if I can minimize this
QT += core charts serialport

include( ../defaults.pri )
! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

TARGET = everything
TEMPLATE = lib
CONFIG += staticlib

SOURCES += $$files("*.cpp")
HEADERS += $$files("*.h")
