import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ScopeView {
    id: pressureView
    name: "Pressure"
    unit: "cmH<sub>2</sub>O"
    dataset: GuiStateContainer.pressureSeries
    // RW-SYS-003
    yMin: 0
    yMax: 60

    alarmPriority: Math.max(
      GuiStateContainer.alarmManager.pipExceededAlarm.effectiveVisualPriority,
      GuiStateContainer.alarmManager.pipNotReachedAlarm.effectiveVisualPriority)
}
