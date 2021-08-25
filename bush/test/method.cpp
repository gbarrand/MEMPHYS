
#include <cstdio>

class A {
public:
  virtual void x(){
    ::printf("A::x()\n");
  }
};

class B : public A {
public:
  virtual void x(){
    ::printf("B::x()\n");
  }
};

int main() {
  typedef void(A::*method)();
  method m = &A::x;

  B a;
  (a.*m)();
  
  return 0;
}
