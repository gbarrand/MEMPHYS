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

G4DATA=/nfs/users/gbarrand/usr/local/geant4/data_82
NeutronHPCrossSections="${G4DATA}/G4NDL";export NeutronHPCrossSections
G4LEVELGAMMADATA="${G4DATA}/PhotonEvaporation2";export G4LEVELGAMMADATA
G4RADIOACTIVEDATA="${G4DATA}/RadioactiveDecay";export G4RADIOACTIVEDATA
G4LEDATA="${G4DATA}/G4EMLOW";export G4LEDATA

cd ../data
../mgr/bin_gnu/MEMPHYS_sim 

sleep 1m
