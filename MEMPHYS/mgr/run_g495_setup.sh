
if [ "`uname -n | grep iridium`" != "" ] ; then
  g4data_home=/nfs/users/gbarrand/usr/local/geant4/data_95
else
  g4data_home=/usr/local/geant4/data_95
fi

G4NEUTRONHPDATA="${g4data_home}/G4NDL";export G4NEUTRONHPDATA
G4LEVELGAMMADATA="${g4data_home}/PhotonEvaporation2.0";export G4LEVELGAMMADATA
G4RADIOACTIVEDATA="${g4data_home}/RadioactiveDecay";export G4RADIOACTIVEDATA
G4LEDATA="${g4data_home}/G4EMLOW6.41";export G4LEDATA

if [ "`uname | grep CYGWIN`" != "" ] ; then
  G4LEVELGAMMADATA="`cygpath -w ${G4LEVELGAMMADATA}`";export G4LEVELGAMMADATA
  G4RADIOACTIVEDATA="`cygpath -w ${G4RADIOACTIVEDATA}`";export G4RADIOACTIVEDATA
  G4LEDATA="`cygpath -w ${G4LEDATA}`";export G4LEDATA
  G4NEUTRONHPDATA="`cygpath -w ${G4NEUTRONHPDATA}`";export G4NEUTRONHPDATA
fi
