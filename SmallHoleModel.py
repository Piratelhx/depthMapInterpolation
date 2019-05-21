#!/usr/bin/env python
# encoding: utf-8

import sys
import os  
import os.path  
import gzip  
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import cv2  


def read_gz_file(path):  
    if os.path.exists(path):  
        with gzip.open(path, 'r') as pf:  
            for line in pf:  
                yield line  
    else:  
        print('the path [{}] is not exist!'.format(path))  

def main():
    image2d_dir_path = "/media/lhx/data2T/datafrom_pixtalks_008/data/dataset/ND-2006/2d_data/"
    image3d_dir_path = "/media/lhx/data2T/datafrom_pixtalks_008/data/dataset/ND-2006/"
    input_path = "/media/lhx/data2T/datafrom_pixtalks_008/data/dataset/deal_data_code/ND2006_box.txt"

    lines0 = open(input_path, 'r').readlines()
    i = 0
    while i < len(lines0):
        line = lines0[i]
        items = line.strip().split(' ')
        image_name = items[0]
        face_num = int(items[-1])
        if face_num == 1 and i > 0:
            box = list(map(float, lines0[i + 1].strip().split(' ')))
            keypoint = list(map(int, map(float, lines0[i + 2].strip().split(' '))))
            image3d_path = image_name[:image_name.rfind('d') + 1] + str(int(image_name[image_name.rfind('d') + 1:image_name.rfind('.')]) - 1) + '.abs.gz'
            print("Face Box: {} \n keypoint: {} \n image_name: {} \n image_3d_path: {}".format(box, keypoint, image_name, image3d_path))
            
            image = cv2.imread(os.path.join(image2d_dir_path, image_name))
            con = read_gz_file(os.path.join(image3d_dir_path, image3d_path))
            lines = list(con)
            flag = np.array(list(map(int, lines[3].decode().strip().split(' '))))
            x = np.array(list(map(float, lines[4].decode().strip().split(' '))))
            y = np.array(list(map(float, lines[5].decode().strip().split(' '))))
            z = np.array(list(map(float, lines[6].decode().strip().split(' '))))
            
            visible = (flag == 1)
            stride = 1  # sample interval
            row = 480
            col = 640
            for q in range(row):
                for j in range(col):
                    if q % stride != 0 or j % stride != 0:
                        visible[q*col+j] = 0
            x = x[visible]
            y = y[visible]
            z = z[visible]

            display = np.zeros((480, 640))
            max_value = -999999.
            # length = 1900
            length = 3400 
            for xx, yy, zz in zip(x, y, z):
                yyy = -int (yy * length / -zz) + 240
                xxx = int(xx * length / -zz) + 320
                if xxx >= 0 and xxx < 640 and yyy >= 0 and yyy < 480:
                    display[yyy, xxx] = zz
                if zz > max_value:
                    max_value = zz


            display = display - (display !=0 ) * max_value
            display = display * (display > -180)
            for point in zip(keypoint[0::2], keypoint[1::2]):
                cv2.circle(display, point, 16, (-180,), -1)
            cv2.rectangle(display, (int(box[0]),int(box[1])), (int(box[2]+box[0]),int(box[3]+box[1])), (255,), 5)
            
            plt.imshow(display)
            plt.show()
            
        i += face_num*2 + 1

if __name__ == "__main__":
    main()
