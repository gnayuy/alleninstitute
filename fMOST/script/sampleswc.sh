
# sh sampleswc.sh input.swc output.swc scale

# input
ISWC=$1
OSWC=$2

SCALE=$3

OFFSET=$4

SR=`echo print 1.0 / $SCALE | python`

#
while read line
do

if [[ $line =~ "#" ]]
then

echo $line >> ${OSWC}

else

str=($line)

N=${str[0]}
TYPE=${str[1]}
X=${str[2]}
Y=${str[3]}
Z=${str[4]}
RADIUS=${str[5]}
PN=${str[6]}

#X=`echo "$X - $OFFSET" | bc`
#Y=`echo "$Y - $OFFSET" | bc`
Z=`echo "$Z - $OFFSET" | bc`

X=`echo "$X * $SR" | bc`
Y=`echo "$Y * $SR" | bc`
Z=`echo "$Z * $SR" | bc`

if [[ $PN -eq -1 ]]
then

echo "$((N-NUM)) $TYPE $X $Y $Z $RADIUS $PN" >> ${OSWC}

else

echo "$((N-NUM)) $TYPE $X $Y $Z $RADIUS $((PN-NUM))" >> ${OSWC}

fi

fi

done < ${ISWC}

