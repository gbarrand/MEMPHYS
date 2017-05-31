// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file bush.license for terms.

#ifndef bush_visual_sys_resource_h
#define bush_visual_sys_resource_h

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned __int64 rlim_t;

struct rlimit {
  rlim_t rlim_cur;
  rlim_t rlim_max;
};

#define	RLIMIT_DATA	2
#define	RLIMIT_STACK	3
#define	RLIMIT_CORE	4
#define	RLIMIT_AS	5
#define	RLIMIT_RSS	RLIMIT_AS

inline int getrlimit(int,struct rlimit*) {return -1;}

#ifdef __cplusplus
}
#endif

#include <winsock.h> //it defines struct timeval.

#ifdef __cplusplus
extern "C" {
#endif

struct rusage {
  struct timeval ru_utime;
  struct timeval ru_stime;
  long ru_maxrss;
  long ru_ixrss;
  long ru_idrss;
  long ru_isrss;
  long ru_minflt;
  long ru_majflt;
  long ru_nswap;
  long ru_inblock;
  long ru_oublock;
  long ru_msgsnd;
  long ru_msgrcv;
  long ru_nsignals;
  long ru_nvcsw;
  long ru_nivcsw;
};

#define	RUSAGE_SELF	 0
#define	RUSAGE_CHILDREN	-1

inline int getrusage(int,struct rusage*) {return -1;}

#ifdef __cplusplus
}
#endif

#endif
