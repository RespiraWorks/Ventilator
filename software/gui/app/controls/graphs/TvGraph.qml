import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ScopeView {
    id: tidalVolumeView
    name: "Tidal Volume"
    unit: "mL"

    // RW-SYS-032 says it should be 0..1000.
    // We leave a bit of negative room because our flow correction algorithm is
    // not perfect and sometimes produces negative volume - it's nice to at
    // least be able to see what it is.
    yMin: -100
    yMax: 1000

    showBottomLine: false
    dataset: GuiStateContainer.tidalSeries
}
