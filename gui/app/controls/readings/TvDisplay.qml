import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterDisplay {
    parameterName: qsTr("TV")
    parameterUnit: qsTr("mL")
    // TODO: Instead of showing the current volume, should this
    // show *max* volume averaged over the past few breaths?  This
    // is what "TV" actually means, and I suspect it's much more
    // meaningful than showing the last-measured patient volume.
    parameterValue: GuiStateContainer.measured_tv.toFixed(0);
}
