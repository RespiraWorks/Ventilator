QT += core testlib
CONFIG += qt warn_on depend_includepath testcase
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
SOURCES = $${TARGET}.cpp
