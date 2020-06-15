import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

ParameterButton {
    parameterName: qsTr("I-time")
    parameterUnit: "sec"
    parameterValue: GuiStateContainer.commanded_i_time
    onValueConfirmed: GuiStateContainer.commanded_i_time = value
    // Per https://respiraworks.slack.com/archives/C015FL7TSG2/p1592266276036600?thread_ts=1592264406.025800&cid=C015FL7TSG2
    parameterMinValue: 0.4
    parameterMaxValue: 3.3
    parameterStepSize: 0.1
    parameterDisplayFormatter: function (value) {
        return Number(value).toFixed(1)
    }
}
