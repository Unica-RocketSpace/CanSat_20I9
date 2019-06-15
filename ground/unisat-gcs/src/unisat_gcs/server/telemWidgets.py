from PyQt5.QtWidgets import QDockWidget
from PyQt5.QtGui import QColor

from .gcs_wid_telem_1 import *
from .gcs_wid_telem_2 import *
from.gcs_wid_telem_buttons import *


class TelemWidgetParent(QDockWidget):
    def __init__(self, parent=None, my_ui=None, title=None, layout=None):
        QDockWidget.__init__(self, parent)
        self.ui = my_ui
        self.ui.setupUi(self)
        self.set_window_title(title)
        self.set_layout(layout)

    def set_label_value(self, _label, value):
        _label.setText(str(value))

    def set_label_name(self, _label, name):
        _label.setText(str(name))

    def set_window_title(self, title):
        self.setWindowTitle(str(title))

    def set_layout(self, layout):
        layout.addWidget(self)


# Widget to accel, gyro and magnetometer
class TelemWidget1(TelemWidgetParent):
    def __init__(self, parent=None, title=None, layout=None):
        TelemWidgetParent.__init__(self, parent, my_ui=Ui_DockWidget_telem1(), title=title, layout=layout)

    def set_value_1(self, value):
        self.set_label_value(self.ui.label_1_value, value)

    def set_value_2(self, value):
        self.set_label_value(self.ui.label_2_value, value)

    def set_value_3(self, value):
        self.set_label_value(self.ui.label_3_value, value)

    def set_value_4(self, value):
        self.set_label_value(self.ui.label_4_value, value)

    def set_name_label_1(self, name):
        self.set_label_name(self.ui.label_1, name)

    def set_name_label_2(self, name):
        self.set_label_name(self.ui.label_2, name)

    def set_name_label_3(self, name):
        self.set_label_name(self.ui.label_3, name)

    def set_name_label_4(self, name):
        self.set_label_name(self.ui.label_4, name)


# Widget to temp, pressure
class TelemWidget2(TelemWidgetParent):
    def __init__(self, parent=None, title=None, layout=None, name_label_1=None, name_label_2=None):
        TelemWidgetParent.__init__(self, parent, my_ui=Ui_DockWidget_telem2(), title=title, layout=layout)
        self.set_name_label_1(name_label_1)
        self.set_name_label_2(name_label_2)

    def set_value_1(self, value):
        self.set_label_value(self.ui.label_1_value, value)

    def set_value_2(self, value):
        self.set_label_value(self.ui.label_2_value, value)

    def set_name_label_1(self, name):
        self.set_label_name(self.ui.label_1, name)

    def set_name_label_2(self, name):
        self.set_label_name(self.ui.label_2, name)


class TelemWidgetButtons(QDockWidget):
    def __init__(self, parent=None, title=None, layout=None):
        QDockWidget.__init__(self, parent)
        self.ui = Ui_DockWidget_telem_buttons()
        self.ui.setupUi(self)
        self.set_window_title(title)
        self.set_layout(layout)
        red_color = QColor(255, 0, 0).name()
        self.set_color(self.ui.toolButton_go_keel, red_color)
        self.set_color(self.ui.toolButton_go_left, red_color)
        self.set_color(self.ui.toolButton_go_right, red_color)
        self.set_color(self.ui.toolButton_wing_left, red_color)
        self.set_color(self.ui.toolButton_wing_right, red_color)

    def set_window_title(self, title):
        self.setWindowTitle(str(title))

    def set_layout(self, layout):
        layout.addWidget(self)

    def set_color(self, button, color):
        button.setStyleSheet('QToolButton { background-color: %s}' % color)
