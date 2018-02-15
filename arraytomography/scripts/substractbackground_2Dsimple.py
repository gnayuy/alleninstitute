# using ilastik "pixel classification" to separate the foreground from the background
# label image is saved as .h5 with multiple labels, e.g. 0 is background and 1 for foreground
# yy 12/20/2017

import time
import sys
import h5py
import SimpleITK as sitk
import numpy as np
import cython
#from libtiff import TIFFfile, TIFFimage, TIFF, TIFF3D

# init
fn_image=""
fn_label=""
fn_out=""

# command line interface
i = 1
while i < len(sys.argv):
	if sys.argv[i] == "-i":
		sys.argv.pop(i)
		fn_image = sys.argv.pop(i)
	elif sys.argv[i] == "-l":
		sys.argv.pop(i)
		fn_label = sys.argv.pop(i)
	elif sys.argv[i] == "-o":
		sys.argv.pop(i)
		fn_out = sys.argv.pop(i)
	else:
		i += 1

print("input image: ", fn_image)
print("input label:", fn_label)
print("output:", fn_out)

# read input image
at_image = sitk.ReadImage(fn_image)
#print(dir(at_image))
print(at_image.GetWidth(), at_image.GetHeight(), at_image.GetDepth())


# read input label
f = h5py.File(fn_label, "r")
p = f.get('exported_data')
thresh = 2*np.mean(p[:,:,1])

print(thresh)

# substract background
t = time.clock()

at_array = sitk.GetArrayFromImage(at_image) * (p[:,:,1] > thresh)

print("time elapse: ", time.clock() - t)

# write
sitk.WriteImage(sitk.GetImageFromArray(at_array), fn_out)


