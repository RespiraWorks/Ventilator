import QtQuick 2.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/binaryAsset.png
Image {
    id: root

    property bool toggled: false
    //ASSET PLACEHOLDER
    property string toggledAsset: "qrc:/assets/img.png"
    //ASSET PLACEHOLDER
    property string asset: "qrc:/assets/img.png"

    //HIDE property source
    source: toggled ? toggledAsset : asset
}
