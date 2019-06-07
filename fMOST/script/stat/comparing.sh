

while read line
do

str=($line)

n2=${str[0]}
n1=${str[1]}
output=${str[2]} 

time sh compare17302swcs.sh $n1 $n2 $output

done < inputs.txt

