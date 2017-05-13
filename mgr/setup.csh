
setenv G4DATA /usr/local/geant4/data_82
setenv NeutronHPCrossSections ${G4DATA}/G4NDL
setenv G4LEVELGAMMADATA ${G4DATA}/PhotonEvaporation
setenv G4RADIOACTIVEDATA ${G4DATA}/RadioactiveDecay
setenv G4LEDATA ${G4DATA}/G4EMLOW

if ( "`uname -n | grep iridium.lunarc.lu.se`" != "" ) then
  set osc_home=/nfs/users/gbarrand/usr/local
else
  set osc_home=/usr/local
endif

set library_path="${osc_home}/osc_batch/17.1/bin:${osc_home}/osc_batch/17.1/lib"

if ( `uname` == "Darwin" ) then
  echo dummy > /dev/null
  #setenv DYLD_LIBRARY_PATH "${library_path}"
else
  setenv LD_LIBRARY_PATH "${library_path}"
endif

#setenv MEMPHYSROOT /Users/barrand/private/dev/exp/MEMPHYS/v8r0

