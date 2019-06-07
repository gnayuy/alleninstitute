
neuron1=$1
neuron2=$2
output=$3

echo "time ./vaa3d -x neuron_distance -f neuron_distance -i ${neuron1} ${neuron2} -o ${output}"

time ./vaa3d -x neuron_distance -f neuron_distance -i ${neuron1} ${neuron2} -o ${output}


