import sys
import time
import os
from math import *

import numpy as np
import pyqtgraph as pg
from .gcs_ui import *
from .telemWidgets import *

from PyQt5 import QtCore, QtWidgets
from PyQt5.QtGui import QQuaternion

from pyqtgraph import Transform3D
import pyquaternion
import pyqtgraph.opengl as gl
import math

from stl import mesh
from itertools import chain

from .mavlink import *

from . import _log as _root_log

_log = _root_log.getChild("main")

'''
TODO:
    проверить все функции для приема на правильность записываемых в файлы данных
    реализовать таблицу
    db
    стрелку на графике GPS
    
'''

# Запись данных в файлы
FILE_WRITE = 1

MESH_PATH = r'C:\Users\MI\PycharmProjects\CanSat_20I9\ground\unisat-gcs\src\unisat_gcs\server\theplane.stl'

if FILE_WRITE:

    file_name = "test"
    expansion = ".txt"

    file_bmp = file_name + "_bmp" + expansion
    file_imu_isc = file_name + "_imu_isc" + expansion
    file_imu_rsc = file_name + "_imu_rsc" + expansion
    file_sensors = file_name + "_imu_bmp" + expansion
    file_gps = file_name + "_gps" + expansion
    file_servo = file_name + "_servo" + expansion
    file_zero_data = file_name + "_zero_data" + expansion

    #
    f = open(file_bmp, 'w')
    f.write("Time" + '\t' + "Pressure" + '\t' + "Temp" + '\t' + "Speed" + '\n')
    f.close()

    f = open(file_sensors, 'w')
    f.write("Time" + '\t' + "Pressure" + '\t' + "Height" + '\t' + "Temp" + '\n')
    f.close()

    f = open(file_imu_isc, 'w')
    f.write("Time" + '\t' + "Accel" + '\t' + '\t' + '\t' + "Compass" + '\t' + '\t' + '\t' + "Quaternion" + '\n')
    f.close()

    f = open(file_imu_rsc, 'w')
    f.write("Time" + '\t' + "Accel" + '\t' + '\t' + '\t' + "Compass" + '\t' + '\t' + '\t' + "Gyro" + '\n')
    f.close()

    f = open(file_gps, 'w')
    f.write("Time" + '\t' + "x" + '\t' + '\t' + '\t' + "y" + '\t' + '\t' + '\t' + "z" + '\n')
    f.close()

    f = open(file_servo, 'w')
    f.write("Time" + '\t' + "angle_left" + '\t' + '\t' + '\t' + "angle_right" + '\t' + '\t' + '\t' + "angle_keel" + '\n')
    f.close()

    f = open(file_zero_data, "w")
    f.write("Time" + '\t' + 'zero_pressure' + '\t' + 'zero_quaternion' + '\t' + 'zero_GPS' + '\t' + 'gyro_staticShift' + '\t' + 'accel_staticShift' + '\n')
    f.close()

log_text = ''
global_vars={'x': 0, 'y': 0}
now_graf = None
str_now_graf = None


