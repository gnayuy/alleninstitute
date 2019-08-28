
import sys
import numpy as np
from PIL import Image


filename = sys.argv[1]
factor = int(sys.argv[2])
output = sys.argv[3]

img = Image.open(filename)
mat = np.array(img)

a = mat.ravel()
hist = np.histogram(a, bins=255)

cs = np.cumsum(hist[0])
intensity = hist[1]

idx = np.searchsorted(cs, np.percentile(cs,20))
low = intensity[idx]

idx = np.searchsorted(cs, np.percentile(cs,factor))
high = intensity[idx]

dist = high - low 

newmat = 255.0 * (mat - low) / dist;

indices = newmat < 0
newmat[indices] = 0
indices = newmat > 255
newmat[indices] = 255

newmat = newmat.astype(np.uint8)

outimg = Image.fromarray(newmat)
outimg.save(output)
