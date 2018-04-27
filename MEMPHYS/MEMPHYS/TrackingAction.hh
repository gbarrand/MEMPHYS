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
  typedef G4UserTrackingAction parent;
public:
  TrackingAction();
  virtual ~TrackingAction();
protected:
  TrackingAction(const TrackingAction& a_from):parent(a_from){}
  TrackingAction& operator=(const TrackingAction& a_from){parent::operator=(a_from);return *this;}
public:  
  virtual void PreUserTrackingAction (const G4Track* aTrack);
  virtual void PostUserTrackingAction(const G4Track* aTrack);
public:
  //JEC 24/1/06
  void SetDrawOpticalPhoton(bool value) {m_draw_optical_photon = value;}
private:
  std::set<G4String> m_process_list;
  std::set<G4int> m_particle_list;
  //JEC 24/1/06 
  TrackingActionMessenger* m_messenger; 
  G4bool m_draw_optical_photon;
};
}

#endif


