import QtQuick 2.0

import QtQuick 2.11
import QtQuick.Controls 2.4
import Respira 1.0
import ".."

/*!
    \qmltype DeviceStatusDisplay
    \brief Display device status information such as battery and clock
*/
Row {
    width: 166; height: 24
    spacing: 4

    Row {
        id: clock
        width: 70; height: 22
        spacing: 4

        Text {
            id: time
            text: format(GuiStateContainer.clock.hour, GuiStateContainer.clock.minute)
            font: Style.clockFont
            color: "white"

            function format(hour, minute) {
                var twelveHour = hour > 12 ? hour -12 : hour
                var hStr = twelveHour.toString()
                var mStr = minute.toString()
                if (hour < 10) {
                    hStr = "0" + hour
                }

                if (minute < 10) {
                    mStr = "0" + minute
                }
                return "%1:%2".arg(hStr).arg(mStr)
            }
        }

        Text {
            id: period
            font: Style.clockFont
            color: "#AFAFAF"
            text: GuiStateContainer.clock.hour >= 12 ? "PM" : "AM"
        }
    }

    Text {
        width: 8; height: 22
        font: Style.clockFont
        color: "white"
        text: "•"
    }

    Row {
        id: battery
        spacing: 4

        Text {
            text: GuiStateContainer.batteryPercentage.toString()
            font: Style.clockFont
            color: "white"
        }

        Text {
            text: "%"
            font: Style.clockFont
            color: "#AFAFAF"
        }

        Item { width: 2; height: 1 }

        Image {
            width: 20; height: 14
            sourceSize: Qt.size(20, 14)
            anchors.verticalCenter: parent.verticalCenter
            fillMode: Image.PreserveAspectCrop
            source: "qrc:/images/" + batterySource(GuiStateContainer.batteryPercentage)

            function batterySource(percentage) {
                if (percentage > 75)
                    return "RW_battery-100_24.svg"
                if (percentage > 50)
                    return "RW_battery-75_24.svg"
                if (percentage > 25)
                    return "RW_battery-25_24.svg"
                return "RW_battery-100_24.svg"
            }
        }
    }
}
