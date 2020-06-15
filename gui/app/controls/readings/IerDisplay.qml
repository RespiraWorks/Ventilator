import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterDisplay {
    parameterName: qsTr("I:E")
    parameterUnit: qsTr("ratio")
    parameterValue: GuiStateContainer.ier.toFixed(1).toString()
}
