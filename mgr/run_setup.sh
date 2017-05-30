
if [ "`uname -n | grep iridium`" != "" ] ; then
  G4DATA=/nfs/users/gbarrand/usr/local/geant4/data_82
else
  G4DATA=/usr/local/geant4/data_82
fi

export G4DATA

NeutronHPCrossSections="${G4DATA}/G4NDL";export NeutronHPCrossSections
G4LEVELGAMMADATA="${G4DATA}/PhotonEvaporation2";export G4LEVELGAMMADATA
G4RADIOACTIVEDATA="${G4DATA}/RadioactiveDecay";export G4RADIOACTIVEDATA
G4LEDATA="${G4DATA}/G4EMLOW";export G4LEDATA


