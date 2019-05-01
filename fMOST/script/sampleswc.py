

import pandas
import numpy as np
import sys


#
inputswc = sys.argv[1]
outputswc = sys.argv[2]

offx=17080
offy=38385
offz=4077

#
# inputneuron = pandas.read_csv(inputswc, names=['n', 'type', 'x', 'y', 'z'], delimiter=' ')

inputneuron = pandas.read_csv(inputswc, delimiter=' ')


# print(dir(inputneuron))

n = inputneuron.size / 10 



#for i in range(0,int(n)):
    #print(i)


x = inputneuron.iloc[:,1]
y = inputneuron.iloc[:,2]
z = inputneuron.iloc[:,3]

print("before process:", np.max(x), np.max(y), np.max(z))


#for i in range(0,5):
#    print(inputneuron.iloc[i,1], inputneuron.iloc[i,2], inputneuron.iloc[i,3], inputneuron.iloc[i,4])
    

inputneuron.iloc[:,1] = 0.125*(inputneuron.iloc[:,1] - offx)
inputneuron.iloc[:,2] = 0.125*(inputneuron.iloc[:,2] - offy)
inputneuron.iloc[:,3] = 0.125*(inputneuron.iloc[:,3] - offz)


x = inputneuron.iloc[:,1]
y = inputneuron.iloc[:,2]
z = inputneuron.iloc[:,3]

print("after process:", np.max(x), np.max(y), np.max(z))

#inputneuron.to_csv(outputswc, index=False)

inputneuron.to_csv(outputswc)

