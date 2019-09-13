

import pandas
import numpy as np
import sys


#
inputswc = sys.argv[1]
outputswc = sys.argv[2]

#
inputneuron = pandas.read_csv(inputswc, delimiter=' ')

print(inputneuron.shape)

#
inputneuron.iloc[:,2] = inputneuron.iloc[:,2]/64
inputneuron.iloc[:,3] = inputneuron.iloc[:,3]/64
inputneuron.iloc[:,4] = inputneuron.iloc[:,4]/16

#
inputneuron.to_csv(outputswc, index=False, sep=' ')

