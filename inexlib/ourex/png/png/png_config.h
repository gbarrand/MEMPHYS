/* G.Barrand */
/* NOTE : in fact it seems that nothing defined (or not)
          in the png_config_<>.h are used ! */

#if defined(WIN32)

#include "png_config_win32.h"

#elif defined(__APPLE__)

#include "png_config_macosx.h"

#elif defined(__linux__)

  #if defined(__x86_64__)
  #include "png_config_linux.h"
  #else
  #include "png_config_linux.h"
  #endif

#else

  #error "Unknown platform."

#endif
