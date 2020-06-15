import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterButton {
    parameterName: qsTr("Peep")
    parameterUnit: qsTr("cmH<sub>2</sub>0")
    parameterMaxValue: 20
    parameterMinValue: 0
    parameterStepSize: 2
    parameterValue: GuiStateContainer.peep
    onValueConfirmed: GuiStateContainer.peep = value
}
