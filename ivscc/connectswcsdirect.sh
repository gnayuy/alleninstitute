# append the following swc files to the 1st swc
# developed by Yang Yu, 5/8/2018

# usage: sh connectswcs.sh swcfilelist.txt output.swc

# arguments
FILES=$1
OUT=$2

#
NUM=0

while read SWCFILE
do

SWC=($SWCFILE)


if [[ $NUM -eq 0 ]]
then

# init

while read line
do

if [[ $line =~ "#" ]]
then

echo $line >> $OUT

else

echo $line >> $OUT

NUM=$((NUM+1))

fi

done < ${SWC[0]}


else

# append

OFFSET=$NUM

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

echo "$N $TYPE $X $Y $Z $RADIUS $PN" >> ${OUT}

fi

NUM=$((NUM+1))

done < ${SWC[0]}

fi

done < $FILES



