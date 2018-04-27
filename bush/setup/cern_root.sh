
save_dir=`pwd`

if [ "`uname -n`" = "barrand-ubuntu" ] ; then # on_my_openstack_ubuntu
 #cd /usr/local/ROOT/6.08.06/bin
  cd /usr/local/ROOT/6.12.06/root/bin
else    
 #cd /usr/local/ROOT/5.34.18/bin
 #Darwin : the 5.34.18 does not permit to pass exlib/exlib/cern_root.
 #cd /usr/local/ROOT/5.34.00.patches/bin
 #cd /usr/local/ROOT/6.08.06/bin
  cd /usr/local/ROOT/6.12.06/root/bin
fi

. ./thisroot.sh

cd ${save_dir}
