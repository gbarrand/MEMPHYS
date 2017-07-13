#include "../MEMPHYS/RunAction.hh"

//Geant4
#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"

//MEMPHYS
#include "../MEMPHYS/Cast.hh"
#include "../MEMPHYS/RunActionMessenger.hh"

//JEC: Fixme: Analyse should implement the RootEvent/RootGeom part...

MEMPHYS::RunAction::RunAction() {
  // Messenger to allow IO options
  messenger = new RunActionMessenger(this);
}//Ctor

//----------------------------------------------------------------------------------

MEMPHYS::RunAction::~RunAction() {
}//Dtor

//----------------------------------------------------------------------------------
void MEMPHYS::RunAction::BeginOfRunAction(const G4Run* ) {
  numberOfEventsGenerated = 0;
  numberOfTimesWaterTubeHit = 0;
  numberOfTimesCatcherHit = 0;
  numberOfTimesRockHit = 0;
}//BeginOfRunAction

//----------------------------------------------------------------------------------

void MEMPHYS::RunAction::EndOfRunAction(const G4Run*) {
  G4cout << "(JEC)>>>>>>> End of RunAction" << G4endl;
  G4cout << "Number of Events Generated: "<< numberOfEventsGenerated << G4endl;
  G4cout<<"Number of times waterTube hit: " << numberOfTimesWaterTubeHit<<G4endl;
  G4cout << "Number of times Catcher hit: " << numberOfTimesCatcherHit<<G4endl;
  G4cout << "Number of times Rock hit: " << numberOfTimesRockHit<<G4endl;
  G4cout << (float(numberOfTimesCatcherHit)/float(numberOfEventsGenerated))*100.
	 << "% through-going (hit Catcher)" << G4endl;
}//EndOfRunAction


