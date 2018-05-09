while read line;
do

targetfolder="/local2/IVSCC_tera/$line"

echo "mkdir $targetfolder"
mkdir $targetfolder

subfolders=${line}"_sorted.txt"

while read subline;
do

str=($subline);

echo "cp -r \"${str[0]}\" $targetfolder/"
cp -r "${str[0]}" $targetfolder/

done < $subfolders


done < specimen25.txt
