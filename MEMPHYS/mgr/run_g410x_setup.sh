
if [ "`uname -n | grep iridium`" != "" ] ; then
  g4data_home=/nfs/users/gbarrand/usr/local/geant4/data_103
else
  g4data_home=/usr/local/geant4/data_103
fi

G4NEUTRONHPDATA="${g4data_home}/G4NDL4.5";export G4NEUTRONHPDATA
G4LEDATA="${g4data_home}/G4EMLOW6.50";export G4LEDATA
G4LEVELGAMMADATA="${g4data_home}/PhotonEvaporation4.3.2";export G4LEVELGAMMADATA
G4RADIOACTIVEDATA="${g4data_home}/RadioactiveDecay5.1.1";export G4RADIOACTIVEDATA
G4ENSDFSTATEDATA="${g4data_home}/G4ENSDFSTATE2.1";export G4ENSDFSTATEDATA
G4PIIDATA="${g4data_home}/G4PII1.3";export G4PIIDATA
G4NEUTRONXSDATA="${g4data_home}/G4NEUTRONXS1.4";export G4NEUTRONXSDATA
G4REALSURFACEDATA="${g4data_home}/RealSurface1.0";export G4REALSURFACEDATA
G4SAIDXSDATA="${g4data_home}/G4SAIDDATA1.1";export G4SAIDXSDATA
G4ABLADATA="${g4data_home}/G4ABLA3.0";export G4ABLADATA

if [ "`uname | grep CYGWIN`" != "" ] ; then
  G4NEUTRONHPDATA="`cygpath -w ${G4NEUTRONHPDATA}`";export G4NEUTRONHPDATA
  G4LEDATA="`cygpath -w ${G4LEDATA}`";export G4LEDATA
  G4LEVELGAMMADATA="`cygpath -w ${G4LEVELGAMMADATA}`";export G4LEVELGAMMADATA
  G4RADIOACTIVEDATA="`cygpath -w ${G4RADIOACTIVEDATA}`";export G4RADIOACTIVEDATA
  G4ENSDFSTATEDATA="`cygpath -w ${G4ENSDFSTATEDATA}`";export G4ENSDFSTATEDATA
  G4PIIDATA="`cygpath -w ${G4PIIDATA}`";export G4PIIDATA
  G4NEUTRONXSDATA="`cygpath -w ${G4NEUTRONXSDATA}`";export G4NEUTRONXSDATA
  G4REALSURFACEDATA="`cygpath -w ${G4REALSURFACEDATA}`";export G4REALSURFACEDATA
  G4SAIDXSDATA="`cygpath -w ${G4SAIDXSDATA}`";export G4SAIDXSDATA
  G4ABLADATA="`cygpath -w ${G4ABLADATA}`";export G4ABLADATA
fi

