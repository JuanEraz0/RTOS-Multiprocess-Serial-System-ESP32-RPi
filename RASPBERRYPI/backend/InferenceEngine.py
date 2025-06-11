import os
import sys
import time
from datetime import datetime
from pathlib import Path
from queue import Queue

import cv2
import numpy as np
from PySide6.QtCore import QSize, QStandardPaths, QThread, Signal, Slot
from PySide6.QtGui import QImage, QPixmap
from PySide6.QtQml import QmlElement, QQmlImageProviderBase
from PySide6.QtQuick import QQuickImageProvider
from PySide6.QtQuickControls2 import QQuickStyle
from ultralytics import YOLO

#Macros for signal emmited to QML for each class:
FISURA_LONGITUDINAL = 0
RODERAS = 1
FISURA_COCODRILO = 2
REPARACION_BACHE = 3
DESPRENDIMIENTO = 4
ESTRIA = 5
FISURA_BLOQUE = 6
FISURA_TRANSVERSAL = 7
ROT_BOR = 8
EXUDACION_ASFALTO = 9
PELADURA = 10
CORRUGACION = 11
FISURA_DE_ARCO = 12
BACHE = 13
HUNDIMIENTO = 14
REFLEXION_DE_JUNTAS = 15
EXUDACION_DE_AGUA = 16
CORRIMIENTO = 17
HINCHAMIENTO = 18


# To be used on the @QmlElement decorator
# (QML_IMPORT_MINOR_VERSION is optional)
QML_IMPORT_NAME = "io.qt.textproperties"
QML_IMPORT_MAJOR_VERSION = 1

allimg = []
# index = 0
Result_in_queue_maxsize = 100
result_que = Queue(maxsize=Result_in_queue_maxsize)

# https://doc.qt.io/qt-6/qtquick-qmlmodule.html
# https://doc.qt.io/qtforpython-6/PySide6/QtQuick/index.html


