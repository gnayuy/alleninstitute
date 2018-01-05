# sh linkswcs.sh inputdir
# assuming inputdir contains only swc files need to be linked into one single ano file
# yy 1/4/2018


INDIR=$1
OUTPUT=${INDIR}/tracings.ano

for i in ${INDIR}/*.swc;
do

echo "SWCFILE=$i" >> $OUTPUT

done
