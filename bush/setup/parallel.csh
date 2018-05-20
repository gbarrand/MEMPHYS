
if ( "`uname -n`" == barrand-centos7.lal.in2p3.fr ) then
  setenv LC_ALL en_US.UTF-8
  if ( -d /usr/local/parallel/20120122/bin ) then
    setenv PATH "/usr/local/parallel/20120122/bin:$PATH"
  endif
endif
