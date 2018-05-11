
OUT="distsgood.csv"

for i in *;
do


SUM=0;
NUM=0;
while read line
do

SUM=`echo "$SUM + $line" | bc`
NUM=$((NUM + 1))

done < ${i}"/goodaxonscores.txt"

DEN=`echo "scale=2; $SUM / $NUM " | bc -l`


SUM=0;
NUM=0;
while read line
do

SUM=`echo "$SUM + $line" | bc`
NUM=$((NUM + 1))

done < ${i}"/gooddenscores.txt"


AXON=`echo "scale=2; $SUM / $NUM " | bc -l`


echo "$i, $DEN, $AXON" >> $OUT


done
