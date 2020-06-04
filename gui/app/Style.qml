pragma Singleton
import QtQuick 2.11

/*!
    \qmltype Style
    \brief Singleton defining all styles of the App.

    It will hold all UI style definitions into a single
    place, making it easy to everyone, including designers
    to contribute.

    For now its defining two themes: lightTheme and darkTheme.
    Those themes it will be defining colors, sizes and other
    style aspects.

    A propery theme will define the current used theme, that could
    be changed at runtime.
*/
QtObject {

    property var theme: darkTheme

    property var lightTheme: QtObject {
        // TODO get all values
    }

    property var darkTheme: QtObject {

        property QtObject color: QtObject {
            property color primary: "#466eeb"
            property color windowBackground: "#000713"
            property color modalBackground: "black"
            property color modalHeaderColor: "#1056A1"
            property color modalButton: "#132D4D"
            property color modalButtonHighlighted: "#498CCD"

            property color headerButton: "#192F42"
            property color headerButtonHighlighted: "#0056A1"

            property color radioButtonColor: "white"
            property color radioButtonHighlightedColor: "red"
        }

        property QtObject font: QtObject {

            // Font for number display on the stepped button
            property font headerButton: Qt.font({
                family: "Noto Sans",
                pointSize: 20,
                letterSpacing: 0,
                weight: Font.Normal
            })

            property font parameterDisplay: Qt.font({
                family: "Noto Sans",
                pointSize: 20,
                letterSpacing: 0,
                weight: Font.Normal
            })

            property font parameterDisplayNotation: Qt.font({
                family: "Noto Sans",
                pointSize: 16,
                letterSpacing: 0,
                weight: Font.Normal
            })

            property font parameterDisplayValue: Qt.font({
                family: "Oxygen",
                pointSize: 48,
                letterSpacing: 0,
                weight: Font.Normal
            })
        }

    }

    // TODO: should margins and fonts be part of a theme or a
    // separate entity?
    property QtObject margin: QtObject {
        property int normal: 10
    }

    // Font for number display on the stepped button
    property font stepButtonFont: Qt.font({
        family: "Times New Roman",
        weight: Font.DemiBold,
        bold: true,
        pixelSize: 38
    })
}
