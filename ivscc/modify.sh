

for i in *.swc
do


headlines=${i%*.swc}.txt

while read line;
do


if [[ $line =~ "# gen" ]]
then

echo "# gen -- $line"
sed -i '1d' $i

elif [[ $line =~ "# source" ]]
then

echo "# source -- $line"
sed -i '1d' $i

elif [[ $line =~ "# id" ]]
then

echo "# id"

fi

done < $headlines


sed 's/,/\ /g' $i > ${i%*.swc}_tmp.swc
sed 's/#\ /#/g' ${i%*.swc}_tmp.swc > ${i%*.swc}_pf.swc

done
