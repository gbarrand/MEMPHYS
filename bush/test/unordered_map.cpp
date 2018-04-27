
#include <unordered_map>

class A;
class B;

using Key=std::pair<const A*,const B*>;

class cycleCountEntry;

class Key_Hash {
public:
  Key_Hash();
  virtual ~Key_Hash();
  Key_Hash(const Key_Hash&);
  Key_Hash& operator=(const Key_Hash&);
public:  
  //inline size_t operator()(const Key& x) const throw() {
  inline size_t operator()(const Key& x) const { //clang/Linux : with throw() it does not compile.
    return 0;
  }
};

class Key_EqualTo {
public:
  Key_EqualTo();
  virtual ~Key_EqualTo();
  Key_EqualTo(const Key_EqualTo&);
  Key_EqualTo& operator=(const Key_EqualTo&);
public:  
  inline bool operator()(const Key& lhs, const Key& rhs ) const {
    return true;
  }
};

using Cache=std::unordered_map<Key,cycleCountEntry*,Key_Hash,Key_EqualTo>;

Cache fastPathCache;
