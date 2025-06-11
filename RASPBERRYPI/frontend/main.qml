import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts


ApplicationWindow {
    width: 1280
    height: 720
    visible: true
    //visibility: "FullScreen"
    title: qsTr("PROYECTO FINAL RTOS")

    StackView{
        id: applicationStackView
        anchors.fill: parent
        initialItem: "../frontend/components/Dashboard.qml"
    }

}