#
class PlaneWidget(gl.GLViewWidget):
    def __init__(self, mesh_path, *args, **kwargs):
        super(PlaneWidget, self).__init__(*args, **kwargs)
        self.setCameraPosition(distance=15)
        self.setBackgroundColor([100, 100, 100, 0])
        g = gl.GLGridItem()
        g.scale(2, 2, 1)
        # g.translate(0, 0, 0)
        self.addItem(g)


        isc_coord = gl.GLAxisItem()
        isc_coord.setSize(100, 100, 100)
        self.addItem(isc_coord)

        self.plane_axis = gl.GLAxisItem()
        self.plane_axis.setSize(x=300, y=300, z=300)
        self.addItem(self.plane_axis)



        verts = self._get_mesh_points(mesh_path)
        faces = np.array([(i, i+1, i+2,) for i in range(0, len(verts), 3)])
        colors = np.array([(0.0, 1.0, 0.0, 1.0,) for i in range(0, len(verts), 3)])
        self.mesh = gl.GLMeshItem(vertexes=verts, faces=faces, faceColors=colors, smooth=False, shader='shaded')

        self.mesh.rotate(180, 0, 0, 1)

        self.addItem(self.mesh)
        # self._update_mesh([0, 0, 1])


    def on_new_records(self, records):
        self._update_mesh(records)


    def _get_mesh_points(self, mesh_path):
        your_mesh = mesh.Mesh.from_file(mesh_path)
        points = your_mesh.points

        points = np.array(list(chain(*points)))
        i = 0
        nd_points = np.ndarray(shape=(len(points)//3, 3,) )
        for i in range(0, len(points)//3):
            nd_points[i] = points[i*3: (i+1)*3]

        return nd_points

    def _transform_object(self, target, move=True, rotate=True, scale=1 / 50):
        target.resetTransform()
        target.scale(scale, scale, scale)
        if move: target.translate(0, 0, -3)
        if rotate:
            target.rotate(degrees(self.rotation.angle), self.rotation.axis[0], self.rotation.axis[1],
                          self.rotation.axis[2])

    def _update_rotation(self, record):
        quat = pyquaternion.Quaternion(record)
        self.rotation = quat

        self._transform_object(self.mesh)
        self.mesh.rotate(180, 0, 1, 0)
        self.mesh.translate(0, 1, 0)
        self._transform_object(self.plane_axis, move=False)


# Главный класс
class MyWin(QtWidgets.QMainWindow):
    new_send_command = pyqtSignal(int)

    def __init__(self, parent=None):
        QtWidgets.QWidget.__init__(self, parent)
        self.ui = Ui_MainWindow()
        # self.ui = Ground.Ui_MainWindow()
        self.plane_widget = PlaneWidget(mesh_path=MESH_PATH, parent=self)

        self.ui.setupUi(self)
        self.setWindowTitle('Unica gcs')

        # Инициализация виджетов для телеметрии
        self.telem_widget_accel = TelemWidget1(title="Accel ICS", layout=self.ui.verticalLayout_telem_left)
        self.telem_widget_gyro = TelemWidget1(title="Gyro", layout=self.ui.verticalLayout_telem_left)
        self.telem_widget_magnetometer = TelemWidget1(title="Magnetometer", layout=self.ui.verticalLayout_telem_left)

        #
        self.telem_widget_temp = TelemWidget2(title="Temp", layout=self.ui.verticalLayout_telem_left,
                                              name_label_x='temp imu bmp', name_label_y='temp bmp')
        self.telem_widget_pressure = TelemWidget2(title="Pressure", layout=self.ui.verticalLayout_telem_left,
                                                  name_label_x='pressure imu bmp', name_label_y='pressure bmp')
        #
        pg.setConfigOption('background', 'w')
        pg.setConfigOption('foreground', 'k')

        # self.accel_f = open("C:\Cansat Unica 2018\calibration\\accel_calibration", "wt")
        # self.compass_f = open("C:\Cansat Unica 2018\calibration\magn_calibration", "wt")

        self.lenght = 150
        self.cut = 11
        self.zero_data = 1

        self.temp_bmp = []
        self.pressure_bmp = []
        self.speed_bmp = []
        self.time_bmp = []

        self.state_time = None
        self.state_mpu_bmp = None
        self.state_master_state = None
        self.state_buttons = None
        self.state_mpu = None
        self.state_bmp = None
        self.state_sd = None
        self.state_nrf = None
        self.state_gps = None
        self.state_fly = None

        self.av_x = []
        self.av_y = []
        self.av_z = []

        self.time_RSC = []

        self.a_ISC_x = []
        self.a_ISC_y = []
        self.a_ISC_z = []

        self.v_x = []
        self.v_y = []
        self.v_z = []

        self.mov_x = []
        self.mov_y = []
        self.mov_z = []

        self.vmf_x = []
        self.vmf_y = []
        self.vmf_z = []

        self.time_ISC = []

        self.quat = []

        self.temp_bmpIMU = []
        self.pressure_bmpIMU = []
        self.time_bmpIMU = []

        self.x = []
        self.y = []
        self.GPS = []
        self.speed_GPS = []

        self.buffer_bmp_msg = []
        self.buffer_imu_isc_msg = []
        self.buffer_imu_rsc_msg = []
        self.buffer_sensors_msg = []
        self.buffer_servo_msg = []
        self.buffer_zero_data = []

        # Верхний ряд графиков
        self.ui.glv_top = pg.GraphicsLayoutWidget(self.ui.centralwidget)
        self.ui.top.addWidget(self.ui.glv_top)
        self.ui.plot_top = pg.GraphicsLayout()
        self.ui.glv_top.addItem(self.ui.plot_top)

        self.sc_item_top1 = pg.PlotItem(title='Accel ISC')
        self.ui.plot_top.addItem(self.sc_item_top1)
        self.graf_top1 = pg.PlotCurveItem()
        self.sc_item_top1.addItem(self.graf_top1)

        self.sc_item_top2 = pg.PlotItem(title='GPS speed')
        self.ui.plot_top.addItem(self.sc_item_top2)
        self.graf_top2 = pg.PlotCurveItem()
        self.sc_item_top2.addItem(self.graf_top2)

        # Создание средних графиков
        self.ui.glv_middle = pg.GraphicsLayoutWidget(self.ui.centralwidget)
        self.ui.middle.addWidget(self.ui.glv_middle)
        self.ui.plot_middle = pg.GraphicsLayout()
        self.ui.glv_middle.addItem(self.ui.plot_middle)

        self.sc_item_middle1 = pg.PlotItem(title='Angles velosity')
        self.ui.plot_middle.addItem(self.sc_item_middle1)
        self.graf_middle1 = pg.PlotCurveItem()
        self.sc_item_middle1.addItem(self.graf_middle1)

        self.sc_item_middle2 = pg.PlotItem(title='Vector of magnetic field')
        self.ui.plot_middle.addItem(self.sc_item_middle2)
        self.graf_middle2 = pg.PlotCurveItem()
        self.sc_item_middle2.addItem(self.graf_middle2)

        # нижний ряд графиков
        self.ui.glv_down = pg.GraphicsLayoutWidget(self.ui.centralwidget)
        self.ui.down.addWidget(self.ui.glv_down)
        self.ui.plot_down = pg.GraphicsLayout()
        self.ui.glv_down.addItem(self.ui.plot_down)

        self.sc_item_down1 = pg.PlotItem(title='Temperature')
        self.ui.plot_down.addItem(self.sc_item_down1)
        self.graf_down1 = pg.PlotCurveItem()
        self.sc_item_down1.addItem(self.graf_down1)

        self.sc_item_down2 = pg.PlotItem(title='Pressure')
        self.ui.plot_down.addItem(self.sc_item_down2)
        self.graf_down2 = pg.PlotCurveItem()
        self.sc_item_down2.addItem(self.graf_down2)

        # Верхний правый ряд графиков
        self.ui.glv_top_2 = pg.GraphicsLayoutWidget(self.ui.centralwidget)
        self.ui.grid_right_2_graf.addWidget(self.ui.glv_top_2)
        self.ui.plot_top_2 = pg.GraphicsLayout()
        self.ui.glv_top_2.addItem(self.ui.plot_top_2)

        self.sc_item_top3 = pg.PlotItem(title='GPS')
        self.ui.plot_top_2.addItem(self.sc_item_top3)
        self.graf_top3 = pg.PlotCurveItem()
        self.sc_item_top3.addItem(self.graf_top3)

        self.sc_item_top4 = pg.PlotItem(title='BMP speed')
        self.ui.plot_top_2.addItem(self.sc_item_top4)
        self.graf_top4 = pg.PlotCurveItem()
        self.sc_item_top4.addItem(self.graf_top4)

        # 3D график
        self.ui.dockwid = QtWidgets.QDockWidget()
        self.ui.grid_3D.addWidget(self.ui.dockwid)

        self.ui.glwid = self.plane_widget
        self.ui.dockwid.setWidget(self.ui.glwid)
        self.isc_coord = gl.GLAxisItem()
        self.isc_coord.setSize(25, 25, 25)
        self.ui.glwid.show()

        #
        self.pl_graf_top1_x = self.sc_item_top1.plot()
        self.pl_graf_top2_x = self.sc_item_top2.plot()
        self.pl_graf_top3_x = self.sc_item_top3.plot()
        self.pl_graf_top4_x = self.sc_item_top4.plot()

        self.pl_graf_top1_y = self.sc_item_top1.plot()
        self.pl_graf_top3_y = self.sc_item_top3.plot()

        self.pl_graf_top1_z = self.sc_item_top1.plot()
        self.pl_graf_top2_z = self.sc_item_top2.plot()

        #
        self.pl_graf_middle1_x = self.sc_item_middle1.plot()
        self.pl_graf_middle2_x = self.sc_item_middle2.plot()

        self.pl_graf_middle1_y = self.sc_item_middle1.plot()
        self.pl_graf_middle2_y = self.sc_item_middle2.plot()

        self.pl_graf_middle1_z = self.sc_item_middle1.plot()
        self.pl_graf_middle2_z = self.sc_item_middle2.plot()

        #
        self.pl_graf_down1_x = self.sc_item_down1.plot()
        self.pl_graf_down2_x = self.sc_item_down2.plot()

        # Здесь прописываем событие нажатия на кнопку
        self.ui.pushButton_3.clicked.connect(self.remove_graf)
        self.ui.commandLinkButton.clicked.connect(self.send_command)

    #
    def write_to_file(self, buffer, file):
        if len(buffer) >= 10:
            f = open(file, 'a')
            for foo in range(len(buffer)):
                f.write(buffer[foo])
            buffer = []
            f.close()

    #
    def send_command(self):
        com = self.ui.textBrowser.toPlainText()

        try:
            com = int(com)
            self.new_send_command.emit(com)
        except BaseException:
            com = None
        self.ui.textBrowser.clear()

    # функция для очистки графиков
    def remove_graf(self):
        global now_graf, str_now_graf
        # if self.sc_item_large == None:
        self.sc_item_top1.removeItem(self.sc_item_top1)
        self.sc_item_top1.clear()

        self.sc_item_top2.removeItem(self.sc_item_top1)
        self.sc_item_top2.clear()

        self.sc_item_middle1.removeItem(self.sc_item_top1)
        self.sc_item_middle1.clear()

        self.sc_item_top3.removeItem(self.sc_item_top1)
        self.sc_item_top3.clear()

        self.sc_item_middle2.removeItem(self.sc_item_top1)
        self.sc_item_middle2.clear()

        self.sc_item_middle3.removeItem(self.sc_item_top1)
        self.sc_item_middle3.clear()

        self.sc_item_down1.removeItem(self.sc_item_top1)
        self.sc_item_down1.clear()

        self.sc_item_down2.removeItem(self.sc_item_top1)
        self.sc_item_down2.clear()

        self.sc_item_down3.removeItem(self.sc_item_top1)
        self.sc_item_down3.clear()

        self.sc_item_large.removeItem(self.sc_item_large)
        self.sc_item_large.clear()
    # FIXME: возможно не работает

    #
    def clear_state(self):
        self.ui.mpu_state.clear()
        self.ui.bmp_state.clear()
        self.ui.sd_state.clear()
        self.ui.nrf_state.clear()
        self.ui.morot_stm_state.clear()
        self.ui.gps_state.clear()

        self.ui.init.clear()
        self.ui.init_param.clear()
        self.ui.in_rocket.clear()
        self.ui.lowering.clear()
        self.ui.falling.clear()
        self.ui.end.clear()

    #
    @QtCore.pyqtSlot(list)
    def bmp_msg(self, msgs):   # сообщения с bmp (не с MPU)!!!!
        for i in range(len(msgs)):
            self.pressure_bmp.append(msgs[i].pressure)
            self.temp_bmp.append(msgs[i].temp)
            self.speed_bmp.append(msgs[i].speed_bmp)
            self.time_bmp.append(msgs[i].time)

            if FILE_WRITE:
                self.buffer_bmp_msg.append(str(msgs[i].time) + '\t' + '\t' +
                                        str(msgs[i].pressure) + '\t' + '\t' +
                                        str(msgs[i].temp) + '\t' + '\t' +
                                        str(msgs[i].speed_bmp) + '\n')

        self.telem_widget_temp.set_value_y(self.temp_bmp[len(self.temp_bmp) - 1])
        self.telem_widget_pressure.set_value_y(self.pressure_bmp[len(self.pressure_bmp) - 1])

        if FILE_WRITE:
            self.write_to_file(self.buffer_bmp_msg, file_bmp)

        if len(self.time_bmp) > self.lenght:
            self.time_bmp = self.time_bmp[self.cut:(self.lenght - 1)]
            self.pressure_bmp = self.pressure_bmp[self.cut:(self.lenght - 1)]
            self.temp_bmp = self.temp_bmp[self.cut:(self.lenght - 1)]
            self.speed_bmp = self.speed_bmp[self.cut:(self.lenght - 1)]
        self.pl_graf_top4_x.setData(x=self.time_bmp, y=self.speed_bmp, pen=('r'))

    #
    @QtCore.pyqtSlot(list)
    def imu_rsc_msg(self, msgs):
        for i in range(len(msgs)):
            self.a_RSC_x.append(msgs[i].accel[0])
            self.a_RSC_y.append(msgs[i].accel[1])
            self.a_RSC_z.append(msgs[i].accel[2])

            # self.accel_f.write("%f\t%f\t%f\n" % (msgs[i].accel[0], msgs[i].accel[1], msgs[i].accel[2]))
            # self.compass_f.write("%f\t%f\t%f\n" % (msgs[i].compass[0], msgs[i].compass[1], msgs[i].compass[2]))

            self.time_RSC.append(msgs[i].time)

            self.av_x.append(msgs[i].gyro[0])
            self.av_y.append(msgs[i].gyro[1])
            self.av_z.append(msgs[i].gyro[2])

            if FILE_WRITE:
                self.buffer_imu_rsc_msg.append(str(msgs[i].time) + '\t' + '\t' +
                                        str(msgs[i].accel[0]) + ' ' + str(msgs[i].accel[1]) + ' ' + str(msgs[i].accel[2]) + '\t' + '\t' +
                                        str(msgs[i].compass[0]) + ' ' + str(msgs[i].compass[1]) + ' ' + str(msgs[i].compass[2]) + '\t' + '\t' +
                                        str(msgs[i].gyro[0]) + ' ' + str(msgs[i].gyro[1]) + ' ' + str(msgs[i].gyro[2]) + '\n')

        self.telem_widget_gyro.set_value_x(self.av_x[len(self.av_x) - 1])
        self.telem_widget_gyro.set_value_y(self.av_y[len(self.av_y) - 1])
        self.telem_widget_gyro.set_value_z(self.av_z[len(self.av_z) - 1])
        # self.telem_widget_gyro.set_value_result()

        if FILE_WRITE:
            self.write_to_file(self.buffer_imu_rsc_msg, file_imu_rsc)

        if len(self.time_RSC) > self.lenght:
            self.time_RSC = self.time_RSC[self.cut:(self.lenght - 1)]
            self.av_x = self.av_x[self.cut:(self.lenght - 1)]
            self.av_y = self.av_y[self.cut:(self.lenght - 1)]
            self.av_z = self.av_z[self.cut:(self.lenght - 1)]

        # self.pl_graf_top2_x.setData(x=self.time_RSC, y=self.a_RSC_x, pen=('r'), width=0.5)
        # self.pl_graf_top2_y.setData(x=self.time_RSC, y=self.a_RSC_y, pen=('g'), width=0.5)
        # self.pl_graf_top2_z.setData(x=self.time_RSC, y=self.a_RSC_z, pen=('b'), width=0.5)

        self.pl_graf_middle1_x.setData(x=self.time_RSC, y=self.av_x, pen=('r'), width=0.5)
        self.pl_graf_middle1_y.setData(x=self.time_RSC, y=self.av_y, pen=('g'), width=0.5)
        self.pl_graf_middle1_z.setData(x=self.time_RSC, y=self.av_z, pen=('b'), width=0.5)

    #
    @QtCore.pyqtSlot(list)
    def imu_isc_msg(self, msgs):
        for i in range(len(msgs)):
            self.a_ISC_x.append(msgs[i].accel[0])
            self.a_ISC_y.append(msgs[i].accel[1])
            self.a_ISC_z.append(msgs[i].accel[2])

            self.time_ISC.append(msgs[i].time)

            self.vmf_x.append(msgs[i].compass[0])
            self.vmf_y.append(msgs[i].compass[1])
            self.vmf_z.append(msgs[i].compass[2])

            if FILE_WRITE:
                self.buffer_imu_isc_msg.append(str(msgs[i].time) + '\t' + '\t' +
                                        str(msgs[i].accel[0]) + ' ' + str(msgs[i].accel[1]) + str(msgs[i].accel[2]) + '\t' + '\t' +
                                        str(msgs[i].compass[0]) + ' ' + str(msgs[i].compass[1]) + ' ' + str(msgs[i].compass[2]) + '\t' + '\t' +
                                        str(msgs[i].quaternion[0]) + ' ' + str(msgs[i].quaternion[1]) + ' ' + str(msgs[i].quaternion[2]) + ' ' + str(msgs[i].quaternion[3]) + '\n')

            quat = pyquaternion.Quaternion(msgs[i].quaternion)
            self.plane_widget._update_rotation(quat)

        self.telem_widget_accel.set_value_x(self.a_ISC_x[len(self.a_ISC_x) - 1])
        self.telem_widget_accel.set_value_y(self.a_ISC_y[len(self.a_ISC_y) - 1])
        self.telem_widget_accel.set_value_z(self.a_ISC_z[len(self.a_ISC_z) - 1])
        # self.telem_widget_accel.set_value_result()

        self.telem_widget_magnetometer.set_value_x(self.vmf_x[len(self.vmf_x) - 1])
        self.telem_widget_magnetometer.set_value_y(self.vmf_y[len(self.vmf_y) - 1])
        self.telem_widget_magnetometer.set_value_z(self.vmf_z[len(self.vmf_z) - 1])
        # self.telem_widget_magnetometer.set_value_result()

        if FILE_WRITE:
            self.write_to_file(self.buffer_imu_isc_msg, file_imu_isc)

        if len(self.time_ISC) > self.lenght:
            self.a_ISC_x = self.a_ISC_x[self.cut:(self.lenght - 1)]
            self.a_ISC_y = self.a_ISC_y[self.cut:(self.lenght - 1)]
            self.a_ISC_z = self.a_ISC_z[self.cut:(self.lenght - 1)]
            self.time_ISC = self.time_ISC[self.cut:(self.lenght - 1)]
            self.vmf_x = self.vmf_x[self.cut:(self.lenght - 1)]
            self.vmf_y = self.vmf_y[self.cut:(self.lenght - 1)]
            self.vmf_z = self.vmf_z[self.cut:(self.lenght - 1)]

        # self.plt.setData(pos=self.GPS, color=(1.0, 1.0, 1.0, 1.0))
        # m = len(self.mov_x)
        # if m != 0:
        #     delta_x = self.mov_x[m - 1] - self.mov_x[m - self.cut]
        #     delta_y = self.mov_y[m - 1] - self.mov_y[m - self.cut]
        #     delta_z = self.mov_z[m - 1] - self.mov_z[m - self.cut]
        #     self.rsc_coord.translate(delta_x, delta_y, delta_z)
        # else:
        #     self.rsc_coord.translate(self.mov_x, self.mov_y, self.mov_z)
        # Цвета в pg.glColor

        self.pl_graf_top1_x.setData(x=self.time_ISC, y=self.a_ISC_x, pen=('r'))
        self.pl_graf_top1_y.setData(x=self.time_ISC, y=self.a_ISC_y, pen=('g'))
        self.pl_graf_top1_z.setData(x=self.time_ISC, y=self.a_ISC_z, pen=('b'))

        self.pl_graf_middle2_x.setData(x=self.time_ISC, y=self.vmf_x, pen=('r'))
        self.pl_graf_middle2_y.setData(x=self.time_ISC, y=self.vmf_y, pen=('g'))
        self.pl_graf_middle2_z.setData(x=self.time_ISC, y=self.vmf_z, pen=('b'))

    #
    @QtCore.pyqtSlot(list)
    def sens_msg(self, msgs):
        for i in range(len(msgs)):
            self.time_bmpIMU.append(msgs[i].time)
            self.pressure_bmpIMU.append(msgs[i].pressure)
            self.temp_bmpIMU.append(msgs[i].temp)

            if FILE_WRITE:
                self.buffer_sensors_msg.append( str(msgs[i].time) + '\t' + '\t' +
                                            str(msgs[i].pressure) + '\t' + '\t' +
                                            str(msgs[i].temp) + '\t' + '\t' +
                                            str(msgs[i].height) + '\n')

        self.telem_widget_temp.set_value_x(self.temp_bmpIMU[len(self.temp_bmpIMU) - 1])
        self.telem_widget_pressure.set_value_x(self.pressure_bmpIMU[len(self.pressure_bmpIMU) - 1])

        if FILE_WRITE:
            self.write_to_file(self.buffer_sensors_msg, file_sensors)

        if len(self.time_sens) > self.lenght:
            self.time_bmpIMU = self.time_bmpIMU[self.cut:(self.lenght - 1)]
            self.pressure_bmpIMU = self.pressure_bmpIMU[self.cut:(self.lenght - 1)]
            self.temp_bmpIMU = self.temp_bmpIMU[self.cut:(self.lenght - 1)]

        self.pl_graf_down1_x.setData(x=self.time_bmpIMU, y=self.temp_bmpIMU, pen=('r'))
        self.pl_graf_down2_x.setData(x=self.time_bmpIMU, y=self.pressure_bmpIMU, pen=('r'))

    #
    @QtCore.pyqtSlot(list)
    def gps_msg(self, msgs):
        time = []
        for i in range(len(msgs)):
            self.x.append(msgs[i].coordinates[0])
            self.y.append(msgs[i].coordinates[1])
            self.speed_GPS.append(msgs[i].speed)
            time.append(msgs[i].time)

            # FIXME: WTF?????????
            y0 = []
            x0 = []
            x0.append(self.x[0])
            y0.append(self.y[0])

        self.pl_graf_top3_y.setData(x=x0, y=y0, pen=('b'), width=10)

        if len(self.x) > self.lenght:
            self.x = self.x[self.cut:(self.lenght - 1)]
            self.y = self.y[self.cut:(self.lenght - 1)]

        self.pl_graf_top3_x.setData(x=self.x, y=self.y, pen=('r'))
        self.pl_graf_top2_x.setData(x=time, y=self.speed_GPS)

    #
    @QtCore.pyqtSlot(list)
    def state_msg(self, msgs):
        for i in range(len(msgs)):
            self.state_time = msgs[i].time
            self.state_mpu = msgs[i].MPU_state
            self.state_mpu_bmp = msgs[i].IMU_BMP_state
            self.state_sd = msgs[i].SD_state
            self.state_nrf = msgs[i].NRF_state
            self.state_bmp = msgs[i].BMP_state
            self.state_gps = msgs[i].GPS_state
            self.state_master_state = msgs[i].master_state
            self.state_buttons = msgs[i].buttons
            self.state_fly = msgs[i].globalStage

            self.clear_state()

            self.ui.mpu_state.setText(str(self.state_mpu))
            self.ui.bmp_state.setText(str(self.state_bmp))
            self.ui.sd_state.setText(str(self.state_sd))
            self.ui.nrf_state.setText(str(self.state_nrf))
            self.ui.gps_state.setText(str(self.state_gps))

        if self.state_fly == 0:
            self.ui.init.setText(str('1'))

        elif self.state_fly == 1:
            self.ui.init_param.setText(str('1'))

        elif self.state_fly == 2:
            self.ui.in_rocket.setText(str('1'))

        elif self.state_fly == 3:
            self.ui.falling.setText(str('1'))

        elif self.state_fly == 4:
            self.ui.lowering.setText(str('1'))

        elif self.state_fly == 5:
            self.ui.end.setText(str('1'))

    #
    @QtCore.pyqtSlot(list)
    def servo_msg(self, msgs):
        for i in range(len(msgs)):
            if FILE_WRITE:
                self.buffer_servo_msg.append(str(msgs[i].time) + '\t' +
                                            str(msgs[i].angle_left) + '\t' + '\t' +
                                            str(msgs[i].angle_right) + '\t' + '\t' +
                                            str(msgs[i].angle_keel) + '\n')

        if FILE_WRITE:
            self.write_to_file(self.buffer_servo_msg, file_servo)

    #
    @QtCore.pyqtSlot(list)
    def zero_data_msg(self, msgs):
        if self.zero_data:
            self.zero_data = 0
            for i in range(len(msgs)):
                if FILE_WRITE:
                    self.buffer_zero_data.append(str(msgs[i].time) + '\t' +
                                                 str(msgs[i].zero_pressure) + '\t' +
                                                 str(msgs[i].zero_quaternion[0]) + ' ' +
                                                 str(msgs[i].zero_quaternion[1]) + ' ' +
                                                 str(msgs[i].zero_quaternion[2]) + ' ' +
                                                 str(msgs[i].zero_quaternion[3]) + '\t' +
                                                 str(msgs[i].zero_GPS[0]) + ' ' +
                                                 str(msgs[i].zero_GPS[1]) + '\t' +
                                                 str(msgs[i].gyro_staticShift) + '\t' +
                                                 str(msgs[i].accel_staticShift) + '\n')

                if FILE_WRITE:
                    self.write_to_file(self.buffer_zero_data, file_zero_data)