class ThreadQ(QThread):
    """
    The thread which read frame from source and send it to the slot.

    read frame from video or image directory or camera.

    send frame signal to the slot of qml.
    """

    updateFrame = Signal(QPixmap)
    sendInferencedClass = Signal(int)

    def __init__(self, parent=None):
        """Initialize variables."""
        QThread.__init__(self, parent)
        #super.__init__(super,parent)
        self.trained_file = None
        self.status = True
        self.cap = True
        self.model = "yolo11n.pt"
        self.input = "camera"
        self.video = ""
        self.checked = False
        self.videowriter = True
        self.image_stop = False
        self.m = None
        w, h = 300, 300
        image = np.ones((w, h, 3),dtype=np.uint8) * 255 
        image = QImage(image.data, w, h, 3 * w, QImage.Format_RGB888)
        self.zeros = QPixmap.fromImage(image)
    
    def set_input(self, fname: str):
        """Set input."""
        self.input = fname

    def set_model(self):
        """Initialize yolo model."""
        if self.m is not None:
            del self.m
            import gc
            gc.collect()
        self.m = YOLO(self.model)
        



    """
    #########################
    method 1
    #########################
    """

    def run(self):
        """Read frame from video or camera or directory then send the frame signal to the slot of qml."""
        self.set_model()
        if self.input == "camera":
            self.cap = cv2.VideoCapture(0)
        elif self.input == "video":
            self.cap = cv2.VideoCapture(self.video)

        if self.checked:
            fps = 24
            sizes = (640, 500)
            if not isinstance(self.cap, bool):
                fps = self.cap.get(cv2.CAP_PROP_FPS)
                sizes = (int(self.cap.get(cv2.CAP_PROP_FRAME_WIDTH)), int(self.cap.get(cv2.CAP_PROP_FRAME_HEIGHT)))
            if fps <= 0:
                fps = 24
            # fps = 10
            movies_location = QStandardPaths.writableLocation(QStandardPaths.MoviesLocation)
            if "/" in movies_location:
                movies_location = movies_location.replace("/", os.sep)
            if "\\" in movies_location:
                movies_location = movies_location.replace("\\", os.sep)
            date = datetime.now().isoformat().__str__().replace(".", "_").replace(":", "_")
            outpath = movies_location + os.sep + date + ".avi"
            self.videowriter = cv2.VideoWriter(outpath, cv2.VideoWriter_fourcc(*"XVID"), fps, sizes)
        # cnt = 0
        if self.input in ["camera", "video"]:
            ret = True
            nu = 0
            while ret:
                if self.image_stop:
                    self.updateFrame.emit(self.zeros)
                    break
                ret, frame = self.cap.read()
                if not ret:
                    continue

                result = self.m.predict(frame, batch=1, stream=False)
                frame = result[0].plot()
                # cv2.putText(frame, str(fps)+"_"+str(sizes), (100, 100), cvfont, 0.5, [255, 0, 0], 1)
                # cv2.putText(frame, str(nu), (360, 360), cvfont, 2, [255, 0, 0], 1)
                nu += 1
                if self.checked:
                    self.videowriter.write(frame)
                # out = r'C:\Users\10696\Desktop\CV\ZouJiu1\MagicTime'
                # cv2.imwrite(os.path.join(out, str(cnt)+".jpg"), frame)
                # cnt += 1
                color_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

                # # Creating and scaling QImage
                h, w, ch = color_frame.shape
                img = QImage(color_frame.data, w, h, ch * w, QImage.Format_RGB888)
                detected_classes = result[0].boxes
                dclasses = [int(j) for j in detected_classes.cls.tolist()]
                for x in dclasses:
                    if x == 0:
                        self.sendInferencedClass.emit(FISURA_LONGITUDINAL)
                        
                    elif x == 1: 
                        self.sendInferencedClass.emit(RODERAS)
                       
                    elif x == 2: 
                        self.sendInferencedClass.emit(FISURA_COCODRILO)
                    
                    elif x == 3:
                        self.sendInferencedClass.emit(REPARACION_BACHE)
                    elif x == 4:
                        self.sendInferencedClass.emit(DESPRENDIMIENTO)
                    elif x == 5:
                        self.sendInferencedClass.emit(ESTRIA)
                    elif x == 6:
                        self.sendInferencedClass.emit(FISURA_BLOQUE)
                    elif x == 7:
                        self.sendInferencedClass.emit(FISURA_TRANSVERSAL)
                    elif x == 8:
                        self.sendInferencedClass.emit(ROT_BOR)
                    elif x == 9:
                        self.sendInferencedClass.emit(EXUDACION_ASFALTO)
                    elif x == 10:
                        self.sendInferencedClass.emit(PELADURA)
                    elif x == 11: 
                        self.sendInferencedClass.emit(CORRUGACION)
                    elif x == 12:
                        self.sendInferencedClass.emit(FISURA_DE_ARCO)
                    elif x == 13: 
                        self.sendInferencedClass.emit(BACHE)
                    elif x == 14:
                        self.sendInferencedClass.emit(HUNDIMIENTO)
                    elif x == 15: 
                        self.sendInferencedClass.emit(REFLEXION_DE_JUNTAS)
                    elif x == 16: 
                        self.sendInferencedClass.emit(EXUDACION_DE_AGUA)
                    elif x == 17: 
                        self.sendInferencedClass.emit(CORRIMIENTO)
                    elif x == 18:
                        self.sendInferencedClass.emit(CORRUGACION)
                    else:
                        continue

                # Emit signal

                self.updateFrame.emit(QPixmap.fromImage(img))
            # sys.exit(-1)
        else:
            # outpath = r'C:\Users\10696\Desktop\CV\ZouJiu1\Pytorch_YOLOV3\log\output'
            # nu = 0
            for i in os.listdir(self.video):
                if ".jpg" in i or ".jpeg" in i or ".png" in i or ".bmp" in i:
                    allimg.append(i)
            for i in allimg:
                if self.image_stop:
                    self.updateFrame.emit(self.zeros)
                    break
                frame = cv2.imread(os.path.join(self.video, i))
                result = self.m.predict(frame, batch=1, stream=False)
                frame = result[0].plot()



                # cv2.imwrite(os.path.join(outpath, str(nu)+".jpg"), frame)
                # nu += 1
                if self.checked:
                    img = cv2.resize(frame, sizes)
                    self.videowriter.write(img)
                color_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

                # Creating and scaling QImage
                h, w, ch = color_frame.shape
                img = QImage(color_frame.data, w, h, ch * w, QImage.Format_RGB888)
                # Emit signal
                self.updateFrame.emit(QPixmap.fromImage(img))
                # allresult.append(result)
        if self.checked:
            self.videowriter.release()
            self.videowriter = True


