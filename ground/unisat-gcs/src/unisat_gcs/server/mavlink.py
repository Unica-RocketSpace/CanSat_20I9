from PyQt5.QtCore import QThread, pyqtSignal, pyqtSlot
from pymavlink.dialects.v20.UNISAT import *
from pymavlink import mavutil

import serial

from . import _log as _root_log

_log = _root_log.getChild("main")

# 172.16.164.211
# ВЫБЕРИТЕ ТИП СОЕДИНЕНИЯ:
UDP = 1
UART = 0
SD = 0

# Файл, из которого мы читаем данные
if SD:
    read_file = r'C:\Users\MI\PycharmProjects\CanSat_20I9\ground\unisat-gcs\src\unisat_gcs\server\radio.bin'

def bytes_to_int(bytes):
    result = 0
    for b in bytes:
        result = result * 256 + int(b)
    return result

def connection_uart(_port, _baudrate=256000):
    while True:
        try:
            port = serial.Serial(
                port=_port,
                baudrate=_baudrate,
                parity=serial.PARITY_NONE,
                stopbits=serial.STOPBITS_ONE,
                bytesize=serial.SEVENBITS,
                xonxoff=False,
                rtscts=False,
                dsrdtr=False
            )
            if not port.is_open:
                port.open()

            print("port_opened")
            return port
        except IOError as e:
            print("error")
            pass


def uart_recive(port):
    leng = bytes_to_int(port.read())
    msg = port.read(leng - 1)
    return msg



class MsgAccumulator:
    def __init__(self, batch_size, signal):
        self.batch_size = batch_size
        self.signal = signal
        self.accumulator = []
        self.all_packet, self.bmp_packet, self.imu_isc_packet, self.imu_rsc_packet, self.sensors_packet, self.gps_packet = 0, 0, 0, 0, 0, 0

    def push_message(self, msg):
        self.accumulator.append(msg)
        if len(self.accumulator) >= self.batch_size:
            self.signal.emit(self.accumulator)
            self.accumulator = []
            # print('PUSH COMPLETED')


class MavlinkThread(QThread):
    new_imu_rsc_record = pyqtSignal(list)
    new_atmega_record = pyqtSignal(list)
    new_imu_isc_record = pyqtSignal(list)
    new_sensors_record = pyqtSignal(list)
    new_gps_record = pyqtSignal(list)
    new_state_record = pyqtSignal(list)
    new_servo_record = pyqtSignal(list)

    def __init__(self):
        QThread.__init__(self)
        self.atmega_accum = MsgAccumulator(10, self.new_atmega_record)
        self.imu_isc_accum = MsgAccumulator(10, self.new_imu_isc_record)
        self.imu_rsc_accum = MsgAccumulator(10, self.new_imu_rsc_record)
        self.sensors_accum = MsgAccumulator(10, self.new_sensors_record)
        self.gps_accum = MsgAccumulator(10, self.new_gps_record)
        self.state_accum = MsgAccumulator(10, self.new_state_record)
        self.servo_accum = MsgAccumulator(10, self.new_servo_record)
        self.uplink_msgs = []


    @pyqtSlot(int)
    def post_msg(self, msg):
        self.uplink_msgs.append(msg)
        self.uplink_msgs = bytes(self.uplink_msgs)

    def process_message(self, msg):
        # _log.debug(msg)
        # _log.info(msg)
        if isinstance(msg, MAVLink_bmp280_message):
            self.atmega_accum.push_message(msg)
            print('bmp msg')

        elif isinstance(msg, MAVLink_state_zero_message):
            print("zero data")

        elif isinstance(msg, MAVLink_imu_rsc_message):
            self.imu_rsc_accum.push_message(msg)
            # print('rsc msg')

        elif isinstance(msg, MAVLink_imu_isc_message):
            self.imu_isc_accum.push_message(msg)
            # print('isc msg')

        elif isinstance(msg, MAVLink_sensors_message):
            self.sensors_accum.push_message(msg)
            # print('sensors msg')

        elif isinstance(msg, MAVLink_state_message):
            self.state_accum.push_message(msg)
            # print('sensors msg')

        elif isinstance(msg, MAVLink_gps_message):
            self.gps_accum.push_message(msg)

        elif isinstance(msg, MAVLink_servo_message):
            self.servo_accum.push_message(msg)

        else:
            _log.warning("неизвестный тип сообщения!")
            _log.info(msg)

    # self.f.close()


    def run(self):
        if UDP:
            _log.info("url:")
            mav1 = mavutil.mavlink_connection("udpin:0.0.0.0:10000")
            mav2 = mavutil.mavlink_connection("udpin:0.0.0.0:10001")

        if UART:
            _log.info("Запускаюсь. Использую serial port")
            mav_serial = mavutil.mavserial("COM6", baud=115200, autoreconnect=True)

        if SD:
            _log.info("Запускаюсь. Использую SD")
            mav_read_file = mavutil.mavlogfile(read_file, robust_parsing=True, notimestamps= True)

        while True:
            if UDP:
                msg1 = mav1.recv_match(blocking=False)
                msg2 = mav2.recv_match(blocking=False)

                if msg1:
                    self.process_message(msg1)
                    if self.uplink_msgs:
                        print(self.uplink_msgs)
                        mav1.write(self.uplink_msgs)
                        self.uplink_msgs = []
                if msg2:
                    self.post_msg(msg2)
                    print(self.uplink_msgs)
                    if self.uplink_msgs:
                        mav2.write(self.uplink_msgs)
                        print(self.uplink_msgs)
                        self.uplink_msgs = []

            if SD:
                msg_read_file = mav_read_file.recv_match(blocking=False)
                if msg_read_file:
                    self.process_message(msg_read_file)

            if UART:
                msg_serial = mav_serial.recv_match(blocking=False)
                if msg_serial:
                    # print(msg_serial)
                    self.process_message(msg_serial)
                    print(msg_serial)

