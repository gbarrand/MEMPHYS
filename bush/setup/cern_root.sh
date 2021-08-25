
save_dir=`pwd`

if [ "`uname -n | grep barrand-centos7`" != "" ] ; then
  LC_ALL="en_US.UTF-8";export LC_ALL  # for manpath used in the ROOT setup files.
 #cd /usr/local/ROOT/6.12.06/root/bin
  cd /usr/local/ROOT/6.24.00/root/bin
  
elif [ "`uname -n | grep barrand-ubuntu`" != "" ] ; then
  cd /usr/local/ROOT/6.12.06/root/bin

elif [ "`uname -n | grep guys-macbook-pro-2.home`" != "" ] ; then  # MacBookPro-2014 at home.
  cd /usr/local/ROOT/6.12.06/root/bin

else
 #cd /usr/local/ROOT/5.34.18/bin
 #Darwin : the 5.34.18 does not permit to pass exlib/exlib/cern_root.
 #cd /usr/local/ROOT/5.34.00.patches/bin
 #cd /usr/local/ROOT/6.08.06/bin
 #cd /usr/local/ROOT/6.12.06/root/bin
  cd /usr/local/ROOT/6.20.04/root/bin
fi

. ./thisroot.sh

cd ${save_dir}
