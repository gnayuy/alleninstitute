# frames_generator.py
# 8/21/2019 Yang Yu gnayuy@gmail.com
#
#
# usage:
# time python frames_generator.py motion_corrected_video.h5 10 motionpreview10x 
# 
# 

import sys
import h5py
import numpy as np
import cv2

#
filename = sys.argv[1]
factor = int(sys.argv[2])
output = sys.argv[3]

#
f = h5py.File(filename, 'r')

# list all groups
print("keys: %s" % f.keys())
a_group_key = list(f.keys())[0]

# get data
data = list(f[a_group_key])

#
n = len(data)
m = int(n/factor)

# enhance contrast
clahe = cv2.createCLAHE(clipLimit = 3.0, tileGridSize=(8,8))

for i in range(m):
    mat = np.array(data[i*factor])
    cl = clahe.apply(mat)
    tmp = 255.0*cl/np.max(cl)
    tmp = tmp.astype('uint8')
    cv2.imwrite(output + "/img-"+str(100000+i)+".tif", tmp)
