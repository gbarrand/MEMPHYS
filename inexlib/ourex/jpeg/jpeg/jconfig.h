#if defined(_WIN32)
#include "platform/jconfig_win32.h"
#elif defined(__APPLE__)
#include "platform/jconfig_macosx.h"
#elif defined(__alpha)
#include "platform/jconfig_alpha.h"
#else
#include "platform/jconfig_macosx.h"
#endif
