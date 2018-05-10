

for i in *manualtraced*.swc;
do

n=${i%*_ds4*}

#echo $n

#
while read line;
do

if [[ $line != *"#"* ]]
then

str=($line)

if(( ${str[1]} == 3 ))
then

goodout="goodden.txt"
badout="badden.txt"

else

goodout="goodaxon.txt"
badout="badaxon.txt"

fi

fi

done < $i

m=${i%*.swc}
m=${m#*manualtraced*}

file=${i%*_resampled*}"_estimated_seg"$m".dist"

if [ -f "$file" ]
then

while read line
do

if [[ $line =~ "bi-directional" ]]
then

score=${line#*=\ *}

fi

done < $file

result=$(echo "$score < 2 && $score > 0" | bc)

if (( $result ))
then

echo $score $goodout

echo $i >> $goodout

else

echo $score $badout

echo $i >> $badout

fi


fi

done