# updateModel = Signal(str)
# @Slot()
# def open_model(path:QUrl):
#     global updateModel
#     model = path.toString()[2**3:]
#     updateModel.emit(model)


@QmlElement  # 装饰器，表示信号传递给这些槽，下面的函数都是槽，信号都在qml文件中
# class Camera_video(QObject):
class Camera_video(QQuickImageProvider):
    """Including slot functions, bind with qml slots."""

    imageChange = Signal(bool)
    classDetected = Signal(int)


    def __init__(self):
        """Initialize variables."""
        # super().__init__(QQuickImageProvider)
        super().__init__(QQmlImageProviderBase.ImageType.Pixmap)
        # Thread in charge of updating the image
        self.th = ThreadQ(self)
        self._model = "yolo11n.pt"
        #updateModel.connect(self.get_model)
        self.th.updateFrame.connect(self.updateImage)
        self.th.sendInferencedClass.connect(self.onClassDetected)
        # self.th.finished.connect(self.setlast)
        # self.nowstatus = self.saveState()

        # inpath = r"C:\Users\10696\Desktop\CV\ZouJiu1\Pytorch_YOLOV3\log\val"
        # self.imagelist = [os.path.join(inpath, i) for i in os.listdir(inpath)]
        self.pixmap = self.th.zeros
    
    
    def requestPixmap(self, id="image_elementll", size=QSize(0, 0), requestedSize=QSize(0, 0)):
        """Qml function, which send frame to qml."""
        # w, h = 640, 600
        # # image = np.zeros((w, h, 3)) * 255
        # image = cv2.imread(np.random.choice(self.imagelist, 1)[0])
        # image = cv2.resize(image, (w, h))
        # h, w, ch = image.shape
        # image = QImage(image.data, w, h, ch * w, QImage.Format_RGB888)
        # pixmap = QPixmap.fromImage(image)
        return self.pixmap
    
    @Slot(QPixmap)
    def updateImage(self, image):
        """A slot which receive frame from thread and send signal to qml'slot."""
        self.pixmap = image
        self.imageChange.emit(True)
    
    @Slot(int)
    def onClassDetected(self, class_id:int):
        """Recibe la clase detectada y la reenvía a QML"""
        self.classDetected.emit(class_id) 
    
    @Slot(str, result=None)
    def get_model(self, model):
        """A slot which receive signal from qml'FileDialog."""
        self.th.model = model.replace("file:///", "")

    @Slot(str, result=None)
    def get_video(self, video):
        """A slot which receive signal from qml'FileDialog."""
        self.th.video = video.replace("file:///", "")

    @Slot(str, result=None)
    def get_type(self, type):
        """A slot which receive signal from qml'Button."""
        self.th.input = type

    @Slot(bool)
    def get_checked(self, ischecked):
        """A slot which receive signal from qml'CheckBox."""
        self.th.checked = ischecked

    @Slot()
    def start(self):
        """A slot which receive signal from qml'Button."""
        self.th.start()
        # self.showFullScreen()

    @Slot()
    def kill_thread(self):
        """A common function called by stop() terminate the thread and reinitialize variable."""
        print("Finishing... PixMapThread")
        if not isinstance(self.th.cap, bool):
            self.th.cap.release()
        if self.th.checked:
            if not isinstance(self.th.videowriter, bool):
                self.th.videowriter.release()
        self.th.status = False
        cv2.destroyAllWindows()
        self.status = False
        # Give time for the thread to finish
        time.sleep(1)
        #self.th.terminate()
        self.th.quit()
        self.th.wait()
        self.th.cap = True
        self.th.videowriter = True
        self.th.image_stop = False

    @Slot()
    def stop(self):
        """A slot which receive signal from qml'Button."""
        self.th.image_stop = True
        self.kill_thread()

