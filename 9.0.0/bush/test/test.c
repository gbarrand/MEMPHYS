
#ifdef _MSC_VER
#include <windows.h>
#endif

#include <stdio.h>

#ifdef pascal
#define old_pascal pascal
#undef pascal
void pascal() {printf("debug : pascal\n");}
#define pascal old_pascal
#else
void pascal() {printf("debug : pascal\n");}
#endif

int main() {
  pascal();
  return 0;
}
