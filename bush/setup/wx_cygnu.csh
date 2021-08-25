
if ( "`uname | grep CYGWIN`" != "" ) then

  set wxWidgets_home=
  if ( -d /usr/include/wx-3.0 ) then
    set wxWidgets_home=/usr
  else
    echo 'wxWidgets not found.'
  endif

  if ( "${wxWidgets_home}" != "" ) then
    if ( -x ${wxWidgets_home}/bin/wx-config-3.0 ) then
      setenv PATH "${wxWidgets_home}/bin:${PATH}"
    endif
  endif

endif
