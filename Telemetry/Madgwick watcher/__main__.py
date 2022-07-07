import sys
from PyQt5 import QtWidgets

from _3d import *
from thread import *

if __name__ == "__main__":
    ######################################
    app = QtWidgets.QApplication(sys.argv)
    myapp = MyWin()

    myapp.show()
    ######################################
    thread = Thread()
    thread.new_record.connect(myapp._show)
    thread.start()
    sys.exit(app.exec_())
