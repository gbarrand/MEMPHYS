
set cling_home=
if ( "`uname`" == "Darwin" ) then
  set cling_home=/usr/local/cling/0.6_dev/inst
else if ( "`uname`" == "Linux" ) then
  if ( "`uname -n`" == deco.lal.in2p3.fr ) then
    set cling_home=/exp/si/barrand/usr/local/cling/0.6_dev/x86_64-centos7-gcc48-opt/inst
  else
    set cling_home=/usr/local/cling/0.6_dev/inst
  endif      
endif

if ( ! -d "${cling_home}" ) then 
  echo "bush/setup/cling.csh : cling not found."
else  
  set ld_path="${cling_home}/lib"
  source ../ld_library_path.csh
  set exe_path="${cling_home}/bin"
  source ../exe_path.csh
endif
