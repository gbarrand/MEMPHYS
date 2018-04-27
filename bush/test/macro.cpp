
#define png_debug(l,m) \
{ \
     int num_tabs=l; \
     fprintf(stdout,"%sxxx" m,(num_tabs==1 ? "\t" : \
       (num_tabs==2 ? "\t\t":(num_tabs>2 ? "\t\t\t":"")))); \
}

#include <stdio.h>

int main() {
  png_debug(1,"314\n");
  return 0;
}
