
if ( "`uname -n | grep iridium`" != "" ) then
  set g4data_home=/nfs/users/gbarrand/usr/local/geant4/data_103
else
  set g4data_home=/usr/local/geant4/data_103
  #set g4data_home=/usr/local/geant4/10.03.p01/share/Geant4-10.3.1/data
endif

setenv G4NEUTRONHPDATA   "${g4data_home}/G4NDL4.5"
setenv G4LEDATA          "${g4data_home}/G4EMLOW6.50"
setenv G4LEVELGAMMADATA  "${g4data_home}/PhotonEvaporation4.3.2"
setenv G4RADIOACTIVEDATA "${g4data_home}/RadioactiveDecay5.1.1"
setenv G4ENSDFSTATEDATA  "${g4data_home}/G4ENSDFSTATE2.1"
setenv G4PIIDATA         "${g4data_home}/G4PII1.3"
setenv G4NEUTRONXSDATA   "${g4data_home}/G4NEUTRONXS1.4"
setenv G4REALSURFACEDATA "${g4data_home}/RealSurface1.0"
setenv G4SAIDXSDATA      "${g4data_home}/G4SAIDDATA1.1"
setenv G4ABLADATA        "${g4data_home}/G4ABLA3.0"

