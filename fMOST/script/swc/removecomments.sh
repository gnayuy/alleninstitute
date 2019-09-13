
# sed -i 's/,/\ /g' file.swc

input=$1


count=0
i=0

while read line
do

i=$((i+1))

if [[ $line =~ "#name" || $line =~ "#comment" ]]
then

echo "remove comment $line"

count=$((count+1))

fi

echo "i $i"

if((i>10))
then
break
fi


done < $input


echo "count $count"

for((i=0; i<$count; i++))
do

sed -i '1d' ${input} 

done
