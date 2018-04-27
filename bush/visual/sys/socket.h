// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file bush.license for terms.

#ifndef bush_visual_sys_socket_h
#define bush_visual_sys_socket_h

//#include <io.h>
#include <winsock.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int socklen_t;
typedef int ssize_t;

typedef unsigned char sa_family_t;

#define SHUT_RDWR 2

#ifdef __cplusplus
}
#endif

#endif
