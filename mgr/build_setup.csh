
if ( "`uname -n | grep iridium.lunarc.lu.se`" != "" ) then
  module load enableATLAS
  setupATLAS
  localSetupROOT
else  
  set bush_home=../../../../softinex/bush
  source ${bush_home}/setup/cern_root.csh
endif
