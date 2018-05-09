IN=$1
OUT=$2

while read line;
do

j=${line%*)*}; 
k=${j#*x*}; 

n=${k#*x*};

echo $line $n >> $OUT

done < $IN
