
if ( "`uname | grep CYGWIN`" != "" ) then
  set wxWidgets_home=/usr/local/wxWidgets/3.1.0

  if ( -d ${wxWidgets_home}/lib/vc140_x64_dll ) then
#    setenv PATH "${wxWidgets_home}/lib/vc140_x64_dll:${PATH}"
    setenv PATH "${wxWidgets_home}/ReleaseDLL/vc140_x64_dll:${PATH}"
  endif

else

  set wxWidgets_home=
  if ( "`uname`" == Darwin ) then
    # sudo port install wxWidgets-3.2    
    set wxWidgets_home=/opt/local/Library/Frameworks/wxWidgets.framework/Versions/wxWidgets/3.1
  else if ( "`uname -n`" == deco.lal.in2p3.fr ) then
    set wxWidgets_home=/exp/si/barrand/usr/local/wxWidgets/3.1.0
  else if ( -d /usr/local/wxWidgets/3.1.3 ) then
    set wxWidgets_home=/usr/local/wxWidgets/3.1.3
  else if ( -d /usr/local/wxWidgets/3.1.1 ) then
    set wxWidgets_home=/usr/local/wxWidgets/3.1.1
  else if ( -d /usr/local/wxWidgets/3.1.0 ) then
    set wxWidgets_home=/usr/local/wxWidgets/3.1.0
  else
    echo 'wxWidgets not found.'
  endif

  if ( "${wxWidgets_home}" != "" ) then
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

endif
