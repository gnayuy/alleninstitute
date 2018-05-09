


#
time for i in *; do time /local2/bin/vaa3d -x IVSCC_import -f import -i $i -o ${i}".v3draw" -p 1 1; done

# paths with RES*
for i in /local2/IVSCC_mouse_inhibitory_tera/*; do if [ -d ${i}"/teraFullTiff" ]; then echo ${i}"/teraFullTiff" >> paths.txt; else echo ${i} >> paths.txt; fi; done

#
#while read line; do for i in  ${line}"/RES*"; do echo $i; done done < paths.txt

#while read line; do for i in  ${line}"/RES*"; do echo ${i} >> folderpaths.txt;  done done < paths.txt 

while read line; do j=${line#*tera/*};  for i in  ${line}"/RES*"; do echo ${i} >> ${j%*/*}".txt"; done done < paths.txt 


while read line; do str=($line); i=${str[0]}; j=${i%*)*}; k=${j#*x*}; echo ${k#*x*};  done < 328023827.txt


## sortscales.sh

IN=$1
OUT=$2

while read line;
do

str=($line)

i=${str[0]}; 
j=${i%*)*}; 
k=${j#*x*}; 

n=${k#*x*};

echo $i $n >> $OUT

i=${str[1]}; 
j=${i%*)*}; 
k=${j#*x*}; 

n=${k#*x*};

echo $i $n >> $OUT

i=${str[2]}; 
j=${i%*)*}; 
k=${j#*x*}; 

n=${k#*x*};

echo $i $n >> $OUT

i=${str[3]}; 
j=${i%*)*}; 
k=${j#*x*}; 

n=${k#*x*};

echo $i $n >> $OUT

i=${str[4]}; 
j=${i%*)*}; 
k=${j#*x*}; 

n=${k#*x*};

echo $i $n >> $OUT

i=${str[5]}; 
j=${i%*)*}; 
k=${j#*x*}; 

n=${k#*x*};

echo $i $n >> $OUT

done < $IN


#
for i in *.txt; do sh sortscales.sh $i ${i%*.txt}"_tobesorted.txt" ; done


#  RES(121x121x9) 9
sort -n -r -k 2 test.txt

for i in *_tobesort*; do  sort -n -r -k 2 $i >> ${i%*_*}_sorted.txt; done



for i in *_sort*; do echo ${i%*_sort*}_ds4.tif; n=0;  while read line; do  n=$((n+1)); if(( $n == 5 )); then str=($line); time /local2/bin/vaa3d -x cropped3D -f teraflyCombine -i "${str[0]}" -o ${i%*_sort*}_ds4.tif; fi;   done < $i;   done


 n=0; for i in *.v3draw; do n=$((n+1)); if(( n < 13 )); then mv $i first/; elif(( n < 25 )); then mv $i second/; elif((n<37)); then mv $i third/; elif((n<49)); then mv $i fourth/; elif((n<61)); then mv $i fifth/; elif((n<73)); then mv $i sixth/; elif((n<85)); then mv $i seventh/; elif((n<97)); then mv $i eighth/; elif((n<108)); then mv $i nineth/; else mv $i tenth/; fi; done


# convert.sh
for i in *.v3draw;
do

mkdir /local2/IVSCC_tera/${i%*.v3draw}
time /local2/teraconverter -s="$i" -d="/local2/IVSCC_tera/${i%*.v3draw}" --halve=max --resolutions=01234 --width=256 --height=256 --depth=256 --sfmt="Vaa3D raw" --dfmt="TIFF (tiled, 3D)" --libtiff_rowsperstrip=-1;

done

#
nohup time sh ../convert.sh &

#
bin/vaa3d -x mapping -f map -i '/data/mat/yang/first25/test/328023827_ds4.v3draw' '/data/mat/yang/first25/test/328023827_ds4.swc' -o '/data/mat/yang/first25/test/328023827_ds4_estimated.swc'


 time /local2/teraconverter -s="319215569.v3draw" -d="/local2/IVSCC_tera/319215569" --halve=max --resolutions=01234 --width=256 --height=256 --depth=256 --sfmt="Vaa3D raw" --dfmt="TIFF (tiled, 3D)" --libtiff_rowsperstrip=-1;


 time for i in /home/yangy/work/ivscc/compare/*; do n=${i#*compare/*}; ori=${i}/${n}_ds4_resampled.swc; est=${i}/${n}_ds4_estimated.swc; out=${i}/${n}_dist.txt; time bin/vaa3d -x neuron_dist -f neuron_distance -i $ori $est >> $out; done

for i in /home/yangy/work/ivscc/compare/*; do n=${i#*compare/*}; ls ${i}/${n}_ds4_axon.swc;  done
for i in /home/yangy/work/ivscc/compare/*; do n=${i#*compare/*}; ls ${i}/${n}_ds4_dendrite.swc;  done



for i in /home/yangy/work/ivscc/compare/*; do n=${i#*compare/*}; ori=${i}/${n}_ds4_axon.swc; est=${i}/estimated_axon.swc; out=${i}/${n}_axon_dist.txt; time bin/vaa3d -x neuron_dist -f neuron_distance -i $ori $est >> $out;  done

for i in /home/yangy/work/ivscc/compare/*; do n=${i#*compare/*}; ori=${i}/${n}_ds4_dendrite.swc; est=${i}/estimated_dendrite.swc; out=${i}/${n}_dendrite_dist.txt; time bin/vaa3d -x neuron_dist -f neuron_distance -i $ori $est >> $out;  done










