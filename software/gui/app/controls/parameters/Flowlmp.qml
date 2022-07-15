import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterButton {
    parameterName: qsTr("FLOW")
    parameterUnit: qsTr("L/min")
    // Per SR-08, RW-SYS-005
    parameterMinValue: 1
    parameterMaxValue: 60
    parameterStepSize: 1
    parameterValue: GuiStateContainer.commanded_flow
    onValueConfirmed: GuiStateContainer.commanded_flow = value
}
