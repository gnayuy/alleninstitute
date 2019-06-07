

import pandas
import numpy as np
import sys


#
inputswc = sys.argv[1]
outputswc = sys.argv[2]

offx=0
offy=0
offz=0

#
# inputneuron = pandas.read_csv(inputswc, names=['n', 'type', 'x', 'y', 'z'], delimiter=' ')

inputneuron = pandas.read_csv(inputswc, delimiter=' ')



#for i in range(0, 10):
#    print(i, inputneuron.iloc[0,i])


# print(dir(inputneuron))

n = inputneuron.size / 10 



#for i in range(0,int(n)):
    #print(i)


x = inputneuron.iloc[:,2]
y = inputneuron.iloc[:,3]
z = inputneuron.iloc[:,4]

print("before process:", np.max(x), np.max(y), np.max(z))


#for i in range(0,5):
#    print(inputneuron.iloc[i,1], inputneuron.iloc[i,2], inputneuron.iloc[i,3], inputneuron.iloc[i,4])
    

inputneuron.iloc[:,2] = 2*(inputneuron.iloc[:,2] - offx)
inputneuron.iloc[:,3] = 2*(inputneuron.iloc[:,3] - offy)
inputneuron.iloc[:,4] = 2*(inputneuron.iloc[:,4] - offz)


x = inputneuron.iloc[:,2]
y = inputneuron.iloc[:,3]
z = inputneuron.iloc[:,4]

print("after process:", np.max(x), np.max(y), np.max(z))

outneuron = inputneuron.iloc[:,0:7]

for i in range(0, 7):
    print(i, outneuron.iloc[0,i])

#inputneuron.to_csv(outputswc, index=False)

outneuron.to_csv(outputswc, float_format='%.3f', sep=' ', index=False)
