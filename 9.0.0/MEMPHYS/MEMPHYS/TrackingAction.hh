#ifndef MEMPHYSTrackingAction_h
#define MEMPHYSTrackingAction_h

//Geant4
#include "G4UserTrackingAction.hh"
#include "globals.hh"

//std
#include <set>


class G4Track;

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {

class TrackingActionMessenger; //JEC 24/1/06 to add a switch for optical photon display 

class TrackingAction : public G4UserTrackingAction {
 public:
  TrackingAction();
  virtual ~TrackingAction();

  void PreUserTrackingAction (const G4Track* aTrack);
  void PostUserTrackingAction(const G4Track* aTrack);

  //JEC 24/1/06
  void SetDrawOpticalPhoton(bool value) {drawOpticalPhoton = value;}

private:
  std::set<G4String> ProcessList;
  std::set<G4int> ParticleList;


  
  //JEC 24/1/06 
  TrackingActionMessenger* messenger; 
  G4bool drawOpticalPhoton;
  
};
}

#endif


