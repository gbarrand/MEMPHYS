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
sleep 1m
