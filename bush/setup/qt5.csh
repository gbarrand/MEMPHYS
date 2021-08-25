
if ( "`uname -n`" == barrand-centos7.lal.in2p3.fr || "`uname -n`" == barrand-centos7 ) then
  setenv QT_XKB_CONFIG_ROOT /usr/share/X11/xkb
endif

if ( "`uname | grep CYGWIN`" != "" ) then
 #set Qt_home=/cygdrive/c/Qt/5.12.3/msvc2017_64
  set Qt_home=/cygdrive/c/Qt/5.15.2/msvc2019_64
  setenv PATH "${Qt_home}/bin:${PATH}"
endif

#set qt_home=/cvmfs/lhcb.cern.ch/lib/lcg/releases/qt5/5.12.1-d6e2e/x86_64-centos7-gcc8-opt
#setenv LD_LIBRARY_PATH "${qt_home}/lib:${LD_LIBRARY_PATH}"

#setenv QT_PLUGIN_PATH "${qt_home}/plugins"
#setenv QT_GRAPHICSSYSTEM_CHECKED 1
#setenv QT_GRAPHICSSYSTEM native

#setenv QTDIR "${qt_home}"
#setenv QTINC "${qt_home}/include"
#setenv QTLIB "${qt_home}/lib"
