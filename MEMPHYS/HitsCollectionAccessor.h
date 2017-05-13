#ifndef MEMPHYS_HitsCollectionAccessor_h
#define MEMPHYS_HitsCollectionAccessor_h

// Inheritance :
#include <G4Lab/HitsCollectionAccessor.h>

namespace MEMPHYS {

class HitsCollectionAccessor : public G4Lab::HitsCollectionAccessor {
public: //Slash::Data::IVisualizer
  virtual void visualize(Slash::Data::IAccessor::Data,void*);
public:
  HitsCollectionAccessor(Slash::Core::ISession&,G4RunManager*,const std::string& = "");
  virtual ~HitsCollectionAccessor();
};

}

#endif
