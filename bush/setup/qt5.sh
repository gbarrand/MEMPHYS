
if [ "`uname -n`" = barrand-centos7.lal.in2p3.fr -o "`uname -n`" = barrand-centos7 ] ; then
  export QT_XKB_CONFIG_ROOT=/usr/share/X11/xkb
fi

if [ "`uname | grep CYGWIN`" != "" ] ; then
 #Qt_home=/cygdrive/c/Qt/5.12.3/msvc2017_64
  Qt_home=/cygdrive/c/Qt/5.15.2/msvc2019_64
  export PATH="${Qt_home}/bin:${PATH}"
fi

#qt_home=/cvmfs/lhcb.cern.ch/lib/lcg/releases/qt5/5.12.1-d6e2e/x86_64-centos7-gcc8-opt
#export LD_LIBRARY_PATH="${qt_home}/lib:${LD_LIBRARY_PATH}"

#export QT_PLUGIN_PATH="${qt_home}/plugins"
#export QT_GRAPHICSSYSTEM_CHECKED=1
#export QT_GRAPHICSSYSTEM=native

#export QTDIR="${qt_home}"
#export QTINC="${qt_home}/include"
#export QTLIB="${qt_home}/lib"
