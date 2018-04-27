// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file bush.license for terms.

#ifndef bush_visual_netdb_h
#define bush_visual_netdb_h

#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NI_NUMERICHOST 0x00000002
#define NI_NUMERICSERV 0x00000008
#define NI_NAMEREQD    0x00000004

inline int getnameinfo(const struct sockaddr*,socklen_t,char*,socklen_t,char*,socklen_t,int) {return -1;}

#ifdef __cplusplus
}
#endif

#endif
