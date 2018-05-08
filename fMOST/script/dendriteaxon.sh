# type 3 dendrites
# type 2 axons


for i in *manualtraced*.swc;
do

echo $i

while read line;
do

if [[ $line != *"#"* ]]
then

str=($line)

echo ${str[1]}

if(( ${str[1]} == 3 ))
then

echo "this is a dendrites"

j=${i%*.swc}
k=${j#*manualtraced*}

name=${i%*_manualtraced*}
file=${name}"_estimated_seg"${k}.swc


if [ -f "$file" ]
then

ls $file

echo $file $k >> estimated_dendrite_tobesorted.txt

fi

break;

else

echo "this is an axon"

j=${i%*.swc}
k=${j#*manualtraced*}

name=${i%*_manualtraced*}
file=${name}"_estimated_seg"${k}.swc


if [ -f "$file" ]
then

ls $file

echo $file $k >> estimated_axon_tobbesorted.txt

fi

break;

fi

fi



done < $i


done
