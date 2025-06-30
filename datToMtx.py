import os
import numpy as np
from numpy import asarray
from io import StringIO
import scipy.io as sio
from scipy.sparse import csr_matrix
import pandas as pd

# lines = np.loadtxt("./webdocs.dat")

text_file = open("./webdocs.dat", "r")
data = text_file.read()
text_file.close()

line = data.splitlines()
row = []
col = []
val = []

print(line[3])
#print(len(line))
mx = 0
for i in range(len(line)):
    arr = line[i].split(" ")
    #print(i)
    for a in arr:
        row.append(i)
        col.append(int(a)-1)
        mx = max(mx, int(a))
        val.append(1)

#print(mx)
out_file = "./webdocs.mtx"
sio.mmwrite(out_file, csr_matrix((val, (row, col)), shape=(1692082, 5267656)))

