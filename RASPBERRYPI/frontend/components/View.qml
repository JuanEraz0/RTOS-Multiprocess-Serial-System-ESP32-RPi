import QtQuick
import QtQuick.Controls
import io.qt.textproperties

Image {
    id: imageInferencedManager
    anchors.fill: parent
    //anchors.centerIn: parent
    fillMode: Image.PreserveAspectFit
    source: "image://My_image_provider/img"
    cache: false
    property int counter: 0

    QtObject {
        id: backend
        property int modifier
    }

    signal model_fileOpened(path: url)

    function reLoadImage(){
        source = "image://My_image_provider/img?id=" + counter
        counter ++
        counter%=999999999
    }


    Connections { //https://doc.qt.io/qt-6/qtqml-syntax-signals.html
        target: my_image_provider
        function onImageChange(image) {
            // console.log("emit")
            imageInferencedManager.reLoadImage()
        }
    }

}
