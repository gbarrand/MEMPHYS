
//c++11

#include <initializer_list>
#include <vector>
class xxx {
public:
  xxx(std::initializer_list<xxx*> a_inputs):m_vec(a_inputs) {} //well that's easy
private:
  std::vector<xxx*> m_vec;
};
int main() {
  xxx* ptr;
  xxx node{ptr, ptr, ptr, ptr};
  return 0;
}
