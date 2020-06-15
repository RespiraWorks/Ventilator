import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterButton {
    parameterName: qsTr("FiO<sub>2</sub>")
    parameterUnit: "%"
    parameterMaxValue: 100
    parameterMinValue: 21
    parameterStepSize: 0.5
    parameterValue: 10
    onValueConfirmed: parameterValue = value
}
