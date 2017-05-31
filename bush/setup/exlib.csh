
#source /usr/local/geant4/geant4-09-03/setup.csh

set geant4_home=/usr/local/geant4/geant4-09-05-ref-00
# for geant4-config :
setenv PATH "${PATH}:${geant4_home}/bin"

source /usr/local/osc_vis/16.11.5/setup.csh

source /usr/local/SOPHYA/2.2/setup.csh

# the below does not work within a .csh script !
#source /usr/local/ROOT/5.28.00/root/bin/thisroot.csh

source ./setup_cern_root.csh

setenv PYTHIA8DATA "`pwd`/../../inexlib/ourex/pythia8/xmldoc"

