import QtQuick 2.0
import GreenHouse.Theming 1.0

//PREVIEW IMAGE qrc:/DecorativeComponentSet/PreviewAssets/colored_image.png
Image {
    //COLOR shadowColor
    property color assetColor: "#80202020"
    //STRING /placeholders/picture.png
    property string assetId: "qrc:/assets/img.png"

    //HIDE property source
    source: GreenHouseThemeManager.theme.coloredAsset(assetId,
                                                      GreenHouseThemeManager.theme.colorId(assetColor))
}
