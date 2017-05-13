#ifndef MEMPHYSWCHit_h
#define MEMPHYSWCHit_h 1

//Geant4
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Transform3D.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4ios.hh"
#include "G4Colour.hh"


//std
#include <algorithm> // for sort, find, count_if
#include <functional> //for less_equal, bind2nd,...

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {

class WCHit : public G4VHit {
 public:
  
  WCHit();
  virtual ~WCHit();
  G4int operator==(const WCHit&) const;
  
  inline void* operator new(size_t);
  inline void  operator delete(void*);
  
  void Draw(); //JEC FIXME what do we do with this?
  void Print();
  //JEC 1/12/05 implement same mechanism as Trajectory
  virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
  virtual std::vector<G4AttValue>* CreateAttValues() const;


  
 public:
  
  void SetTubeID       (G4int tube)                 { tubeID = tube; }
  //JEC 6/4/06  void SetTrackID      (G4int track)                { trackID = track; }
  //JEC 6/4/06  void SetEdep         (G4double de)                { edep = de; }
  void SetTransform (G4Transform3D aTrans) { transform = aTrans; }
  void SetLogicalVolume(G4LogicalVolume* logV)      { pLogV = logV;}
  void SetColour(G4Colour aColour) { colour = aColour; }
  void UpdateColour(); //modify color at the end of the event

  // This is temporarily used for the drawing scale
  void SetMaxPe(G4int number = 0)  {maxPe   = number;}

  void AddPe(G4float hitTime);
  void AddTrk(G4int trackID);                //JEC 6/4/06
  void AddEdep(G4double energyDeposition);   //JEC 6/4/06 

  G4int         GetTubeID()     { return tubeID; }
  //obsolete G4int         GetTrackID()    { return trackID; }
  G4int         GetTotalPe()    { return totalPe;}
  G4float       GetTime(G4int i)  { return time[i]; }
  G4int         GetTrack(G4int i) {return trkId[i];}
  
  G4Transform3D GetTransform()  { return transform; }
  G4LogicalVolume* GetLogicalVolume() { return pLogV; }
  G4Colour GetColour() { return colour; }
  
  
  void SortHitTimes() {   sort(time.begin(),time.end()); }
  G4int GetPeInGate(double pmtgate,double evgate);


 private:
  WCHit(const WCHit&);
  const WCHit& operator=(const WCHit&);
  
  G4int            tubeID;
  //JEC 6/4/06 G4int            trackID;
  //JEC 6/4/06 G4double         edep;

  //JEC to GB
  G4Colour         colour;
  G4Transform3D    transform;
  G4LogicalVolume* pLogV;
  G4int                 totalPe;
  std::vector<G4float>    time;
  std::vector<G4int>    trkId;   //JEC 6/4/06
  std::vector<G4double> edep; //JEC 6/4/06
  
  //for digitization
  G4int                 totalPeInGate;

  // This is temporarily used for the drawing scale
  // Since its static *every* WChit sees the same value for this.
  static G4int     maxPe;

};
}

//----------------------------------------------------------------

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {
  typedef G4THitsCollection<WCHit> WCHitsCollection;
  extern G4Allocator<WCHit> WCHitAllocator;
}

inline void* MEMPHYS::WCHit::operator new(size_t) {
  void *aHit;
  aHit = (void *) MEMPHYS::WCHitAllocator.MallocSingle();
  return aHit;
}

inline void MEMPHYS::WCHit::operator delete(void *aHit) {
  MEMPHYS::WCHitAllocator.FreeSingle((MEMPHYS::WCHit*) aHit);
}

#endif
