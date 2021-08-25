
on_my_openstack_centos7=no
if [ "`uname -n`" = barrand-centos7.lal.in2p3.fr -o "`uname -n`" = barrand-centos7 ] ; then
  on_my_openstack_centos7=yes
fi

if [ ${on_my_openstack_centos7} = yes ] ; then
  lib_path="/usr/lib64/openmpi/lib"
  if [ -z "${LD_LIBRARY_PATH}" ] ; then
    LD_LIBRARY_PATH="${lib_path}"
    export LD_LIBRARY_PATH
  else
    ld_status=1;(echo $LD_LIBRARY_PATH | grep "${lib_path}" > /dev/null  ) && ld_status=0
    if [ $ld_status != 0 ] ; then
      LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${lib_path}"
      export LD_LIBRARY_PATH
    fi
    unset ld_status
  fi
  unset lib_path
fi

