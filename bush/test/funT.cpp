
#include <cstdio>

template <class FUNC>
class A {
public:
  A(FUNC a_func) {    
    ::printf("A::A : %g\n",a_func(0));
  }
};

#include <cmath>

int main() {
  typedef double(*_func)(double);
  A<_func> a(::cos);
  return 0;
}
