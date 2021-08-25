
#include <stdio.h>

int main() {
  printf("ALLDEF.kumac %lu\n",(unsigned long)fopen("ALLDEF.kumac","r"));
  printf("ALLDEF.KUMAC %lu\n",(unsigned long)fopen("ALLDEF.KUMAC","r"));
  printf("alldef.kumac %lu\n",(unsigned long)fopen("alldef.kumac","r"));
  return 0;
}
