// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file bush.license for terms.

#ifndef bush_pthread_h
#define bush_pthread_h

#define NOMINMAX
#include <windows.h>

class pthread_t {
public:
  pthread_t():m_id(0),m_handle(0){}
public:
  DWORD  m_id;
  HANDLE m_handle;
};

#ifdef __cplusplus
extern "C" {
#endif

inline int pthread_create(pthread_t* a_thread,void*,void*(*a_func)(void*),void* a_tag){
  a_thread->m_handle = ::CreateThread(0,0,(LPTHREAD_START_ROUTINE)a_func,a_tag,0,(DWORD*)&(a_thread->m_id));
  return 0;
}

inline int pthread_cancel(pthread_t) {return -1;}
inline int pthread_kill(pthread_t,int) {return -1;}
inline int pthread_join(pthread_t,void**) {return -1;}

inline void pthread_cleanup_push(void (*routine)(void *),void*) {}
inline void pthread_cleanup_pop(int) {}

#define PTHREAD_CANCEL_ENABLE        0x01
#define PTHREAD_CANCEL_DEFERRED      0x02

inline int pthread_setcancelstate(int state, int *oldstate) {return -1;}
inline int pthread_setcanceltype(int type, int *oldtype) {return -1;}

typedef unsigned int pthread_mutex_t;

typedef int pthread_mutexattr_t;
inline int pthread_mutex_init(pthread_mutex_t*,const pthread_mutexattr_t*) {return -1;}
inline int pthread_mutex_destroy(pthread_mutex_t*) {return -1;}

inline int pthread_mutex_lock(pthread_mutex_t*) {return -1;}
inline int pthread_mutex_unlock(pthread_mutex_t*) {return -1;}

typedef unsigned int pthread_cond_t;

typedef int pthread_condattr_t;
inline int pthread_cond_init(pthread_cond_t*,const pthread_condattr_t*) {return -1;}
inline int pthread_cond_destroy(pthread_cond_t*) {return -1;}

inline int pthread_cond_wait(pthread_cond_t*,pthread_mutex_t*) {return -1;}
inline int pthread_cond_signal(pthread_cond_t*) {return -1;}
inline int pthread_cond_broadcast(pthread_cond_t*) {return -1;}

inline void pthread_exit(void*) {}

struct pthread_attr_t {
  int dummy;
};

inline int pthread_attr_init(pthread_attr_t*) {return -1;}
inline int pthread_attr_setstacksize(pthread_attr_t*,size_t) {return -1;}
     
#ifdef __cplusplus
}
#endif

#endif
