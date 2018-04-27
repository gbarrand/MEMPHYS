#include "../MEMPHYS/RunActionMessenger.hh"

//Geant4
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"

//MEMPHYS
#include "../MEMPHYS/RunAction.hh"

MEMPHYS::RunActionMessenger::RunActionMessenger(MEMPHYS::RunAction* /*MEMPHYSRA*/)
//:MEMPHYSRun(MEMPHYSRA)
{ 
  MEMPHYSIODir = new G4UIdirectory("/MEMPHYSIO/");
  MEMPHYSIODir->SetGuidance("Commands to select I/O options");
  
}//Ctor

//----------------------------------------------------------------------------------

MEMPHYS::RunActionMessenger::~RunActionMessenger() {

  delete MEMPHYSIODir;

}//Dtor

//----------------------------------------------------------------------------------

void MEMPHYS::RunActionMessenger::SetNewValue(G4UIcommand* /*command*/,
					      G4String /*newValue*/) {    
  

}//SetNewValue
