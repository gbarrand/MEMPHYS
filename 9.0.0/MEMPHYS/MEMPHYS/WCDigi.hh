#ifndef MEMPHYSWCDigi_h
#define MEMPHYSWCDigi_h 1

#include "G4VDigi.hh"
#include "G4TDigiCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"

//std
#include <vector>


//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {

class WCDigi : public G4VDigi {

public:
  
  WCDigi();
  virtual ~WCDigi();
  int operator==(const WCDigi&) const;
  
  inline void* operator new(size_t);
  inline void  operator delete(void*);
  
  void Draw(); //JEC FIXME what do we do with this?
  void Print();
  //JEC 1/12/05 implement same mechanism as Trajectory
  virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
  virtual std::vector<G4AttValue>* CreateAttValues() const;

private:

  //JEC make Ctor and assignation private
  WCDigi(const WCDigi&);
  const WCDigi& operator=(const WCDigi&);
  
  G4int   tubeID;
  G4float pe;   
  G4float time; 

  //JEC for vizu
  G4Colour         colour;
  G4Transform3D    transform;
  G4LogicalVolume* pLogV;
 
public:
  
  inline void SetTubeID(G4int tube) {tubeID = tube;}
  inline void SetPe(G4float Q)      {pe     = Q;}
  inline void SetTime(G4float T)    {time   = T;}

  //JEC 1/12/05 for vizu
  void SetColour(G4Colour aColour) { colour = aColour; }
  void UpdateColour() {/* not yet implemented */}
  inline void SetTransform(G4Transform3D aTrans) { transform = aTrans; }
  inline void SetLogicalVolume(G4LogicalVolume* logV) {pLogV = logV;}


  inline G4int   GetTubeID() {return tubeID;}
  inline G4float GetPe()     {return pe;}
  inline G4float GetTime()   {return time;}
  
};
}
//-----------------------------------------------------------------------

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {

  typedef G4TDigiCollection<WCDigi> WCDigitsCollection;
  extern G4Allocator<WCDigi> WCDigiAllocator;
}

inline void* MEMPHYS::WCDigi::operator new(size_t) {
  void* aDigi;
  aDigi = (void*) MEMPHYS::WCDigiAllocator.MallocSingle();
  return aDigi;
}

inline void MEMPHYS::WCDigi::operator delete(void* aDigi) {
  MEMPHYS::WCDigiAllocator.FreeSingle((MEMPHYS::WCDigi*) aDigi);
}

#endif









