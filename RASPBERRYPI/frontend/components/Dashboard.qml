import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtCharts 6.2
import io.qt.textproperties
import io.qt.gyroscopedata

Page {
    id: dashboard
    anchors.fill: parent
    property font customFont: Qt.font({ family: "Segoe UI", pointSize: 4 })
    readonly property int windowSize: 50
    
    //InferencedClassesCounters

    property int counterInferencedBache: 0
    property int counterInferencedCorrimiento: 0
    property int counterInferencedCorrugado: 0
    property int counterInferencedDesprendimiento: 0
    property int counterInferencedEstria: 0
    property int counterInferencedExudacionAsfalto: 0
    property int counterInferencedExudacionAgua: 0
    property int counterInferencedFisuraBloque: 0
    property int counterInferencedFisuraCocodrilo: 0
    property int counterInferencedFisuraArco: 0
    property int counterInferencedFisuraLongitudinal: 0
    property int counterInferencedFisuraTransversal: 0
    property int counterInferencedHinchamiento: 0
    property int counterInferencedHundimiento: 0
    property int counterInferencedPeladura: 0
    property int counterInferencedReflexionDeJuntas: 0
    property int counterInferencedReparacionBache: 0
    property int counterInferencedRoderas: 0
    property int counterInferencedRotBor: 0

    function updateLabelBasedOnClass(classId) {
        switch(classId) {
            case 0:
                counterInferencedFisuraLongitudinal += 1
                break
            case 1:
                counterInferencedRoderas += 1
                break
            case 2:
                counterInferencedFisuraCocodrilo += 1
                break
            case 3:
                counterInferencedReparacionBache += 1
                break
            case 4:
                counterInferencedDesprendimiento += 1
                break
            case 5:
                counterInferencedEstria += 1
                break
            case 6:
                counterInferencedFisuraBloque += 1
                break
            case 7:
                counterInferencedFisuraTransversal += 1
                break
            case 8:
                counterInferencedRotBor += 1
                break
            case 9:
                counterInferencedExudacionAsfalto += 1
                break
            case 10:
                counterInferencedPeladura += 1
                break
            case 11:
                counterInferencedCorrugado += 1
                break
            case 12:
                counterInferencedFisuraArco += 1
                break
            case 13:
                counterInferencedBache += 1
                break
            case 14:
                counterInferencedHundimiento += 1
                break
            case 15:
                counterInferencedReflexionDeJuntas += 1
                break
            case 16:
                counterInferencedExudacionAgua += 1
                break
            case 17:
                counterInferencedCorrimiento += 1
                break
            case 18:
                counterInferencedHinchamiento += 1
                break
            default:
                console.log("Clase no reconocida:", classId)
        }
    }





    RowLayout{
        id: rowLayoutDashboard
        anchors.fill: parent

        Rectangle{
            id: cameraInferenceDataArea
            Layout.fillWidth:true
            Layout.fillHeight: true
            Layout.preferredWidth: 2
            ColumnLayout{
                id: cameraInferenceDataColumnLayout
                anchors.fill: parent
                Rectangle{
                    radius: 10
                    id: cameraInferenceArea
                    color: "transparent"
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight: 8
                    Layout.leftMargin: 2
                    View{

                    }
                }
                       
                Rectangle{
                    id: inferencedRTData
                    //radius: 5
                    Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.margins: 0.5
                           // border.width: 1
                           // Layout.preferredWidth: 2
                           
                            ColumnLayout{
                                anchors.fill: parent
                                anchors.margins: 5
                                Text{
                                    font.family: gridLayoutInferenceClasses.inferencedClassesFont.family
                                    font.bold: true
                                    text: "Detalles de recorrido"
                                    font.pointSize: 8
                                }

                                GridLayout{
                                    id: gridLayoutInferencedRTData
                                    columns: 4 
                                    
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    //Layout.margins: 2
                                    //Layout.preferredHeight: 2
                                    columnSpacing: 20
                                    rowSpacing: 5
                                    RowLayout{
                                        id: inferencedRTDataLongitude
                                        Layout.fillWidth: true


                                        Text{
                                            text: "Longitud:"
                                            font: gridLayoutInferenceClasses.inferencedClassesFont

                                        }
                                        Label{
                                            text: "-99.005235 "
                                            font: gridLayoutInferenceClasses.inferencedClassesFont
                                        }
                                    }
                                    RowLayout{
                                        id: inferencedRTDataAccelZ
                                        Layout.fillWidth: true


                                        Text{
                                            text: "AccelZ:"
                                            font: gridLayoutInferenceClasses.inferencedClassesFont
                                        }
                                        Label{
                                            id: labelInferencedRTDataAccelZ
                                            text: " "
                                            font: gridLayoutInferenceClasses.inferencedClassesFont
                                        }
                                    }
                                    RowLayout{
                                        id: inferencedRTDataAccelX
                                        Layout.fillWidth: true


                                        Text{
                                            text: "AccelX:"
                                            font: gridLayoutInferenceClasses.inferencedClassesFont
                                        }
                                        Label{
                                            text: " "
                                            id: labelInferencedRTDataAccelX
                                            font: gridLayoutInferenceClasses.inferencedClassesFont
                                        }
                                    }
                                    RowLayout{
                                        id: inferencedRTDataAccelY
                                        Layout.fillWidth: true


                                        Text{
                                            text: "AccelY:"
                                            font: gridLayoutInferenceClasses.inferencedClassesFont
                                        }
                                        Label{
                                            id: labelInferencedRTDataAccelY
                                            font: gridLayoutInferenceClasses.inferencedClassesFont
                                        }
                                    }
                                    RowLayout{
                                        id: inferencedRTDataLatitude
                                        Layout.fillWidth: true


                                        Text{
                                            text: "Latitud:"
                                            font: gridLayoutInferenceClasses.inferencedClassesFont
                                        }
                                        Label{
                                            text: "19.6050944 " 
                                            font: gridLayoutInferenceClasses.inferencedClassesFont
                                        }
                                    }
                                    RowLayout{
                                        id: inferencedRTDataVelocity
                                        Layout.fillWidth: true


                                        Text{
                                            text: "Velocidad:"
                                            font: gridLayoutInferenceClasses.inferencedClassesFont
                                        }
                                        Label{

                                            font: gridLayoutInferenceClasses.inferencedClassesFont
                                        }
                                    }
                                    RowLayout{
                                        id: inferencedRTDataTimeStamp
                                        Layout.fillWidth: true


                                        Text{
                                            text: "TimeStamp:"
                                            font: gridLayoutInferenceClasses.inferencedClassesFont
                                        }
                                        Label{
                                            id: labelInferencedRTDataTimeStamp
                                            font: gridLayoutInferenceClasses.inferencedClassesFont
                                        }
                                    }
                                    RowLayout{
                                        id: inferencedRTDataIri
                                        Layout.fillWidth: true


                                        Text{
                                            text: "IRI:"
                                            font: gridLayoutInferenceClasses.inferencedClassesFont
                                        }
                                        Label{
                                            id: labelInferencedRTDataIRI
                                            font: gridLayoutInferenceClasses.inferencedClassesFont
                                        }
                                    }
                                }
                            }

                        }  
                Rectangle{
                    id: dataInferenceArea
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight: 2
                    RowLayout{
                        anchors.fill: parent

                        Rectangle{
                            radius: 5
                            id: inferenceClassificationCounts
                            Layout.fillWidth: true
                            Layout.fillHeight:true
                            Layout.preferredWidth:6
                            //border.width: 1
                            Layout.margins: 2

                            GridLayout{
                                property font inferencedClassesFont : Qt.font({ family: "Segoe UI", pointSize: 5.5, bold: true })
                                id: gridLayoutInferenceClasses
                                anchors.fill: parent
                                columns: 4
                                anchors.margins:2
                                RowLayout{
                                    id: layoutInferencedBache
                                    Layout.fillWidth: true

                                    IndicatorCircle{
                                        color: circleColor[0]
                                    }

                                    Text{
                                        text: "BACHE"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id: labelValueInferencedBache
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedBache
                                    }

                                }
                                RowLayout{
                                    id: layoutInferencedCorrimiento
                                    Layout.fillWidth: true
                                    IndicatorCircle{
                                        color: circleColor[1]
                                    }
                                    Text{
                                        text: "CORRIMIENTO"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id:  labelValueInferencedCorrimiento
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedCorrimiento
                                    }

                                }
                                RowLayout{
                                    id: layoutInferencedCorrugado
                                    Layout.fillWidth: true
                                    IndicatorCircle{
                                        color: circleColor[2]
                                    }
                                    Text{
                                        text: "CORRUGADO"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id: labelValueInferencedCorrugado
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedCorrugado
                                    }

                                }
                                RowLayout{
                                    id: layoutInferencedDesprendimiento
                                    Layout.fillWidth: true
                                    IndicatorCircle{
                                        color: circleColor[3]
                                    }
                                    Text{
                                        text: "DESPRENDIMIENTO"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id: labelValueInferencedDesprendimiento
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedDesprendimiento
                                    }

                                }
                                RowLayout{
                                    id: layoutInferencedEstria
                                    Layout.fillWidth: true
                                    IndicatorCircle{
                                        color: circleColor[4]
                                    }

                                    Text{
                                        text: "ESTRIA"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id: labelValueInferencedEstria
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedEstria
                                    }

                                }
                                RowLayout{
                                    id: layoutInferencedExudacionAsfalto
                                    Layout.fillWidth: true
                                    IndicatorCircle{
                                        color: circleColor[5]
                                    }
                                    Text{
                                        text: "EXUDACIÓN ASFALTO"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id: labelValueInferencedExudacionAsfalto
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedExudacionAsfalto
                                    }

                                }
                                RowLayout{
                                    id: layoutInferencedExudacionAgua
                                    Layout.fillWidth: true
                                    IndicatorCircle{
                                        color: circleColor[6]
                                    }

                                    Text{
                                        text: "EXUDACION DE AGUA"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id: labelValueInferencedExudacionAgua
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedExudacionAgua
                                    }

                                }
                                RowLayout{
                                    id: layoutInferencedFisuraBloque
                                    Layout.fillWidth: true
                                    IndicatorCircle{
                                        color: circleColor[7]
                                    }

                                    Text{
                                        text: "FISURA BLOQUE"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id: labelValueInferencedFisuraBloque
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedFisuraBloque
                                    }

                                }
                                RowLayout{
                                    id: layoutInferencedFisuraCocodrilo
                                    Layout.fillWidth: true
                                    IndicatorCircle{
                                        color: circleColor[8]
                                    }

                                    Text{
                                        text: "FISURA COCODRILO"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id: labelValueInferencedFisuraCocodrilo
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedFisuraCocodrilo
                                    }

                                }
                                RowLayout{
                                    id: layoutInferencedFisuraArco
                                    Layout.fillWidth: true
                                    IndicatorCircle{
                                        color: circleColor[9]
                                    }

                                    Text{
                                        text: "FISURA DE ARCO"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id: labelValueInferencedFisuraArco
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedFisuraArco
                                    }

                                }
                                RowLayout{
                                    id: layoutInferencedFisuraLongitudinal
                                    Layout.fillWidth: true
                                    IndicatorCircle{
                                        color: circleColor[10]
                                    }

                                    Text{
                                        text: "FISURA LONGITUDINAL"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id: labelValueInferencedFisuraLongitudinal
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedFisuraLongitudinal
                                    }

                                }
                                RowLayout{
                                    id: layoutInferencedFisuraTransversal
                                    Layout.fillWidth: true
                                    IndicatorCircle{
                                        color: circleColor[11]
                                    }

                                    Text{
                                        text: "FISURA TRANSVERSAL"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id: labelValueInferencedFisuraTransversal
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedFisuraTransversal
                                    }

                                }
                                RowLayout{
                                    id: layoutInferencedHinchamiento
                                    Layout.fillWidth: true
                                    IndicatorCircle{
                                        color: circleColor[12]
                                    }

                                    Text{
                                        text: "HINCHAMIENTO"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id: labelValueInferencedHinchamiento
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedHinchamiento
                                    }

                                }
                                RowLayout{
                                    id: layoutInferencedHundimiento
                                    Layout.fillWidth: true
                                    IndicatorCircle{
                                        color: circleColor[13]
                                    }

                                    Text{
                                        text: "HUNDIMIENTO"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id: labelValueInferencedHundimiento
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedHundimiento
                                    }

                                }
                                RowLayout{
                                    id: layoutInferencedPeladura
                                    Layout.fillWidth: true
                                    IndicatorCircle{
                                        color: circleColor[14]
                                    }

                                    Text{
                                        text: "PELADURA"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id: labelValueInferencedPeladura
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedPeladura
                                    }

                                }
                                RowLayout{
                                    id: layoutInferencedReflexionDeJuntas
                                    Layout.fillWidth: true
                                    IndicatorCircle{
                                        color: circleColor[15]
                                    }

                                    Text{
                                        text: "REFLEXIÓN DE JUNTAS"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id: labelValueInferencedReflexionDeJuntas
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedReflexionDeJuntas
                                    }

                                }
                                RowLayout{
                                    id: layoutInferencedReparacionBache
                                    Layout.fillWidth: true
                                    IndicatorCircle{
                                        color: circleColor[16]
                                    }

                                    Text{
                                        text: "REPARACION BACHE"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id: labelValueInferencedReparacionBache
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedReparacionBache
                                    }

                                }
                                RowLayout{
                                    id: layoutInferencedRoderas
                                    Layout.fillWidth: true
                                    IndicatorCircle{
                                        color: circleColor[17]
                                    }

                                    Text{
                                        text: "RODERAS"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id: labelValueInferencedRoderas
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedRoderas
                                    }

                                }
                                RowLayout{
                                    id: layoutInferencedRotBor
                                    Layout.fillWidth: true
                                    IndicatorCircle{
                                        color: circleColor[18]
                                    }

                                    Text{
                                        text: "ROT BOR"
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                    }
                                    Label{
                                        id: labelValueInferencedRotBor
                                        font: gridLayoutInferenceClasses.inferencedClassesFont
                                        text: counterInferencedRotBor
                                    }

                                }

                            }

                            Connections{
                                target: my_image_provider
                                function onClassDetected(class_id){
                                    updateLabelBasedOnClass(class_id)
                                }
                            }

                        }
 




                    }
                }
            }
        }
        Item{
            id: graphDataArea
            Layout.fillWidth:true
            Layout.fillHeight: true

            ColumnLayout{
                anchors.fill: parent
                Rectangle{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight: 10
                    //color: "blue"
                    ColumnLayout{
                        anchors.fill: parent
                        Item{
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.preferredHeight: 3
                            StackView{
                                id: graphAreaStackView
                                anchors.fill: parent
                                initialItem: iriComponentChartView
                                replaceEnter: Transition {
                                    PropertyAnimation{
                                        property: "opacity"
                                        from: 0
                                        to: 1
                                        duration: 200

                                    }
                                }
                                replaceExit: Transition {
                                    PropertyAnimation{
                                        property: "opacity"
                                        from: 1
                                        to: 0
                                        duration: 200

                                    }
                                }

                            }   
                        }
                        Component{
                            id: iriComponentChartView
                            Rectangle{
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.preferredHeight: 3

                                ChartView{
                                    id: iriChartView
                                    anchors.fill: parent
                                    theme: ChartView.ChartThemeLight
                                    antialiasing: true
                                    title: " IRI en RT"
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
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked:{
                                        graphAreaStackView.replace(accelerationsComponentChartView)
                                    }
                                    
                                }
                                Connections{
                                    target: gyroscope_worker
                                    function onGyroDataListener(ax,ay,az,wx,wy,wz,rollx,pitchy,yawz,iri){
                                        var xValue1 = iriGraphLine.count;
                                        iriGraphLine.append(xValue1,iri);
                                        if (iriGraphLine.count > 100) {
                                            iriGraphLine.remove(0, 1); // Elimina el punto más antiguo
                                            axisXiriRealTime.min += 1;
                                            axisXiriRealTime.max += 1;
                                        }
                                        
                                    }
                                }

                            
                            
                            }
                            

                        }

                        Component{
                            id: accelerationsComponentChartView
                            Rectangle{
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.preferredHeight: 3

                            ChartView{
                                id: accelerationsChartView
                                anchors.fill: parent
                                theme: ChartView.ChartThemeLight
                                antialiasing: true
                                title: " Aceleraciones en RT"
                                legend.visible: false
                                titleFont.family: dashboard.customFont.family
                                titleFont.pointSize: 10
                                DateTimeAxis{
                                    id: axisXiri
                                    gridVisible: true
                                    format: "ss"
                                    tickCount: 8
                                    //titleText: "Tiempo"
                                    titleFont.family: dashboard.customFont.family
                                    titleFont.pointSize: 5

                                }
                                ValueAxis{
                                    id:axisYaccelerations
                                    min: -10
                                    max: 20
                                    tickCount: 6
                                    //titleText: "IRI en RT"
                                    titleFont.family: dashboard.customFont.family
                                    titleFont.pointSize : 5

                                }
                                ValueAxis{
                                    id: axisX
                                    min:0
                                    max: 100
                                    tickCount: 8
                                    labelsVisible: false   
                                    gridVisible: false
                                }

                                LineSeries{
                                    id: accelerationsZGraphLine
                                    color: "blue"
                                    axisX: axisX
                                    axisY: axisYaccelerations
                                }
                                LineSeries{
                                    id: accelerationsYGraphLine
                                    color: "green"
                                    axisX: axisX

                                    axisY: axisYaccelerations
                                }
                                LineSeries{
                                    id: accelerationsXGraphLine
                                    color: "red"
                                    axisX: axisX

                                    axisY: axisYaccelerations
                                }
                                }
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked:{
                                        graphAreaStackView.replace(iriComponentChartView)
                                    }
                                    
                                }
                                Connections{
                                    target: gyroscope_worker
                                    function onGyroDataListener(ax,ay,az,wx,wy,wz,rollx,pitchy,yawz,iri){
                                        var xValue1 = accelerationsZGraphLine.count;
                                        accelerationsZGraphLine.append(xValue1,az);
                                        accelerationsXGraphLine.append(xValue1,ax);
                                        accelerationsYGraphLine.append(xValue1,ay);

                                        if (accelerationsZGraphLine.count > 100) {
                                            accelerationsZGraphLine.remove(0, 1);
                                            accelerationsXGraphLine.remove(0, 1);
                                            accelerationsYGraphLine.remove(0, 1);
                                            axisX.min += 1;
                                            axisX.max += 1;
                                        }
                                        
                                    }
                                }
                            }
                            
                        }


                        Item{
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                              
                        }
                        Item{
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            RowLayout{
                                anchors.fill: parent
                                Button{
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 50
                                    Layout.preferredWidth: 50
                                    font.family: dashboard.customFont.family
                                    font.pointSize: 10
                                    text: " Iniciar "
                                    onClicked:{
                                        my_image_provider.start()
                                        gyroscope_worker.start()

                                    }
                                }
                                Button{
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 50
                                    Layout.preferredWidth: 50
                                    font.family: dashboard.customFont.family
                                    font.pointSize: 10
                                    text: " Detener "
                                    onClicked:{
                                        my_image_provider.stop()
                                        //gyroscope_worker.stop()
                                    }
                                }
                            }
                        }
                        Item{
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Button{
                                width: 100
                                height: 50
                                anchors.centerIn: parent
                                font.family: dashboard.customFont.family
                                font.pointSize: 10
                                text: " Regresar "
                                onClicked:{
                                    applicationStackView.push("MainWindow.qml")
                                }

                            }
                        }
                    }
                }
                Rectangle{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.bottomMargin: 1

                    LogoJflow{
                        fillMode: Image.PreserveAspectFit
                    }
                }

            }
        }
    
        Dialog{
            width: 300
            height: 300
            id: bleConnectionDialog
            title: "Conexión BLE"
            modal: true
            anchors.centerIn: parent

            Timer {
                id: autoCloseTimer
                interval: 2000  // 3 segundos
                running: false
                repeat: false
                onTriggered: bleConnectionDialog.close()
            }
            
            contentItem: Rectangle {
            anchors.fill: parent
            color: "white"

            Label {
                id: bleConnectionDialogText
                anchors.centerIn: parent
                font.family: dashboard.customFont.family
                font.pointSize: 10
            }
            }
        }
    }
    Connections{
        target: gyroscope_worker
        function onTimeStampListener(timeStamp){
            labelInferencedRTDataTimeStamp.text = timeStamp
        }
        function onGyroDataListener(ax,ay,az,wx,wy,wz,rollx,pitchy,yawz,iri){
            labelInferencedRTDataAccelZ.text= az
            labelInferencedRTDataAccelX.text= ax
            labelInferencedRTDataAccelY.text= ay
            labelInferencedRTDataIRI.text= iri
        }
        function onConnectionBLEStatusMessage(message){
            bleConnectionDialogText.text= message
        }
        function onConnectionBLEStatus(status){
            if(status){
                bleConnectionDialogText.color = "green"          
                autoCloseTimer.restart()
                
            }else{
                
                bleConnectionDialogText.color = "red"
                
            }
             bleConnectionDialog.open()  

        }

    }


}
