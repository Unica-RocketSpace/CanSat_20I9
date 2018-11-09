import sys
import time
import os
from math import *

import numpy as np
import pyqtgraph as pg
from .gcs_ui import *

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import *

from .mavlink import read_file

import pyqtgraph.opengl as gl

from PyQt5.QtCore import QThread
import logging
import math

from . import _log as _root_log


_log = _root_log.getChild("main")

# index = 0
# print(way)


point = "."
i = read_file.index(point)
file_name = read_file[0:i]

expansion = ".txt"


file_bmp = file_name + "_bmp" + expansion
file_imu_isc = file_name + "_imu_isc" + expansion
file_imu_rsc = file_name + "_imu_rsc" + expansion
file_sensors = file_name + "_sensors" + expansion


f = open(file_bmp, 'w')
f.write("Time" + '\t' + "Pressure" + '\t' + "Temp" + '\n')
f.close()

f = open(file_sensors, 'w')
f.write("Time" + '\t' + "pressure" + '\t' + "Temp" + '\t' + "Height" + '\n')
f.close()

f = open(file_imu_isc, 'w')
f.write("Time" + '\t' + "Accel" + '\t' + '\t' + '\t' + "Compass" + '\t' + '\t' + '\t' + "Quaternion" + '\n')
f.close()

f = open(file_imu_rsc, 'w')
f.write("Time" + '\t' + "Accel" + '\t' + '\t' + '\t' + "Compass" + '\t' + '\t' + '\t' + "Gyro" + '\n')
f.close()


log_text = ''
global_vars={'x': 0, 'y': 0}
now_graf = None
str_now_graf = None

