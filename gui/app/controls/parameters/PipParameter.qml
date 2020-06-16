import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterButton {
    parameterName: qsTr("PIP")
    parameterUnit: qsTr("cmH<sub>2</sub>0")
    parameterValue: GuiStateContainer.commanded_pip
    onValueConfirmed: GuiStateContainer.commanded_pip = value
}
