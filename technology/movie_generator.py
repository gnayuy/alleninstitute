# movie_generator.py
# 8/29/2019 Yang Yu gnayuy@gmail.com
#
#
# usage:
# python movie_generator.py input.h5 downsample_factor output.avi saturation_low_thresh saturation_high_thresh
#
# example:
# time python movie_generator.py motion_corrected_video.h5 10 motionpreview10x.avi 15 85
#

import sys
import h5py
import numpy as np
import cv2

#
filename = sys.argv[1]
factor = int(sys.argv[2])
output = sys.argv[3]
low_thresh = int(sys.argv[4])
high_thresh = int(sys.argv[5])

#
f = h5py.File(filename, 'r')

# list all groups
print("keys: %s" % f.keys())
a_group_key = list(f.keys())[0]

# get data
data = list(f[a_group_key])

#
img0 = np.array(data[0])

#
n = len(data)
m = int(n/factor)

# save as a movie
fid = cv2.VideoWriter(output, cv2.VideoWriter_fourcc(*('MJPG')), 30, img0.shape, False)

# 
for i in range(m):
    mat = np.array(data[i*factor])

    # contrast enhancement
    a = mat.ravel()
    hist = np.histogram(a, bins=255)

    cs = np.cumsum(hist[0])
    intensity = hist[1]

    idx = np.searchsorted(cs, np.percentile(cs,low_thresh))
    low = intensity[idx]

    idx = np.searchsorted(cs, np.percentile(cs,high_thresh))
    high = intensity[idx]

    dist = high - low 

    newmat = 255.0 * (mat - low) / dist;

    indices = newmat < 0
    newmat[indices] = 0
    indices = newmat > 255
    newmat[indices] = 255

    newmat = newmat.astype(np.uint8)

    fid.write(newmat)

