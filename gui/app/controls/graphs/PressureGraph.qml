import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ScopeView {
    id: pressureView
    name: "Pressure"
    unit: "cmH<sub>2</sub>O"
    // TODO: Are these reasonable lower and upper bounds?
    // Source for current value:
    // https://www.rtmagazine.com/public-health/pediatrics/neonatal/selecting-appropriate-ventilator-parameters/
    // mentions values in the range 5-30 cmH2O.
    dataset: GuiStateContainer.pressureSeries
    yMin: -3
    yMax: 30

    alarmPriority: Math.max(
      GuiStateContainer.alarmManager.pipExceededAlarm.effectiveVisualPriority,
      GuiStateContainer.alarmManager.pipNotReachedAlarm.effectiveVisualPriority)
}
