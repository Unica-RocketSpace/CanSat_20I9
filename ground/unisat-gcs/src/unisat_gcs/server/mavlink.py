from PyQt5.QtCore import QThread, pyqtSignal, pyqtSlot
from pymavlink.dialects.v20.UNISAT import *
from pymavlink import mavutil

from . import _log as _root_log

_log = _root_log.getChild("main")


# Файл, из которого мы читаем данные
read_file = "U2.bin"




class MsgAccumulator:

    def __init__(self, batch_size, signal):
        self.batch_size = batch_size
        self.signal = signal
        self.accumulator = []

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

    def __init__(self):
        QThread.__init__(self)
        self.atmega_accum = MsgAccumulator(10, self.new_atmega_record)
        self.imu_isc_accum = MsgAccumulator(10, self.new_imu_isc_record)
        self.imu_rsc_accum = MsgAccumulator(10, self.new_imu_rsc_record)
        self.sensors_accum = MsgAccumulator(10, self.new_sensors_record)
        self.gps_accum = MsgAccumulator(10, self.new_gps_record)
        self.state_accum = MsgAccumulator(10, self.new_state_record)
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
            # print('bmp msg')

        elif isinstance(msg, MAVLink_imu_rsc_message):
            self.imu_rsc_accum.push_message(msg)
            # print('rsc msg')

        elif isinstance(msg, MAVLink_imu_isc_message):
            self.imu_isc_accum.push_message(msg)
            # print('isc msg')

        elif isinstance(msg, MAVLink_sensors_message):
            self.sensors_accum.push_message(msg)
            # print('sensors msg')

        else:
            # _log.warning("неизвестный тип сообщения! %s", msg.data())
            pass
    # self.f.close()


    def run(self):
        _log.info("Запускаюсь. Использую url:")
        # mav1 = mavutil.mavlink_connection("udpin:0.0.0.0:22466")
        mav2 = mavutil.mavlink_connection("udpin:0.0.0.0:22467")
        mav_read_file = mavutil.mavlogfile(read_file, robust_parsing= False, notimestamps= True)

        while True:
        # msg1 = mav1.recv_match(blocking=False)
        # msg2 = mav2.recv_match(blocking=False)
            msg_read_file = mav_read_file.recv_match(blocking=False)

            # if msg1:
            #     self.process_message(msg1)
            #     if self.uplink_msgs:
            #         print(self.uplink_msgs)
            #         mav1.write(self.uplink_msgs)
            #         self.uplink_msgs = []
            # if msg2:
            #     self.process_message(msg2)
            #     if self.uplink_msgs:
            #         mav2.write(self.uplink_msgs)
            #         self.uplink_msgs = []

            if msg_read_file:
                self.process_message(msg_read_file)
