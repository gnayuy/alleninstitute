
ISWC=$1
OSWC=$2

NLINE=0;
NUM=1000000
while read line
do

if [[ $line =~ "#" ]]
then

echo $line >> ${OSWC}

else

NLINE=$((NLINE+1))

str=($line)

N=${str[0]}

if((N < NUM))
then

NUM=$N

fi

fi

done < ${ISWC}

NUM=$((NUM-1))

echo $NUM

PRENUM=$NUM

NPROCESS=0;

while((NPROCESS < NLINE))
do


while read line
do

if [[ $line =~ "#" ]]
then

echo "$NPROCESS < $NLINE"

else

str=($line)

N=${str[0]}
TYPE=${str[1]}
X=${str[2]}
Y=${str[3]}
Z=${str[4]}
RADIUS=${str[5]}
PN=${str[6]}



ERR=$((N-PRENUM))

if [[ $ERR -eq 1 ]]
then

PRENUM=$N
NPROCESS=$((NPROCESS+1))

if [[ $PN -eq -1 ]]
then

echo "$((N-NUM)) $TYPE $X $Y $Z $RADIUS $PN" >> ${OSWC}

else

echo "$((N-NUM)) $TYPE $X $Y $Z $RADIUS $((PN-NUM))" >> ${OSWC}

fi

fi

fi

done < ${ISWC}


done
