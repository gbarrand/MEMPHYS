if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/in2p3.fr/home/throng/lalorsay/CMT/v1r18p20061003
endif
source ${CMTROOT}/mgr/setup.csh
set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=MEMPHYS -version=v8r0 -path=/afs/in2p3.fr/home/throng/lalorsay/contrib $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

