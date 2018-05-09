
# sh sampleswc.sh input.swc output.swc scale

# input
ISWC=$1

DS1=$2
DS2=$3
DS3=$4
DS4=$5
DS5=$6

#OFFSET=$4

#SR1=`echo print 1.0 / $SCALE | python`

SR=0.5

#
while read line
do

if [[ $line =~ "#" ]]
then

echo $line >> ${DS1}
echo $line >> ${DS2}
echo $line >> ${DS3}
echo $line >> ${DS4}
echo $line >> ${DS5}

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
#Z=`echo "$Z - $OFFSET" | bc`

X=`echo "$X * $SR" | bc`
Y=`echo "$Y * $SR" | bc`
Z=`echo "$Z * $SR" | bc`
RADIUS=`echo "$RADIUS * $SR" | bc`

echo "$N $TYPE $X $Y $Z $RADIUS $PN" >> ${DS1}

X=`echo "$X * $SR" | bc`
Y=`echo "$Y * $SR" | bc`
Z=`echo "$Z * $SR" | bc`
RADIUS=`echo "$RADIUS * $SR" | bc`

echo "$N $TYPE $X $Y $Z $RADIUS $PN" >> ${DS2}

X=`echo "$X * $SR" | bc`
Y=`echo "$Y * $SR" | bc`
Z=`echo "$Z * $SR" | bc`
RADIUS=`echo "$RADIUS * $SR" | bc`

echo "$N $TYPE $X $Y $Z $RADIUS $PN" >> ${DS3}

X=`echo "$X * $SR" | bc`
Y=`echo "$Y * $SR" | bc`
Z=`echo "$Z * $SR" | bc`
RADIUS=`echo "$RADIUS * $SR" | bc`

echo "$N $TYPE $X $Y $Z $RADIUS $PN" >> ${DS4}

X=`echo "$X * $SR" | bc`
Y=`echo "$Y * $SR" | bc`
Z=`echo "$Z * $SR" | bc`
RADIUS=`echo "$RADIUS * $SR" | bc`

echo "$N $TYPE $X $Y $Z $RADIUS $PN" >> ${DS5}

fi

done < ${ISWC}

