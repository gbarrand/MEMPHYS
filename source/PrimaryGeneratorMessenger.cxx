#include "MEMPHYS/PrimaryGeneratorMessenger.hh"

//Geant4
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4ios.hh"

//MEMPHYS
#include "MEMPHYS/PrimaryGeneratorAction.hh"


MEMPHYS::PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(MEMPHYS::PrimaryGeneratorAction* pointerToAction)
:myAction(pointerToAction) {

  mydetDirectory = new G4UIdirectory("/mygen/");
  mydetDirectory->SetGuidance("MEMPHYS detector control commands.");

  genCmd = new G4UIcmdWithAString("/mygen/generator",this);
  genCmd->SetGuidance("Select primary generator.");
  genCmd->SetGuidance(" Available generators : muline, normal");
  genCmd->SetParameterName("generator",true);
  genCmd->SetDefaultValue("normal");
  genCmd->SetCandidates("muline normal");

  fileNameCmd = new G4UIcmdWithAString("/mygen/vecfile",this);
  fileNameCmd->SetGuidance("Select the file of vectors.");
  fileNameCmd->SetGuidance(" Enter the file name of the vector file");
  fileNameCmd->SetParameterName("fileName",true);
  fileNameCmd->SetDefaultValue("inputvectorfile");
}//Ctor

//--------------------------------------------------------------------------------------------

MEMPHYS::PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger() {
  delete genCmd;
  delete fileNameCmd; //JEC 24/1/06
  delete mydetDirectory;
}//Dtor

//--------------------------------------------------------------------------------------------

void MEMPHYS::PrimaryGeneratorMessenger::SetNewValue(G4UIcommand * command,G4String newValue) {
  if ( command == genCmd ) {
    if ( newValue == "muline" ) {
      myAction->SetMulineEvtGenerator(true);
      myAction->SetNormalEvtGenerator(false);
    } else if ( newValue == "normal" ) {
      myAction->SetMulineEvtGenerator(false);
      myAction->SetNormalEvtGenerator(true);
    }
  }

  if ( command == fileNameCmd ) {
    myAction->OpenVectorFile(newValue);
    G4cout << "Input vector file set to " << newValue << G4endl;
  }

}//SetNewValue

//--------------------------------------------------------------------------------------------


G4String MEMPHYS::PrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand* command) {
  G4String cv;
  
  if( command == genCmd ) {
    if( myAction->IsUsingMulineEvtGenerator() ) { cv = "muline"; }
    if( myAction->IsUsingNormalEvtGenerator() ) { cv = "normal"; }
  }
  
  return cv;
}//GetCurrentValue

