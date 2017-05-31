
#include <stdio.h>

int main() {
  double array[4][3][2];  //[D1][D2][D3] 
  array[3][2][1] = 3.14;  //[i1][i2][i3]
  double* parray = (double*)array;
  printf("%g\n",*(parray+3+2*4+1*(4*3))); //i1+i2*D1+i3*(D1*D2)
  return 0;
}
