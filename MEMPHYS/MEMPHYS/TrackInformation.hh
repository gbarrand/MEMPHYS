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

  TrackInformation(const G4Track*) {
    saveit = false;
    drawit = false;
    status = active;
  }

  virtual ~TrackInformation() {}
  
  void* operator new(size_t) { 
    return (void*)allocator()->MallocSingle();
  }

  void operator delete(void *aTrackInfo){ 
    allocator()->FreeSingle((MEMPHYS::TrackInformation*)aTrackInfo);
  }

  int operator ==(const TrackInformation& right) const {return (this==&right);}

  
  //Sets the track status to s (does not check validity of flags)
  void SetTrackStatusFlags(G4int s){status=s;}
  //Does a smart add of track status flags (disabling old flags that conflict)
  //If s conflicts with itself it will not be detected
  void AddTrackStatusFlag(G4int s) {
    if(s&active) //track is now active
      status&=~inactive; //remove any flags indicating it is inactive 
    else if(s&inactive) //track is now inactive
      status&=~active; //remove any flags indicating it is active
    status|=s; //add new flags
  }//AddTrackStatusFlag

//------------------------------------------------------------------------

  int GetTrackStatus()  {return status;}

  void SetDrawOption(G4bool b) {drawit = b;}
  G4bool GetDrawOption() {return drawit;}

  void SetSaveOption(G4bool b) {saveit = b;}
  G4bool GetSaveOption() {return saveit;}


  void Print() const {
    G4cout << "TrackInformation : [" << saveit 
           << "]-[" << drawit 
           << "]-[" << status << "]"
           << G4endl;
  }

private:
  static G4Allocator<TrackInformation>* allocator() {
    //warning : it can't be on the stack, since it is managed by the G4RunManager.
    static G4Allocator<TrackInformation>* s_allocator = new G4Allocator<TrackInformation>;
    return s_allocator;
  }
private:
  G4bool saveit; 
  G4bool drawit;
  G4int  status; 

};

}

#endif
