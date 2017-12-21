# convert label image from .h5 to .nii.gz
# label image is saved as .h5 with multiple labels, e.g. 0 is background and 1 for foreground
# yy 12/20/2017

import time
import sys
import h5py
import SimpleITK as sitk
import numpy as np
#from libtiff import TIFFfile, TIFFimage, TIFF, TIFF3D

# init
fn_label=""
fn_out=""

# command line interface
i = 1
while i < len(sys.argv):
	if sys.argv[i] == "-i":
		sys.argv.pop(i)
		fn_label = sys.argv.pop(i)
	elif sys.argv[i] == "-o":
		sys.argv.pop(i)
		fn_out = sys.argv.pop(i)
	else:
		i += 1

print("input: ", fn_label)
print("output:", fn_out)

# read input label
f = h5py.File(fn_label, "r")
p = f.get('exported_data')

# write
sitk.WriteImage(sitk.GetImageFromArray(p[:,:,:,1]), fn_out)

