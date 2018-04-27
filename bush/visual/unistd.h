// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file bush.license for terms.

#ifndef bush_visual_unistd_h
#define bush_visual_unistd_h

#define NOMINMAX
#include <windows.h>

#ifdef __cplusplus
extern "C" {

inline void sleep(unsigned int a_secs) {
  ::Sleep(a_secs*1000);
}

inline void usleep(unsigned int a_micro_secs) {
  LARGE_INTEGER ft; 
  ft.QuadPart = -10*int(a_micro_secs); // Convert to 100 nanosecond interval, negative value indicates relative time
  HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL); 
  SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
  WaitForSingleObject(timer, INFINITE); 
  CloseHandle(timer);
}

inline int nice(int) {return -1;}

inline unsigned alarm(unsigned seconds) {return 0;}

}
#endif

#include <process.h> //getpid

#endif
