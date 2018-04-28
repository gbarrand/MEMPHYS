/* G.Barrand : keep only _WIN32 not _WIN32. */

#if defined(_WIN32)

#include <stdlib.h>	    /* For _MAX_PATH definition */
#include <stdio.h>
#include <malloc.h>

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOIME
#define NOMINMAX

#define _WIN32_WINNT 0x0400
#ifndef STRICT
  #define STRICT 1
#endif

#include <windows.h>

/* Disable warnings */
#pragma warning(disable : 4101)
#pragma warning(disable : 4244)
#pragma warning(disable : 4761)

#if defined(_MSC_VER) && _MSC_VER >= 1200 && _MSC_VER < 1300
#pragma comment(linker, "/OPT:NOWIN98")
#endif

#else

/* Disable Microsoft-specific keywords */
#define GLAPIENTRY
#define WINGDIAPI

#endif
