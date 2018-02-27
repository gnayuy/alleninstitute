# Y-flip image and invert intensity
# yy 2/27/2018

import time
import sys
import h5py
import SimpleITK as sitk
import numpy as np
import cython

# init
fn_image=""
fn_out=""

# command line interface
i = 1
while i < len(sys.argv):
	if sys.argv[i] == "-i":
		sys.argv.pop(i)
		fn_image = sys.argv.pop(i)
	elif sys.argv[i] == "-o":
		sys.argv.pop(i)
		fn_out = sys.argv.pop(i)
	else:
		i += 1

print("input image: ", fn_image)
print("output:", fn_out)

# read input image
image = sitk.ReadImage(fn_image)
#print(dir(at_image))
#print(at_image.GetWidth(), at_image.GetHeight(), at_image.GetDepth())

# flip and invert
t = time.clock()

array = sitk.GetArrayFromImage(image)

array = np.flip(array,0)

array = 255 - array

print("time elapse: ", time.clock() - t)

# write
sitk.WriteImage(sitk.GetImageFromArray(array), fn_out)


