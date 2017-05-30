#!/bin/sh
#SBATCH -J "simple job"
#SBATCH --time=1
#SBATCH --nodes=3
#SBATCH --array=0,1,2
#SBATCH --ntasks-per-node=1
echo "we are on the node :"
echo "hostname : `hostname`"
echo "whoami : `whoami`"
echo "pwd : `pwd`"

. ../run_setup.sh

# build output root file name :
root_file="MEMPHYS_${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.root"

../bin/MEMPHYS_sim ${root_file}

# -pid_root : it will create an output file name of the form MEMPHYS_<host>_<pid>.root.
#../mgr/bin_gnu/MEMPHYS_sim -pid_root

sleep 1m
