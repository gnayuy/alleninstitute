
import pandas
import numpy as np
import sys


#
inputapo = sys.argv[1]
outputapo = sys.argv[2]

#
inputpoints = pandas.read_csv(inputapo, delimiter=' ')

#
inputpoints.iloc[:,2] = inputpoints.iloc[:,4]/16
inputpoints.iloc[:,3] = inputpoints.iloc[:,5]/64
inputpoints.iloc[:,4] = inputpoints.iloc[:,6]/64

inputpoints.iloc[:,10] = inputpoints.iloc[:,10]/100

#
inputpoints.to_csv(outputapo, index=False)
