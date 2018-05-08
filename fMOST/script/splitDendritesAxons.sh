
SWCFILE=$1

DENDRITE=$2
AXON=$3

while read line
do

if [[ $line =~ "#" ]]
then

echo $line >> $DENDRITE
echo $line >> $AXON

else

str=($line)

N=${str[0]}
TYPE=${str[1]}
X=${str[2]}
Y=${str[3]}
Z=${str[4]}
RADIUS=${str[5]}
PN=${str[6]}

if [[ $TYPE -eq 3 ]]
then

echo $line >> $DENDRITE

else

echo $line >> $AXON

fi

fi


done < $SWCFILE
