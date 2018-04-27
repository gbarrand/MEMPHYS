
set Python_home="`pwd`/../../inexlib/ourex/Python"

if ( "`uname | grep CYGWIN`" != "" ) then
  set Python_home=`cygpath -w ${Python_home}`   # not in "" because of PC-CARRIOCAS $barrand
endif

setenv PYTHONHOME "${Python_home}"

