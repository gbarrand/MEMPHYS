#
#  Usage :
#   csh flavoured shell>  source setup.csh
#

#---------------------------------------------------
  # CLHEP :
  setenv CLHEP_BASE_DIR /nfs/users/gbarrand/user/local/CLHEP/2.0.4.2
  if ( "`printenv LD_LIBRARY_PATH`" == "" ) then
    setenv LD_LIBRARY_PATH "${CLHEP_BASE_DIR}/lib"
  else
    setenv LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${CLHEP_BASE_DIR}/lib"
  endif
  # core :
  setenv CPPVERBOSE 1
  setenv G4INSTALL /nfs/users/gbarrand/usr/local/geant4/8.2
  setenv G4LIB ${G4INSTALL}/lib
  setenv G4SYSTEM  Linux-g++
  #setenv G4DEBUG   1
  # Shareable library
  #setenv LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:$G4LIB/$G4SYSTEM"
  #setenv G4LIB_BUILD_SHARED 1
  # to find liblist :
  #setenv PATH "${PATH}:$G4LIB/$G4SYSTEM"
  #
  setenv G4TMP .
  setenv G4BIN ${G4TMP}/bin
  #
  # UI and VIS use flags :
  setenv G4UI_NONE 1
  setenv G4VIS_NONE 1
  # data :
  setenv G4DATA /nfs/users/gbarrand/usr/local/geant4/data
  setenv NeutronHPCrossSections $G4DATA/G4NDL
  setenv G4LEVELGAMMADATA $G4DATA/PhotonEvaporation
  setenv G4RADIOACTIVEDATA $G4DATA/RadioactiveDecay
  setenv G4LEDATA $G4DATA/G4EMLOW
