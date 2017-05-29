#!/bin/sh
#SBATCH -J "simple job"
#SBATCH --time=1
echo "we are on the node"
hostname
who
sleep 2m
