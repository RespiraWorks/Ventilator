import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterButton {
    parameterName: qsTr("PEEP")
    parameterUnit: qsTr("cmH<sub>2</sub>0")
    // Per SR-08, RW-SYS-005
    parameterMinValue: 0
    parameterMaxValue: 20
    parameterStepSize: 1
    parameterValue: GuiStateContainer.commanded_peep
    onValueConfirmed: GuiStateContainer.commanded_peep = value
}
