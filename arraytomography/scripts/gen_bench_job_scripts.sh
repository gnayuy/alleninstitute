#!/bin/bash
#
#This is a shell program to batch generate probabilities of foreground
#

function write_ilastik_job_config {

  inputfn=$1;
  outputScript=$2;

  echo "# /bin/sh" >> $outputScript;
  echo "## Check which queue you may use" >> $outputScript;
#  echo "#PBS -q dque" >> $outputScript;
  echo "#PBS -q celltypes" >> $outputScript;
  echo "# Declare that your job will use no more than some amount of memory _at_peak_" >> $outputScript;
  echo "#PBS -l mem=64g" >> $outputScript;
  echo "# Allow up to 10min of walltime.  Default is 12 hours" >> $outputScript;
  echo "#PBS -l walltime=1:00:00" >> $outputScript;
  echo "# Request just one core on the host" >> $outputScript;
  echo "#PBS -l ncpus=16" >> $outputScript;
  echo "# Give your job a descriptive name. This is visible in qstat and other job reports.  Also serves as the default basename for log files" >> $outputScript;
  echo "#PBS -N ${outputScript}" >> $outputScript;
  echo "# Should torque automatically re-run the job on error?" >> $outputScript;
  echo "#PBS -r n" >> $outputScript;
  echo "# Merge STDOUT into STDERR" >> $outputScript;
  echo "#PBS -j oe" >> $outputScript;
  echo "# location for stderr/stdout log files _after_ job completion" >> $outputScript;
  echo "#PBS -o ${outputScript}.out" >> $outputScript;

  echo "#" >> $outputScript;
  echo "#" >> $outputScript;

#  echo "# send email on job error" >> $outputScript;
#  echo "#PBS -m a" >> $outputScript;

  echo "export DISPLAY=:\$RANDOM" >> $outputScript;
  echo "Xvfb \$DISPLAY -auth /dev/null &" >> $outputScript;
  echo "export PYTHONHOME=/usr" >> $outputScript; 
  echo "export PYTHONPATH=/usr/lib64/python2.7/site-package" >> $outputScript;
  #echo "cd /data/mat/yang/cluster_test/" >> $outputScript;
  echo "time /data/mat/yang/cluster_test/ilastik-1.3.0b3-Linux/run_ilastik.sh --headless --project=/data/mat/yang/cluster_test/ivscc.ilp $inputfn" >> $outputScript;

}

#copy the names
image=$1;
jobScriptFile=$2;

#generate the batch script configuration
if [ -f $jobScriptFile ]; then
  rm $jobScriptFile;
fi;

write_ilastik_job_config $image $jobScriptFile

# precisely kill the job

echo "kill %1" >> $jobScriptFile;

# 
# for i in *.qsub; do [ -f ../475905865_tiffs/${i%.v3draw*}.h5 ] && echo "$i exist" || qsub $i; done
