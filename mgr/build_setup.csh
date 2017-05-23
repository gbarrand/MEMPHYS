
if ( "`uname -n | grep iridium.lunarc.lu.se`" != "" ) then
  #module load enableATLAS
  #source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/user/atlasLocalSetup.csh 
  #localSetupROOT
  set save_dir="`pwd`"
  cd /nfs/users/gbarrand/usr/local/ROOT/5.34.00.patches/bin
  source thisroot.csh
  cd ${save_dir}
else  
  set bush_home=../../../../softinex/bush
  source ${bush_home}/setup/cern_root.csh
endif
