
if [ "`uname -n`" != barrand-centos7.lal.in2p3.fr -a "`uname -n`" != barrand-centos7 ] ; then
  echo 'not on barrand-centos7.'
else
  if [ ! -d /usr/local/gcc/6.3.0//bin ] ; then
    echo '/usr/local/gcc/6.3.0//bin not found.'
  else
    export PATH="/usr/local/gcc/6.3.0/bin:$PATH"
    lib_paths="/usr/local/mpc/1.1.0/lib"
    lib_paths="${lib_paths} /usr/local/isl/0.18/lib"
    lib_paths="${lib_paths} /usr/local/gcc/6.3.0/lib64"
    for item in ${lib_paths} ; do
      #echo "==> ${item}"
      lib_curr=`printenv LD_LIBRARY_PATH`
      if [ "${lib_curr}" = "" ] ; then
        export LD_LIBRARY_PATH="${item}"
      else
        if [ "`echo ${lib_curr} | grep ${item}`" = "" ] ; then
          export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${item}"
        fi
      fi
      unset lib_curr
    done
    unset lib_paths
  fi
fi
