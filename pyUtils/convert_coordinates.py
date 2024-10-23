import os
import json
from osgeo import ogr, osr

ROOT_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), '../')
RESOURCE_PATH = os.path.join(ROOT_PATH, 'resource')

def convert_coordinates(description_path: str):

    with open(os.path.join(RESOURCE_PATH, description_path), 'r', encoding='utf-8') as file:
        descriptors = json.load(file)

    source_srs = osr.SpatialReference()
    source_srs.ImportFromEPSG(descriptors['source_epsg'])

    target_srs = osr.SpatialReference()
    target_srs.ImportFromEPSG(descriptors['target_epsg'])

    coord_transform = osr.CoordinateTransformation(source_srs, target_srs)

    input_file = os.path.join(RESOURCE_PATH, descriptors['raw_file'])
    output_file = os.path.join(RESOURCE_PATH, descriptors['input_file'])

    with open(input_file, 'r') as f:
        lines = f.readlines()

    with open(output_file, 'w') as f:

        f.write(lines[0])
        f.write(lines[1])

        attribute_num = len(lines[1].strip().split())

        for line in lines[2:]:
            data = line.strip().split()
            source_x, source_y = float(data[1]), float(data[0])

            point = ogr.Geometry(ogr.wkbPoint)
            point.AddPoint(source_x, source_y)
            point.Transform(coord_transform)

            lat, lon = point.GetX(), point.GetY()

            attributes = ''
            for i in range(2, attribute_num):
                attributes += f' {data[i]}'

            f.write(f"{lon} {lat}{attributes}\n")

if __name__ == '__main__':

    convert_coordinates('description.json')