#include "MEMPHYS/TrackingAction.hh"

//Geant4
#include "G4ParticleTypes.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"

//MEMPHYS
#include "MEMPHYS/Trajectory.hh"
#include "MEMPHYS/TrackInformation.hh"
#include "MEMPHYS/TrackingActionMessenger.hh" //JEC 24/1/06

MEMPHYS::TrackingAction::TrackingAction() {
  ProcessList.insert("Decay") ;
  ProcessList.insert("MuonMinusCaptureAtRest") ; //JEC 15/12/05 uncomment this process
  ParticleList.insert(111); // pi0
  ParticleList.insert(211); // pion+
  ParticleList.insert(-211);
  ParticleList.insert(321);
  ParticleList.insert(-321); // kaon-
  ParticleList.insert(311); // kaon0
  ParticleList.insert(-311); // kaon0 bar
  // don't put gammas there or there'll be too many

  //JEC 24/1/06 make the TrackingAction Messenger vailable
  messenger = new TrackingActionMessenger(this);
  drawOpticalPhoton = false;

}//Ctor

//--------------------------------------------------------------------------------------------------

MEMPHYS::TrackingAction::~TrackingAction(){
  //JEC 24/1/06
  delete messenger;
  messenger = 0;
}//Dtor

//---------------------------------------------------------------------------------------------------

void MEMPHYS::TrackingAction::PreUserTrackingAction(const G4Track* aTrack) {

  //  G4cout << "(JEC) PreUserTrackingAction : INFO : BEGIN" << G4endl;

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

  G4double thresholdTobeSaved = 10.0*MeV; //JEC FIXME put in the messenger

  if( ( aTrack->GetParentID()==0 ) || 
      ( (creatorProcess!=0) && ProcessList.count(creatorProcess->GetProcessName()) ) || 
      ( ParticleList.count(aTrack->GetDefinition()->GetPDGEncoding()) ) || 
      ( (aTrack->GetDefinition()->GetPDGEncoding() == 22) && 
	(aTrack->GetTotalEnergy() > thresholdTobeSaved) )  ) {

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


}//PreUserTrackingAction

//---------------------------------------------------------------------------------------------------


void MEMPHYS::TrackingAction::PostUserTrackingAction(const G4Track* aTrack) {

  // Get the Track Information 
  TrackInformation* aInfo = (TrackInformation*)(aTrack->GetUserInformation());
  

//   G4cout << "(JEC) PostUserTrackingAction : INFO : Track Status <"
// 	 << aInfo->GetTrackStatus() << ">"
// 	 << G4endl;

  Trajectory *currentTrajectory = (Trajectory*)fpTrackingManager->GimmeTrajectory();

  if ( aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() ) {

//     G4cout << "(JEC) PostUserTrackingAction : INFO : Track is an Optical Photon"
// 	   << G4endl;

    //JEC 24/1/06 add a condition based on messenger info to draw or not to draw
    if ( (aInfo->GetTrackStatus()&hitPMT) && (drawOpticalPhoton) ) {
	aInfo->SetDrawOption(true);
// 	G4cout << "(JEC) PostUserTrackingAction : INFO : Opticel Photon hit a PMT"
// 	       << G4endl;
      }

  }//Partcile type "optical photon" or not


  if ( aInfo->GetSaveOption() ) {
//     G4cout << "(JEC) PostUserTrackingAction : INFO : Track should be saved"
// 	   << G4endl;
    G4ThreeVector currentPosition      = aTrack->GetPosition();
    G4VPhysicalVolume* currentVolume   = aTrack->GetVolume();
    
    currentTrajectory->SetStoppingPoint(currentPosition);
    currentTrajectory->SetStoppingVolume(currentVolume);
    currentTrajectory->SetSaveFlag(true); //For saving into Tuple (see EventAction)
  }//save the trajectory
  
  if ( ! ( aInfo->GetDrawOption() || aInfo->GetSaveOption() ) ) {
    //do no keep the trajectory
    fpTrackingManager->SetStoreTrajectory(false);
  }//remove the trajectory

}//PostUserTrackingAction




