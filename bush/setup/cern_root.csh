
set save_dir=`pwd`

if ( "`uname -n`" == "barrand-ubuntu" ) then
  cd /usr/local/ROOT/6.08.06/bin
else  
  cd /usr/local/ROOT/5.34.18/bin
endif


source ./thisroot.csh

cd ${save_dir}
