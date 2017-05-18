#include "../MEMPHYS/PhysicsMessenger.hh"

//Geant4
#include "G4UIdirectory.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4UIcmdWithAString.hh"

//MEMPHYS
#include "../MEMPHYS/PhysicsList.hh"


MEMPHYS::PhysicsMessenger::PhysicsMessenger(MEMPHYS::PhysicsList* MEMPHYSPhys)
:MEMPHYSPhysics(MEMPHYSPhys) {
  
  MEMPHYSDir = new G4UIdirectory("/MEMPHYS/physics/secondaries/");
  MEMPHYSDir->SetGuidance("Commands to change secondary interaction model for protons");

  hadmodelCmd = new G4UIcmdWithAString("/MEMPHYS/physics/secondaries/model",this);
  hadmodelCmd->SetGuidance("Available options: GHEISHA BERTINI BINARY");
  hadmodelCmd->SetGuidance("Description:");
  hadmodelCmd->SetGuidance("GHEISHA = standard, fast G4 hadronic interaction model");
  hadmodelCmd->SetGuidance("BERTINI = Bertini cascade model");
  hadmodelCmd->SetGuidance("BINARY  = Binary cascade model (2KM default)");
  hadmodelCmd->SetParameterName("secondaries", true, false);
  hadmodelCmd->SetDefaultValue("BINARY");
  hadmodelCmd->SetCandidates("GHEISHA BERTINI BINARY");

}//Ctor

//---------------------------------------------------------------------------------

MEMPHYS::PhysicsMessenger::~PhysicsMessenger() {
  delete hadmodelCmd;
  delete MEMPHYSDir;
}//Dtor

//---------------------------------------------------------------------------------

void MEMPHYS::PhysicsMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
  if (command == hadmodelCmd) MEMPHYSPhysics->SetSecondaryHad(newValue);
}//SetNewValue
