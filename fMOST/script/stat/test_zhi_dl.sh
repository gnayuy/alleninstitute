


#for i in /home/yangy/Documents/all_soma_markers_corrected_name/*.marker

while read markerfile
do

echo $i;

timeout 5m ./vaa3d -x ultratracer -f trace_APP2_GD -i '/home/yangy/fmost-data/fMOST_terafly/mouseID_321237-17302/RES(27300x17206x4923)' -p ${markerfile} 512 1 1 AUTO

done < restmarkers.txt

