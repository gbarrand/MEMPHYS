# Copyright (C) 2010, Guy Barrand. All rights reserved.
# See the file bush.license for terms.

# to be sourced from another build script.

if ( "${ld_path}" == "" ) then

  echo 'bush/ld_library_path : variable ld_path not defined.'

else
  
  if ( "`uname | grep CYGWIN`" != "" ) then
    if ( "`printenv PATH`" == "" ) then
      setenv PATH "${ld_path}"
    else 
      setenv PATH "${PATH}:${ld_path}"
    endif

    #if ( "${build_verbose}" == "yes" ) then
    #  echo "PATH : ${PATH}"
    #endif
    
  else if ( `uname` == "Darwin" ) then
    if ( "`printenv DYLD_LIBRARY_PATH`" == "" ) then
      setenv DYLD_LIBRARY_PATH "${ld_path}"
    else 
      setenv DYLD_LIBRARY_PATH "${DYLD_LIBRARY_PATH}:${ld_path}"
    endif
    #if ( "${build_verbose}" == "yes" ) then
    #  echo "DYLD_LIBRARY_PATH : ${DYLD_LIBRARY_PATH}"
    #endif

  else
    if ( "`printenv LD_LIBRARY_PATH`" == "" ) then
      setenv LD_LIBRARY_PATH "${ld_path}"
    else 
      setenv LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${ld_path}"
    endif
    #if ( "${build_verbose}" == "yes" ) then
    #  echo "LD_LIBRARY_PATH : ${LD_LIBRARY_PATH}"
    #endif
  endif
  
endif

