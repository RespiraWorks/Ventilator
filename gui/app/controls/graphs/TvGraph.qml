import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ScopeView {
    id: tidalVolumeView
    name: "Tidal Volume"
    unit: "mL"

    // Software requirement 49 says TV should be in the range of
    // 0..3000 mL.
    yMin: -3000
    yMax: 3000

    showBottomLine: false
    dataset: GuiStateContainer.tidalSeries
}
