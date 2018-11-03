import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtGui
import pyqtgraph.opengl as gl




## Start Qt event loop unless running in interactive mode.
if __name__ == '__main__':
	import sys

	app = QtGui.QApplication([])

	win = gl.GLViewWidget()

	# data_all = fs.readall("/home/developer/")


	if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
		QtGui.QApplication.instance().exec_()

