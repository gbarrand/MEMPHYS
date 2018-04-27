
set save_dir="`pwd`"

if ( "`uname -n | grep iridium.lunarc.lu.se`" != "" ) then
  set root_home=/nfs/users/gbarrand/usr/local/ROOT/5.34.00.patches
else if ( "`uname -n | grep euronunet`" != "" ) then
  set root_home=/home/euronunet/soft/root_genie-6.08.00
else  
  set root_home=/usr/local/ROOT/6.08.06
  if ( `uname` == Darwin ) then
    set on_my_mac="`networksetup -listallhardwareports | grep '78:31'`"
    if ( "${on_my_mac}" != "" ) then
     #set root_home=/usr/local/ROOT/5.34.00.patches
      set root_home=/usr/local/ROOT/6.12.06/root
    endif
  endif
endif

cd ${root_home}/bin

source ./thisroot.csh

cd ${save_dir}
