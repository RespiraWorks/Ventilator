#include "setup.hpp"

#include <QQmlEngine>
#include <QQmlContext>

#include "waveformchartwidget.h"
#include "datamodelwaveform.h"
#include "filteredwaveformvaluedata.h"
#include "trendchartwidget.h"

namespace VentilatorUI {

void setup()
{
    // Runtime type registrations
    qmlRegisterType<WaveformChartWidget>("com.ics.waveformchart", 1, 0, "WaveformChartWidget");
    qmlRegisterType<DataModelWaveform>("com.ics.waveformchart", 1, 0, "DataModelWaveform");
    qmlRegisterType<FilteredWaveformValueData>("com.ics.waveformchart", 1, 0,
                                               "FilteredWaveformValueData");
    qmlRegisterType<TrendChartWidget>("com.ics.waveformchart", 1, 0, "TrendChartWidget");

    // Runtime setup
}

}
