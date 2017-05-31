// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file bush.license for terms.

#ifndef bush_visual_sys_time_h
#define bush_visual_sys_time_h

#include <sys/timeb.h>

#include <winsock.h> //it defines struct timeval.
//struct timeval {
//  time_t tv_sec;  //seconds.
//  unsigned int tv_usec; //microseconds.
//};

#ifdef __cplusplus
extern "C" {
#endif

inline int gettimeofday(timeval* a_t,void*) {
  struct timeb tb;
  ::ftime(&tb);
  //a_t->tv_sec = tb.time;
  a_t->tv_sec = (long)tb.time;
  a_t->tv_usec = tb.millitm * 1000;
  return 0;
}

#ifdef __cplusplus
}
#endif

#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCTION__ 
#endif

#endif
