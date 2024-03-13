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

#Download and extract the data:  http://cs231n.stanford.edu/tiny-imagenet-200.zip
#You can also do wget http://cs231n.stanford.edu/tiny-imagenet-200.zip

#Data_dir = '/scratch/bell/sferdou/tiny-imagenet'
Data_dir = './'

#train images
Data_train = os.path.join(Data_dir, 'tiny-imagenet-200/train')
#test images
Data_test = os.path.join(Data_dir, 'tiny-imagenet-200/test')
#validation images
Data_val = os.path.join(Data_dir, 'tiny-imagenet-200/val')

#output paths
Out_train = os.path.join(Data_dir,'tiny-imagenet-vectors/train')
Out_test = os.path.join(Data_dir,'tiny-imagenet-vectors/test')
Out_val = os.path.join(Data_dir,'tiny-imagenet-vectors/val')


#all train image files
train_files= glob.glob(Data_train+"/*/images/*.JPEG")
print(len(train_files))

test_files= glob.glob(Data_test+"/images/*.JPEG")
print(len(test_files))

val_files= glob.glob(Data_val+"/images/*.JPEG")
print(len(val_files))



"""
image = Image.open(train_files[0])

vec = asarray(image)

print(vec.shape)

vec1 = vec.flatten()

print(vec1.shape)
"""

#read images and make them vectors
n = len(train_files)
#n=10
nf = 64*64*3
Im_data = np.zeros([n, nf])

i=0
for f_count in range(n):
    image = Image.open(train_files[f_count])
    vec = asarray(image).flatten()
    Im_data[i,0:len(vec)] = vec
    i = i+1

#mean subtraction
Im_data_mean = np.mean(Im_data,axis=1)
Im_data_mean = Im_data_mean[:,np.newaxis]
Im_data = Im_data - Im_data_mean

#normalized
Im_data_norm = np.linalg.norm(Im_data,axis=1)
Im_data_norm = Im_data_norm[:,np.newaxis]
Im_data = Im_data / Im_data_norm

print(Im_data.shape)


#write it as mtx or csv
mtx=True
if mtx==True:
    out_file = Out_train+"/tiny_image_train.mtx"
    sio.mmwrite(out_file,csr_matrix(Im_data))
    out_fileName= Out_train+"/tiny_image_trainName.txt"
    with open(out_fileName, 'w') as f:
        for line in train_files:
            f.write(line + "\n")
else:
    out_file = Out_train+"/tiny_image_train.csv"
    first_row = np.zeros((1,2))
    first_row[0][0] = n
    first_row[0][1] = nf
    df = pd.DataFrame(data=first_row.astype(int))
    df.to_csv(out_file,mode='w',sep=' ', header=False, index=False)
    df = pd.DataFrame(data=Im_data.astype(float))
    df.to_csv(out_file,mode='a',sep=' ', header=False, index=False)