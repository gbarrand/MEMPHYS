
if ( "`uname | grep CYGWIN`" != "" ) then
  set wxWidgets_home=/usr/local/wxWidgets/3.1.0

  if ( -d ${wxWidgets_home}/lib/vc140_x64_dll ) then
#    setenv PATH "${wxWidgets_home}/lib/vc140_x64_dll:${PATH}"
    setenv PATH "${wxWidgets_home}/ReleaseDLL/vc140_x64_dll:${PATH}"
  endif

else

  set wxWidgets_home=/usr/local/wxWidgets/3.1.0
  if ( -x ${wxWidgets_home}/bin/wx-config ) then
    setenv PATH "${wxWidgets_home}/bin:${PATH}"
  
    if ( `uname` == "Linux" ) then
      set lib_path="${wxWidgets_home}/lib"
      set lib_curr=`printenv LD_LIBRARY_PATH`
      if ( "${lib_curr}" == "" ) then
        setenv LD_LIBRARY_PATH "${lib_path}"
      else
        if ( `echo "${lib_curr}" | grep "${lib_path}" ` == "" ) then
          setenv LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${lib_path}"
        endif
      endif
      unset lib_curr
      unset lib_path
    endif

  endif

endif
