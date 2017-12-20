# check invalid tif file
# yy 12/20/2017

# Usage:
# sh imagecheck.sh INDIR images.txt 
#grep -C 6 "not" images.txt  >> error.txt

DIR=$1
OUT=$2

for i in $DIR/*.tif
do

echo $i >> $OUT

time bin/tiffinfo -D $i &>> $OUT

echo " " >> $OUT


done