# Главный класс
class MyWin(QtWidgets.QMainWindow):
    new_send_command = pyqtSignal(int)

    def __init__(self, parent=None):
        QtWidgets.QWidget.__init__(self, parent)
        self.ui = Ui_MainWindow()
        # self.ui = Ground.Ui_MainWindow()
        self.ui.setupUi(self)
        self.setWindowTitle('Unica gcs')


        pg.setConfigOption('background', 'w')
        pg.setConfigOption('foreground', 'k')

        self.accel_f = open("C:\Cansat Unica 2018\calibration\\accel_calibration", "wt")
        self.compass_f = open("C:\Cansat Unica 2018\calibration\magn_calibration", "wt")

        self.lenght = 150
        self.cut = 11

        self.temp_atmega = []
        self.pressure_atmega = []
        self.time_atm = []

        self.state_atm_motor = None
        self.state_amt_para = None
        self.state_mpu = None
        self.state_bmp = None
        self.state_sd = None
        self.state_nrf = None
        self.state_stm_motor = None
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

        self.ui.glv_top = pg.GraphicsLayoutWidget(self.ui.centralwidget)
        self.ui.top.addWidget(self.ui.glv_top)
        self.ui.plot_top = pg.GraphicsLayout()
        self.ui.glv_top.addItem(self.ui.plot_top)
        # Создаем title для графика
        # title="title"
        # График для нескольких прямых
        # self.sc_item = self.ui.plot.addPlot(title="title")
        #######

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
        # self.graf_large = pg.PlotCurveItem()
        # self.sc_item_large.addItem(self.graf_large)

        self.ui.dockwid = QtWidgets.QDockWidget()
        self.ui.grid_3D.addWidget(self.ui.dockwid)

        self.ui.glwid = gl.GLViewWidget()
        self.ui.dockwid.setWidget(self.ui.glwid)
        self.gx = gl.GLGridItem()
        self.gx.rotate(90, 0, 1, 0)
        self.gx.translate(0, 10, 10)
        self.ui.glwid.addItem(self.gx)
        self.gy = gl.GLGridItem()
        self.gy.rotate(90, 1, 0, 0)
        self.gy.translate(10, 0, 10)
        self.ui.glwid.addItem(self.gy)
        self.gz = gl.GLGridItem()
        self.gz.translate(10, 10, 0)
        self.ui.glwid.addItem(self.gz)
        self.plt = gl.GLLinePlotItem()
        self.ui.glwid.addItem(self.plt)
        self.isc_coord = gl.GLAxisItem()
        self.isc_coord.setSize(25, 25, 25)
        self.rsc_coord = gl.GLAxisItem()

        self.ui.glwid.addItem(self.isc_coord)
        self.ui.glwid.addItem(self.rsc_coord)
        self.ui.glwid.show()

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
        i = 0


        for i in range(len(msgs)):
            self.time_atm.append(msgs[i].time)
            self.pressure_atmega.append(msgs[i].pressure)
            self.temp_atmega.append(msgs[i].temp)

            self.buffer_bmp_msg.append(  str(msgs[i].time) +'\t' + '\t' +
                                    str(msgs[i].pressure) + '\t' + '\t' +
                                    str(msgs[i].temp) + '\n')


            self.ui.textBrowser_2.append(
                "ATmega {n: %ld, time : %0.3f, pressure : %0.3f, temp : %0.1f}"
                %
                (msgs[i].get_header().seq, msgs[i].time, msgs[i].pressure, msgs[i].temp)
            )


        if len(self.buffer_bmp_msg) >= 9:
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

            self.accel_f.write("%f\t%f\t%f\n" % (msgs[i].accel[0], msgs[i].accel[1], msgs[i].accel[2]))
            self.compass_f.write("%f\t%f\t%f\n" % (msgs[i].compass[0], msgs[i].compass[1], msgs[i].compass[2]))

            self.time_RSC.append(msgs[i].time)

            self.av_x.append(msgs[i].gyro[0])
            self.av_y.append(msgs[i].gyro[1])
            self.av_z.append(msgs[i].gyro[2])

            self.buffer_imu_rsc_msg.append(  str(msgs[i].time) + '\t' + '\t' +
                                        str(msgs[i].accel[0]) + ' ' + str(msgs[i].accel[1]) + str(msgs[i].accel[2]) + '\t' + '\t' +
                                        str(msgs[i].compass[0]) + ' ' + str(msgs[i].compass[1]) + ' ' + str(msgs[i].compass[2]) + '\t' + '\t' +
                                        str(msgs[i].gyro[0]) + ' ' + str(msgs[i].gyro[1]) + ' ' + str(msgs[i].gyro[2]) + '\n')


            self.ui.textBrowser_2.append(
                "IMU_RSC\t {n: %ld, time: %0.3f, A: [%0.4f, %0.4f, %0.4f] G: [%0.4f, %0.4f, %0.4f] M: [%0.3f, %0.3f, %0.3f]}"
                    %
                    (msgs[i].get_header().seq, msgs[i].time, *msgs[i].accel, *msgs[i].gyro, *msgs[i].compass)
                )

        if len(self.buffer_imu_rsc_msg) >= 9:
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

            self.buffer_imu_isc_msg.append(  str(msgs[i].time) + '\t' + '\t' +
                                        str(msgs[i].accel[0]) + ' ' + str(msgs[i].accel[1]) + str(msgs[i].accel[2]) + '\t' + '\t' +
                                        str(msgs[i].compass[0]) + ' ' + str(msgs[i].compass[1]) + ' ' + str(msgs[i].compass[2]) + '\t' + '\t' +
                                        str(msgs[i].quaternion[0]) + ' ' + str(msgs[i].quaternion[1]) + ' ' + str(msgs[i].quaternion[2]) + '\n')

            q_0 = msgs[i].quaternion[0]
            teta = 2 * acos(q_0)
            sin_teta = sqrt(1 - q_0*q_0)
            self.rsc_coord.rotate(180*teta/math.pi, - msgs[i].quaternion[1] / sin_teta, - msgs[i].quaternion[2] / sin_teta, - msgs[i].quaternion[3] / sin_teta)

            self.ui.textBrowser_2.append(
                "IMU_ISC\t {n: %ld, time: %0.3f, A: [%0.4f, %0.4f, %0.4f] M: [%0.3f, %0.3f, %0.3f]}"
                %
                (msgs[i].get_header().seq, msgs[i].time, *msgs[i].accel, *msgs[i].compass)
            )
            self.ui.textBrowser_2.append(
                "QUAT\t {n: %ld, time: %0.3f, quat: [%0.4f, %0.4f, %0.4f, %0.4f]}"
                %
                (msgs[i].get_header().seq, msgs[i].time, *msgs[i].quaternion)
            )
            # self.ui.textBrowser_2.append(
            #     "POS\t {n: %ld, time: %0.3f, velo: [%0.3f, %0.3f, %0.3f], pos: [%0.3f, %0.3f, %0.3f]}"
            #     %
            #     (msgs[i].get_header().seq, msgs[i].time, *msgs[i].velocities, *msgs[i].coordinates)
            # )

        if len(self.buffer_imu_isc_msg) >= 9:
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
            self.v_x = self.v_x[self.cut:(self.lenght - 1)]
            self.v_y = self.v_y[self.cut:(self.lenght - 1)]
            self.v_z = self.v_z[self.cut:(self.lenght - 1)]
            self.mov_x = self.mov_x[self.cut:(self.lenght - 1)]
            self.mov_y = self.mov_y[self.cut:(self.lenght - 1)]
            self.mov_z = self.mov_z[self.cut:(self.lenght - 1)]

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

            self.buffer_sensors_msg.append( str(time) + '\t' + '\t' +
                                            str(msgs[i].pressure) + '\t' + '\t' +
                                            str(msgs[i].temp) + '\t' + '\t' +
                                            str(msgs[i].height) + '\n')


            self.ui.textBrowser_2.append(
                "SENSORS  {n: %ld, time: %0.3f, temp: %0.3f, pressure: %0.3f}"
                %
                (msgs[i].get_header().seq, msgs[i].time, msgs[i].temp, msgs[i].pressure)
            )

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
        # i = 0
        # for i in range(len(msgs)):
        #     self.x.append(msgs[i].coordinates[0])
        #     self.y.append(msgs[i].coordinates[1])
        #     self.z.append(msgs[i].coordinates[2])
        #
        #     self.ui.textBrowser_2.append(
        #         "GPS\t {n: %ld, time: %0.3f, coordinates: [%0.5f, %0.5f, %0.5f]}"
        #         %
        #         (msgs[i].get_header().seq, msgs[i].time, msgs[i].coordinates[1], msgs[i].coordinates[0], msgs[i].coordinates[2])
        #     )
        #
        #     y0 = []
        #     x0 = []
        #     x0.append(self.x[0])
        #     y0.append(self.y[0])
        #
        # self.pl_graf_down3_y.setData(x=x0, y=y0, pen=('b'), width=10)
        #
        # # if len(self.x) > self.lenght:
        # #     self.x = self.x[self.cut:(self.lenght - 1)]
        # #     self.y = self.y[self.cut:(self.lenght - 1)]
        # #     self.z = self.z[self.cut:(self.lenght - 1)]
        # #

        # self.pl_graf_down3_x.setData(x=self.x, y=self.y, pen=('r'))
        pass

    @QtCore.pyqtSlot(list)
    def state_msg(self, msgs):
        i = 0
        for i in range(len(msgs)):
            self.state_fly = msgs[i].globalStage
            self.state_bmp = msgs[i].BMP_state
            self.state_gps = msgs[i].GPS_state
            self.state_mpu = msgs[i].MPU_state
            self.state_nrf = msgs[i].NRF_state
            self.state_sd = msgs[i].SD_state
            self.state_stm_motor = msgs[i].MOTOR_state


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

            self.ui.mpu_state.setText(str(self.state_mpu))
            self.ui.bmp_state.setText(str(self.state_bmp))
            self.ui.sd_state.setText(str(self.state_sd))
            self.ui.nrf_state.setText(str(self.state_nrf))
            self.ui.morot_stm_state.setText(str(self.state_stm_motor))
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
