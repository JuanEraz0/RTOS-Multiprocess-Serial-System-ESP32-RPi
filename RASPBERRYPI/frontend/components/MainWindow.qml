import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtCharts 6.2
import io.qt.textproperties


Page {
    id: mainWindowPage
    anchors.fill: parent
    property font customFont: Qt.font({ family: "Segoe UI", pointSize: 15 })

    Rectangle{
        id: mainWindowOptionsArea
        anchors.fill: parent
        ColumnLayout{
            anchors.centerIn: parent
            width: 300
            height: 300
            Item{
                Layout.fillWidth: true
                Layout.fillHeight: true
                    LogoJflow{
                        fillMode: Image.PreserveAspectFit
                    }
            }
            Item{
                Layout.fillWidth: true
                Layout.fillHeight: true
                ColumnLayout{
                    anchors.fill: parent
                    Rectangle{
                        //radius: 5
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Text{
                            text: "Check Up Carretero"
                            font.family: mainWindowPage.customFont.family
                            font.bold: true
                            font.pointSize: 15
                            anchors.centerIn: parent
                        }
                    }
                    Rectangle{
                        radius: 5
                        Layout.preferredHeight: 8
                        //border.width: 2
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        Column {
                            anchors.topMargin: 20
                            anchors.fill: parent
                            spacing: 20
                            Button {
                                text: " Inspección de carretera "
                                width: 200
                                height: 50
                                font.family: mainWindowPage.customFont.family
                                font.pointSize: 10
                                anchors.horizontalCenter: parent.horizontalCenter
                                onClicked:{
                                    my_image_provider.get_type("video")
                                    my_image_provider.get_model("/mnt/499829ea-d324-4eb8-9da7-058982331148/JFLOW/checkUpCarreteroPyBackEnd/models/inspeccionCarretera/best.pt")
                                    my_image_provider.get_video("/mnt/499829ea-d324-4eb8-9da7-058982331148/JFLOW/checkUpCarreteroPyBackEnd/assets/videos/psv.MP4")
                                    my_image_provider.get_checked(false)
                                    console.log("modelo y videos cargados")
                                    //my_image_provider.start()
                                    //my_image_provider.stop()
                                    startCheckUpTimer.start()

                                }
                            }
                            Button {
                                text: "Señalamiento vertical"
                                width: 200
                                height: 50
                                font.family: mainWindowPage.customFont.family
                                font.pointSize: 10
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            Timer {
                                id: startCheckUpTimer
                                interval: 500
                                onTriggered:{
                                    //my_image_provider.start()
                                    //my_image_provider.stop()
                                    //my_image_provider.start()
                                    applicationStackView.push("Dashboard.qml")  
                                }

                            }
                            Timer{
                                id: startVerticalSignsTimer
                                interval: 1000
                            }
                        }

                    }
                }
            }



        }
    }



}