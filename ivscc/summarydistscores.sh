
curnum=0

for i in */*distance*;
do

num=${i%*/*}

if [[ $curnum -eq $num ]]
then

n=$((n+1))

else

n=0
curnum=$num

fi

if [[ $i =~ axon ]]
then

#
while read line
do

ascore=${line}

done < $i



elif [[ $i =~ dendrite ]]
then

#
while read line
do

dscore=${line}

done < $i

else

#
while read line
do

score=${line}

done < $i


fi

if((n==2))
then
echo "$num, $score, $ascore, $dscore"
fi


done

