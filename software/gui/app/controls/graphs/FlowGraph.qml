import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ScopeView {
    id: flowView
    name: "Flow"
    unit: "L/min"
    // https://respiraworks.slack.com/archives/C011UMNUWGZ/p1592606104221700?thread_ts=1592603466.221100&cid=C011UMNUWGZ
    yMin: -120
    yMax: 120
    dataset: GuiStateContainer.flowSeries
}
