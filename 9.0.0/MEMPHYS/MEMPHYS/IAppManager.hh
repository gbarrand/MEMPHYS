#ifndef MEMPHYS_IAppManager_hh
#define MEMPHYS_IAppManager_hh

namespace MEMPHYS {

class IAppManager  {
public:        
  virtual ~IAppManager() {}
public:        
  virtual bool initialize() = 0;
};

}

#endif
