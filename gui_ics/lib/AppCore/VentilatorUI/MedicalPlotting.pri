QT += network

HEADERS += \
    $$PWD/datamodelwaveform.h \
    $$PWD/filteredsample.h \
    $$PWD/filteredwaveformvaluedata.h \
    $$PWD/global_constants.h \
    $$PWD/medicalplottingcomponentplugin_log.h \
    $$PWD/waveformchartwidget.h \
    $$PWD/waveformfilter.h \
    $$PWD/waveformsample.h \
    $$PWD/sweeptimer.h \
    $$PWD/datareader.h \
    $$PWD/datasocket.h \
    $$PWD/trendchartwidget.h

SOURCES += \
    $$PWD/datamodelwaveform.cpp \
    $$PWD/filteredsample.cpp \
    $$PWD/filteredwaveformvaluedata.cpp \
    $$PWD/medicalplottingcomponentplugin_log.cpp \
    $$PWD/waveformfilter.cpp \
    $$PWD/waveformsample.cpp \
    $$PWD/sweeptimer.cpp \
    $$PWD/waveformchartwidget.cpp \
    $$PWD/datareader.cpp \
    $$PWD/datasocket.cpp \
    $$PWD/trendchartwidget.cpp

RESOURCES += \
    $$PWD/demo_data.qrc
