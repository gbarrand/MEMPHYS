
if ( "`uname -n | grep iridium.lunarc.lu.se`" != "" ) then
  setenv G4DATA /nfs/users/gbarrand/usr/local/geant4/data_82
else
  setenv G4DATA /usr/local/geant4/data_82
endif

setenv NeutronHPCrossSections ${G4DATA}/G4NDL
setenv G4LEVELGAMMADATA ${G4DATA}/PhotonEvaporation2
setenv G4RADIOACTIVEDATA ${G4DATA}/RadioactiveDecay
setenv G4LEDATA ${G4DATA}/G4EMLOW

