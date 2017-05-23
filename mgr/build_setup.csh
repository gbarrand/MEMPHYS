
if ( "`uname -n | grep iridium.lunarc.lu.se`" != "" ) then
  #module load enableATLAS
  #source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/user/atlasLocalSetup.csh 
  #localSetupROOT
  source /nfs/users/gbarrand/usr/local/ROOT/5.34.00.patches/bin/thisroot.csh
else  
  set bush_home=../../../../softinex/bush
  source ${bush_home}/setup/cern_root.csh
endif
