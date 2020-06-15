import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterDisplay {
    parameterName: qsTr("PIP")
    parameterUnit: qsTr("cmH<sub>2</sub>O")
    parameterValue: GuiStateContainer.pip.toString()
}
