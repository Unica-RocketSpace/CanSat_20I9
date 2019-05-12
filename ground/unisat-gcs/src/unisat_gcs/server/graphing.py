import sys
import time
import os
from math import *

import numpy as np
import pyqtgraph as pg
from .gcs_ui import *

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


    f = open(file_bmp, 'w')
    f.write("Time" + '\t' + "Pressure" + '\t' + "Temp" + '\n')
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

log_text = ''
global_vars={'x': 0, 'y': 0}
now_graf = None
str_now_graf = None


#
class PlaneWidget(gl.GLViewWidget):
    def __init__(self, mesh_path, *args, **kwargs):
        super(PlaneWidget, self).__init__(*args, **kwargs)
        self.setCameraPosition(distance=600)
        self.setBackgroundColor([100, 100, 100, 0])
        g = gl.GLGridItem()
        g.scale(2, 2, 1)
        # g.translate(0, 0, 0)
        self.addItem(g)


        isc_coord = gl.GLAxisItem()
        isc_coord.setSize(100, 100, 100)
        self.addItem(isc_coord)

        self.plane_axis = gl.GLAxisItem()
        self.plane_axis.setSize(x=50, y=50, z=50)
        self.addItem(self.plane_axis)

        verts = self._get_mesh_points(mesh_path)
        faces = np.array([(i, i+1, i+2,) for i in range(0, len(verts), 3)])
        colors = np.array([(0.0, 1.0, 0.0, 1.0,) for i in range(0, len(verts), 3)])
        self.mesh = gl.GLMeshItem(vertexes=verts, faces=faces, faceColors=colors, smooth=False, shader='shaded')
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
        # self.acc.setCords(
        #     *quat.rotate(record.acc))  # Поворачиваем ускорение не при отображении, а действительно меняя вектор
        # record.mag = [axis * 40 for axis in record.mag]
        # self.mag.setCords(*quat.rotate(record.mag))
        self._transform_object(self.mesh)
        self._transform_object(self.plane_axis, move=False)
        # self._transform_object(self.acc, move=False, rotate=False, scale=10)  # Не поворачиваем ускорение, т.к. уже повернули его


    # def _update_mesh(self, nadir_v):
    #
    #     # nadir_v_original = np.array([0, 0, 1], dtype='double')
    #     # nadir_v_original /= np.linalg.norm(nadir_v_original)
    #     # print(nadir_v_original)
    #     #
    #     # nadir_v = np.array(nadir_v, dtype='double')
    #     # nadir_v /= np.linalg.norm(nadir_v)
    #     #
    #     # angle = np.dot(nadir_v_original, nadir_v)
    #     # angle = acos(angle)
    #     # angle = angle*180/np.pi
    #     # axis = np.cross(nadir_v_original, nadir_v)
    #
    #     # print(axis, angle)
    #
    #
    #     def do_things(target, move=True):
    #         # target.resetTransform()
    #         # target.scale(1/50, 1/50, 1/50)
    #         # target.rotate(90, 1, 0, 0)
    #         # target.rotate(180, 0, 0, 1)
    #         # if move: target.translate(0, -3, 0)
    #         # target.rotate(angle, axis[0], axis[1], axis[2])
    #         target.setTransform(nadir_v)
    #
    #     do_things(self.mesh)
    #     do_things(self.plane_axis, move=False)



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


        pg.setConfigOption('background', 'w')
        pg.setConfigOption('foreground', 'k')

        # self.accel_f = open("C:\Cansat Unica 2018\calibration\\accel_calibration", "wt")
        # self.compass_f = open("C:\Cansat Unica 2018\calibration\magn_calibration", "wt")

        self.lenght = 150
        self.cut = 11

        self.temp_atmega = []
        self.pressure_atmega = []
        self.time_atm = []

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

        self.a_RSC_x = []
        self.a_RSC_y = []
        self.a_RSC_z = []

        self.av_x = []
        self.av_y = []
        self.av_z = []

        self.time_RSC = []

        self.a_ISC_x = []
        self.a_ISC_y = []
        self.a_ISC_z = []

        # self.v_x = []
        #
        # self.v_y = []
        # self.v_z = []

        # self.mov_x = []
        # self.mov_y = []
        # self.mov_z = []
        #
        self.vmf_x = []
        self.vmf_y = []
        self.vmf_z = []

        self.time_ISC = []

        self.quat = []

        self.temp_sensors = []
        self.pressure_sensors = []
        self.time_sens = []

        self.x = []
        self.y = []
        self.z = []

        self.GPS = []

        self.buffer_bmp_msg = []
        self.buffer_imu_isc_msg = []
        self.buffer_imu_rsc_msg = []
        self.buffer_sensors_msg = []
        self.buffer_servo_msg = []

        self.ui.glv_top = pg.GraphicsLayoutWidget(self.ui.centralwidget)
        self.ui.top.addWidget(self.ui.glv_top)
        self.ui.plot_top = pg.GraphicsLayout()
        self.ui.glv_top.addItem(self.ui.plot_top)


        self.sc_item_top1 = pg.PlotItem(title='Accel ISC', labels={'left': 'accel', 'bottom': 'time'})
        self.ui.plot_top.addItem(self.sc_item_top1)
        self.graf_top1 = pg.PlotCurveItem()
        self.sc_item_top1.addItem(self.graf_top1)

        self.sc_item_top2 = pg.PlotItem(title='Accel RSC')
        self.ui.plot_top.addItem(self.sc_item_top2)
        self.graf_top2 = pg.PlotCurveItem()
        self.sc_item_top2.addItem(self.graf_top2)

        self.sc_item_top3 = pg.PlotItem(title='Velosity')
        self.ui.plot_top.addItem(self.sc_item_top3)
        self.graf_top3 = pg.PlotCurveItem()
        self.sc_item_top3.addItem(self.graf_top3)


        self.ui.glv_middle = pg.GraphicsLayoutWidget(self.ui.centralwidget)
        self.ui.middle.addWidget(self.ui.glv_middle)
        self.ui.plot_middle = pg.GraphicsLayout()
        self.ui.glv_middle.addItem(self.ui.plot_middle)

        self.sc_item_middle1 = pg.PlotItem(title='Angles velosity')
        self.ui.plot_middle.addItem(self.sc_item_middle1)
        self.graf_middle1 = pg.PlotCurveItem()
        self.sc_item_middle1.addItem(self.graf_middle1)

        self.sc_item_middle2 = pg.PlotItem(title='Movement')
        self.ui.plot_middle.addItem(self.sc_item_middle2)
        self.graf_middle2 = pg.PlotCurveItem()
        self.sc_item_middle2.addItem(self.graf_middle2)

        self.sc_item_middle3 = pg.PlotItem(title='Vector of magnetic field')
        self.ui.plot_middle.addItem(self.sc_item_middle3)
        self.graf_middle3 = pg.PlotCurveItem()
        self.sc_item_middle3.addItem(self.graf_middle3)


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

        self.sc_item_down3 = pg.PlotItem(title='GPS')

        self.point = pg.PlotItem()
        self.ui.plot_down.addItem(self.sc_item_down3)
        self.graf_down3 = pg.PlotCurveItem()
        self.sc_item_down3.addItem(self.graf_down3)


        self.ui.glv_large = pg.GraphicsLayoutWidget(self.ui.centralwidget)
        self.ui.grid_large_graf.addWidget(self.ui.glv_large)
        self.ui.plot_large = pg.GraphicsLayout()
        self.ui.glv_large.addItem(self.ui.plot_large)
        self.sc_item_large = pg.PlotItem()
        self.ui.plot_large.addItem(self.sc_item_large)
        self.graf_large = pg.PlotCurveItem()
        self.sc_item_large.addItem(self.graf_large)


        self.ui.dockwid = QtWidgets.QDockWidget()
        self.ui.grid_3D.addWidget(self.ui.dockwid)

        self.ui.glwid = self.plane_widget
        self.ui.dockwid.setWidget(self.ui.glwid)
        # self.gx = gl.GLGridItem()
        # self.gx.rotate(90, 0, 1, 0)
        # self.gx.translate(0, 10, 10)
        # self.ui.glwid.addItem(self.gx)
        # self.gy = gl.GLGridItem()
        # self.gy.rotate(90, 1, 0, 0)
        # self.gy.translate(10, 0, 10)
        # self.ui.glwid.addItem(self.gy)
        # self.gz = gl.GLGridItem()
        # self.gz.translate(10, 10, 0)
        # self.ui.glwid.addItem(self.gz)
        # self.plt = gl.GLLinePlotItem()
        # self.ui.glwid.addItem(self.plt)
        self.isc_coord = gl.GLAxisItem()
        self.isc_coord.setSize(25, 25, 25)
        # self.rsc_coord = gl.GLAxisItem()


        # self.ui.glwid.addItem(self.isc_coord)
        # self.ui.glwid.addItem(self.rsc_coord)
        self.ui.glwid.show()
        # self.plane_widget.show()

        self.pl_graf_top1_x = self.sc_item_top1.plot()
        self.pl_graf_top2_x = self.sc_item_top2.plot()
        self.pl_graf_top3_x = self.sc_item_top3.plot()

        self.pl_graf_top1_y = self.sc_item_top1.plot()
        self.pl_graf_top2_y = self.sc_item_top2.plot()
        self.pl_graf_top3_y = self.sc_item_top3.plot()

        self.pl_graf_top1_z = self.sc_item_top1.plot()
        self.pl_graf_top2_z = self.sc_item_top2.plot()
        self.pl_graf_top3_z = self.sc_item_top3.plot()


        self.pl_graf_middle1_x = self.sc_item_middle1.plot()
        self.pl_graf_middle2_x = self.sc_item_middle2.plot()
        self.pl_graf_middle3_x = self.sc_item_middle3.plot()

        self.pl_graf_middle1_y = self.sc_item_middle1.plot()
        self.pl_graf_middle2_y = self.sc_item_middle2.plot()
        self.pl_graf_middle3_y = self.sc_item_middle3.plot()

        self.pl_graf_middle1_z = self.sc_item_middle1.plot()
        self.pl_graf_middle2_z = self.sc_item_middle2.plot()
        self.pl_graf_middle3_z = self.sc_item_middle3.plot()


        self.pl_graf_down1_x = self.sc_item_down1.plot()
        self.pl_graf_down2_x = self.sc_item_down2.plot()
        self.pl_graf_down3_x = self.sc_item_down3.plot()

        self.pl_graf_down1_y = self.sc_item_down1.plot()
        self.pl_graf_down2_y = self.sc_item_down2.plot()
        self.pl_graf_down3_y = self.sc_item_down3.plot()

        self.pl_graf_down1_z = self.sc_item_down1.plot()
        self.pl_graf_down2_z = self.sc_item_down2.plot()
        self.pl_graf_down3_z = self.sc_item_down3.plot()


        # Здесь прописываем событие нажатия на кнопку
        self.ui.pushButton_3.clicked.connect(self.Remove_graf)
        self.ui.commandLinkButton.clicked.connect(self.send_command)
        self.ui.pushButton.clicked.connect(self.clear_telem)
        self.ui.pushButton_13.clicked.connect(self.transfer_graf_top1)
        self.ui.pushButton_14.clicked.connect(self.transfer_graf_top2)
        self.ui.pushButton_12.clicked.connect(self.transfer_graf_top3)
        self.ui.pushButton_6.clicked.connect(self.transfer_graf_middle1)
        self.ui.pushButton_7.clicked.connect(self.transfer_graf_middle2)
        self.ui.pushButton_5.clicked.connect(self.transfer_graf_middle3)
        self.ui.pushButton_8.clicked.connect(self.transfer_graf_down1)
        self.ui.pushButton_9.clicked.connect(self.transfer_graf_down2)
        self.ui.pushButton_10.clicked.connect(self.transfer_graf_down3)


    def clear_log(self):
        global log_text
        log_text = ''
        self.ui.textBrowser_2.setText(log_text)


    def log_add(self, log_msg):
        global log_text
        log_text = log_text + str(log_msg) + "\n"
        self.ui.textBrowser_2.setText(log_text)


    def send_command(self):
        com = self.ui.textBrowser.toPlainText()

        try:
            com = int(com)
            self.new_send_command.emit(com)
        except BaseException:
            com = None
        self.ui.textBrowser.clear()


    def Remove_graf(self):
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

    def clear_telem(self):
        self.ui.textBrowser_2.clear()


    def check_now_graf(self):
        global now_graf, str_now_graf

        if now_graf == None:
            return 0, 0, 0, 0

        if str_now_graf == 'graf_top1':
            graf_1 = self.sc_item_top1
            graf_2_x = self.pl_graf_top1_x
            graf_2_y = self.pl_graf_top1_y
            graf_2_z = self.pl_graf_top1_z
            return graf_1, graf_2_x, graf_2_y, graf_2_z

        if str_now_graf == 'graf_top2':
            graf_1 = self.sc_item_top2
            graf_2_x = self.pl_graf_top2_x
            graf_2_y = self.pl_graf_top2_y
            graf_2_z = self.pl_graf_top2_z
            return graf_1, graf_2_x, graf_2_y, graf_2_z

        if str_now_graf == 'graf_top3':
            graf_1 = self.sc_item_top3
            graf_2_x = self.pl_graf_top3_x
            graf_2_y = self.pl_graf_top3_y
            graf_2_z = self.pl_graf_top3_z
            return graf_1, graf_2_x, graf_2_y, graf_2_z

        if str_now_graf == 'graf_middle1':
            graf_1 = self.sc_item_middle1
            graf_2_x = self.pl_graf_middle1_x
            graf_2_y = self.pl_graf_middle1_y
            graf_2_z = self.pl_graf_middle1_z
            return graf_1, graf_2_x, graf_2_y, graf_2_z

        if str_now_graf == 'graf_middle2':
            graf_1 = self.sc_item_middle2
            graf_2_x = self.pl_graf_middle2_x
            graf_2_y = self.pl_graf_middle2_y
            graf_2_z = self.pl_graf_middle2_z
            return graf_1, graf_2_x, graf_2_y, graf_2_z

        if str_now_graf == 'graf_middle3':
            graf_1 = self.sc_item_middle3
            graf_2_x = self.pl_graf_middle3_x
            graf_2_y = self.pl_graf_middle3_y
            graf_2_z = self.pl_graf_middle3_z
            return graf_1, graf_2_x, graf_2_y, graf_2_z

        if str_now_graf == 'graf_down1':
            graf_1 = self.sc_item_down1
            graf_2_x = self.pl_graf_down1_x
            graf_2_y = self.pl_graf_down1_y
            graf_2_z = self.pl_graf_down1_z
            return graf_1, graf_2_x, graf_2_y, graf_2_z

        if str_now_graf == 'graf_down2':
            graf_1 = self.sc_item_down2
            graf_2_x = self.pl_graf_down2_x
            graf_2_y = self.pl_graf_down2_y
            graf_2_z = self.pl_graf_down2_z
            return graf_1, graf_2_x, graf_2_y, graf_2_z

        if str_now_graf == 'graf_down3':
            graf_1 = self.sc_item_down3
            graf_2_x = self.pl_graf_down3_x
            graf_2_y = self.pl_graf_down3_y
            graf_2_z = self.pl_graf_down3_z
            return graf_1, graf_2_x, graf_2_y, graf_2_z


    def transfer_graf_top1(self):
        global now_graf, str_now_graf

        grafs_1, grafs_2_x, grafs_2_y, grafs_2_z = self.check_now_graf()

        if grafs_1 != 0:
            grafs_1.addItem(grafs_2_x)
            grafs_1.addItem(grafs_2_y)
            grafs_1.addItem(grafs_2_z)


        self.sc_item_large.removeItem(self.sc_item_large)
        self.sc_item_large.clear()

        self.sc_item_large.addItem(self.pl_graf_top1_x)
        self.sc_item_large.addItem(self.pl_graf_top1_y)
        self.sc_item_large.addItem(self.pl_graf_top1_z)

        self.sc_item_top1.clear()

        now_graf = self.graf_top1
        str_now_graf = 'graf_top1'


    def transfer_graf_top2(self):
        global now_graf, str_now_graf

        grafs_1, grafs_2_x, grafs_2_y, grafs_2_z = self.check_now_graf()

        if grafs_1 != 0:
            grafs_1.addItem(grafs_2_x)
            grafs_1.addItem(grafs_2_y)
            grafs_1.addItem(grafs_2_z)

        self.sc_item_large.removeItem(self.sc_item_large)
        self.sc_item_large.clear()

        self.sc_item_large.addItem(self.pl_graf_top2_x)
        self.sc_item_large.addItem(self.pl_graf_top2_y)
        self.sc_item_large.addItem(self.pl_graf_top2_z)

        self.sc_item_top2.clear()
        now_graf = self.graf_top2
        str_now_graf = 'graf_top2'

    def transfer_graf_top3(self):
        global now_graf, str_now_graf

        grafs_1, grafs_2_x, grafs_2_y, grafs_2_z = self.check_now_graf()

        if grafs_1 != 0:
            grafs_1.addItem(grafs_2_x)
            grafs_1.addItem(grafs_2_y)
            grafs_1.addItem(grafs_2_z)

        self.sc_item_large.removeItem(self.sc_item_large)
        self.sc_item_large.clear()

        self.sc_item_large.addItem(self.pl_graf_top3_x)
        self.sc_item_large.addItem(self.pl_graf_top3_y)
        self.sc_item_large.addItem(self.pl_graf_top3_z)

        self.sc_item_top3.clear()
        now_graf = self.graf_top3
        str_now_graf = 'graf_top3'


    def transfer_graf_middle1(self):
        global now_graf, str_now_graf

        grafs_1, grafs_2_x, grafs_2_y, grafs_2_z = self.check_now_graf()

        if grafs_1 != 0:
            grafs_1.addItem(grafs_2_x)
            grafs_1.addItem(grafs_2_y)
            grafs_1.addItem(grafs_2_z)

        self.sc_item_large.removeItem(self.sc_item_large)
        self.sc_item_large.clear()

        self.sc_item_large.addItem(self.pl_graf_middle1_x)
        self.sc_item_large.addItem(self.pl_graf_middle1_y)
        self.sc_item_large.addItem(self.pl_graf_middle1_z)

        self.sc_item_middle1.clear()
        now_graf = self.graf_middle1
        str_now_graf = 'graf_middle1'


    def transfer_graf_middle2(self):
        global now_graf, str_now_graf

        grafs_1, grafs_2_x, grafs_2_y, grafs_2_z = self.check_now_graf()

        if grafs_1 != 0:
            grafs_1.addItem(grafs_2_x)
            grafs_1.addItem(grafs_2_y)
            grafs_1.addItem(grafs_2_z)

        self.sc_item_large.removeItem(self.sc_item_large)
        self.sc_item_large.clear()

        self.sc_item_large.addItem(self.pl_graf_middle2_x)
        self.sc_item_large.addItem(self.pl_graf_middle2_y)
        self.sc_item_large.addItem(self.pl_graf_middle2_z)

        self.sc_item_middle2.clear()
        now_graf = self.graf_middle2
        str_now_graf = 'graf_middle2'


    def transfer_graf_middle3(self):
        global now_graf, str_now_graf

        grafs_1, grafs_2_x, grafs_2_y, grafs_2_z = self.check_now_graf()

        if grafs_1 != 0:
            grafs_1.addItem(grafs_2_x)
            grafs_1.addItem(grafs_2_y)
            grafs_1.addItem(grafs_2_z)

        self.sc_item_large.removeItem(self.sc_item_large)
        self.sc_item_large.clear()

        self.sc_item_large.addItem(self.pl_graf_middle3_x)
        self.sc_item_large.addItem(self.pl_graf_middle3_y)
        self.sc_item_large.addItem(self.pl_graf_middle3_z)

        self.sc_item_middle3.clear()
        now_graf = self.graf_middle3
        str_now_graf = 'graf_middle3'


    def transfer_graf_down1(self):
        global now_graf, str_now_graf

        grafs_1, grafs_2_x, grafs_2_y, grafs_2_z = self.check_now_graf()

        if grafs_1 != 0:
            grafs_1.addItem(grafs_2_x)
            grafs_1.addItem(grafs_2_y)
            grafs_1.addItem(grafs_2_z)

        self.sc_item_large.removeItem(self.sc_item_large)
        self.sc_item_large.clear()

        self.sc_item_large.addItem(self.pl_graf_down1_x)
        self.sc_item_large.addItem(self.pl_graf_down1_y)
        self.sc_item_large.addItem(self.pl_graf_down1_z)

        self.sc_item_down1.clear()
        now_graf = self.graf_down1
        str_now_graf = 'graf_down1'


    def transfer_graf_down2(self):
        global now_graf, str_now_graf

        grafs_1, grafs_2_x, grafs_2_y, grafs_2_z = self.check_now_graf()

        if grafs_1 != 0:
            grafs_1.addItem(grafs_2_x)
            grafs_1.addItem(grafs_2_y)
            grafs_1.addItem(grafs_2_z)

        self.sc_item_large.removeItem(self.sc_item_large)
        self.sc_item_large.clear()

        self.sc_item_large.addItem(self.pl_graf_down2_x)
        self.sc_item_large.addItem(self.pl_graf_down2_y)
        self.sc_item_large.addItem(self.pl_graf_down2_z)

        self.sc_item_down2.clear()
        now_graf = self.graf_down2
        str_now_graf = 'graf_down2'


    def transfer_graf_down3(self):
        global now_graf, str_now_graf

        grafs_1, grafs_2_x, grafs_2_y, grafs_2_z = self.check_now_graf()

        if grafs_1 != 0:
            grafs_1.addItem(grafs_2_x)
            grafs_1.addItem(grafs_2_y)
            grafs_1.addItem(grafs_2_z)

        self.sc_item_large.removeItem(self.sc_item_large)
        self.sc_item_large.clear()

        self.sc_item_large.addItem(self.pl_graf_down3_x)
        self.sc_item_large.addItem(self.pl_graf_down3_y)
        self.sc_item_large.addItem(self.pl_graf_down3_z)

        self.sc_item_down3.clear()
        now_graf = self.graf_down3
        str_now_graf = 'graf_down3'


    @QtCore.pyqtSlot(list)
    def atm_msg(self, msgs):
        for i in range(len(msgs)):

            self.pressure_atmega.append(msgs[i].pressure)
            self.temp_atmega.append(msgs[i].temp)
            self.time_atm.append(msgs[i].time)

            if FILE_WRITE:
                self.buffer_bmp_msg.append(  str(msgs[i].time) +'\t' + '\t' +
                                        str(msgs[i].pressure) + '\t' + '\t' +
                                        str(msgs[i].temp) + '\n')


            self.ui.textBrowser_2.append(
                "bmp {n: %ld, time : %0.3f, pressure : %0.3f, temp : %0.1f}"
                %
                (msgs[i].get_header().seq, msgs[i].time, msgs[i].pressure, msgs[i].temp)
            )


        if FILE_WRITE:
            if len(self.buffer_bmp_msg) >= 10:
                f = open(file_bmp, 'a')
                for foo in range(len(self.buffer_bmp_msg)):
                    f.write(self.buffer_bmp_msg[foo])
                self.buffer_bmp_msg = []
                f.close()


        if len(self.time_atm) > self.lenght:
            self.time_atm = self.time_atm[self.cut:(self.lenght-1)]
            self.pressure_atmega = self.pressure_atmega[self.cut:(self.lenght-1)]
            self.temp_atmega = self.temp_atmega[self.cut:(self.lenght-1)]

        self.pl_graf_down1_x.setData(x=self.time_atm, y=self.temp_atmega, pen=('r'))
        self.pl_graf_down2_x.setData(x=self.time_atm, y=self.pressure_atmega, pen=('r'))
        #
        # self.ui.state_atm_motor.clear()
        # self.ui.state_atm_motor.setText(str(self.state_atm_motor))
        #
        # self.ui.state_para.clear()
        # self.ui.state_para.setText(str(self.state_amt_para))


    @QtCore.pyqtSlot(list)
    def imu_rsc_msg(self, msgs):
        i = 0
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


            self.ui.textBrowser_2.append(
                "IMU_RSC\t {n: %ld, time: %0.3f, A: [%0.4f, %0.4f, %0.4f] G: [%0.4f, %0.4f, %0.4f] M: [%0.3f, %0.3f, %0.3f]}"
                    %
                    (msgs[i].get_header().seq, msgs[i].time, msgs[i].accel[0], msgs[i].accel[1], msgs[i].accel[2], msgs[i].gyro[0], msgs[i].gyro[1], msgs[i].gyro[2], msgs[i].compass[0], msgs[i].compass[1], msgs[i].compass[2])
                )

        if FILE_WRITE:
            if len(self.buffer_imu_rsc_msg) >= 10:
                f = open(file_imu_rsc, 'a')
                for foo in range(len(self.buffer_imu_rsc_msg)):
                    f.write(self.buffer_imu_rsc_msg[foo])
                self.buffer_imu_rsc_msg = []
                f.close()


        if len(self.time_RSC) > self.lenght:
            self.a_RSC_x = self.a_RSC_x[self.cut:(self.lenght - 1)]
            self.a_RSC_y = self.a_RSC_y[self.cut:(self.lenght - 1)]
            self.a_RSC_z = self.a_RSC_z[self.cut:(self.lenght - 1)]
            self.time_RSC = self.time_RSC[self.cut:(self.lenght - 1)]
            self.av_x = self.av_x[self.cut:(self.lenght - 1)]
            self.av_y = self.av_y[self.cut:(self.lenght - 1)]
            self.av_z = self.av_z[self.cut:(self.lenght - 1)]

        self.pl_graf_top2_x.setData(x=self.time_RSC, y=self.a_RSC_x, pen=('r'), width=0.5)
        self.pl_graf_top2_y.setData(x=self.time_RSC, y=self.a_RSC_y, pen=('g'), width=0.5)
        self.pl_graf_top2_z.setData(x=self.time_RSC, y=self.a_RSC_z, pen=('b'), width=0.5)

        self.pl_graf_middle1_x.setData(x=self.time_RSC, y=self.av_x, pen=('r'), width=0.5)
        self.pl_graf_middle1_y.setData(x=self.time_RSC, y=self.av_y, pen=('g'), width=0.5)
        self.pl_graf_middle1_z.setData(x=self.time_RSC, y=self.av_z, pen=('b'), width=0.5)




        # Вывод в лог #
        # log = '' + 'sfkslk'+ "\n"
        # self.ui.textBrowser_2.append(log)
        #  #

    @QtCore.pyqtSlot(list)
    def imu_isc_msg(self, msgs):
        i = 0
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


            # tr = Transform3D()
            quat = pyquaternion.Quaternion(msgs[i].quaternion)
            # quat = QQuaternion(msgs[i].quaternion[0], msgs[i].quaternion[1], msgs[i].quaternion[2], msgs[i].quaternion[3])

            # tr.rotate(quat)
            self.plane_widget._update_rotation(quat)

            self.ui.textBrowser_2.append(
                "IMU_ISC\t {n: %ld, time: %0.3f, A: [%0.4f, %0.4f, %0.4f] M: [%0.3f, %0.3f, %0.3f]}"
                %
                (msgs[i].get_header().seq, msgs[i].time, msgs[i].accel[0], msgs[i].accel[1], msgs[i].accel[2], msgs[i].compass[0], msgs[i].compass[1], msgs[i].compass[2])
            )
            self.ui.textBrowser_2.append(
                "QUAT\t {n: %ld, time: %0.3f, quat: [%0.4f, %0.4f, %0.4f, %0.4f]}"
                %
                (msgs[i].get_header().seq, msgs[i].time, msgs[i].quaternion[0], msgs[i].quaternion[1], msgs[i].quaternion[2], msgs[i].quaternion[3])
            )
            # self.ui.textBrowser_2.append(
            #     "POS\t {n: %ld, time: %0.3f, velo: [%0.3f, %0.3f, %0.3f], pos: [%0.3f, %0.3f, %0.3f]}"
            #     %
            #     (msgs[i].get_header().seq, msgs[i].time, *msgs[i].velocities, *msgs[i].coordinates)
            # )

        if FILE_WRITE:
            if len(self.buffer_imu_isc_msg) >= 10:
                f = open(file_imu_isc, 'a')
                for foo in range(len(self.buffer_imu_isc_msg)):
                    f.write(self.buffer_imu_isc_msg[foo])
                self.buffer_imu_isc_msg = []
                f.close()


        if len(self.time_ISC) > self.lenght:
            self.a_ISC_x = self.a_ISC_x[self.cut:(self.lenght - 1)]
            self.a_ISC_y = self.a_ISC_y[self.cut:(self.lenght - 1)]
            self.a_ISC_z = self.a_ISC_z[self.cut:(self.lenght - 1)]
            self.time_ISC = self.time_ISC[self.cut:(self.lenght - 1)]
            self.vmf_x = self.vmf_x[self.cut:(self.lenght - 1)]
            self.vmf_y = self.vmf_y[self.cut:(self.lenght - 1)]
            self.vmf_z = self.vmf_z[self.cut:(self.lenght - 1)]
            # self.v_x = self.v_x[self.cut:(self.lenght - 1)]
            # self.v_y = self.v_y[self.cut:(self.lenght - 1)]
            # self.v_z = self.v_z[self.cut:(self.lenght - 1)]
            # self.mov_x = self.mov_x[self.cut:(self.lenght - 1)]
            # self.mov_y = self.mov_y[self.cut:(self.lenght - 1)]
            # self.mov_z = self.mov_z[self.cut:(self.lenght - 1)]

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

        self.pl_graf_middle3_x.setData(x=self.time_ISC, y=self.vmf_x, pen=('r'))
        self.pl_graf_middle3_y.setData(x=self.time_ISC, y=self.vmf_y, pen=('g'))
        self.pl_graf_middle3_z.setData(x=self.time_ISC, y=self.vmf_z, pen=('b'))

        # self.pl_graf_top3_x.setData(x=self.time_ISC, y=self.v_x, pen=('r'))
        # self.pl_graf_top3_y.setData(x=self.time_ISC, y=self.v_y, pen=('g'))
        # self.pl_graf_top3_z.setData(x=self.time_ISC, y=self.v_z, pen=('b'))

        # self.pl_graf_middle2_x.setData(x=self.time_ISC, y=self.mov_x, pen=('r'))
        # self.pl_graf_middle2_y.setData(x=self.time_ISC, y=self.mov_y, pen=('g'))
        # self.pl_graf_middle2_z.setData(x=self.time_ISC, y=self.mov_z, pen=('b'))


    @QtCore.pyqtSlot(list)
    def sens_msg(self, msgs):
        i = 0
        for i in range(len(msgs)):
            self.time_sens.append(msgs[i].time)
            self.pressure_sensors.append(msgs[i].pressure)
            self.temp_sensors.append(msgs[i].temp)

            time = msgs[i].time

            if FILE_WRITE:
                self.buffer_sensors_msg.append( str(time) + '\t' + '\t' +
                                            str(msgs[i].pressure) + '\t' + '\t' +
                                            str(msgs[i].temp) + '\t' + '\t' +
                                            str(msgs[i].height) + '\n')


            self.ui.textBrowser_2.append(
                "imu_bmp  {n: %ld, time: %0.3f, temp: %0.3f, pressure: %0.3f}"
                %
                (msgs[i].get_header().seq, msgs[i].time, msgs[i].temp, msgs[i].pressure)
            )

        if FILE_WRITE:
            if len(self.buffer_sensors_msg) >= 10:
                f = open(file_sensors, 'a')
                for foo in range(len(self.buffer_sensors_msg)):
                    f.write(self.buffer_sensors_msg[foo])
                self.buffer_sensors_msg = []
                f.close()


        if len(self.time_sens) > self.lenght:
            self.time_sens = self.time_sens[self.cut:(self.lenght - 1)]
            self.pressure_sensors = self.pressure_sensors[self.cut:(self.lenght - 1)]
            self.temp_sensors = self.temp_sensors[self.cut:(self.lenght - 1)]

        self.pl_graf_down1_y.setData(x=self.time_sens, y=self.temp_sensors, pen=('b'))
        self.pl_graf_down2_y.setData(x=self.time_sens, y=self.pressure_sensors, pen=('b'))



    @QtCore.pyqtSlot(list)
    def gps_msg(self, msgs):
        i = 0
        speed = 0
        for i in range(len(msgs)):
            self.x.append(msgs[i].coordinates[0])
            self.y.append(msgs[i].coordinates[1])
            speed = msgs[i].speed

            self.ui.textBrowser_2.append(
                "GPS {n: %ld, time: %0.3f, coordinates: [%0.5f, %0.5f], speed: %0.3f}"
                %
                (msgs[i].get_header().seq, msgs[i].time, msgs[i].coordinates[1], msgs[i].coordinates[0], speed)
            )

            y0 = []
            x0 = []
            x0.append(self.x[0])
            y0.append(self.y[0])

        self.pl_graf_down3_y.setData(x=x0, y=y0, pen=('b'), width=10)

        if len(self.x) > self.lenght:
            self.x = self.x[self.cut:(self.lenght - 1)]
            self.y = self.y[self.cut:(self.lenght - 1)]
            self.z = self.z[self.cut:(self.lenght - 1)]


        self.pl_graf_down3_x.setData(x=self.x, y=self.y, pen=('r'))


    @QtCore.pyqtSlot(list)
    def state_msg(self, msgs):
        i = 0
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


            self.ui.mpu_state.clear()
            self.ui.bmp_state.clear()
            self.ui.sd_state.clear()
            self.ui.nrf_state.clear()
            # self.ui.morot_stm_state.clear()
            self.ui.gps_state.clear()


            self.ui.init.clear()
            self.ui.init_param.clear()
            self.ui.in_rocket.clear()
            self.ui.lowering.clear()
            self.ui.falling.clear()
            self.ui.end.clear()

            self.ui.mpu_state.setText(str(self.state_mpu))
            self.ui.bmp_state.setText(str(self.state_bmp))
            self.ui.sd_state.setText(str(self.state_sd))
            self.ui.nrf_state.setText(str(self.state_nrf))
            self.ui.gps_state.setText(str(self.state_gps))

        if self.state_fly == 0:
            self.ui.init.setText(str('1'))

        if self.state_fly == 1:
            self.ui.init_param.setText(str('1'))

        if self.state_fly == 2:
            self.ui.in_rocket.setText(str('1'))

        if self.state_fly == 3:
            self.ui.falling.setText(str('1'))

        if self.state_fly == 4:
            self.ui.lowering.setText(str('1'))

        if self.state_fly == 5:
            self.ui.end.setText(str('1'))


    @QtCore.pyqtSlot(list)
    def servo_msg(self, msgs):
        for i in range(len(msgs)):
            if FILE_WRITE:
                self.buffer_servo_msg.append(str(msgs[i].time) + '\t' +
                                            str(msgs[i].angle_left) + '\t' + '\t' +
                                            str(msgs[i].angle_right) + '\t' + '\t' +
                                            str(msgs[i].angle_keel) + '\n')


            self.ui.textBrowser_2.append(
                "SERVO  {n: %ld, angle left: %0.3f, angle_right: %0.3f, angle_keel: %0.3f}"
                %
                (msgs[i].get_header().seq, msgs[i].angle_left, msgs[i].angle_right, msgs[i].angle_keel)
            )

        if FILE_WRITE:
            if len(self.buffer_servo_msg) >= 10:
                f = open(file_servo, 'a')
                for foo in range(len(self.buffer_servo_msg)):
                    f.write(self.buffer_servo_msg[foo])
                self.buffer_servo_msg = []
                f.close()