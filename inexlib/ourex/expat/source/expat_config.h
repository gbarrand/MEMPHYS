/* G.Barrand : keep only what is needed. */

/* 1234 = LIL_ENDIAN, 4321 = BIGENDIAN */

/*G.Barrand :
  BE = Big Endian, LE = Little Endian.
  LE :
  - the intels x86
  - mac intel
  - iPad.
  - Android : at least the Samsung Galaxy S.
  - Linux i386
  - Linux x86_64
  BE :
  - Mac PPC
*/

/* G.Barrand : from luaconf.h : */
#if defined(__i386__) || defined(__i386) || \
    defined(__X86__)  || defined (__x86_64)
#define BYTEORDER 1234
#elif defined(__POWERPC__) || defined(__ppc__)
#define BYTEORDER 4321
#endif

#define HAVE_MEMMOVE 1

#define XML_CONTEXT_BYTES 1024
#define XML_DTD 1
#define XML_NS 1

/*
#define HAVE_BCOPY 1
#define HAVE_DLFCN_H 1
#define HAVE_FCNTL_H 1
#define HAVE_GETPAGESIZE 1
#define HAVE_INTTYPES_H 1
#define HAVE_MEMORY_H 1
#define HAVE_MMAP 1
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRINGS_H 1
#define HAVE_STRING_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_UNISTD_H 1

#define PACKAGE_BUGREPORT "expat-bugs@libexpat.org"
#define PACKAGE_NAME "expat"
#define PACKAGE_STRING "expat 2.0.1"
#define PACKAGE_TARNAME "expat"
#define PACKAGE_VERSION "2.0.1"
*/
/*#define STDC_HEADERS 1*/
