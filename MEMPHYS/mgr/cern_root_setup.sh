
save_dir="`pwd`"

if [ "`uname -n | grep iridium.lunarc.lu.se`" != "" ] ; then
  root_home=/nfs/users/gbarrand/usr/local/ROOT/5.34.00.patches
elif [ "`uname -n | grep euronunet`" != "" ] ; then
  root_home=/home/euronunet/soft/root_genie-6.08.00
else  
  root_home=/usr/local/ROOT/5.34.00.patches
 #root_home=/usr/local/ROOT/6.08.06
fi

cd ${root_home}/bin

. ./thisroot.sh

cd ${save_dir}
