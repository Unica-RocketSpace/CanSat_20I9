# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'gcs_ui.ui'
#
# Created by: PyQt5 UI code generator 5.10.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.setEnabled(True)
        MainWindow.resize(1298, 572)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout(self.centralwidget)
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.verticalLayout_3 = QtWidgets.QVBoxLayout()
        self.verticalLayout_3.setSizeConstraint(QtWidgets.QLayout.SetFixedSize)
        self.verticalLayout_3.setSpacing(6)
        self.verticalLayout_3.setObjectName("verticalLayout_3")
        self.horizontalLayout_3 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_3.setSizeConstraint(QtWidgets.QLayout.SetFixedSize)
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.textBrowser = QtWidgets.QTextBrowser(self.centralwidget)
        self.textBrowser.setEnabled(True)
        self.textBrowser.setMaximumSize(QtCore.QSize(200, 30))
        self.textBrowser.setReadOnly(False)
        self.textBrowser.setObjectName("textBrowser")
        self.horizontalLayout_3.addWidget(self.textBrowser)
        self.commandLinkButton = QtWidgets.QCommandLinkButton(self.centralwidget)
        self.commandLinkButton.setMaximumSize(QtCore.QSize(200, 60))
        self.commandLinkButton.setObjectName("commandLinkButton")
        self.horizontalLayout_3.addWidget(self.commandLinkButton)
        self.verticalLayout_3.addLayout(self.horizontalLayout_3)
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setSizeConstraint(QtWidgets.QLayout.SetMinimumSize)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.verticalLayout_2 = QtWidgets.QVBoxLayout()
        self.verticalLayout_2.setSizeConstraint(QtWidgets.QLayout.SetFixedSize)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.horizontalLayout.addLayout(self.verticalLayout_2)
        self.verticalLayout_5 = QtWidgets.QVBoxLayout()
        self.verticalLayout_5.setObjectName("verticalLayout_5")
        self.horizontalLayout.addLayout(self.verticalLayout_5)
        self.verticalLayout_3.addLayout(self.horizontalLayout)
        self.gridLayou_telem = QtWidgets.QGridLayout()
        self.gridLayou_telem.setSizeConstraint(QtWidgets.QLayout.SetFixedSize)
        self.gridLayou_telem.setObjectName("gridLayou_telem")
        self.horizontalLayout_telem_3 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_telem_3.setObjectName("horizontalLayout_telem_3")
        self.verticalLayout_left_3 = QtWidgets.QVBoxLayout()
        self.verticalLayout_left_3.setObjectName("verticalLayout_left_3")
        self.horizontalLayout_telem_3.addLayout(self.verticalLayout_left_3)
        self.verticalLayout_right_3 = QtWidgets.QVBoxLayout()
        self.verticalLayout_right_3.setObjectName("verticalLayout_right_3")
        self.horizontalLayout_telem_3.addLayout(self.verticalLayout_right_3)
        self.gridLayou_telem.addLayout(self.horizontalLayout_telem_3, 2, 0, 1, 1)
        self.horizontalLayout_telem_2 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_telem_2.setObjectName("horizontalLayout_telem_2")
        self.verticalLayout_left_2 = QtWidgets.QVBoxLayout()
        self.verticalLayout_left_2.setObjectName("verticalLayout_left_2")
        self.horizontalLayout_telem_2.addLayout(self.verticalLayout_left_2)
        self.verticalLayout_right_2 = QtWidgets.QVBoxLayout()
        self.verticalLayout_right_2.setObjectName("verticalLayout_right_2")
        self.horizontalLayout_telem_2.addLayout(self.verticalLayout_right_2)
        self.gridLayou_telem.addLayout(self.horizontalLayout_telem_2, 1, 0, 1, 1)
        self.horizontalLayout_telem_1 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_telem_1.setObjectName("horizontalLayout_telem_1")
        self.verticalLayout_left_1 = QtWidgets.QVBoxLayout()
        self.verticalLayout_left_1.setObjectName("verticalLayout_left_1")
        self.horizontalLayout_telem_1.addLayout(self.verticalLayout_left_1)
        self.verticalLayout_right_1 = QtWidgets.QVBoxLayout()
        self.verticalLayout_right_1.setObjectName("verticalLayout_right_1")
        self.horizontalLayout_telem_1.addLayout(self.verticalLayout_right_1)
        self.gridLayou_telem.addLayout(self.horizontalLayout_telem_1, 0, 0, 1, 1)
        self.verticalLayout_3.addLayout(self.gridLayou_telem)
        self.horizontalLayout_2.addLayout(self.verticalLayout_3)
        self.verticalLayout_4 = QtWidgets.QVBoxLayout()
        self.verticalLayout_4.setSizeConstraint(QtWidgets.QLayout.SetDefaultConstraint)
        self.verticalLayout_4.setObjectName("verticalLayout_4")
        self.top = QtWidgets.QGridLayout()
        self.top.setSizeConstraint(QtWidgets.QLayout.SetMinimumSize)
        self.top.setObjectName("top")
        self.verticalLayout_4.addLayout(self.top)
        self.middle = QtWidgets.QGridLayout()
        self.middle.setObjectName("middle")
        self.verticalLayout_4.addLayout(self.middle)
        self.down = QtWidgets.QGridLayout()
        self.down.setObjectName("down")
        self.verticalLayout_4.addLayout(self.down)
        self.horizontalLayout_2.addLayout(self.verticalLayout_4)
        self.verticalLayout = QtWidgets.QVBoxLayout()
        self.verticalLayout.setSizeConstraint(QtWidgets.QLayout.SetFixedSize)
        self.verticalLayout.setObjectName("verticalLayout")
        self.grid_right_2_graf = QtWidgets.QGridLayout()
        self.grid_right_2_graf.setSizeConstraint(QtWidgets.QLayout.SetMinimumSize)
        self.grid_right_2_graf.setObjectName("grid_right_2_graf")
        self.verticalLayout.addLayout(self.grid_right_2_graf)
        self.grid_3D = QtWidgets.QGridLayout()
        self.grid_3D.setObjectName("grid_3D")
        self.verticalLayout.addLayout(self.grid_3D)
        self.horizontalLayout_2.addLayout(self.verticalLayout)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1298, 21))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.action = QtWidgets.QAction(MainWindow)
        self.action.setObjectName("action")
        self.action_2 = QtWidgets.QAction(MainWindow)
        self.action_2.setObjectName("action_2")
        self.action_3 = QtWidgets.QAction(MainWindow)
        self.action_3.setObjectName("action_3")
        self.action_4 = QtWidgets.QAction(MainWindow)
        self.action_4.setObjectName("action_4")

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.commandLinkButton.setText(_translate("MainWindow", "Отправить команду"))
        self.action.setText(_translate("MainWindow", "Показать график"))
        self.action_2.setText(_translate("MainWindow", "Показать график"))
        self.action_3.setText(_translate("MainWindow", "Показать график"))
        self.action_4.setText(_translate("MainWindow", "Показать график"))

