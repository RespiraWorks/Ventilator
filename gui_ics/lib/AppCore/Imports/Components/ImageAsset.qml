import QtQuick 2.0

//PREVIEW IMAGE qrc:/DefaultComponentSet/PreviewAssets/imageAsset.png
Image {
    id: root

    //ASSET /placeholders/picture.png
    property string asset: "qrc:/assets/img.png"

    //HIDE property source
    source: asset
}
