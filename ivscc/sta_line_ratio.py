
# statistics a line ratio in a swc
#
#
# input : a swc, a threshold -- number of pixels average distance of points to line
# output: ratio of the length of line to the total length
# Yang Yu, gnayuy@gmail.com
# 8/20/2019

#
import sys
import numpy as np
import pandas as pd
from scipy import optimize

#
swcfile = sys.argv[1]
threshold = sys.argv[2]

# basic functions
def affine_line(s, theta, phi, x0, y0, z0):
    a = np.sin(theta) * np.cos(phi)
    b = np.sin(theta) * np.sin(phi)
    c = np.cos(theta)
    return np.array([s*a + x0, s*b+y0, s*c+z0])


def point_to_line_distance(x,y,z,theta,phi,x0,y0,z0):
    xx = x - x0
    yy = y - y0
    zz = z - z0
    
    a = np.sin(theta) * np.cos(phi)
    b = np.sin(theta) * np.sin(phi)
    c = np.cos(theta)
    
    r = np.array([xx, yy, zz])
    t = np.array([a,b,c])
    
    return np.linalg.norm(r - np.dot(r,t)*t)


def residuals(parameters, fixpoint, data, weights=None):
    theta, phi = parameters
    x0, y0, z0 = fixpoint
    if weights is None:
        w = np.ones(len(data))
    else:
        w = np.array(weights)
        
    diff = np.array([ point_to_line_distance(x,y,z,theta,phi,*fixpoint) for x,y,z in data])
    diff = diff*w
    return diff


def point_to_point_distance(p,q):
    squared_dist = np.sum((p-q)**2, axis=0)
    dist = np.sqrt(squared_dist)
    return dist

#
df = pd.read_csv(swcfile, delimiter=' ')
swc = df.values

#
n, m = swc.shape
branches = np.zeros(n)

for i in range(1, n-1):
    branches[ int(swc[i,6])-1 ] = branches[ int(swc[i,6])-1 ] + 1
    
idxBranchNodes = np.where(branches >= 2)

idxNodes = idxBranchNodes[0]

print(idxNodes)

nBranches = idxNodes.size

print(nBranches)

length = np.zeros(1)
linelike = np.zeros(1) # average distance of points to line

for i in range(0, nBranches-1):
    
    print("explore branch", i)
    index = idxNodes[i]
    
    val = swc[index,0]
    
    nextNodes = np.where(swc[:,6] == val)
    
    fragments = nextNodes[0]
    
    nFrags = fragments.size
    
    print(nFrags)
    
    x = np.array([ swc[index,2] ])
    y = np.array([ swc[index,3] ])
    z = np.array([ swc[index,4] ])

    #data[:,0] = np.array( [ swc[index,2], swc[index,3], swc[index,4] ])
    
    for j in range(0, nFrags):
        node = fragments[j]
        
        result = np.where(swc[:,6]==swc[node,0])
        
        x = np.insert(x, x.size, swc[node,2])
        y = np.insert(y, y.size, swc[node,3])
        z = np.insert(z, z.size, swc[node,4])
        
        while result[0].size > 0:
            node = result[0][0]
            
            x = np.insert(x, x.size, swc[node,2])
            y = np.insert(y, y.size, swc[node,3])
            z = np.insert(z, z.size, swc[node,4])
            
            result = np.where(swc[:,6]==swc[node,0])
            
        data = np.transpose(np.array([x,y,z]))
        nData,mData = data.shape
        print("frag",j,data.shape)
        
        fixpoint = np.array([ np.mean(data[:,0]), np.mean(data[:,1]), np.mean(data[:,2]) ] )
        bestfitW, success = optimize.leastsq(residuals, [0,0], args=(fixpoint, data) )
                                             
        diff = residuals(bestfitW, fixpoint, data)
        linelike = np.insert(linelike, linelike.size, np.mean(diff))

        lengthFrag = 0
        for i in range(1,nData):
            p = data[i-1]
            q = data[i]
            lengthFrag = lengthFrag + point_to_point_distance(p,q)

        length = np.insert(length, length.size, lengthFrag)

# remove the first element init by 0
length = np.delete(length, 0,0)
linelike = np.delete(linelike, 0,0)

print(length.shape, linelike.shape)

totallength = np.sum(length)

print(totallength)
print(linelike)


sum = 0
for i in range(0, linelike.size):
    if linelike[i]<float(threshold):
        sum = sum + length[i]
        
print("length of line ratio: ", sum/totallength)

