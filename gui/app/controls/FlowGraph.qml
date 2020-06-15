import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ScopeView {
    id: flowView
    name: "Flow"
    unit: "L/min"
    // TODO: Are these reasonable lower and upper bounds?
    // Source for current value:
    // https://www.sciencedirect.com/topics/medicine-and-dentistry/peak-inspiratory-flow
    // "Most modern ventilators can deliver flow rates between
    // 60 and 120 L/min. "
    yMin: -150
    yMax: 150
    dataset: GuiStateContainer.flowSeries
}
