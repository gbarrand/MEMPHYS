#!/bin/sh
#SBATCH -J "simple job"
#SBATCH --time=1
echo "we are on the node :"
echo "hostname : `hostname`"
echo "whoami : `whoami`"
sleep 2m
