import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterButton {
    parameterName: qsTr("FiO<sub>2</sub>")
    parameterUnit: "%"
    // RW-SYS-072 requests 21-100% in increments of 5%.
    parameterMinValue: 21
    parameterMaxValue: 100
    parameterStepSize: 5
    parameterValue: GuiStateContainer.commanded_fio2_percent
    onValueConfirmed: GuiStateContainer.commanded_fio2_percent = value
    parameterDisplayFormatter: function (value) {
      return Number(value).toFixed(0)
    }
    parameterFixup: function(value) {
      return Math.max(parameterMinValue, value - value % parameterStepSize)
    }
}
