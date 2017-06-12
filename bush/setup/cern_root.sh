
save_dir=`pwd`

if [ "`uname -n`" = "barrand-ubuntu" ] ; then # on_my_openstack_ubuntu
  cd /usr/local/ROOT/6.08.06/bin
else    
  cd /usr/local/ROOT/5.34.18/bin
  #cd /usr/local/ROOT/6.00.01/bin
  #Loading of inlib, exlib plugins fails.
  #cd /usr/local/ROOT/5.34.09/bin
fi

. ./thisroot.sh

cd ${save_dir}
