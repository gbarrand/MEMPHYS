#include "MEMPHYS/DetectorMessenger.hh"

//Geant 4
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"

//MEMPHYS
#include "MEMPHYS/DetectorConstruction.hh"

MEMPHYS::DetectorMessenger::DetectorMessenger(MEMPHYS::DetectorConstruction* MEMPHYSDet)
: MEMPHYSDetector(MEMPHYSDet) { 

  MEMPHYSDir = new G4UIdirectory("/MEMPHYS/");
  MEMPHYSDir->SetGuidance("Commands to change the geometry of the simulation");
  
  
  PMTConfig = new G4UIcmdWithAString("/MEMPHYS/WCgeom",this);
  PMTConfig->SetGuidance("Set the geometry configuration for the WC.");
  PMTConfig->SetGuidance("Available options 8inch");
  PMTConfig->SetParameterName("PMTConfig", false);
  PMTConfig->SetDefaultValue("8inch");
  PMTConfig->SetCandidates("8inch");
  PMTConfig->AvailableForStates(G4State_PreInit, G4State_Idle);
  
} //MEMPHYSDetectorMessenger

MEMPHYS::DetectorMessenger::~DetectorMessenger()
{
  delete PMTConfig;
  delete MEMPHYSDir;
} //MEMPHYSDetectorMessenger

void MEMPHYS::DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue) {    
  if( command == PMTConfig )
  { 
    if (newValue == "8inch") {
      MEMPHYSDetector->Set8inchPMTGeometry();
    } else {
      G4cout << "That geometry choice not defined!" << G4endl;
    }
  }

  MEMPHYSDetector->UpdateGeometry();

}//SetNewValue
