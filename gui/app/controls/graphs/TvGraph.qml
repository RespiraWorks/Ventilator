import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ScopeView {
    id: tidalVolumeView
    name: "Tidal Volume"
    unit: "mL"

    // RW-SYS-032
    yMin: 0
    yMax: 1000

    showBottomLine: false
    dataset: GuiStateContainer.tidalSeries
}
