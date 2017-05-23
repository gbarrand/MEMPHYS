
if ( "`uname -n | grep iridium.lunarc.lu.se`" != "" ) then
  module load enableATLAS
  source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/user/atlasLocalSetup.csh 
  localSetupROOT
else  
  set bush_home=../../../../softinex/bush
  source ${bush_home}/setup/cern_root.csh
endif
