
set save_dir="`pwd`"

if ( "`uname -n | grep iridium.lunarc.lu.se`" != "" ) then
  set root_home=/nfs/users/gbarrand/usr/local/ROOT/5.34.00.patches
else  
  set root_home=/usr/local/ROOT/5.34.00.patches
 #set root_home=/usr/local/ROOT/6.08.06
endif

cd ${root_home}

source ./thisroot.csh

cd ${save_dir}
