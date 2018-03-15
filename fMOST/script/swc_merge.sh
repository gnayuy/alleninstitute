# append the 2nd swc file to the 1st swc
# developed by Yang Yu, 3/14/2018

# usage: sh swc_merge.sh input1.swc input2.swc output.swc

# arguments
SWC1=$1
SWC2=$2

OUT=$3

#
NUM=0
while read line
do

if [[ $line =~ "#" ]]
then

echo $line >> $OUT

else

echo $line >> $OUT

NUM=$((NUM+1))

fi

done < ${SWC1}


while read line
do

if [[ $line =~ "#" ]]
then

echo "skip comment lines"

else

str=($line)

N=${str[0]}
TYPE=${str[1]}
X=${str[2]}
Y=${str[3]}
Z=${str[4]}
RADIUS=${str[5]}
PN=${str[6]}

if [[ $PN -eq -1 ]]
then

echo "$((N+NUM)) $TYPE $X $Y $Z $RADIUS $PN" >> ${OUT}

else

echo "$((N+NUM)) $TYPE $X $Y $Z $RADIUS $((PN+NUM))" >> ${OUT}

fi

fi

done < ${SWC2}



