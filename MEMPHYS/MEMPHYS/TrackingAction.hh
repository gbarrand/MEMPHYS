#ifndef MEMPHYSTrackingAction_h
#define MEMPHYSTrackingAction_h

#include "Trajectory.hh"
#include "TrackInformation.hh"

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"
#include "G4TrackingManager.hh"
#include "G4ParticleTypes.hh"

#include "G4UImessenger.hh"
#include "G4UIcmdWithABool.hh"

#include <set>

namespace MEMPHYS {

class TrackingAction : public G4UserTrackingAction {
  typedef G4UserTrackingAction parent;
public:
  TrackingAction() {
    m_process_list.insert("Decay") ;
    m_process_list.insert("MuonMinusCaptureAtRest") ; //JEC 15/12/05 uncomment this process
  
    m_particle_list.insert(111); // pi0
    m_particle_list.insert(211); // pion+
    m_particle_list.insert(-211);
    m_particle_list.insert(321);
    m_particle_list.insert(-321); // kaon-
    m_particle_list.insert(311); // kaon0
    m_particle_list.insert(-311); // kaon0 bar
    // don't put gammas there or there'll be too many
  
    m_messenger = new TrackingActionMessenger(this);
    m_draw_optical_photon = false;
  }

  virtual ~TrackingAction() {
    delete m_messenger;
    m_messenger = 0;
  }

protected:
  TrackingAction(const TrackingAction& a_from):parent(a_from){}
  TrackingAction& operator=(const TrackingAction& a_from){parent::operator=(a_from);return *this;}
public:  
  virtual void PreUserTrackingAction (const G4Track* aTrack) {
    Trajectory* aTrajectory = new Trajectory(aTrack);
    //Use custom trajectory class
    fpTrackingManager->SetTrajectory(aTrajectory);
    fpTrackingManager->SetStoreTrajectory(true); // (JEC) 14/12/05 keep it a priori and delete it a posteriori

    TrackInformation* aInfo = new TrackInformation(aTrack);

    // Code from M Fechner as a priori some tracks are worth to be saved
    const G4VProcess* creatorProcess = aTrack->GetCreatorProcess();
    // is it a primary ?
    // is the process in the set ? 
    // is the particle in the set ?
    // is it a gamma 
    // due to lazy evaluation of the 'or' in C++ the order is important
    //    G4cout << aTrack->GetParentID() << "\n";

    G4double thresholdTobeSaved = 10.0*CLHEP::MeV; //JEC FIXME put in the messenger

    if( ( aTrack->GetParentID()==0 ) || 
        ( (creatorProcess!=0) && m_process_list.count(creatorProcess->GetProcessName()) ) || 
        ( m_particle_list.count(aTrack->GetDefinition()->GetPDGEncoding()) ) ||
        
        ( (aTrack->GetDefinition()->GetPDGEncoding() == 22) && (aTrack->GetTotalEnergy() > thresholdTobeSaved) )  ) {
  
      // if so the track is worth saving and drawing a priori
      
  //     G4cout << "(JEC) PreUserTrackingAction: Save & Draw a priori Particle <" 
  // 	   << aTrack->GetTrackID() << "> is a " 
  // 	   << aTrack->GetDefinition()->GetParticleName();
  //     if(creatorProcess !=0) {
  //       G4cout << " created by " <<   creatorProcess->GetProcessName();
  //     } 
  //     G4cout << G4endl;
      aInfo->SetSaveOption(true);
      aInfo->SetDrawOption(true);
      
    }//eo if it is worth to save this track
    
    fpTrackingManager->SetUserTrackInformation(aInfo);
    
    //  G4cout << "(JEC) PreUserTrackingAction : INFO : END" << G4endl; 
  }

  virtual void PostUserTrackingAction(const G4Track* aTrack) {
    // Get the Track Information 
    TrackInformation* aInfo = (TrackInformation*)(aTrack->GetUserInformation());

  //   G4cout << "(JEC) PostUserTrackingAction : INFO : Track Status <"
  // 	 << aInfo->GetTrackStatus() << ">"
  // 	 << G4endl;
  
    if ( aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() ) {
  
  //     G4cout << "(JEC) PostUserTrackingAction : INFO : Track is an Optical Photon"
  // 	   << G4endl;
  
      //JEC 24/1/06 add a condition based on messenger info to draw or not to draw
      if ( (aInfo->GetTrackStatus()&hitPMT) && (m_draw_optical_photon) ) {
  	aInfo->SetDrawOption(true);
  // 	G4cout << "(JEC) PostUserTrackingAction : INFO : Opticel Photon hit a PMT"
  // 	       << G4endl;
        }
  
    }//Partcile type "optical photon" or not
  
    if ( aInfo->GetSaveOption() ) {
  //     G4cout << "(JEC) PostUserTrackingAction : INFO : Track should be saved"
  // 	   << G4endl;
      
      Trajectory* currentTrajectory = (Trajectory*)fpTrackingManager->GimmeTrajectory();

      currentTrajectory->SetStoppingPoint(aTrack->GetPosition());
      currentTrajectory->SetStoppingDirection(aTrack->GetMomentumDirection());
      currentTrajectory->SetStoppingVolume(aTrack->GetVolume());
      currentTrajectory->SetSaveFlag(true); //For saving into Tuple (see EventAction)
    }//save the trajectory
    
    if ( ! ( aInfo->GetDrawOption() || aInfo->GetSaveOption() ) ) {
      //do no keep the trajectory
      fpTrackingManager->SetStoreTrajectory(false);
    }//remove the trajectory
  
  }

public:
  void SetDrawOpticalPhoton(bool value) {m_draw_optical_photon = value;}

private:
  class TrackingActionMessenger:  public G4UImessenger {
  public:
    TrackingActionMessenger(TrackingAction* MEMPHYSTrackAction):MEMPHYSTrackingAction(MEMPHYSTrackAction) {
      MEMPHYSDir = new G4UIdirectory("/MEMPHYS/tracking/drawing/");
      MEMPHYSDir->SetGuidance("Commands to define particle drawing option");
  
      drawOpticalPhotonCmd = new G4UIcmdWithABool("/MEMPHYS/tracking/drawing/optical",this);
      drawOpticalPhotonCmd->SetGuidance("Available options: true false");
      drawOpticalPhotonCmd->SetGuidance("Description:");
      drawOpticalPhotonCmd->SetGuidance("true  = optical photons are stored to be drawn");
      drawOpticalPhotonCmd->SetGuidance("flase = no optical photons stored");
      drawOpticalPhotonCmd->SetParameterName("OptPhotonOption", false);
      drawOpticalPhotonCmd->SetDefaultValue(false);
    }
    virtual ~TrackingActionMessenger() {
      delete drawOpticalPhotonCmd;
      delete MEMPHYSDir;
    }
  public:  
    virtual void SetNewValue(G4UIcommand* command, G4String newValue) {
      if (command == drawOpticalPhotonCmd) {
        MEMPHYSTrackingAction->SetDrawOpticalPhoton(StoB(newValue));
      }
    }
  private:
    TrackingAction* MEMPHYSTrackingAction;
  private:
    G4UIdirectory*      MEMPHYSDir;
    G4UIcmdWithABool*   drawOpticalPhotonCmd;
  };

private:
  std::set<G4String> m_process_list;
  std::set<G4int> m_particle_list;
  TrackingActionMessenger* m_messenger; 
  G4bool m_draw_optical_photon;
};
}

#endif


