import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterDisplay {
    parameterName: qsTr("FiO<sub>2</sub>")
    parameterUnit: qsTr("%")
    parameterValue: GuiStateContainer.measured_fio2_percent.toFixed(0)
}
