#ifndef MEMPHYSTrackInformation_h
#define MEMPHYSTrackInformation_h 1


#include "globals.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"

//JEC 14/12/05: use code from G4 extended optical LXe example
//for Track Status


//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {

enum TrackStatus { active=1, hitPMT=2, absorbed=4, boundaryAbsorbed=8,
		    inactive=14};

/*TrackStatus:
  active: still being tracked
  hitPMT: stopped by being detected in a PMT
  absorbed: stopped by being absorbed with G4OpAbsorbtion
  boundaryAbsorbed: stopped by being aborbed with G4OpAbsorbtion
  hitSphere: track hit the sphere at some point
  inactive: track is stopped for some reason
   -This is the sum of all stopped flags so can be used to remove stopped flags
  
 */

class TrackInformation : public G4VUserTrackInformation {

public:
  TrackInformation(): saveit(false), drawit(false), status(active) {}
  //  TrackInformation(const TrackInformation* aninfo) 
  // { saveit = aninfo->saveit;}
  TrackInformation(const G4Track*);
  virtual ~TrackInformation() {}
  
  inline void *operator new(size_t);
  inline void operator delete(void *aTrackInfo);
  inline int operator ==(const TrackInformation& right) const
  {return (this==&right);}

  
  //Sets the track status to s (does not check validity of flags)
  void SetTrackStatusFlags(G4int s){status=s;}
  //Does a smart add of track status flags (disabling old flags that conflict)
  //If s conflicts with itself it will not be detected
  void AddTrackStatusFlag(G4int s);
  int GetTrackStatus()  {return status;}

  void SetDrawOption(G4bool b) {drawit = b;}
  G4bool GetDrawOption() {return drawit;}

  void SetSaveOption(G4bool b) {saveit = b;}
  G4bool GetSaveOption() {return saveit;}


  void Print() const;


private:
  G4bool saveit; 
  G4bool drawit;
  G4int  status; 

};
}

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {
  extern G4Allocator<TrackInformation> aTrackInfoAllocator;
}

inline void* MEMPHYS::TrackInformation::operator new(size_t) { 
  void* aTrackInfo;
  aTrackInfo = (void*)MEMPHYS::aTrackInfoAllocator.MallocSingle();
  return aTrackInfo;
}

inline void MEMPHYS::TrackInformation::operator delete(void *aTrackInfo){ 
  MEMPHYS::aTrackInfoAllocator.FreeSingle((MEMPHYS::TrackInformation*)aTrackInfo);
}


#endif
