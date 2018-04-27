#include <cstdio>
#include <cmath>

int main() {

  int i = -1;
  
  ::printf("debug i %x\n",i);
  ::printf("debug i << : \n");
  ::printf("debug <<1  : %x\n",i<<1);
  ::printf("debug <<2  : %x\n",i<<2);
  ::printf("debug <<3  : %x\n",i<<3);
  ::printf("debug <<16 : %x\n",i<<16);
  ::printf("debug <<31 : %x\n",i<<31);
  ::printf("debug <<32 : %x\n",i<<32);
  
  ::printf("debug >> : \n");
  ::printf("debug >>1  : %x\n",i>>1);
  ::printf("debug >>2  : %x\n",i>>2);
  ::printf("debug >>3  : %x\n",i>>3);
  ::printf("debug >>16 : %x\n",i>>16);
  ::printf("debug >>31 : %x\n",i>>31);
  ::printf("debug >>32 : %x\n",i>>32);
  
  unsigned int j = 0xffffffff;
  ::printf("debug j %x\n",j);
  ::printf("debug j << : \n");
  ::printf("debug <<1  : %x\n",j<<1);
  ::printf("debug <<2  : %x\n",j<<2);
  ::printf("debug <<3  : %x\n",j<<3);
  ::printf("debug <<16 : %x\n",j<<16);
  ::printf("debug <<31 : %x\n",j<<31);
  ::printf("debug <<32 : %x\n",j<<32);

  ::printf("debug j >> : \n");
  ::printf("debug >>1  : %x\n",j>>1);
  ::printf("debug >>2  : %x\n",j>>2);
  ::printf("debug >>3  : %x\n",j>>3);
  ::printf("debug >>16 : %x\n",j>>16);
  ::printf("debug >>31 : %x\n",j>>31);
  ::printf("debug >>32 : %x\n",j>>32);

  ::printf("debug end\n");

  return 0;
}
