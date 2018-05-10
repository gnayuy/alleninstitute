
curnum=0

for i in */*dist.txt;
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

if [[ $line =~ "bi-directional" ]]
then

ascore=${line#*=\ *}

elif [[ $line =~ "differen-structure-average" ]]
then

adscore=${line#*=\ *}

elif [[ $line =~ "percent of different-structure (average)" ]]
then

adpscore=${line#*=\ *}

fi


done < $i



elif [[ $i =~ dendrite ]]
then

#
while read line
do

if [[ $line =~ "bi-directional" ]]
then

dscore=${line#*=\ *}

elif [[ $line =~ "differen-structure-average" ]]
then

ddscore=${line#*=\ *}

elif [[ $line =~ "percent of different-structure (average)" ]]
then

ddpscore=${line#*=\ *}

fi


done < $i

else

#
while read line
do

if [[ $line =~ "bi-directional" ]]
then

wscore=${line#*=\ *}

elif [[ $line =~ "differen-structure-average" ]]
then

wdscore=${line#*=\ *}

elif [[ $line =~ "percent of different-structure (average)" ]]
then

wdpscore=${line#*=\ *}

fi

done < $i


fi

if((n==2))
then
echo "$num, $wscore, $wdscore, $wdpscore, $ascore, $adscore, $adpscore, $dscore, $ddscore, $ddpscore"
fi


done

