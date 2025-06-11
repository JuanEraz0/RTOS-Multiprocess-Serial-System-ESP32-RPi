""" Class to get WT901BLECL5 Gyro+Accel data via BLE,  
    Data analysis to obtain IRI through WT901BLECL5 obtained data 
    
    JFLOW ECOSYSTEMS SA de CV
"""


from bluepy.btle import Peripheral, Scanner, ADDR_TYPE_RANDOM, BTLEDisconnectError, DefaultDelegate
from PySide6.QtCore import QThread, QObject, Signal, Slot, QDateTime
from PySide6.QtQml import QmlElement
import time
import datetime

#macros

# this UUID is the only one with permissions of Write/Read in the device, 
# characteristic needed to access in order to get the data from the ble device 
UUID_NOTIFICATION = "0000FFE4-0000-1000-8000-00805F9A34FB"

#MAC address of the device

WT901BLECL5_MAC_ADRESS = "EE:D8:AE:FC:A6:A1"
WT901BLECL5_CONNECTION_STATUS = False
SECONDS_SHOW_ON_GRAPH = 60
AVERAGE_VELOCITY_KMH = 80

CONST_K1 = 220000  # Constante para el resorte primario (neumático)
CONST_K2 = 0.15  # Constante para el resorte secundario (suspensión)
CONST_C1 = 0.05  # Constante para el amortiguador primario
CONST_C2 = 0.1  # Constante para el amortiguador secundario
    
# Parámetros del cuarto de carro
CONST_M1 = 40  # Masa no suspendida (kg)
CONST_M2 = 320  # Masa suspendida (kg)

QML_IMPORT_NAME = "io.qt.gyroscopedata"
QML_IMPORT_MAJOR_VERSION = 1


#end macros
class IRIAnalyzer():
    def __init__(self, az):
        self.az = az
        self.k1 = CONST_K1
        self.k2 = CONST_K2
        self.c1 = CONST_C1
        self.c2 = CONST_C2
        self.m1 = CONST_M1
        self.m2 = CONST_M2
        self.z_dotdot = 0
        self.dt = 0.1
        self.iri = 0

    def calculateIRI(self):
        # Cálculo de la aceleración vertical de la masa suspendida
        self.z_dotdot = (self.k1 * self.az + self.k2 * (self.az - self.az)) / self.m2
        desplazamientos = self.z_dotdot*self.dt*self.dt
        self.iri = (abs(desplazamientos)) / (AVERAGE_VELOCITY_KMH)
        return self.iri

        



class GyroProcessor():
    def __init__(self, data=None):
        # TODO: differentiate between flags  of data 
        # from the differents services that can be configured in the ble device

        # Assuming the packet data is for acceleration, angular velocity and angle Data 
        # (Ignoring the packet header and flag bit)
        if isinstance(data[2], int):
            hex_data = list(data[2:])
        else:
            hex_data = [ord(el) for el in data[2:]]
        #hex_data = [ord(el) for el in data[2:]]
        # It is requirement of the manufacturer to cast all values to signed short values, 
        # but python doesnt have this data type, thats why the following transformation exists
        transformed_hex_values = [val if val <= 127 else (256-val)*-1 for val in hex_data]
        for i in range(0,3):
            if i == 0:
                # "Acceleration"
                self.ax = float((int(transformed_hex_values[i+1])<<8)| (int(transformed_hex_values[i]) & 255))/32768*(16*9.8)
                self.ay = float((int(transformed_hex_values[i+3])<<8)| (int(transformed_hex_values[i+2]) & 255))/32768*(16*9.8)
                self.az = float((int(transformed_hex_values[i+5])<<8)| (int(transformed_hex_values[i+4]) & 255))/32768*(16*9.8)

            if i == 1:
                # "Angular Velocity" 
                self.wx = float((int(transformed_hex_values[i+6])<<8)| (int(transformed_hex_values[i+5]) & 255))/32768*2000
                self.wy = float((int(transformed_hex_values[i+8])<<8)| (int(transformed_hex_values[i+7]) & 255))/32768*2000
                self.wz = float((int(transformed_hex_values[i+10])<<8)| (int(transformed_hex_values[i+9]) & 255))/32768*2000
            
            if i == 2:
                # "Angles"
                self.rollx = float((int(transformed_hex_values[i+11])<<8)| (int(transformed_hex_values[i+10]) & 255))/32768*180
                self.pitchy = float((int(transformed_hex_values[i+13])<<8)| (int(transformed_hex_values[i+12]) & 255))/32768*180
                self.yawz = float((int(transformed_hex_values[i+15])<<8)| (int(transformed_hex_values[i+14]) & 255))/32768*180
    
