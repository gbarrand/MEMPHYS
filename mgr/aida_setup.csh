
if ( "`uname -n | grep iridium.lunarc.lu.se`" != "" ) then
  set osc_home=/nfs/users/gbarrand/usr/local
else
  set osc_home=/usr/local
endif

source ${osc_home}/osc_batch/17.1/aida-setup.csh
