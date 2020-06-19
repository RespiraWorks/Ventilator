import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterDisplay {
    parameterName: qsTr("I:E")
    parameterUnit: qsTr("ratio")
    parameterValue: {
      var inv = 1.0 / GuiStateContainer.measured_ier
      return "1:" + inv.toFixed(Number.isInteger(inv) ? 0 : 1)
    }
}
