import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterButton {
    parameterName: qsTr("VIV")
    parameterUnit: qsTr("mL")
    // Per SR-08, RW-SYS-005
    parameterMinValue: 50
    parameterMaxValue: 600
    parameterStepSize: 25


    parameterValue: GuiStateContainer.commanded_viv
    onValueConfirmed: GuiStateContainer.commanded_viv = value
}
