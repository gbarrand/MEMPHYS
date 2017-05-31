
if [ "`uname | grep CYGWIN`" != "" ] ; then
  wxWidgets_home=/usr/local/wxWidgets/3.1.0

  if [ -d ${wxWidgets_home}/lib/vc140_x64_dll ] ; then
#   PATH="${wxWidgets_home}/lib/vc140_x64_dll:${PATH}"
    PATH="${wxWidgets_home}/ReleaseDLL/vc140_x64_dll:${PATH}"
  fi

else

  if [ -x /usr/local/wxWidgets/3.1.0/bin/wx-config ] ; then
    wxWidgets_home=/usr/local/wxWidgets/3.1.0
    PATH="${wxWidgets_home}/bin:${PATH}"
    if [ `uname` = "Linux" ]; then
      lib_path="${wxWidgets_home}/lib"
      if [ -z "${LD_LIBRARY_PATH}" ] ; then
        LD_LIBRARY_PATH="${lib_path}"
      else
        wx_status=1;(echo $LD_LIBRARY_PATH | grep "${lib_path}" > /dev/null  ) && wx_status=0
        if [ $wx_status != 0 ] ; then
          LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${lib_path}"
        fi
        unset wx_status
      fi
      unset lib_path
    fi
  fi

fi
