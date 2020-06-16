import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterButton {
    parameterName: qsTr("RR")
    parameterUnit: "b/min"
    parameterMaxValue: 30
    parameterMinValue: 10
    parameterStepSize: 1
    parameterValue: GuiStateContainer.commanded_rr
    onValueConfirmed: GuiStateContainer.commanded_rr = value
}
