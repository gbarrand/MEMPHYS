if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/in2p3.fr/home/throng/lalorsay/CMT/v1r18p20061003; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=MEMPHYS -version=v8r0 -path=/afs/in2p3.fr/home/throng/lalorsay/contrib $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

