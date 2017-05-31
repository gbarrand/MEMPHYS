// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file bush.license for terms.

#ifndef bush_visual_dlfcn_h
#define bush_visual_dlfcn_h

#define RTLD_NOW 0x2

#ifdef __cplusplus
extern "C" {
#endif

inline void* dlopen(const char*,int) {return 0;}
inline int dlclose(void*) {return -1;}
inline void* dlsym(void*,const char*) {return 0;}
inline const char* dlerror() {
  static const char* s_v = "bush/visual/dlfcn : not implemented";
  return s_v;
}

#ifdef __cplusplus
}
#endif

#endif
