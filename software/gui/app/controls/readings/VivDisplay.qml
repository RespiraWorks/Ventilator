import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterDisplay {
    parameterName: qsTr("VIV")
    parameterUnit: qsTr("ml")//"cmH<sub>2</sub>O")
    parameterValue: GuiStateContainer.measured_viv.toString()
    alarmPriority: Math.max(
      GuiStateContainer.alarmManager.vivExceededAlarm.effectiveVisualPriority,
      GuiStateContainer.alarmManager.vivNotReachedAlarm.effectiveVisualPriority)
}
