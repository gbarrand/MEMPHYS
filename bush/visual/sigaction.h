// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file bush.license for terms.

#ifndef bush_visual_sigaction_h
#define bush_visual_sigaction_h

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int sigset_t;

struct  sigaction {
  void (*sa_handler)(int);
  sigset_t sa_mask;
  int sa_flags;
};

inline int sigaction(int, const struct sigaction*,struct sigaction*) {return -1;}

#ifdef __cplusplus
}
#endif

#endif
