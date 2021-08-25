
#
#  Usage :
#    sh> . <osc_vis_path>/py-setup.sh
#

#set -x

Python_home=/cvmfs/sft.cern.ch/lcg/releases/Python/3.7.6-b96a9/x86_64-centos7-gcc9-opt
Python_exe=/cvmfs/sft.cern.ch/lcg/releases/Python/3.7.6-b96a9/x86_64-centos7-gcc9-opt/bin/python

if [ -d "${Python_home}" ] ; then
    
if [ `uname` = "Linux" ] ; then
  lib_path="${Python_home}/lib"
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

bin_path="${Python_home}/bin"
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

else
  echo "directory ${Python_home} not found."
fi

unset Python_home

