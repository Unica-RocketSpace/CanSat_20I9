# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'gcs_wid_telem_1.ui'
#
# Created by: PyQt5 UI code generator 5.10.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_DockWidget_telem1(object):
    def setupUi(self, DockWidget_telem1):
        DockWidget_telem1.setObjectName("DockWidget_telem1")
        DockWidget_telem1.setWindowModality(QtCore.Qt.NonModal)
        DockWidget_telem1.resize(238, 160)
        DockWidget_telem1.setMaximumSize(QtCore.QSize(238, 160))
        DockWidget_telem1.setWindowTitle("")
        self.dockWidgetContents = QtWidgets.QWidget()
        self.dockWidgetContents.setObjectName("dockWidgetContents")
        self.label_1 = QtWidgets.QLabel(self.dockWidgetContents)
        self.label_1.setGeometry(QtCore.QRect(10, 10, 91, 20))
        self.label_1.setObjectName("label_1")
        self.label_1_value = QtWidgets.QLabel(self.dockWidgetContents)
        self.label_1_value.setGeometry(QtCore.QRect(120, 10, 101, 20))
        self.label_1_value.setText("")
        self.label_1_value.setObjectName("label_1_value")
        self.line = QtWidgets.QFrame(self.dockWidgetContents)
        self.line.setGeometry(QtCore.QRect(0, 30, 231, 16))
        self.line.setFrameShape(QtWidgets.QFrame.HLine)
        self.line.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line.setObjectName("line")
        self.label_2 = QtWidgets.QLabel(self.dockWidgetContents)
        self.label_2.setGeometry(QtCore.QRect(10, 40, 91, 20))
        self.label_2.setObjectName("label_2")
        self.label_2_value = QtWidgets.QLabel(self.dockWidgetContents)
        self.label_2_value.setGeometry(QtCore.QRect(120, 40, 101, 20))
        self.label_2_value.setText("")
        self.label_2_value.setObjectName("label_2_value")
        self.line_2 = QtWidgets.QFrame(self.dockWidgetContents)
        self.line_2.setGeometry(QtCore.QRect(100, 10, 20, 121))
        self.line_2.setFrameShape(QtWidgets.QFrame.VLine)
        self.line_2.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_2.setObjectName("line_2")
        self.line_3 = QtWidgets.QFrame(self.dockWidgetContents)
        self.line_3.setGeometry(QtCore.QRect(0, 0, 231, 16))
        self.line_3.setFrameShape(QtWidgets.QFrame.HLine)
        self.line_3.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_3.setObjectName("line_3")
        self.line_4 = QtWidgets.QFrame(self.dockWidgetContents)
        self.line_4.setGeometry(QtCore.QRect(0, 60, 231, 16))
        self.line_4.setFrameShape(QtWidgets.QFrame.HLine)
        self.line_4.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_4.setObjectName("line_4")
        self.line_5 = QtWidgets.QFrame(self.dockWidgetContents)
        self.line_5.setGeometry(QtCore.QRect(0, 90, 231, 16))
        self.line_5.setFrameShape(QtWidgets.QFrame.HLine)
        self.line_5.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_5.setObjectName("line_5")
        self.label_3 = QtWidgets.QLabel(self.dockWidgetContents)
        self.label_3.setGeometry(QtCore.QRect(10, 70, 91, 20))
        self.label_3.setObjectName("label_3")
        self.line_6 = QtWidgets.QFrame(self.dockWidgetContents)
        self.line_6.setGeometry(QtCore.QRect(220, 10, 20, 121))
        self.line_6.setFrameShape(QtWidgets.QFrame.VLine)
        self.line_6.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_6.setObjectName("line_6")
        self.label_3_value = QtWidgets.QLabel(self.dockWidgetContents)
        self.label_3_value.setGeometry(QtCore.QRect(120, 70, 101, 20))
        self.label_3_value.setText("")
        self.label_3_value.setObjectName("label_3_value")
        self.label_4 = QtWidgets.QLabel(self.dockWidgetContents)
        self.label_4.setGeometry(QtCore.QRect(10, 100, 91, 20))
        self.label_4.setObjectName("label_4")
        self.label_4_value = QtWidgets.QLabel(self.dockWidgetContents)
        self.label_4_value.setGeometry(QtCore.QRect(120, 100, 91, 20))
        self.label_4_value.setText("")
        self.label_4_value.setObjectName("label_4_value")
        self.line_7 = QtWidgets.QFrame(self.dockWidgetContents)
        self.line_7.setGeometry(QtCore.QRect(0, 120, 231, 16))
        self.line_7.setFrameShape(QtWidgets.QFrame.HLine)
        self.line_7.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_7.setObjectName("line_7")
        DockWidget_telem1.setWidget(self.dockWidgetContents)

        self.retranslateUi(DockWidget_telem1)
        QtCore.QMetaObject.connectSlotsByName(DockWidget_telem1)

    def retranslateUi(self, DockWidget_telem1):
        _translate = QtCore.QCoreApplication.translate
        self.label_1.setText(_translate("DockWidget_telem1", "x"))
        self.label_2.setText(_translate("DockWidget_telem1", "y"))
        self.label_3.setText(_translate("DockWidget_telem1", "z"))
        self.label_4.setText(_translate("DockWidget_telem1", "result"))
