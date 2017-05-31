
#include <cstdio>

class A {
public:
  A() {
    ::printf("A::A()\n");
  }
  virtual ~A() {
    ::printf("A::~A()\n");
  }
};

class B {
public:
  B() {
    ::printf("B::B()\n");
  }
  virtual ~B() {
    ::printf("B::~B()\n");
  }
};

class C : public A, public B {
public:
  C() {
    ::printf("C::C()\n");
  }
  virtual ~C() {
    ::printf("C::~C()\n");
  }
};

int main() {
  C c;
  return 0;
}
