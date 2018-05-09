
DIR=$1

for i in ${DIR}/*.swc;
do

time ~/work/tools/v3d_external/bin/vaa3d -x IVSCC_scal -f scale -i ${i} -o ${i%*.swc}"_z0.swc" -p 1.0 1.0 0.0 1.0

j=${i%*.swc}"_ds1.swc"

time ~/work/tools/v3d_external/bin/vaa3d -x IVSCC_scal -f scale -i ${i} -o ${j} -p 0.5 0.5 0.5 0.5
time ~/work/tools/v3d_external/bin/vaa3d -x IVSCC_scal -f scale -i ${j} -o ${j%*.swc}"_z0.swc" -p 1.0 1.0 0.0 1.0

i=$j
j=${j%*_*}"_ds2.swc"

time ~/work/tools/v3d_external/bin/vaa3d -x IVSCC_scal -f scale -i ${i} -o ${j} -p 0.5 0.5 0.5 0.5
time ~/work/tools/v3d_external/bin/vaa3d -x IVSCC_scal -f scale -i ${j} -o ${j%*.swc}"_z0.swc" -p 1.0 1.0 0.0 1.0

i=$j
j=${j%*_*}"_ds3.swc"

time ~/work/tools/v3d_external/bin/vaa3d -x IVSCC_scal -f scale -i ${i} -o ${j} -p 0.5 0.5 0.5 0.5
time ~/work/tools/v3d_external/bin/vaa3d -x IVSCC_scal -f scale -i ${j} -o ${j%*.swc}"_z0.swc" -p 1.0 1.0 0.0 1.0

i=$j
j=${j%*_*}"_ds4.swc"

time ~/work/tools/v3d_external/bin/vaa3d -x IVSCC_scal -f scale -i ${i} -o ${j} -p 0.5 0.5 0.5 0.5
time ~/work/tools/v3d_external/bin/vaa3d -x IVSCC_scal -f scale -i ${j} -o ${j%*.swc}"_z0.swc" -p 1.0 1.0 0.0 1.0

i=$j
j=${j%*_*}"_ds5.swc"

time ~/work/tools/v3d_external/bin/vaa3d -x IVSCC_scal -f scale -i ${i} -o ${j} -p 0.5 0.5 0.5 0.5
time ~/work/tools/v3d_external/bin/vaa3d -x IVSCC_scal -f scale -i ${j} -o ${j%*.swc}"_z0.swc" -p 1.0 1.0 0.0 1.0

done 




 
