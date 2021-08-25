
if ( "`uname -n | grep iridium`" != "" ) then
  set g4data_home=/nfs/users/gbarrand/usr/local/geant4/data_103
else
  set g4data_home=/usr/local/geant4/data_103
  #set g4data_home=/usr/local/geant4/10.03.p01/share/Geant4-10.3.1/data
endif

setenv G4NEUTRONHPDATA   "${g4data_home}/G4NDL4.5"
setenv G4LEDATA          "${g4data_home}/G4EMLOW6.50"
setenv G4LEVELGAMMADATA  "${g4data_home}/PhotonEvaporation4.3.2"
setenv G4ENSDFSTATEDATA  "${g4data_home}/G4ENSDFSTATE2.1"
setenv G4SAIDXSDATA      "${g4data_home}/G4SAIDDATA1.1"

if ( "`uname | grep CYGWIN`" != "" ) then
  setenv G4NEUTRONHPDATA    "`cygpath -w ${G4NEUTRONHPDATA}`"
  setenv G4LEDATA           "`cygpath -w ${G4LEDATA}`"
  setenv G4LEVELGAMMADATA   "`cygpath -w ${G4LEVELGAMMADATA}`"
  setenv G4ENSDFSTATEDATA   "`cygpath -w ${G4ENSDFSTATEDATA}`"
  setenv G4SAIDXSDATA       "`cygpath -w ${G4SAIDXSDATA}`"
endif

