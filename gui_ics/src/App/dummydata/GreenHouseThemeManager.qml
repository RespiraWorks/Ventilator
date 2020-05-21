import QtQuick 2.0

QtObject {
    id: root

    property QtObject theme: QtObject {
        property var asset: function(name) {
            return "../../Themes/DefaultTheme" + name
        }
    }
}
