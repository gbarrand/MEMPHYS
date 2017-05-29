#!/bin/sh
#SBATCH -J "simple job"
#SBATCH --time=1
echo "we are on the node :"
echo "hostname : `hostname`"
echo "whoami : `whoami`"
echo "pwd : `pwd`"
sleep 1m
