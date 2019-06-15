from PyQt5.QtWidgets import QDockWidget

from .gcs_wid_telem_1 import *
from .gcs_wid_telem2 import *


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

    def set_value_x(self, value):
        self.set_label_value(self.ui.label_x_value, value)

    def set_value_y(self, value):
        self.set_label_value(self.ui.label_y_value, value)

    def set_value_z(self, value):
        self.set_label_value(self.ui.label_z_value, value)

    def set_value_result(self, value):
        self.set_label_value(self.ui.label_result_value, value)


# Widget to temp, pressure
class TelemWidget2(TelemWidgetParent):
    def __init__(self, parent=None, title=None, layout=None, name_label_x=None, name_label_y=None):
        TelemWidgetParent.__init__(self, parent, my_ui=Ui_DockWidget_telem2(), title=title, layout=layout)
        self.set_name_label_x(name_label_x)
        self.set_name_label_y(name_label_y)

    def set_value_x(self, value):
        self.set_label_value(self.ui.label_x_value, value)

    def set_value_y(self, value):
        self.set_label_value(self.ui.label_y_value, value)

    def set_name_label_x(self, name):
        self.set_label_name(self.ui.label_x, name)

    def set_name_label_y(self, name):
        self.set_label_name(self.ui.label_y, name)
