import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterButton {
    parameterName: qsTr("I-time")
    parameterUnit: "sec"
    parameterValue: Number(GuiStateContainer.ier).toFixed(1)
    parameterMaxValue: 1
    parameterMinValue: 0
    parameterStepSize: 0.05
    parameterDisplayFormatter: function (value) {
        return Number(value).toFixed(1)
    }
}
