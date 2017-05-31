
#include <cstdio>

class IA0 {
public:
  virtual void xxx() = 0;
public:
  //virtual 
  virtual ~IA0() {
    ::printf("IA0::~IA0\n");
  }
};

class A0 
: public IA0 
{
public:
  virtual void xxx() {}
public:
  //virtual 
  ~A0() {
    ::printf("A0::~A0\n");
  }
};

class A1 : public A0 {
public:
  //virtual 
  ~A1() {
    ::printf("A1::~A1\n");
  }
};

class A2 : public A1 {
public:
  //virtual 
  ~A2() {
    ::printf("A2::~A2\n");
  }
};

class B0 {
public:
  //virtual 
  ~B0() {
    ::printf("B0::~B0\n");
  }
};

class B1 : public B0 {
public:
  //virtual 
  ~B1() {
    ::printf("B1::~B1\n");
  }
};

class B2 : public B1 {
public:
  //virtual 
  ~B2() {
    ::printf("B2::~B2\n");
  }
};



int main() {
  ::printf("///////////\n");
  IA0* a = new A2;
  //IA0* ia = a;
  delete a;

  ::printf("///////////\n");
  B0* b0 = new B2;
  delete b0;

  ::printf("///////////\n");
  B1* b1 = new B2;
  delete b1;
  
  return 0;
}
