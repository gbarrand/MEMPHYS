# Copyright (C) 2010, Guy Barrand. All rights reserved.
# See the file bush.license for terms.

# to be sourced from another build script.

if ( "${exe_path}" == "" ) then

  echo 'bush/exe_path : variable exe_path not defined.'

else
  
  if ( "`printenv PATH`" == "" ) then
    setenv PATH "${exe_path}"
  else 
    setenv PATH "${exe_path}:${PATH}"
  endif
  #if ( "${build_verbose}" == "yes" ) then
  #  echo "PATH : ${PATH}"
  #endif
  
endif

