import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtCharts 6.2
//import io.qt.serialdata


Page {
    id: dashboard
    anchors.fill: parent
    property font customFont: Qt.font({ family: "Segoe UI", pointSize: 4 })
    readonly property int windowSize: 50

    ColumnLayout{
        anchors.fill: parent
        RowLayout{
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle{
                Layout.fillWidth: true
                Layout.fillHeight: true
                                                ChartView{
                                    id: iriChartView
                                    anchors.fill: parent
                                    theme: ChartView.ChartThemeLight
                                    antialiasing: true
                                    title: " ADC en RT"
                                    legend.visible: false
                                    titleFont.family: dashboard.customFont.family
                                    titleFont.pointSize: 10

                                ValueAxis{
                                    id:axisYiri
                                    min: 0
                                    max: 10
                                    tickCount: 6
                                    //titleText: "IRI en RT"
                                    titleFont.family: dashboard.customFont.family
                                    titleFont.pointSize : 5
                                }

                                ValueAxis{
                                        id: axisXiriRealTime
                                        min:0
                                        max: 100
                                        tickCount: 8
                                        labelsVisible: false   
                                        gridVisible: false
                                }

                                LineSeries{
                                    id: iriGraphLine
                                    color: "#FF0019"
                                    axisX: axisXiriRealTime
                                    axisY: axisYiri

                                }
                                }
            }

            Rectangle{
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "green"
            }
        }
        RowLayout{
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle{
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "blue"
            }

            Rectangle{
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "purple"
            }
        }

        
    }

}
