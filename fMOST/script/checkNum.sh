# check missing files (nonconsecutive labeled files)
# yy 12/20/2017

#Usage:
# ls montage/*.tif > filelist.txt
# while read line; do i=${line%*_*}; j=${i#*_*}; echo $j >> fileNum.txt; done < filelist.txt 
# sh checkNum.sh fileNum.txt

filelist=$1


n=10

while read line
do

num=${line#0*}
num=${num#0*}
num=${num#0*}

if [ $n -lt $(($num)) ]
then

echo $n $line
n=$(($num))

fi

n=$(($n+1))

done < $filelist
