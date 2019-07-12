import sys
import time
import os
from math import *

import numpy as np
import pyqtgraph as pg
from .gcs_ui import *
from .telemWidgets import *
from .gcsGraph import *

from PyQt5 import QtCore, QtWidgets, QtGui
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

TELEM = 1

MESH_PATH = r'C:\Users\MI\PycharmProjects\CanSat_20I9\ground\unisat-gcs\src\unisat_gcs\server\kopia_litaka.stl'

color_red = QtGui.QColor(255, 0, 0).name()
color_green = QtGui.QColor(0, 255, 0).name()


def result(x, y, z):
    return sqrt(x*x + y*y + z*z)


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
    file_fc_logs = file_name + "_fc_logs" + expansion

    #
    f = open(file_bmp, 'w')
    f.write("Time" + '\t' + 'Pressure' + '\t' + 'Temp' + '\t' + 'Speed' + '\n')
    f.close()

    f = open(file_sensors, 'w')
    f.write("Time" + '\t' + 'Pressure' + '\t' + 'Temp' + '\t' + 'Height' + '\n')
    f.close()

    f = open(file_imu_isc, 'w')
    f.write("Time" + '\t' + 'Accel' + '\t' + '\t' + '\t' + 'Compass' + '\t' + '\t' + '\t' + 'Quaternion' + '\n')
    f.close()

    f = open(file_imu_rsc, 'w')
    f.write("Time" + '\t' + 'Accel' + '\t' + '\t' + '\t' + 'Compass' + '\t' + '\t' + '\t' + 'Gyro' + '\n')
    f.close()

    f = open(file_gps, 'w')
    f.write("Time" + '\t' + 'x' + '\t' + '\t' + '\t' + 'y' + '\t' + '\t' + '\t' + 'speed' + '\t' + 'course' + '\n')
    f.close()

    f = open(file_servo, 'w')
    f.write("Time" + '\t' + 'angle_left' + '\t' + '\t' + '\t' + 'angle_right' + '\t' + '\t' + '\t' + 'angle_keel' + '\n')
    f.close()

    f = open(file_zero_data, "w")
    f.write("Time" + '\t' + 'zero_pressure' + '\t' + 'zero_quaternion' + '\t' + 'zero_GPS' + '\t' + 'gyro_staticShift' + '\t' + 'accel_staticShift' + '\n')
    f.close()

    f = open(file_fc_logs, 'w')
    f.write("Time" + '\t' + 'State' + '\t' + 'angle left' + '\t' + 'angle right' + '\t' + 'angle keel' + '\t' + 'critical angle' + '\n')
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
        # g.scale(2, 2, 1)
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

        # self.mesh.rotate(180, 0, 0, 1)

        self.addItem(self.mesh)
        # self._update_mesh([0, 0, 1])

    def on_new_records(self, records):
        self._update_mesh(records)

    def _get_mesh_points(self, mesh_path):
        your_mesh = mesh.Mesh.from_file(mesh_path)
        points = your_mesh.points

        points = np.array(list(chain(*points)))
        i = 0
        nd_points = np.ndarray(shape=(len(points)//3, 3,))
        for i in range(0, len(points)//3):
            nd_points[i] = points[i*3: (i+1)*3]

        return nd_points

    def _transform_object(self, target, move=True, rotate=True, scale=1 / 50):
        target.resetTransform()
        target.scale(scale, scale, scale)
        if move: target.translate(0, 0, 0.1)
        if rotate:
            target.rotate(degrees(self.rotation.angle), self.rotation.axis[0], self.rotation.axis[1],
                          self.rotation.axis[2])

    def _update_rotation(self, record):
        quat = pyquaternion.Quaternion(record)
        self.rotation = quat

        self._transform_object(self.mesh)

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

        if TELEM:
            # Инициализация виджетов для телеметрии
            self.telem_widget_stage = TelemWidgetState(layout=self.ui.verticalLayout_2)
            self.telem_widget_stage.set_name_label_1('Инициализация')
            self.telem_widget_stage.set_name_label_2('Погрузка в ракету')
            self.telem_widget_stage.set_name_label_3('Определение нач. состояния')
            self.telem_widget_stage.set_name_label_4('Полет в ракете')
            self.telem_widget_stage.set_name_label_5('Свободное падение')
            self.telem_widget_stage.set_name_label_6('Спуск')
            self.telem_widget_stage.set_name_label_7('Управляемый полет')
            self.telem_widget_stage.set_name_label_8('Режим ожидания')

            self.telem_widget_state = TelemWidgetState(title="State SC sensors", layout=self.ui.verticalLayout_5)
            self.telem_widget_state.set_name_label_1('IMU')
            self.telem_widget_state.set_name_label_2('IMU BMP')
            self.telem_widget_state.set_name_label_3('SD')
            self.telem_widget_state.set_name_label_4('NRF')
            self.telem_widget_state.set_name_label_5('GPS')
            self.telem_widget_state.set_name_label_6('BMP')
            self.telem_widget_state.set_name_label_8('buttons')
            self.telem_widget_state.set_name_label_7('Master')

            self.telem_widget_accel = TelemWidget1(title="Accel RCS", layout=self.ui.verticalLayout_left_1)

            self.telem_widget_magnetometer = TelemWidget1(title="Magnetometer", layout=self.ui.verticalLayout_left_1)

            # self.telem_widget_temp = TelemWidget2(title="Temp", layout=self.ui.verticalLayout_left_3,\
            #                                       name_label_1='temp imu bmp', name_label_2='temp bmp')
            self.telem_widget_pressure = TelemWidget2(title="Pressure", layout=self.ui.verticalLayout_left_3,
                                                      name_label_1='pressure imu bmp', name_label_2='pressure bmp')

            #
            self.telem_widget_gyro = TelemWidget1(title="Gyro", layout=self.ui.verticalLayout_right_1)

            self.telem_widget_quat = TelemWidget1(title="Quaternion", layout=self.ui.verticalLayout_right_1)
            self.telem_widget_quat.set_name_label_1('teta')
            self.telem_widget_quat.set_name_label_2('x')
            self.telem_widget_quat.set_name_label_3('y')
            self.telem_widget_quat.set_name_label_4('z')

            self.telem_widget_speed = TelemWidget2(title="Speed", layout=self.ui.verticalLayout_left_3,
                                                   name_label_1='speed bmp', name_label_2='speed GPS')
            self.telem_widget_coord = TelemWidget3(title="Coordinates", layout=self.ui.verticalLayout_right_2, name_label_1='x', name_label_2='y', name_label_3='height')
            self.telem_widget_angles = TelemWidget3(title="Angles", layout=self.ui.verticalLayout_left_2, name_label_1='left', name_label_2='right', name_label_3='keel')

            self.telem_widget_buttons = TelemWidgetButtons(title="Buttons", layout=self.ui.verticalLayout_right_3)

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
        self.a_RSC_x = []
        self.a_RSC_y = []
        self.a_RSC_z = []

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

        self.time_GPS = []
        self.x = []
        self.y = []
        self.GPS = []
        self.speed_GPS = []

        self.buffer_bmp_msg = []
        self.buffer_imu_isc_msg = []
        self.buffer_imu_rsc_msg = []
        self.buffer_sensors_msg = []
        self.buffer_gps_msg = []
        self.buffer_servo_msg = []
        self.buffer_zero_data = []
        self.buffer_fc_logs = []

        self.glw_top = GraphLayout(centralwidget=self.ui.centralwidget, layout=self.ui.top, count_of_graphs=2,
                                   count_plot_lines=3, graph_name=['Accel', 'GPS speed'])
        self.glw_middle = GraphLayout(centralwidget=self.ui.centralwidget, layout=self.ui.middle, count_of_graphs=2,
                                      count_plot_lines=3, graph_name=['Angular speed', 'Vector of magnetic field'])
        self.glw_down = GraphLayout(centralwidget=self.ui.centralwidget, layout=self.ui.down, count_of_graphs=2,
                                    count_plot_lines=3, graph_name=['Temperature', 'Pressure'])
        self.glw_top_2 = GraphLayout(centralwidget=self.ui.centralwidget, layout=self.ui.grid_right_2_graf,
                                     count_of_graphs=2, count_plot_lines=3, graph_name=['GPS', 'BMP speed'])

        # Верхний ряд графиков
        # self.ui.glv_top = pg.GraphicsLayoutWidget(self.ui.centralwidget)
        # self.ui.top.addWidget(self.ui.glv_top)
        # self.ui.plot_top = pg.GraphicsLayout()
        # self.ui.glv_top.addItem(self.ui.plot_top)
        #
        # self.sc_item_top1 = pg.PlotItem(title='Accel RSC')
        # self.ui.plot_top.addItem(self.sc_item_top1)
        # self.graf_top1 = pg.PlotCurveItem()
        # self.sc_item_top1.addItem(self.graf_top1)
        #
        # self.pl_graf_top1_x = self.sc_item_top1.plot()
        # self.pl_graf_top1_y = self.sc_item_top1.plot()
        # self.pl_graf_top1_z = self.sc_item_top1.plot()
        #
        # self.sc_item_top2 = pg.PlotItem(title='GPS speed')
        # self.ui.plot_top.addItem(self.sc_item_top2)
        # self.graf_top2 = pg.PlotCurveItem()
        # self.sc_item_top2.addItem(self.graf_top2)
        #
        # self.pl_graf_top2_x = self.sc_item_top2.plot()
        # self.pl_graf_top2_z = self.sc_item_top2.plot()
        #
        #
        # # Создание средних графиков
        # self.ui.glv_middle = pg.GraphicsLayoutWidget(self.ui.centralwidget)
        # self.ui.middle.addWidget(self.ui.glv_middle)
        # self.ui.plot_middle = pg.GraphicsLayout()
        # self.ui.glv_middle.addItem(self.ui.plot_middle)
        #
        # self.sc_item_middle1 = pg.PlotItem(title='Angular speed')
        # self.ui.plot_middle.addItem(self.sc_item_middle1)
        # self.graf_middle1 = pg.PlotCurveItem()
        # self.sc_item_middle1.addItem(self.graf_middle1)
        #
        # self.pl_graf_middle1_x = self.sc_item_middle1.plot()
        # self.pl_graf_middle1_y = self.sc_item_middle1.plot()
        # self.pl_graf_middle1_z = self.sc_item_middle1.plot()
        #
        # self.sc_item_middle2 = pg.PlotItem(title='Vector of magnetic field')
        # self.ui.plot_middle.addItem(self.sc_item_middle2)
        # self.graf_middle2 = pg.PlotCurveItem()
        # self.sc_item_middle2.addItem(self.graf_middle2)
        #
        # self.pl_graf_middle2_x = self.sc_item_middle2.plot()
        # self.pl_graf_middle2_y = self.sc_item_middle2.plot()
        # self.pl_graf_middle2_z = self.sc_item_middle2.plot()
        #
        # # нижний ряд графиков
        # self.ui.glv_down = pg.GraphicsLayoutWidget(self.ui.centralwidget)
        # self.ui.down.addWidget(self.ui.glv_down)
        # self.ui.plot_down = pg.GraphicsLayout()
        # self.ui.glv_down.addItem(self.ui.plot_down)
        #
        # self.sc_item_down1 = pg.PlotItem(title='Temperature')
        # self.ui.plot_down.addItem(self.sc_item_down1)
        # self.graf_down1 = pg.PlotCurveItem()
        # self.sc_item_down1.addItem(self.graf_down1)
        #
        # self.pl_graf_down1_x = self.sc_item_down1.plot()
        #
        # self.sc_item_down2 = pg.PlotItem(title='Pressure')
        # self.ui.plot_down.addItem(self.sc_item_down2)
        # self.graf_down2 = pg.PlotCurveItem()
        # self.sc_item_down2.addItem(self.graf_down2)
        #
        # self.pl_graf_down2_x = self.sc_item_down2.plot()
        #
        # # Верхний правый ряд графиков
        # self.ui.glv_top_2 = pg.GraphicsLayoutWidget(self.ui.centralwidget)
        # self.ui.grid_right_2_graf.addWidget(self.ui.glv_top_2)
        # self.ui.plot_top_2 = pg.GraphicsLayout()
        # self.ui.glv_top_2.addItem(self.ui.plot_top_2)
        #
        # self.sc_item_top3 = pg.PlotItem(title='GPS')
        # self.ui.plot_top_2.addItem(self.sc_item_top3)
        # self.graf_top3 = pg.PlotCurveItem()
        # self.sc_item_top3.addItem(self.graf_top3)
        #
        # self.pl_graf_top3_x = self.sc_item_top3.plot()
        # self.pl_graf_top3_y = self.sc_item_top3.plot()
        #
        # self.sc_item_top4 = pg.PlotItem(title='BMP speed')
        # self.ui.plot_top_2.addItem(self.sc_item_top4)
        # self.graf_top4 = pg.PlotCurveItem()
        # self.sc_item_top4.addItem(self.graf_top4)
        #
        # self.pl_graf_top4_x = self.sc_item_top4.plot()

        # 3D график
        self.ui.dockwid = QtWidgets.QDockWidget()
        self.ui.grid_3D.addWidget(self.ui.dockwid)

        self.ui.glwid = self.plane_widget
        self.ui.dockwid.setWidget(self.ui.glwid)
        self.isc_coord = gl.GLAxisItem()
        self.isc_coord.setSize(25, 25, 25)
        self.ui.glwid.show()

        # Здесь прописываем событие нажатия на кнопку
        # self.ui.pushButton_3.clicked.connect(self.remove_graf)
        self.ui.commandLinkButton.clicked.connect(self.send_command)

    # функция для записи в файл
    def write_to_file(self, buffer, file):
        f = open(file, 'a')
        for foo in range(len(buffer)):
            f.write(buffer[foo])
        f.close()

    # функция для отправки команд на борт
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

    # Слот для разбора пакета bmp
    @QtCore.pyqtSlot(list)
    def bmp_msg(self, msgs):   # сообщения с bmp (не с MPU)!!!!
        print('msg_bmp')
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

        if TELEM:
            # self.telem_widget_temp.set_value_2(round(self.temp_bmp[len(self.temp_bmp) - 1], 3))
            self.telem_widget_pressure.set_value_2(round(self.pressure_bmp[len(self.pressure_bmp) - 1], 3))
            self.telem_widget_speed.set_value_1(round(self.speed_bmp[len(self.speed_bmp) - 1], 3))

        if FILE_WRITE:
            self.write_to_file(self.buffer_bmp_msg, file_bmp)
            self.buffer_bmp_msg = []

        if len(self.time_bmp) > self.lenght:
            self.time_bmp = self.time_bmp[self.cut:(self.lenght - 1)]
            self.pressure_bmp = self.pressure_bmp[self.cut:(self.lenght - 1)]
            self.temp_bmp = self.temp_bmp[self.cut:(self.lenght - 1)]
            self.speed_bmp = self.speed_bmp[self.cut:(self.lenght - 1)]
        self.glw_top_2.graphs[1].plot_lines[0].setData(x=self.time_bmp, y=self.speed_bmp, pen=('r'))
        # self.pl_graf_top4_x.setData(x=self.time_bmp, y=self.speed_bmp, pen=('r'))

    # Слот для разбора пакета imu_rsc
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

            self.vmf_x.append(msgs[i].compass[0])
            self.vmf_y.append(msgs[i].compass[1])
            self.vmf_z.append(msgs[i].compass[2])

            if FILE_WRITE:
                self.buffer_imu_rsc_msg.append(str(msgs[i].time) + '\t' + '\t' +
                                        str(msgs[i].accel[0]) + ' ' + str(msgs[i].accel[1]) + ' ' + str(msgs[i].accel[2]) + '\t' + '\t' +
                                        str(msgs[i].compass[0]) + ' ' + str(msgs[i].compass[1]) + ' ' + str(msgs[i].compass[2]) + '\t' + '\t' +
                                        str(msgs[i].gyro[0]) + ' ' + str(msgs[i].gyro[1]) + ' ' + str(msgs[i].gyro[2]) + '\n')

        if TELEM:
            last_x = self.av_x[len(self.av_x) - 1]
            last_y = self.av_y[len(self.av_y) - 1]
            last_z = self.av_z[len(self.av_z) - 1]
            self.telem_widget_gyro.set_value_1(round(last_x, 3))
            self.telem_widget_gyro.set_value_2(round(last_y, 3))
            self.telem_widget_gyro.set_value_3(round(last_z, 3))
            self.telem_widget_gyro.set_value_4(round(result(last_x, last_y, last_z), 3))

            last_x = self.vmf_x[len(self.vmf_x) - 1]
            last_y = self.vmf_y[len(self.vmf_y) - 1]
            last_z = self.vmf_z[len(self.vmf_z) - 1]
            self.telem_widget_magnetometer.set_value_1(round(last_x, 3))
            self.telem_widget_magnetometer.set_value_2(round(last_y, 3))
            self.telem_widget_magnetometer.set_value_3(round(last_z, 3))
            self.telem_widget_magnetometer.set_value_4(round(result(last_x, last_y, last_z), 3))

            last_x = self.a_RSC_x[len(self.a_RSC_x) - 1]
            last_y = self.a_RSC_y[len(self.a_RSC_y) - 1]
            last_z = self.a_RSC_z[len(self.a_RSC_z) - 1]
            self.telem_widget_accel.set_value_1(round(last_x, 3))
            self.telem_widget_accel.set_value_2(round(last_y, 3))
            self.telem_widget_accel.set_value_3(round(last_z, 3))
            self.telem_widget_accel.set_value_4(result(last_x, last_y, last_z))

        if FILE_WRITE:
            self.write_to_file(self.buffer_imu_rsc_msg, file_imu_rsc)
            self.buffer_imu_rsc_msg = []

        if len(self.time_RSC) > self.lenght:
            self.time_RSC = self.time_RSC[self.cut:(self.lenght - 1)]
            self.av_x = self.av_x[self.cut:(self.lenght - 1)]
            self.av_y = self.av_y[self.cut:(self.lenght - 1)]
            self.av_z = self.av_z[self.cut:(self.lenght - 1)]

            self.vmf_x = self.vmf_x[self.cut:(self.lenght - 1)]
            self.vmf_y = self.vmf_y[self.cut:(self.lenght - 1)]
            self.vmf_z = self.vmf_z[self.cut:(self.lenght - 1)]

            self.a_RSC_x = self.a_RSC_x[self.cut:(self.lenght - 1)]
            self.a_RSC_y = self.a_RSC_y[self.cut:(self.lenght - 1)]
            self.a_RSC_z = self.a_RSC_z[self.cut:(self.lenght - 1)]

        self.glw_top.graphs[0].plot_lines[0].setData(x=self.time_RSC, y=self.a_RSC_x, pen=('r'), width=0.5)
        self.glw_top.graphs[0].plot_lines[1].setData(x=self.time_RSC, y=self.a_RSC_y, pen=('g'), width=0.5)
        self.glw_top.graphs[0].plot_lines[2].setData(x=self.time_RSC, y=self.a_RSC_z, pen=('b'), width=0.5)

        self.glw_middle.graphs[0].plot_lines[0].setData(x=self.time_RSC, y=self.av_x, pen=('r'), width=0.5)
        self.glw_middle.graphs[0].plot_lines[1].setData(x=self.time_RSC, y=self.av_y, pen=('g'), width=0.5)
        self.glw_middle.graphs[0].plot_lines[2].setData(x=self.time_RSC, y=self.av_z, pen=('b'), width=0.5)

        self.glw_middle.graphs[1].plot_lines[0].setData(x=self.time_RSC, y=self.vmf_x, pen=('r'), width=0.5)
        self.glw_middle.graphs[1].plot_lines[1].setData(x=self.time_RSC, y=self.vmf_y, pen=('g'), width=0.5)
        self.glw_middle.graphs[1].plot_lines[2].setData(x=self.time_RSC, y=self.vmf_z, pen=('b'), width=0.5)
        # self.pl_graf_top1_x.setData(x=self.time_RSC, y=self.a_RSC_x, pen=('r'), width=0.5)
        # self.pl_graf_top1_y.setData(x=self.time_RSC, y=self.a_RSC_y, pen=('g'), width=0.5)
        # self.pl_graf_top1_z.setData(x=self.time_RSC, y=self.a_RSC_z, pen=('b'), width=0.5)
        #
        # self.pl_graf_middle1_x.setData(x=self.time_RSC, y=self.av_x, pen=('r'), width=0.5)
        # self.pl_graf_middle1_y.setData(x=self.time_RSC, y=self.av_y, pen=('g'), width=0.5)
        # self.pl_graf_middle1_z.setData(x=self.time_RSC, y=self.av_z, pen=('b'), width=0.5)
        #
        # self.pl_graf_middle2_x.setData(x=self.time_RSC, y=self.vmf_x, pen=('r'), width=0.5)
        # self.pl_graf_middle2_y.setData(x=self.time_RSC, y=self.vmf_y, pen=('g'), width=0.5)
        # self.pl_graf_middle2_z.setData(x=self.time_RSC, y=self.vmf_z, pen=('b'), width=0.5)

    # Слот для разбора пакета imu_isc
    @QtCore.pyqtSlot(list)
    def imu_isc_msg(self, msgs):
        print('msg_imu_isc')
        for i in range(len(msgs)):
            self.quat.append(msgs[i].quaternion)

            if FILE_WRITE:
                self.buffer_imu_isc_msg.append(str(msgs[i].time) + '\t' + '\t' +
                                        str(msgs[i].accel[0]) + ' ' + str(msgs[i].accel[1]) + ' ' + str(msgs[i].accel[2]) + '\t' + '\t' +
                                        str(msgs[i].compass[0]) + ' ' + str(msgs[i].compass[1]) + ' ' + str(msgs[i].compass[2]) + '\t' + '\t' +
                                        str(msgs[i].quaternion[0]) + ' ' + str(msgs[i].quaternion[1]) + ' ' + str(msgs[i].quaternion[2]) + ' ' + str(msgs[i].quaternion[3]) + '\n')

            quat = pyquaternion.Quaternion(msgs[i].quaternion)
            self.plane_widget._update_rotation(quat)

        if TELEM:
            self.telem_widget_quat.set_value_1(round(self.quat[len(self.quat) - 1][0], 3))
            self.telem_widget_quat.set_value_2(round(self.quat[len(self.quat) - 1][1], 3))
            self.telem_widget_quat.set_value_3(round(self.quat[len(self.quat) - 1][2], 3))
            self.telem_widget_quat.set_value_4(round(self.quat[len(self.quat) - 1][3], 3))

        if FILE_WRITE:
            self.write_to_file(self.buffer_imu_isc_msg, file_imu_isc)
            self.buffer_imu_isc_msg = []

    # Слот для разбора пакета IMUbmp
    @QtCore.pyqtSlot(list)
    def sens_msg(self, msgs):
        print('msg_sensors')
        height= 0
        for i in range(len(msgs)):
            self.time_bmpIMU.append(msgs[i].time)
            self.pressure_bmpIMU.append(msgs[i].pressure)
            self.temp_bmpIMU.append(msgs[i].temp)
            height = msgs[i].height

            if FILE_WRITE:
                self.buffer_sensors_msg.append(str(msgs[i].time) + '\t' + '\t' +
                                               str(msgs[i].pressure) + '\t' + '\t' +
                                               str(msgs[i].temp) + '\t' + '\t' +
                                               str(msgs[i].height) + '\n')

        if TELEM:
            # self.telem_widget_temp.set_value_1(round(self.temp_bmpIMU[len(self.temp_bmpIMU) - 1], 3))
            self.telem_widget_pressure.set_value_1(round(self.pressure_bmpIMU[len(self.pressure_bmpIMU) - 1], 3))
            self.telem_widget_coord.set_value_3(height)

        if FILE_WRITE:
            self.write_to_file(self.buffer_sensors_msg, file_sensors)
            self.buffer_sensors_msg = []

        if len(self.time_bmpIMU) > self.lenght:
            self.time_bmpIMU = self.time_bmpIMU[self.cut:(self.lenght - 1)]
            self.pressure_bmpIMU = self.pressure_bmpIMU[self.cut:(self.lenght - 1)]
            self.temp_bmpIMU = self.temp_bmpIMU[self.cut:(self.lenght - 1)]

        self.glw_down.graphs[0].plot_lines[0].setData(x=self.time_bmpIMU, y=self.temp_bmpIMU, pen=('r'))
        self.glw_down.graphs[1].plot_lines[0].setData(x=self.time_bmpIMU, y=self.pressure_bmpIMU, pen=('r'))
        # self.pl_graf_down1_x.setData(x=self.time_bmpIMU, y=self.temp_bmpIMU, pen=('r'))
        # self.pl_graf_down2_x.setData(x=self.time_bmpIMU, y=self.pressure_bmpIMU, pen=('r'))

    # Слот для разбора пакета GPS
    @QtCore.pyqtSlot(list)
    def gps_msg(self, msgs):
        print('msg_gps')
        course = []
        for i in range(len(msgs)):
            self.x.append(msgs[i].coordinates[0])
            self.y.append(msgs[i].coordinates[1])
            self.speed_GPS.append(msgs[i].speed)
            self.time_GPS.append(msgs[i].time)
            course.append(msgs[i].course)
            print(msgs[i].course)

            if FILE_WRITE:
                self.buffer_gps_msg.append(str(msgs[i].time) + '\t' +
                                           str(msgs[i].coordinates[0]) + '\t' + '\t' +
                                           str(msgs[i].coordinates[1]) + '\t' + '\t' +
                                           str(msgs[i].speed) + '\t' +
                                           str(msgs[i].course) + '\n')

            # FIXME: WTF?????????
            y0 = []
            x0 = []
            x0.append(self.x[0])
            y0.append(self.y[0])

        if FILE_WRITE:
            self.write_to_file(self.buffer_gps_msg, file_gps)
            self.buffer_gps_msg = []

        if TELEM:
            self.telem_widget_speed.set_value_2(round(self.speed_GPS[len(self.speed_GPS) - 1], 3))
            self.telem_widget_coord.set_value_1(round(self.x[len(self.x) - 1], 3))
            self.telem_widget_coord.set_value_2(round(self.y[len(self.y) - 1], 3))
            # self.telem_widget_coord.set_value_4(course[len(course) - 1])

        if len(self.x) > self.lenght:
            self.x = self.x[self.cut:(self.lenght - 1)]
            self.y = self.y[self.cut:(self.lenght - 1)]

        self.glw_top_2.graphs[0].plot_lines[0].setData(x=self.x, y=self.y, pen=('r'))
        self.glw_top.graphs[1].plot_lines[0].setData(x=self.time_GPS, y=self.speed_GPS, pen=('r'))
        # self.pl_graf_top3_x.setData(x=self.x, y=self.y, pen=('r'))
        # self.pl_graf_top2_x.setData(x=self.time_GPS, y=self.speed_GPS, pen=('r'))

    # Слот для разбора пакета state
    @QtCore.pyqtSlot(list)
    def state_msg(self, msgs):
        print('msg_state')
        for i in range(len(msgs)):
            if TELEM:
                self.state_fly = msgs[i].globalStage
                self.state_buttons = msgs[i].buttons

                self.telem_widget_stage.clear_values()
                self.telem_widget_state.clear_values()

                self.telem_widget_state.set_value_1(msgs[i].MPU_state)
                self.telem_widget_state.set_value_2(msgs[i].IMU_BMP_state)
                self.telem_widget_state.set_value_3(msgs[i].SD_state)
                self.telem_widget_state.set_value_4(msgs[i].NRF_state)
                self.telem_widget_state.set_value_5(msgs[i].GPS_state)
                self.telem_widget_state.set_value_6(msgs[i].BMP_state)
                self.telem_widget_state.set_value_7(msgs[i].master_state)
                self.telem_widget_state.set_value_8(msgs[i].buttons)

            #
            if self.state_fly == 0:
                self.telem_widget_stage.set_value_1(1)

            elif self.state_fly == 1:
                self.telem_widget_stage.set_value_2(1)

            elif self.state_fly == 2:
                self.telem_widget_stage.set_value_3(1)

            elif self.state_fly == 3:
                self.telem_widget_stage.set_value_4(1)

            elif self.state_fly == 4:
                self.telem_widget_stage.set_value_5(1)

            elif self.state_fly == 5:
                self.telem_widget_stage.set_value_6(1)

            elif self.state_fly == 6:
                self.telem_widget_stage.set_value_7(1)

            elif self.state_fly == 7:
                self.telem_widget_stage.set_value_8(1)

            #
            if self.state_buttons == 1:
                self.telem_widget_buttons.set_color(self.telem_widget_buttons.ui.toolButton_wing_left, color_green)

            elif self.state_buttons == 2:
                self.telem_widget_buttons.set_color(self.telem_widget_buttons.ui.toolButton_wing_right, color_green)

            elif self.state_buttons == 3:
                self.telem_widget_buttons.set_color(self.telem_widget_buttons.ui.toolButton_go_left, color_green)

            elif self.state_buttons == 4:
                self.telem_widget_buttons.set_color(self.telem_widget_buttons.ui.toolButton_go_right, color_green)

            elif self.state_buttons == 5:
                self.telem_widget_buttons.set_color(self.telem_widget_buttons.ui.toolButton_go_keel, color_green)

    # Слот для разбора пакета servo
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
            self.buffer_servo_msg = []

    # Слот для разбора пакета zero_data
    @QtCore.pyqtSlot(list)
    def zero_data_msg(self, msgs):
        if self.zero_data:
            zero_pressure = []
            for i in range(len(msgs)):
                zero_pressure.append(msgs[i].zero_pressure)

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
                self.buffer_zero_data = []

            if TELEM:
                self.telem_widget_coord.set_value_3(zero_pressure[len(zero_pressure) - 1])

    # Слот для разбора пакета fc_logs
    @QtCore.pyqtSlot(list)
    def fc_logs_msg(self, msgs):
        print("Fc_logs")
        left = []
        right = []
        keel = []
        for i in range(len(msgs)):
            left.append(msgs[i].angle_left)
            right.append(msgs[i].angle_right)
            keel.append(msgs[i].angle_keel)

            if FILE_WRITE:
                self.buffer_fc_logs.append(str(msgs[i].time) + '\t' +
                                           str(msgs[i].FC_stage) + '\t' +
                                           str(msgs[i].angle_left) + '\t' +
                                           str(msgs[i].angle_right) + '\t' +
                                           str(msgs[i].angle_keel) + '\t' +
                                           str(msgs[i].critical_angles) + '\n')
        #
        if FILE_WRITE:
            self.write_to_file(self.buffer_fc_logs, file_fc_logs)
            self.buffer_fc_logs = []

        if TELEM:
            self.telem_widget_angles.set_value_1(left[len(left) - 1])
            self.telem_widget_angles.set_value_2(right[len(right) - 1])
            self.telem_widget_angles.set_value_3(keel[len(keel) - 1])
