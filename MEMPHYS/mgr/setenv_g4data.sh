
if [ "`uname -n | grep iridium`" != "" ] ; then
  g4data_home=/nfs/users/gbarrand/usr/local/geant4/data_103
else
  g4data_home=/usr/local/geant4/data_103
fi

if [ ! -d ${g4data_home} ] ; then
  echo "setenv_g4data_103.sh : directory ${g4data_home} not found."
else

  G4NEUTRONHPDATA="${g4data_home}/G4NDL4.5";export G4NEUTRONHPDATA
  G4LEDATA="${g4data_home}/G4EMLOW6.50";export G4LEDATA
  G4LEVELGAMMADATA="${g4data_home}/PhotonEvaporation4.3.2";export G4LEVELGAMMADATA
  G4ENSDFSTATEDATA="${g4data_home}/G4ENSDFSTATE2.1";export G4ENSDFSTATEDATA
  G4SAIDXSDATA="${g4data_home}/G4SAIDDATA1.1";export G4SAIDXSDATA

  if [ "`uname | grep CYGWIN`" != "" ] ; then
    G4NEUTRONHPDATA="`cygpath -w ${G4NEUTRONHPDATA}`";export G4NEUTRONHPDATA
    G4LEDATA="`cygpath -w ${G4LEDATA}`";export G4LEDATA
    G4LEVELGAMMADATA="`cygpath -w ${G4LEVELGAMMADATA}`";export G4LEVELGAMMADATA
    G4ENSDFSTATEDATA="`cygpath -w ${G4ENSDFSTATEDATA}`";export G4ENSDFSTATEDATA
    G4SAIDXSDATA="`cygpath -w ${G4SAIDXSDATA}`";export G4SAIDXSDATA
  fi    

fi    

