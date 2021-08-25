
if ( "`uname -n | grep iridium`" != "" ) then
  set g4data_home=/nfs/users/gbarrand/usr/local/geant4/data_95
else
  set g4data_home=/usr/local/geant4/data_95
endif

setenv G4NEUTRONHPDATA        "${g4data_home}/G4NDL3.12"
setenv G4LEDATA               "${g4data_home}/G4EMLOW6.41"
setenv G4LEVELGAMMADATA       "${g4data_home}/PhotonEvaporation2.0"
setenv G4RADIOACTIVEDATA      "${g4data_home}/RadioactiveDecay"

if ( "`uname | grep CYGWIN`" != "" ) then
  setenv G4NEUTRONHPDATA    "`cygpath -w ${G4NEUTRONHPDATA}`"
  setenv G4LEDATA           "`cygpath -w ${G4LEDATA}`"
  setenv G4LEVELGAMMADATA   "`cygpath -w ${G4LEVELGAMMADATA}`"
  setenv G4RADIOACTIVEDATA  "`cygpath -w ${G4RADIOACTIVEDATA}`"
endif
