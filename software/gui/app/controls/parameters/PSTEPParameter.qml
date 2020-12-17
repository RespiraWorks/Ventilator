import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterButton {
    parameterName: qsTr("PSTEP")
    parameterUnit: qsTr("cmH<sub>2</sub>0")
    // Per SR-08, RW-SYS-005
    parameterMinValue: 1
    parameterMaxValue: 5
    parameterStepSize: 1

    parameterValue: GuiStateContainer.commanded_pstep
    onValueConfirmed: GuiStateContainer.commanded_pstep = value
}
