import numpy as np
import matplotlib.pyplot as plt
import os
import random
import h5py

def txt2numpy(save_file = None, H = 192, W = 128):
    pc = np.array([map(float, line.strip().split()) for line in open(save_file, "r").readlines()])
    img = np.zeros((H, W), dtype = np.float32)
    for i in range(pc.shape[0]):
        h = int(pc[i][0])
        w = int(pc[i][1])
        img[h][w] = pc[i][2]
    # plt.imshow(img)
    # plt.show()
    return img

def show():
    root_dir = "/home/lhx/ND_BOS_uniform/ND_BOS_4096_POINT_GENERATE_30000SUPERVISED/depth_map/"
    files = os.listdir(root_dir)
    for f in files:
        keyname = f[:-4]
        print "NAME: {}".format(keyname)
        img = txt2numpy(os.path.join(root_dir, f))
        plt.imshow(img)
        plt.show()

def txt2hdf5():
    root_dir = "/home/lhx/ND_BOS_uniform/ND_BOS_4096_POINT/depth_map//"
    f = h5py.File("/home/lhx/ND_BOS_uniform/ND_BOS_4096_POINT/ND_depth_map.hdf5", "w")

    for file in os.listdir(root_dir):
        keyname = file[:-4]
        f[keyname] = txt2numpy(os.path.join(root_dir, file))
        print keyname

if __name__ == "__main__":
    txt2hdf5()

