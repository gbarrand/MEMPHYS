
export BUSH_BUILD_CONFIG=LCG_97apython3_gcc9

#. /cvmfs/sft.cern.ch/lcg/views/LCG_97apython3/x86_64-centos7-gcc9-opt/setup.sh

# gcc :
. ./gcc.sh

# gtk :
export PKG_CONFIG_PATH="${PKG_CONFIG_PATH}:/usr/lib64/pkgconfig"

# Qt and Python for building are defined for this BUSH_BUILD_CONFIG in :
#   inexlib/exlib/mgr/use_Python
#   inexlib/exlib/mgr/use_Qt
# Qt and Python for running are defined in the final packing in :
#  <osc_path>/<version>/[Qt,py]-setup.[csh.sh]

# They should point to :
#  /cvmfs/sft.cern.ch/lcg/releases/qt5/5.12.4-d91b2/x86_64-centos7-gcc9-opt
#  /cvmfs/sft.cern.ch/lcg/releases/Python/3.7.6-b96a9/x86_64-centos7-gcc9-opt

. ./Qt-setup.sh
. ./py-setup.sh

#mesa_home=/cvmfs/sft.cern.ch/lcg/contrib/mesa/18.0.5/x86_64-centos7
#export LIBGL_DRIVERS_PATH="${mesa_home}/lib64/dri"
#export LD_LIBRARY_PATH="${mesa_home}/lib64:${mesa_home}/lib64/dri:${LD_LIBRARY_PATH}"

#export COIN_DEBUG_GLGLUE=1
#export COIN_PREFER_GLPOLYGONOFFSET_EXT=1
#export COIN_FULL_INDIRECT_RENDERING=1
#export COIN_FORCE_GL1_0_ONLY=1
 