class DeviceDelegate(DefaultDelegate, QObject):

    gyroDataReceived = Signal(float, float, float, float, float, float, float, float, float, float)
    dataTimeObtained = Signal(str)
    dataTimeObtaindedSeconds = Signal(int)


    def __init__(self):
        DefaultDelegate.__init__(self)
        QObject.__init__(self)


    # this portion of code will execute everytime the BLE device emits a notification
    def handleNotification(self, cHandle, data): 
        fecha_actual = datetime.datetime.now()
        now = QDateTime.currentDateTime()
        startTime = now.toSecsSinceEpoch()
        processor = GyroProcessor(data)
        iriAnalyzer = IRIAnalyzer(processor.az)
        iri= iriAnalyzer.calculateIRI()
        ax = processor.ax
        ay = processor.ay
        az = processor.az
        wx = processor.wx
        wy = processor.wy
        wz = processor.wz
        rollx = processor.rollx
        pitchy = processor.pitchy
        yawz = processor.yawz

        print("accel in Y: {}, accel in x: {}, accel in Z:{}, roll: {}, fecha: {}, iri: {}".format(ay, ax, az, rollx, fecha_actual, iri))
        self.dataTimeObtained.emit(fecha_actual.strftime("%Y-%m-%d %H:%M:%S"))
        self.dataTimeObtaindedSeconds.emit(startTime)
        self.gyroDataReceived.emit(ax,ay,az,wx,wy,wz,rollx,pitchy,yawz,iri)
        

"""
#here you can change the bluetooth MAC address of the device
MAC_ble_device = "EE:D8:AE:FC:A6:A1"
addr = False
try:
#it connects to the ble device
    ble_device = Peripheral(MAC_ble_device, ADDR_TYPE_RANDOM,0).withDelegate(DeviceDelegate())
    addr = ble_device.addr
except BTLEDisconnectError:
    print("Trying to reconnect every minute...")
    while not addr:
        try:
            ble_device = Peripheral(MAC_ble_device, ADDR_TYPE_RANDOM,0).withDelegate(DeviceDelegate())
            addr = ble_device.addr
        except:
            pass
        time.sleep(60)
finally:
    print("Connected to {}".format(addr))

    """





# descriptors values:
# {16: 'NOTIFY', 1: 'BROADCAST', 2: 'READ', 4: 'WRITE NO RESPONSE', 32: 'INDICATE', 8: 'WRITE', 64: 'WRITE SIGNED', 128: 'EXTENDED PROPERTIES'}
# I modify the descriptor with 16 (Notify) in bytes to get the data by notifications
#ble_descriptors[0].write(bytes(16))s

