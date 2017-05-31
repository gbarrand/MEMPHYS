
set geant4_home=/usr/local/geant4/geant4-09-05-ref-00
# for geant4-config :
setenv PATH "${PATH}:${geant4_home}/bin"

setenv G4DATA /usr/local/geant4/data
#setenv NeutronHPCrossSections $G4DATA/G4NDL3.14
#setenv G4LEVELGAMMADATA $G4DATA/PhotonEvaporation2.0
#setenv G4RADIOACTIVEDATA $G4DATA/RadioactiveDecay3.2

# needed for A01 :
setenv G4LEDATA $G4DATA/G4EMLOW6.23

