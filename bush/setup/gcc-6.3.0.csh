
if ( "`uname -n`" != barrand-centos7.lal.in2p3.fr && "`uname -n`" != barrand-centos7 ) then
  echo 'not on barrand-centos7.'
else
  if ( ! -d /usr/local/gcc/6.3.0//bin ) then
    echo '/usr/local/gcc/6.3.0//bin not found.'
  else
    setenv PATH "/usr/local/gcc/6.3.0/bin:$PATH"
    set lib_paths="/usr/local/mpc/1.1.0/lib"
    set lib_paths="${lib_paths} /usr/local/isl/0.18/lib"
    set lib_paths="${lib_paths} /usr/local/gcc/6.3.0/lib64"
    foreach item ( ${lib_paths} )
      #echo "==> ${item}"
      set lib_curr=`printenv LD_LIBRARY_PATH`
      if ( "${lib_curr}" == "" ) then
        setenv LD_LIBRARY_PATH "${item}"
      else
        if ( "`echo ${lib_curr} | grep ${item}`" == "" ) then
          setenv LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${item}"
        endif
      endif
      unset lib_curr
    end
    unset lib_paths
  endif
endif
