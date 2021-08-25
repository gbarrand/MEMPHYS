
set g4data_home=/usr/local/geant4/data_105

setenv G4ABLADATA        "${g4data_home}/G4ABLA3.1"
setenv G4LEDATA          "${g4data_home}/G4EMLOW7.7"
setenv G4ENSDFSTATEDATA  "${g4data_home}/G4ENSDFSTATE2.2"
setenv G4NEUTRONHPDATA   "${g4data_home}/G4NDL4.5"
setenv G4PARTICLEXSDATA  "${g4data_home}/G4PARTICLEXS1.1"
setenv G4PIIDATA         "${g4data_home}/G4PII1.3"
setenv G4SAIDXSDATA      "${g4data_home}/G4SAIDDATA2.0"
setenv G4LEVELGAMMADATA  "${g4data_home}/PhotonEvaporation5.3"
setenv G4RADIOACTIVEDATA "${g4data_home}/RadioactiveDecay5.3"
setenv G4NEUTRONXSDATA   "${g4data_home}/G4NEUTRONXS1.4"
setenv G4REALSURFACEDATA "${g4data_home}/RealSurface2.1.1"

if ( "`uname | grep CYGWIN`" != "" ) then
  setenv G4ABLADATA         "`cygpath -w ${G4ABLADATA}`"
  setenv G4LEDATA           "`cygpath -w ${G4LEDATA}`"
  setenv G4ENSDFSTATEDATA   "`cygpath -w ${G4ENSDFSTATEDATA}`"
  setenv G4NEUTRONHPDATA    "`cygpath -w ${G4NEUTRONHPDATA}`"
  setenv G4PARTICLEXSDATA   "`cygpath -w ${G4PARTICLEXSDATA}`"
  setenv G4PIIDATA          "`cygpath -w ${G4PIIDATA}`"
  setenv G4SAIDXSDATA       "`cygpath -w ${G4SAIDXSDATA}`"
  setenv G4LEVELGAMMADATA   "`cygpath -w ${G4LEVELGAMMADATA}`"
  setenv G4RADIOACTIVEDATA  "`cygpath -w ${G4RADIOACTIVEDATA}`"
  setenv G4NEUTRONXSDATA    "`cygpath -w ${G4NEUTRONXSDATA}`"
  setenv G4REALSURFACEDATA  "`cygpath -w ${G4REALSURFACEDATA}`"
endif

