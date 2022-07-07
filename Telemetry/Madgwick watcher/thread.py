import os
from PyQt5.QtCore import QThread, pyqtSignal

import time

data_file = os.path.abspath('quat.csv')


# class Packet():
#     def __init__(self, data_line):
#         self.data = data_line
#
#     def parse(self):

class Thread(QThread):
    new_record = pyqtSignal(list)

    def __init__(self):
        QThread.__init__(self)
        self.now_time = 0

    def run(self):
        data = []
        time_start = time.time() / 2 + 10
        f = open(data_file)
        while True:
            ln = f.readline().split('\t')

            ln_time = float(ln[0])
            data.append(float(ln[1]))
            data.append(float(ln[2]))
            data.append(float(ln[3]))
            data.append(float(ln[4]))
            # print(round(ln_time + 19.59, 4))
            self.now_time = time.time() / 2 - time_start
            if ln_time > self.now_time:
                time.sleep(ln_time - self.now_time)
            print(ln_time)
            self.new_record.emit(data)
            data = []
        # f.close()
