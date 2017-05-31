
#include <stdlib.h>

void srand48(long a_seed) {
  srand((unsigned int)a_seed);
}

/* drand48 could be faked with :
#define drand48(a__dummy) ((double)rand())/((double)RAND_MAX)

double drand48() {
  return ((double)rand())/((double)RAND_MAX);
}
*/

unsigned short* seed48(unsigned short a_s[3]){
  static unsigned short s_v[3] = {0,0,0};
  (void)a_s;
  return s_v;
}

#include <time.h>
#include <stdio.h>

char *strptime(const char *rbuf, const char *format, struct tm *tm) {
  sscanf(rbuf,"%2d/%2d/%4d %2d:%2d:%2d GMT",tm->tm_mday,tm->tm_mon,tm->tm_year,
                                            tm->tm_hour,tm->tm_min,tm->tm_sec);
  tm->tm_mon--;
  tm->tm_year -= 1900;
}
