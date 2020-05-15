import QtQuick 2.0
import QtLocation 5.9
import QtPositioning 5.9
import GreenHouse.Theming 1.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/direction_icon.png
Item {
    id: root

    width: 200
    height: 200

    property int directionId: 0
    Image {
        id: icon
        anchors.fill: parent

        fillMode: Image.PreserveAspectFit

        source: {
            var asset = ""
            switch (root.directionId) {
            default:
            case RouteManeuver.NoDirection:
            case RouteManeuver.DirectionForward:
                asset = "/directions/direction_continue.png"
                break;
            case RouteManeuver.DirectionBearRight:
                asset = "/directions/direction_continue_right.png"
                break;
            case RouteManeuver.DirectionLightRight:
                asset = "/directions/direction_turn_slight_right.png"
                break;
            case RouteManeuver.DirectionRight:
                asset = "/directions/direction_turn_right.png"
                break;
            case RouteManeuver.DirectionHardRight:
                asset = "/directions/direction_turn_sharp_right.png"
                break;
            case RouteManeuver.DirectionUTurnRight:
                asset = "/directions/direction_uturn_right.png"
                break;
            case RouteManeuver.DirectionUTurnLeft:
                asset = "/directions/direction_uturn_left.png"
                break;
            case RouteManeuver.DirectionBearLeft:
                asset = "/directions/direction_continue_left.png"
                break;
            case RouteManeuver.DirectionLightLeft:
                asset = "/directions/direction_turn_slight_left.png"
                break;
            case RouteManeuver.DirectionLeft:
                asset = "/directions/direction_turn_left.png"
                break;
            case RouteManeuver.DirectionHardLeft:
                asset = "/directions/direction_turn_sharp_left.png"
                break;
            }
            return GreenHouseThemeManager.theme.asset(asset)
        }
    }
}
