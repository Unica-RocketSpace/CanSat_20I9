from PyQt5.QtCore import QThread, pyqtSignal, pyqtSlot
from pymavlink.dialects.v20.UNISAT import *
from pymavlink import mavutil

import serial

from . import _log as _root_log

_log = _root_log.getChild("main")

ACCUM_LEN = 10

# 172.16.164.211
# ВЫБЕРИТЕ ТИП СОЕДИНЕНИЯ:
UDP = 1
UART = 0
SD = 0

# Файл, из которого мы читаем данные
if SD:
    read_file = r'C:\Users\MI\PycharmProjects\CanSat_20I9\ground\unisat-gcs\src\unisat_gcs\server\U318.txt'


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
    new_bmp_record = pyqtSignal(list)
    new_imu_isc_record = pyqtSignal(list)
    new_sensors_record = pyqtSignal(list)
    new_gps_record = pyqtSignal(list)
    new_state_record = pyqtSignal(list)
    new_servo_record = pyqtSignal(list)
    new_zero_data_record = pyqtSignal(list)
    new_fc_logs_record = pyqtSignal(list)

    def __init__(self):
        QThread.__init__(self)
        self.bmp_accum = MsgAccumulator(ACCUM_LEN, self.new_bmp_record)
        self.imu_isc_accum = MsgAccumulator(2, self.new_imu_isc_record)
        self.imu_rsc_accum = MsgAccumulator(ACCUM_LEN, self.new_imu_rsc_record)
        self.sensors_accum = MsgAccumulator(ACCUM_LEN, self.new_sensors_record)
        self.gps_accum = MsgAccumulator(ACCUM_LEN, self.new_gps_record)
        self.state_accum = MsgAccumulator(ACCUM_LEN, self.new_state_record)
        self.zero_data_accum = MsgAccumulator(ACCUM_LEN, self.new_zero_data_record)
        self.servo_accum = MsgAccumulator(ACCUM_LEN, self.new_servo_record)
        self.fc_logs_accum = MsgAccumulator(ACCUM_LEN, self.new_fc_logs_record)
        self.uplink_msgs = []


    @pyqtSlot(int)
    def post_msg(self, msg):
        self.uplink_msgs.append(msg)
        self.uplink_msgs = bytes(self.uplink_msgs)

    def process_message(self, msg):
        # _log.debug(msg)
        # _log.info(msg)
        if isinstance(msg, MAVLink_bmp280_message):
            self.bmp_accum.push_message(msg)
            # print('bmp msg')

        elif isinstance(msg, MAVLink_state_zero_message):
            self.zero_data_accum.push_message(msg)
            # print("zero data")

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
            # print('state msg')

        elif isinstance(msg, MAVLink_gps_message):
            self.gps_accum.push_message(msg)
            # print('gps msg')

        elif isinstance(msg, MAVLink_servo_message):
            self.servo_accum.push_message(msg)

        elif isinstance(msg, MAVLink_fclogs_message):
            self.fc_logs_accum.push_message(msg)
            # print('FC_logs msg')

        else:
            _log.warning("неизвестный тип сообщения!")
            _log.info(msg)

    # self.f.close()

    def run(self):
        if UDP:
            _log.info("url:")
            mav1 = mavutil.mavlink_connection("udpin:0.0.0.0:10000")
            mav2 = mavutil.mavlink_connection("udpin:0.0.0.0:10001")

            while True:
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

        elif UART:
            _log.info("Запускаюсь. Использую serial port")
            mav_serial = mavutil.mavserial("COM6", baud=115200, autoreconnect=True)

            while True:
                msg_serial = mav_serial.recv_match(blocking=False)
                if msg_serial:
                    # print(msg_serial)
                    self.process_message(msg_serial)
                    print(msg_serial)

        elif SD:
            _log.info("Запускаюсь. Использую SD")
            mav_read_file = mavutil.mavlogfile(read_file, robust_parsing=True, notimestamps= True)

            while True:
                msg_read_file = mav_read_file.recv_match(blocking=False)
                if msg_read_file:
                    self.process_message(msg_read_file)
