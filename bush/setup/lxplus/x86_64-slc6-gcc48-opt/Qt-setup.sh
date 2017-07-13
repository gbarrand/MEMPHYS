
#
#  Usage :
#    sh> . <osc_vis_path>/Qt-setup.sh
#

#set -x

Qt_home="/afs/cern.ch/sw/lcg/external/qt/4.8.4/x86_64-slc6-gcc48-opt"

if [ `uname` = "Linux" ]; then
  lib_path="${Qt_home}/lib"
  if [ -z "${LD_LIBRARY_PATH}" ] ; then
    LD_LIBRARY_PATH="${lib_path}"
    export LD_LIBRARY_PATH
  else
    osc_status=1;(echo $LD_LIBRARY_PATH | grep "${lib_path}" > /dev/null  ) && osc_status=0
    if [ $osc_status != 0 ] ; then
      LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${lib_path}"
      export LD_LIBRARY_PATH
    fi
    unset osc_status
  fi
  unset lib_path
fi

bin_path="${Qt_home}/bin"
if [ -z "${PATH}" ] ; then
  PATH="${bin_path}"
  export PATH
else
  osc_status=1;(echo $PATH | grep "${bin_path}" > /dev/null  ) && osc_status=0
  if [ $osc_status != 0 ] ; then
    PATH="${PATH}:${bin_path}"
    export PATH
  fi
  unset osc_status
fi
unset bin_path

unset Qt_home

