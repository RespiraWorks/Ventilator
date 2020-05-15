import QtQuick 2.0
import QtLocation 5.9
import QtPositioning 5.9

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/map.png
Item {
    id: root

    width: 256
    height: 256

    property double centerLat: 50.822
    property double centerLon: 19.0587
    property double currentLat: 50.822
    property double currentLon: 19.0587
    property int markerImageXOffset: 0
    property int markerImageYOffset: 0
    property real markerImageRotation: 0
    property int destinationMarkerImageXOffset: 0
    property int destinationMarkerImageYOffset: 0
    property bool destinationMarkerVisible: true
    property real zoomLevel: 16
    property bool markerVisible: true
    property string accessToken: "DEVELOPER"
    property string styleUrls: "mapbox://styles/mapbox/navigation-guidance-night-v4"
    //DELEGATE
    property Component markerItem: null
    //DELEGATE
    property Component destinationMarkerItem: null
    property int mapTypeIndex: 0
    property real routeLineWidth: 8.0
    //COLOR TEXTCOLOR
    property color routeLineColor: "gray"
    property var routePointList: []
    property bool offlineRouteEnabled: false

    property string mappingPluginName: "mapboxgl"
    property string routingPluginName: "mapbox"
    property string mappingPluginCacheFolderParameter: ""
    property string mappingPluginCacheFolderName: ""
    property bool travelByCar: true
    property bool travelByFoot: false
    property bool travelByBicycle: false
    property bool travelByPublicTransit: false
    property bool travelByTruck: false

    signal zoomLevelSync(real zoomLevel)
    signal routeDetailsChanged(string segmentList)
    signal pressAndHoldCoordinates(double lat, double lon)
    signal centerLatitudeSync(double lat)
    signal centerLongitudeSync(double lon)


    Loader {
        id: mapLoader
        anchors.fill: parent

    }
    onAccessTokenChanged: {
        mapLoader.sourceComponent = null
        mapLoader.sourceComponent = mapComponent
    }
    onStyleUrlsChanged: {
        mapLoader.sourceComponent = null
        mapLoader.sourceComponent = mapComponent
    }
    Component.onCompleted: mapLoader.sourceComponent = mapComponent

    Component {
        id: mapComponent

        Item {
            anchors.fill: parent
            Plugin {
                id: mapPlugin

                name: root.mappingPluginName
                PluginParameter {
                    //we are using duplicate of accessTokenParameter instead of cache parameter, if it is not set
                    name: root.mappingPluginCacheFolderParameter === "" ? accessTokenParameter.name
                                                                        : root.mappingPluginCacheFolderParameter
                    value: root.mappingPluginCacheFolderName === "" ? accessTokenParameter.value
                                                                    : root.mappingPluginCacheFolderName
                }
                PluginParameter {
                    id: accessTokenParameter
                    name: root.mappingPluginName + ".access_token"
                    value: root.accessToken
                }
                PluginParameter {
                    name: root.mappingPluginName + ".mapping.additional_style_urls"
                    value: root.styleUrls
                }
            }

            Plugin {
                id: routePlugin

                name: root.routingPluginName
                PluginParameter {
                    name: root.routingPluginName + ".access_token"
                    value: root.accessToken
                }
            }

            RouteQuery {
                id: routeQuery
            }

            RouteModel {
                id: routeModel

                query: routeQuery
                autoUpdate: false
                property double destinationLat: root.currentLat
                property double destinationLon: root.currentLon

                onStatusChanged: {
                    var segmentList = "[";
                    if (status === RouteModel.Ready && count > 0) {
                        var route = get(0);
                        for (var i = 0; i < route.segments.length; ++i) {
                            if (i !== 0)
                                segmentList += ",";
                            segmentList += "{ \"direction\": " + route.segments[i].maneuver.direction
                            + ", \"distance\": " + route.segments[i].distance
                            + ", \"distanceToNextInstruction\": " + route.segments[i].maneuver.distanceToNextInstruction
                            + ", \"instructionText\": \"" + route.segments[i].maneuver.instructionText + "\"}"
                        }
                        var pathLenght = route.path.length
                        if(pathLenght > 0) {
                            destinationLat = route.path[pathLenght-1].latitude
                            destinationLon = route.path[pathLenght-1].longitude
                        }
                    }
                    segmentList += "]";
                    root.routeDetailsChanged(segmentList);
                }
            }

            Map {
                id: map

                anchors.fill: parent

                activeMapType: mapPlugin ? supportedMapTypes[root.mapTypeIndex] : null
                property Route offlineRoute: Route { }

                MapItemView {
                    id: routeView
                    visible: root.offlineRouteEnabled || routeModel.count > 0 //to avoid 'Layer not found' warning. QTBUG-73285
                    model: root.offlineRouteEnabled ? 1 : routeModel
                    delegate: MapRoute {
                        route: root.offlineRouteEnabled ? map.offlineRoute : routeData
                        smooth: true
                        line.color: root.routeLineColor
                        line.width: root.routeLineWidth
                        visible: routeView.visible
                    }
                }

                MapQuickItem {
                    id: destinationMarker

                    coordinate: QtPositioning.coordinate(routeModel.destinationLat, routeModel.destinationLon)
                    anchorPoint.x: root.destinationMarkerImageXOffset
                    anchorPoint.y: root.destinationMarkerImageYOffset
                    visible: root.destinationMarkerVisible && (routeModel.count > 0 || root.offlineRouteEnabled)

                    sourceItem: root.destinationMarkerItem ? root.destinationMarkerItem.createObject(map) : null
                }

                MapQuickItem {
                    id: currentLocationMarker

                    coordinate: QtPositioning.coordinate(root.currentLat, root.currentLon)
                    anchorPoint.x: root.markerImageXOffset
                    anchorPoint.y: root.markerImageYOffset
                    rotation: root.markerImageRotation - map.bearing
                    visible: root.markerVisible

                    sourceItem: root.markerItem ? root.markerItem.createObject(map) : null
                }

                //NOTE: Needed to prevent the outside binding to loop when we report----------------
                //internally triggered zoom or center changes (mouse wheel, pinch to zoom etc.)---------------
                property bool __zoomLevelInSync: true
                onZoomLevelChanged: {
                    if (__zoomLevelInSync)
                        root.zoomLevelSync(zoomLevel)
                }
                Connections {
                    target: root
                    onZoomLevelChanged: {
                        map.__zoomLevelInSync = false
                        map.zoomLevel = root.zoomLevel
                        map.__zoomLevelInSync = true
                    }
                }
                property bool __centerInSync: true
                onCenterChanged: {
                    if (__centerInSync) {
                        root.centerLatitudeSync(map.center.latitude)
                        root.centerLongitudeSync(map.center.longitude)
                    }
                }
                Connections {
                    target: root
                    onCenterLatChanged: {
                        map.__centerInSync = false
                        if (map.center.latitude !== root.centerLat)
                            map.center = QtPositioning.coordinate(root.centerLat, root.centerLon)
                        map.__centerInSync = true
                    }
                    onCenterLonChanged: {
                        map.__centerInSync = false
                        if (map.center.longitude !== root.centerLon)
                            map.center = QtPositioning.coordinate(root.centerLat, root.centerLon)
                        map.__centerInSync = true
                    }
                }
                //----------------------------------------------------------------------------------

                function updateRoute() {
                    routeQuery.clearWaypoints();
                    if (root.offlineRouteEnabled) {
                        routeModel.reset()
                        var resultPath = []
                        for (var j = 0; j < root.routePointList.length; ++j) {
                            var c = root.routePointList[j];
                            if (c.latitude === undefined || c.longitude === undefined)
                                continue;
                            resultPath.push(QtPositioning.coordinate(c.latitude, c.longitude))
                            //setting last point to destination
                            if (j+1 == root.routePointList.length) {
                                routeModel.destinationLat = c.latitude
                                routeModel.destinationLon = c.longitude
                            }
                        }
                        map.offlineRoute.path = resultPath
                        return;
                    }
                    for (var i = 0; i < root.routePointList.length; ++i) {
                        var coord = root.routePointList[i];
                        if (coord.latitude === undefined || coord.longitude === undefined)
                            continue;
                        routeQuery.addWaypoint(QtPositioning.coordinate(coord.latitude, coord.longitude))
                    }
                    routeQuery.travelModes = 0;
                    if (root.travelByCar)
                        routeQuery.travelModes |= RouteQuery.CarTravel
                    if (root.travelByFoot)
                        routeQuery.travelModes |= RouteQuery.PedestrianTravel
                    if (root.travelByBicycle)
                        routeQuery.travelModes |= RouteQuery.BicycleTravel
                    if (root.travelByPublicTransit)
                        routeQuery.travelModes |= RouteQuery.PublicTransit
                    if (root.travelByTruck)
                        routeQuery.travelModes |= RouteQuery.TruckTravel
                    if (routeQuery.waypoints.length > 0) {
                        routeModel.update()
                    } else {
                        routeModel.reset()
                    }
                }
                Connections {
                    target: root
                    onRoutePointListChanged: { map.updateRoute() }
                    onTravelByCarChanged: { map.updateRoute() }
                    onTravelByFootChanged: { map.updateRoute() }
                    onTravelByBicycleChanged: { map.updateRoute() }
                    onTravelByPublicTransitChanged: { map.updateRoute() }
                    onTravelByTruckChanged: { map.updateRoute() }
                    onOfflineRouteEnabledChanged: { map.updateRoute() }
                }
                MouseArea {
                    anchors.fill: parent
                    onPressAndHold:  {
                        var coord = map.toCoordinate(Qt.point(mouseX, mouseY));
                        root.pressAndHoldCoordinates(coord.latitude, coord.longitude);
                    }
                }
            }

            Component.onCompleted: {
                map.plugin = mapPlugin
                routeModel.plugin = routePlugin
                map.zoomLevel = root.zoomLevel
                map.center = QtPositioning.coordinate(root.centerLat, root.centerLon)
                map.updateRoute()
            }
        }
    }

}
