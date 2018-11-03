import sys
import logging
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import QThread

from .mavlink import *
from .graphing import *

if __name__ == "__main__":
    logging.basicConfig(
        stream=sys.stdout, level=logging.INFO,
        format="%(asctime)-15s %(message)s"
    )

    ######################################
    app = QtWidgets.QApplication(sys.argv)
    myapp = MyWin()

    myapp.show()
    ######################################


    thread = MavlinkThread()
    thread.new_atmega_record.connect(myapp.atm_msg)
    thread.new_imu_isc_record.connect(myapp.imu_isc_msg)
    thread.new_imu_rsc_record.connect(myapp.imu_rsc_msg)
    thread.new_sensors_record.connect(myapp.sens_msg)
    thread.new_gps_record.connect(myapp.gps_msg)
    thread.new_state_record.connect(myapp.state_msg)
    thread.start()

    myapp.new_send_command.connect(thread.post_msg)

    sys.exit(app.exec_())
    MyWin.accel_f.close()
    MyWin.compass_f.close()
    # thread.exit()
    # exit(main(sys.argv[1:]))

