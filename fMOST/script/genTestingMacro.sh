

# generate fiji macro to test bdv performance on fMOST mouse brains

# usage: sh genTestingMacro.sh 4 3 bdvs4


NUM=$1
LEVEL=$2
OUTPUT=$3

N=3 # tests

for(( i=1; i<=$N; i++ ))
do

OUTSCRIPT=${OUTPUT}"t"${i}"l"${LEVEL}".ijm"

if (( $LEVEL == 3 ))
then

echo "3 resolutions"

logfile="/local3/test${i}slice${NUM}l3.txt"
echo "f = File.open(\"${logfile}\");" >> $OUTSCRIPT

N1=$((NUM/2))
N2=$((N1/2))

echo "ts1 = getTime();" >> $OUTSCRIPT
echo "run(\"Image Sequence...\", \"open=/local3/paper/slice${NUM}/test_06550_mon.tif sort\");" >> $OUTSCRIPT
echo "ts2 = getTime();" >> $OUTSCRIPT
echo "print(f, \"load time (seconds): \" + (ts2-ts1)/1000);"  >> $OUTSCRIPT

echo "ts2 = getTime();"  >> $OUTSCRIPT
echo "run(\"Export Current Image as XML/HDF5\", \"manual_mipmap_setup subsampling_factors=[{ {1,1,1}, {2,2,2}, {4,4,4} }] hdf5_chunk_sizes=[{ {256,256,${NUM}}, {256,256,${N1}}, {256,256,${N2}} }] value_range=[Use values specified below] min=0 max=2048 timepoints_per_partition=0 setups_per_partition=0 use_deflate_compression export_path=./bdv${NUM}slicesl3.xml\");"  >> $OUTSCRIPT
echo "ts3 = getTime();"  >> $OUTSCRIPT
echo "print(f, \"conversion time (seconds): \" + (ts3-ts2)/1000);"  >> $OUTSCRIPT

#echo "saveAs(\"Text\", \"/local3/test${i}slice${NUM}l3.txt\");"  >> $OUTSCRIPT
# log window is sometime not active during long time running #

echo "close();"  >> $OUTSCRIPT
echo "run(\"Quit\")"  >> $OUTSCRIPT


elif (( $LEVEL == 4 ))
then

echo "4 resolutions"

logfile="/local3/test${i}slice${NUM}l4.txt"
echo "f = File.open(\"${logfile}\");" >> $OUTSCRIPT

N1=$((NUM/2))
N2=$((N1/2))
N3=$((N2/2))

echo "ts1 = getTime();" >> $OUTSCRIPT
echo "run(\"Image Sequence...\", \"open=/local3/paper/slice${NUM}/test_06550_mon.tif sort\");" >> $OUTSCRIPT
echo "ts2 = getTime();" >> $OUTSCRIPT
echo "print(f, \"load time (seconds): \" + (ts2-ts1)/1000);"  >> $OUTSCRIPT

echo "ts2 = getTime();"  >> $OUTSCRIPT
echo "run(\"Export Current Image as XML/HDF5\", \"manual_mipmap_setup subsampling_factors=[{ {1,1,1}, {2,2,2}, {4,4,4}, {8,8,8} }] hdf5_chunk_sizes=[{ {256,256,${NUM}}, {256,256,${N1}}, {256,256,${N2}}, {256,256,${N3}} }] value_range=[Use values specified below] min=0 max=2048 timepoints_per_partition=0 setups_per_partition=0 use_deflate_compression export_path=./bdv${NUM}slicesl4.xml\");"  >> $OUTSCRIPT
echo "ts3 = getTime();"  >> $OUTSCRIPT
echo "print(f, \"conversion time (seconds): \" + (ts3-ts2)/1000);"  >> $OUTSCRIPT

#echo "saveAs(\"Text\", \"/local3/test${i}slice${NUM}l4.txt\");"  >> $OUTSCRIPT
echo "close();"  >> $OUTSCRIPT
echo "run(\"Quit\")"  >> $OUTSCRIPT

elif (( $LEVEL == 5 ))
then

echo "5 resolutions"

logfile="/local3/test${i}slice${NUM}l5.txt"
echo "f = File.open(\"${logfile}\");" >> $OUTSCRIPT

N1=$((NUM/2))
N2=$((N1/2))
N3=$((N2/2))
N4=$((N3/2))

echo "ts1 = getTime();" >> $OUTSCRIPT
echo "run(\"Image Sequence...\", \"open=/local3/paper/slice${NUM}/test_06550_mon.tif sort\");" >> $OUTSCRIPT
echo "ts2 = getTime();" >> $OUTSCRIPT
echo "print(f, \"load time (seconds): \" + (ts2-ts1)/1000);"  >> $OUTSCRIPT

echo "ts2 = getTime();"  >> $OUTSCRIPT
echo "run(\"Export Current Image as XML/HDF5\", \"manual_mipmap_setup subsampling_factors=[{ {1,1,1}, {2,2,2}, {4,4,4}, {8,8,8}, {16,16,16} }] hdf5_chunk_sizes=[{ {256,256,${NUM}}, {256,256,${N1}}, {256,256,${N2}}, {256,256,${N3}}, {256,256,${N4}} }] value_range=[Use values specified below] min=0 max=2048 timepoints_per_partition=0 setups_per_partition=0 use_deflate_compression export_path=./bdv${NUM}slicesl5.xml\");"  >> $OUTSCRIPT
echo "ts3 = getTime();"  >> $OUTSCRIPT
echo "print(f, \"conversion time (seconds): \" + (ts3-ts2)/1000);"  >> $OUTSCRIPT

#echo "saveAs(\"Text\", \"/local3/test${i}slice${NUM}l5.txt\");"  >> $OUTSCRIPT
echo "close();"  >> $OUTSCRIPT
echo "run(\"Quit\")"  >> $OUTSCRIPT

else

echo "Invalid level input"

fi

done


