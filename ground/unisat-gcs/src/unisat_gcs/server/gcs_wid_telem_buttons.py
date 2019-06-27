# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'gcs_wid_telem_buttons.ui'
#
# Created by: PyQt5 UI code generator 5.10.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_DockWidget_telem_buttons(object):
    def setupUi(self, DockWidget_telem_buttons):
        DockWidget_telem_buttons.setObjectName("DockWidget_telem_buttons")
        DockWidget_telem_buttons.resize(238, 172)
        DockWidget_telem_buttons.setMaximumSize(QtCore.QSize(238, 172))
        DockWidget_telem_buttons.setWindowTitle("")
        self.dockWidgetContents = QtWidgets.QWidget()
        self.dockWidgetContents.setObjectName("dockWidgetContents")
        self.toolButton_wing_left = QtWidgets.QToolButton(self.dockWidgetContents)
        self.toolButton_wing_left.setEnabled(False)
        self.toolButton_wing_left.setGeometry(QtCore.QRect(44, 46, 21, 21))
        self.toolButton_wing_left.setText("")
        self.toolButton_wing_left.setCheckable(False)
        self.toolButton_wing_left.setObjectName("toolButton_wing_left")
        self.toolButton_wing_right = QtWidgets.QToolButton(self.dockWidgetContents)
        self.toolButton_wing_right.setEnabled(False)
        self.toolButton_wing_right.setGeometry(QtCore.QRect(170, 46, 21, 21))
        self.toolButton_wing_right.setText("")
        self.toolButton_wing_right.setCheckable(False)
        self.toolButton_wing_right.setObjectName("toolButton_wing_right")
        self.toolButton_go_keel = QtWidgets.QToolButton(self.dockWidgetContents)
        self.toolButton_go_keel.setEnabled(False)
        self.toolButton_go_keel.setGeometry(QtCore.QRect(107, 110, 16, 19))
        self.toolButton_go_keel.setText("")
        self.toolButton_go_keel.setCheckable(False)
        self.toolButton_go_keel.setObjectName("toolButton_go_keel")
        self.toolButton_go_left = QtWidgets.QToolButton(self.dockWidgetContents)
        self.toolButton_go_left.setEnabled(False)
        self.toolButton_go_left.setGeometry(QtCore.QRect(74, 128, 16, 16))
        self.toolButton_go_left.setText("")
        self.toolButton_go_left.setCheckable(False)
        self.toolButton_go_left.setObjectName("toolButton_go_left")
        self.toolButton_go_right = QtWidgets.QToolButton(self.dockWidgetContents)
        self.toolButton_go_right.setEnabled(False)
        self.toolButton_go_right.setGeometry(QtCore.QRect(140, 128, 16, 16))
        self.toolButton_go_right.setText("")
        self.toolButton_go_right.setCheckable(False)
        self.toolButton_go_right.setObjectName("toolButton_go_right")
        self.label = QtWidgets.QLabel(self.dockWidgetContents)
        self.label.setGeometry(QtCore.QRect(-4, -8, 241, 171))
        self.label.setMaximumSize(QtCore.QSize(241, 181))
        self.label.setText("")
        self.label.setObjectName("label")
        self.label.raise_()
        self.toolButton_wing_left.raise_()
        self.toolButton_wing_right.raise_()
        self.toolButton_go_keel.raise_()
        self.toolButton_go_left.raise_()
        self.toolButton_go_right.raise_()
        DockWidget_telem_buttons.setWidget(self.dockWidgetContents)

        self.retranslateUi(DockWidget_telem_buttons)
        QtCore.QMetaObject.connectSlotsByName(DockWidget_telem_buttons)

    def retranslateUi(self, DockWidget_telem_buttons):
        pass

