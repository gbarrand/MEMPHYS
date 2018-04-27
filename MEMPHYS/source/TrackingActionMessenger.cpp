#include "../MEMPHYS/TrackingActionMessenger.hh"

//Geant4
#include "G4UIdirectory.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4UIcmdWithABool.hh"

//MEMPHYS
#include "../MEMPHYS/TrackingAction.hh"


MEMPHYS::TrackingActionMessenger::TrackingActionMessenger(MEMPHYS::TrackingAction* MEMPHYSTrackAction)
:MEMPHYSTrackingAction(MEMPHYSTrackAction) {
  
  MEMPHYSDir = new G4UIdirectory("/MEMPHYS/tracking/drawing/");
  MEMPHYSDir->SetGuidance("Commands to define particle drawing option");

  drawOpticalPhotonCmd = new G4UIcmdWithABool("/MEMPHYS/tracking/drawing/optical",this);
  drawOpticalPhotonCmd->SetGuidance("Available options: true false");
  drawOpticalPhotonCmd->SetGuidance("Description:");
  drawOpticalPhotonCmd->SetGuidance("true  = optical photons are stored to be drawn");
  drawOpticalPhotonCmd->SetGuidance("flase = no optical photons stored");
  drawOpticalPhotonCmd->SetParameterName("OptPhotonOption", false);
  drawOpticalPhotonCmd->SetDefaultValue(false);
}//Ctor

//---------------------------------------------------------------------------------

MEMPHYS::TrackingActionMessenger::~TrackingActionMessenger() {
  delete drawOpticalPhotonCmd;
  delete MEMPHYSDir;
}//Dtor

//---------------------------------------------------------------------------------

void MEMPHYS::TrackingActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
  if (command == drawOpticalPhotonCmd) {
    MEMPHYSTrackingAction->SetDrawOpticalPhoton(StoB(newValue));
  }
}//SetNewValue
