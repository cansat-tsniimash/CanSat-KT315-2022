import os

from PyQt5 import QtCore, QtWidgets
import pyqtgraph.opengl as gl
import numpy as np
import pyquaternion
from math import *
from stl import mesh
from itertools import chain

MESH_PATH = os.path.abspath('model.stl')


class PlaneWidget(gl.GLViewWidget):
    def __init__(self, mesh_path, *args, **kwargs):
        super(PlaneWidget, self).__init__(*args, **kwargs)
        self.setCameraPosition(distance=15)
        self.setBackgroundColor([100, 100, 100, 0])
        g = gl.GLGridItem()
        self.addItem(g)

        isc_coord = gl.GLAxisItem()
        isc_coord.setSize(100, 100, 100)
        self.addItem(isc_coord)

        self.plane_axis = gl.GLAxisItem()
        self.plane_axis.setSize(x=300, y=300, z=300)
        self.addItem(self.plane_axis)

        verts = self._get_mesh_points(mesh_path)
        faces = np.array([(i, i + 1, i + 2,) for i in range(0, len(verts), 3)])
        colors = np.array([(0.0, 1.0, 0.0, 1.0,) for i in range(0, len(verts), 3)])
        self.mesh = gl.GLMeshItem(vertexes=verts, faces=faces, faceColors=colors, smooth=False, shader='shaded')

        # self.mesh.rotate(180, 0, 0, 1)

        self.addItem(self.mesh)
        # self._update_mesh([0, 0, 1])

    def on_new_records(self, records):
        self._update_mesh(records)

    def _get_mesh_points(self, mesh_path):
        your_mesh = mesh.Mesh.from_file(mesh_path)
        points = your_mesh.points

        points = np.array(list(chain(*points)))
        i = 0
        nd_points = np.ndarray(shape=(len(points) // 3, 3,))
        for i in range(0, len(points) // 3):
            nd_points[i] = points[i * 3: (i + 1) * 3]

        return nd_points

    def _transform_object(self, target, move=True, rotate=True, scale=1 / 50):
        target.resetTransform()
        target.scale(scale, scale, scale)
        if move: target.translate(0, 0, 0.1)
        if rotate:
            target.rotate(degrees(self.rotation.angle), self.rotation.axis[0], self.rotation.axis[1],
                          self.rotation.axis[2])

    def _update_rotation(self, record):
        quat = pyquaternion.Quaternion(record)
        self.rotation = quat

        self._transform_object(self.mesh)

        self._transform_object(self.plane_axis, move=False)


class MyWin(QtWidgets.QDockWidget):

    def __init__(self, parent=None):
        QtWidgets.QDockWidget.__init__(self)
        self.plane_widget = PlaneWidget(mesh_path=MESH_PATH, parent=self)
        self.setFixedSize(1666, 1000)

        self.setWidget(self.plane_widget)
        self.isc_coord = gl.GLAxisItem()
        self.isc_coord.setSize(25, 25, 25)
        self.show()

    @QtCore.pyqtSlot(list)
    def _show(self, msg):
        quat = pyquaternion.Quaternion(msg)
        self.plane_widget._update_rotation(quat)
