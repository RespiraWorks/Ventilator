import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterDisplay {
    parameterName: qsTr("Flow")
    parameterUnit: qsTr("L/min")
    parameterValue: GuiStateContainer.flowReadout.toFixed(0)
}
