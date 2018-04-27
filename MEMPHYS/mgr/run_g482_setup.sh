
if [ "`uname -n | grep iridium`" != "" ] ; then
  g4data_home=/nfs/users/gbarrand/usr/local/geant4/data_82
else
  g4data_home=/usr/local/geant4/data_82
fi

NeutronHPCrossSections="${g4data_home}/G4NDL";export NeutronHPCrossSections
G4LEVELGAMMADATA="${g4data_home}/PhotonEvaporation2.0";export G4LEVELGAMMADATA
G4RADIOACTIVEDATA="${g4data_home}/RadioactiveDecay";export G4RADIOACTIVEDATA
G4LEDATA="${g4data_home}/G4EMLOW";export G4LEDATA

