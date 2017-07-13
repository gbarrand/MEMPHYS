
if ( "`uname -n | grep iridium`" != "" ) then
  set g4data_home=/nfs/users/gbarrand/usr/local/geant4/data_82
else
  set g4data_home=/usr/local/geant4/data_82
endif

setenv NeutronHPCrossSections "${g4data_home}/G4NDL3.12"
setenv G4LEDATA               "${g4data_home}/G4EMLOW"
setenv G4LEVELGAMMADATA       "${g4data_home}/PhotonEvaporation2.0"

#setenv G4LEDATA               /usr/local/geant4/data_103/G4EMLOW6.50             #it is ok too.
#setenv G4LEVELGAMMADATA       /usr/local/geant4/data_103/PhotonEvaporation4.3.2  #it is ok too.

#setenv G4RADIOACTIVEDATA      "${g4data_home}/RadioactiveDecay"


