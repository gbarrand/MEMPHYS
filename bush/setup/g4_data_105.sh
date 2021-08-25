
g4data_home=/usr/local/geant4/data_105

export G4ABLADATA="${g4data_home}/G4ABLA3.1"
export G4LEDATA="${g4data_home}/G4EMLOW7.7"
export G4ENSDFSTATEDATA="${g4data_home}/G4ENSDFSTATE2.2"
export G4NEUTRONHPDATA="${g4data_home}/G4NDL4.5"
export G4PARTICLEXSDATA="${g4data_home}/G4PARTICLEXS1.1"
export G4PIIDATA="${g4data_home}/G4PII1.3"
export G4SAIDXSDATA="${g4data_home}/G4SAIDDATA2.0"
export G4LEVELGAMMADATA="${g4data_home}/PhotonEvaporation5.3"
export G4RADIOACTIVEDATA="${g4data_home}/RadioactiveDecay5.3"
export G4NEUTRONXSDATA="${g4data_home}/G4NEUTRONXS1.4"
export G4REALSURFACEDATA="${g4data_home}/RealSurface2.1.1"

if [ "`uname | grep CYGWIN`" != "" ] ; then
  export G4ABLADATA="`cygpath -w ${G4ABLADATA}`"
  export G4LEDATA="`cygpath -w ${G4LEDATA}`"
  export G4ENSDFSTATEDATA="`cygpath -w ${G4ENSDFSTATEDATA}`"
  export G4NEUTRONHPDATA="`cygpath -w ${G4NEUTRONHPDATA}`"
  export G4PARTICLEXSDATA="`cygpath -w ${G4PARTICLEXSDATA}`"
  export G4PIIDATA="`cygpath -w ${G4PIIDATA}`"
  export G4SAIDXSDATA="`cygpath -w ${G4SAIDXSDATA}`"
  export G4LEVELGAMMADATA="`cygpath -w ${G4LEVELGAMMADATA}`"
  export G4RADIOACTIVEDATA="`cygpath -w ${G4RADIOACTIVEDATA}`"
  export G4NEUTRONXSDATA="`cygpath -w ${G4NEUTRONXSDATA}`"
  export G4REALSURFACEDATA="`cygpath -w ${G4REALSURFACEDATA}`"
fi

