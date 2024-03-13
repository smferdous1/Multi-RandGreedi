import csv
import os
import glob
import PIL
from PIL import Image
import numpy as np
from numpy import asarray
import scipy.io as sio
from scipy.sparse import csr_matrix
import pandas as pd
import json

Data_dir = './'

#train images
Data_train = os.path.join(Data_dir, 'tiny-imagenet-200/train')

#output paths
Out_train = os.path.join(Data_dir,'tiny-imagenet-vectors/train')

#all train image files
# train_files= glob.glob(Data_train+"/*/images/*.JPEG")
with open(Out_train+"/tiny_image_trainName.txt") as f:
    train_files = f.read().splitlines()
print(len(train_files))


# Arr = [76541, 4656, 22925, 54768, 53777, 23226, 69674, 31010, 58810, 26200, 43324, 55968, 48385, 30611, 7787, 44060]
Arr = [76541, 54524, 20472, 6972, 53950, 74503, 35386, 30857, 48603, 23044, 6867, 54885, 21757, 6896, 27185, 49479]
n = len(Arr)
imgs = [Image.open(train_files[Arr[0]])]*n
for i in range(n):
    imgs[i] = Image.open(train_files[Arr[i]])


rows = 4
cols = n // rows
w, h = imgs[0].size
grid = Image.new('RGB', size=(cols * w, rows * h))
grid_w, grid_h = grid.size

for i, img in enumerate(imgs):
    grid.paste(img, box=(i % cols * w, i // cols * h))

grid.show()
grid.save("tiny4x4_b2.png")