import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterDisplay {
    parameterName: qsTr("I:E")
    parameterUnit: qsTr("ratio")
    parameterValue: {
      var inv = 1.0 / GuiStateContainer.measured_ier
      var invStr = inv.toFixed(1)
      // Strip the trailing .0 in the formatted number.
      // Javascript doesn't have a standard function like toFixed but
      // without trailing zeros.
      return "1:" + invStr.replace('\.0', '')
    }
}
