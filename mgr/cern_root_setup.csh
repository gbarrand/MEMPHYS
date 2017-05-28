
set save_dir="`pwd`"

if ( "`uname -n | grep iridium.lunarc.lu.se`" != "" ) then
  cd /nfs/users/gbarrand/usr/local/ROOT/5.34.00.patches/bin
else  
# cd /usr/local/ROOT/5.34.00.patches/bin
cd /usr/local/ROOT/6.08.06/bin
endif

source ./thisroot.csh

cd ${save_dir}
