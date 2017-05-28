
if ( "`uname -n | grep iridium.lunarc.lu.se`" != "" ) then
  set osc_home=/nfs/users/gbarrand/usr/local
else
  set osc_home=/usr/local
endif
set library_path="${osc_home}/osc_batch/17.1/bin:${osc_home}/osc_batch/17.1/lib"
if ( `uname` == "Darwin" ) then
  setenv DYLD_LIBRARY_PATH "${library_path}"
else
  setenv LD_LIBRARY_PATH "${library_path}"
endif
