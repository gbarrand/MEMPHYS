
#
#  Usage :
#    csh> source <osc_vis_path>/Qt-setup.csh
#

#set verbose
set Qt_home="/afs/cern.ch/sw/lcg/external/qt/4.8.4/x86_64-slc6-gcc48-opt"

if ( `uname` == "Linux" ) then
  set lib_path="${Qt_home}/lib"
  set lib_curr=`printenv LD_LIBRARY_PATH`
  if ( "${lib_curr}" == "" ) then
    setenv LD_LIBRARY_PATH "${lib_path}"
  else
    if ( `echo "${lib_curr}" | grep "${lib_path}" ` == "" ) then
      setenv LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${lib_path}"
    endif
  endif
  unset lib_curr
  unset lib_path
endif

set bin_path="${Qt_home}/bin"
set bin_curr=`printenv PATH`
if ( "${bin_curr}" == "" ) then
 setenv PATH "${bin_path}"
else
 if ( `echo "${bin_curr}" | grep "${bin_path}" ` == "" ) then
   setenv PATH "${PATH}:${bin_path}"
 endif
endif
unset bin_curr
unset bin_path

unset Qt_home