class IRIAnalyzerThread(QThread):
    """
    The thread which read BLE obtained data from WT901BLECL5 and send it to the slot.

    read data from sensor

    send data signal to the slot of qml.
    """
    #signals to send
    bleConnectionStatus = Signal(bool)
    bleConnectionStatusMessage = Signal(str)


    def __init__(self, parent = None):
        """Initialize variables"""
        QThread.__init__(self, parent)
        self.addr = False
        self.delegate = DeviceDelegate()
        self.ble_device = None
        
    def run(self):
        try:
            self.ble_device = Peripheral(WT901BLECL5_MAC_ADRESS, ADDR_TYPE_RANDOM,0).withDelegate(self.delegate)
            self.addr = self.ble_device.addr
        except BTLEDisconnectError:
            print("Trying to reconnect every minute...")
            self.bleConnectionStatus.emit(WT901BLECL5_CONNECTION_STATUS)
            self.bleConnectionStatusMessage.emit("Intentando conectarse a sensor cada minuto")
            while not self.addr:
                try:
                    self.ble_device = Peripheral(WT901BLECL5_MAC_ADRESS, ADDR_TYPE_RANDOM,0).withDelegate(self.delegate)
                    self.addr = self.ble_device.addr
                except:
                    pass
                time.sleep(60)
        finally:
            self.bleConnectionStatus.emit(not WT901BLECL5_CONNECTION_STATUS)
            self.bleConnectionStatusMessage.emit("Connected to {}".format(self.addr))
            # extract the services and characteristics of this UUID

        notif_char = self.ble_device.getCharacteristics(uuid=UUID_NOTIFICATION)[0]
        cccd_handle = notif_char.getHandle() + 1
        self.ble_device.writeCharacteristic(cccd_handle, b"\x01\x00", withResponse=True)

        while True:
            try:
                if self.ble_device.waitForNotifications(15):
                    continue
            except BTLEDisconnectError:
                self.addr = False
                print("Trying to reconnect every minute...")
                self.bleConnectionStatus.emit(WT901BLECL5_CONNECTION_STATUS)
                self.bleConnectionStatusMessage.emit("Intentando conectarse a sensor cada minuto")
                while not self.addr:
                    try:
                        self.ble_device = Peripheral(WT901BLECL5_MAC_ADRESS, ADDR_TYPE_RANDOM,0).withDelegate(self.delegate)
                        self.addr = self.ble_device.addr
                    except: 
                        time.sleep(60)
                        pass
                    finally:
                        self.bleConnectionStatus.emit(not WT901BLECL5_CONNECTION_STATUS)
                        self.bleConnectionStatusMessage.emit("Connected to {}".format(self.addr))
            except Exception as e:
                print("an error happened:\n{}".format(e))
            
@QmlElement
class GyroscopeWorker(QObject):
    connectionBLEStatus = Signal(bool)
    connectionBLEStatusMessage = Signal(str)
    gyroDataListener = Signal(float, float, float, float, float, float, float, float, float,float)
    timeStampListener = Signal(str)

    def __init__(self, parent= None):
        super().__init__(parent)
        self.th = IRIAnalyzerThread(self)
        self.th.bleConnectionStatus.connect(self.get_bleStatusConnection)
        self.th.bleConnectionStatusMessage.connect(self.get_bleStatusConnectionMessage)
        self.th.delegate.gyroDataReceived.connect(self.get_gyroscopeBleObtainedData)
        self.th.delegate.dataTimeObtained.connect(self.get_timeStamp)

    @Slot(bool)
    def get_bleStatusConnection(self, status: bool):
        self.connectionBLEStatus.emit(status)
    
    @Slot(str)
    def get_bleStatusConnectionMessage(self, message: str):
        self.connectionBLEStatusMessage.emit(message)
    
    @Slot(float, float, float, float, float, float, float, float, float,float)
    def get_gyroscopeBleObtainedData(self, ax, ay, az, wx, wy, wz, rollx, pitchy, yawz,iri):
        self.gyroDataListener.emit(ax, ay, az, wx, wy, wz, rollx, pitchy, yawz,iri)
    
    @Slot(str)
    def get_timeStamp(self, timeStamp: str):
        self.timeStampListener.emit(timeStamp)
    
    @Slot()
    def start(self):
        self.th.start()
    
    @Slot()
    def kill_thread(self):
        print("Finishing... GyroscopeGettingDataThread")
        time.sleep(1)
        #self.th.quit()
        self.th.terminate()
        self.th.wait()


    
    @Slot()
    def stop(self):
        self.kill_thread()

    

    


    