
if [ "`uname | grep CYGWIN`" != "" ] ; then
  wxWidgets_home=/usr/local/wxWidgets/3.1.1

  if [ -d ${wxWidgets_home}/lib/vc140_x64_dll ] ; then
#   PATH="${wxWidgets_home}/lib/vc140_x64_dll:${PATH}"
    PATH="${wxWidgets_home}/ReleaseDLL/vc140_x64_dll:${PATH}"
  fi

else

  if [ "`uname -n`" = deco.lal.in2p3.fr ] ; then
    wxWidgets_home=/exp/si/barrand/usr/local/wxWidgets/3.1.1
  elif [ -d /usr/local/wxWidgets/3.1.1 ] ; then
    wxWidgets_home=/usr/local/wxWidgets/3.1.1
  else
    echo 'wxWidgets not found.'
  fi

  if [ "${wxWidgets_home}" != "" ] ; then
  if [ -x ${wxWidgets_home}/bin/wx-config ] ; then
    PATH="${wxWidgets_home}/bin:${PATH}"
    if [ `uname` = "Linux" ] ; then
      lib_path="${wxWidgets_home}/lib"
      if [ -z "${LD_LIBRARY_PATH}" ] ; then
        LD_LIBRARY_PATH="${lib_path}"
        export LD_LIBRARY_PATH
      else
        wx_status=1;(echo $LD_LIBRARY_PATH | grep "${lib_path}" > /dev/null  ) && wx_status=0
        if [ $wx_status != 0 ] ; then
          LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${lib_path}"
          export LD_LIBRARY_PATH
        fi
        unset wx_status
      fi
      unset lib_path
    fi
  fi
  fi

fi
