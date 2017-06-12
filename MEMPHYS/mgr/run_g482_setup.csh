
if ( "`uname -n | grep iridium`" != "" ) then
  set g4data_home=/nfs/users/gbarrand/usr/local/geant4/data_82
else
  set g4data_home=/usr/local/geant4/data_82
endif

setenv NeutronHPCrossSections "${g4data_home}/G4NDL"
setenv G4LEVELGAMMADATA       "${g4data_home}/PhotonEvaporation2.0"
setenv G4RADIOACTIVEDATA      "${g4data_home}/RadioactiveDecay"
setenv G4LEDATA               "${g4data_home}/G4EMLOW"


