
# sed -i 's/,/\ /g' file.swc

input=$1
output=$2

while read line
do

if [[ $line =~ "#name" || $line =~ "#comment" ]];
then

echo "remove comment $line"

else

echo $line >> $output

fi


done < $input
