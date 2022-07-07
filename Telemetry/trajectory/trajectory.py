from collections import namedtuple
import fastkml
from fastkml import kml
from shapely.geometry import Polygon, LineString

INPUT = 'input.csv'
OUTPUT = 'output.kml'

stream = open(INPUT, mode="r")

Point = namedtuple("Point", ["lon", "lat", "height"])
points = []

while True:
    line = stream.readline()
    if not line:
        break

    line = line.strip()
    lon, lat, height = line.split(";")
    point = Point(float(lon), float(lat), float(height))
    points.append(point)


polygons = []

prev_point = points[0]
for i in range(1, len(points)):
    cur_point = points[i]
    polygon = [
        prev_point,
        cur_point,
        Point(cur_point.lon, cur_point.lat, 0),
        Point(prev_point.lon, prev_point.lat, 0),
        prev_point,
    ]

    polygons.append(polygon)
    prev_point = cur_point


k = kml.KML()
ns = '{http://www.opengis.net/kml/2.2}'

polystyle = fastkml.styles.PolyStyle(ns=ns, color='ccccccaa', outline=0)
linestyle = fastkml.styles.LineStyle(ns=ns, color='ff0000ff', width=10)
styles = fastkml.styles.Style(
    ns=ns, id='TheStyle', styles=[polystyle, linestyle]
)
d = kml.Document(ns, 'docid', 'doc name', 'doc description', styles=[styles])
k.append(d)
f = kml.Folder(ns, 'fid', 'f name', 'f description')
d.append(f)

for polygon in polygons:
    p = kml.Placemark(ns, 'id', 'name', 'description', styleUrl='#TheStyle')
    p.geometry = fastkml.geometry.Geometry(
        ns=ns,
        geometry=Polygon(polygon),
        altitude_mode='absolute',
    )
    f.append(p)


p = kml.Placemark(ns, styleUrl='#TheStyle')
p.geometry = fastkml.geometry.Geometry(
    ns=ns,
    geometry=LineString(points),
    altitude_mode='absolute'
)
f.append(p)

with open(OUTPUT, mode="w") as outstream:
    outstream.write(k.to_string(prettyprint=True))