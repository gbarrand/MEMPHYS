#ifndef MEMPHYSRunAction_h
#define MEMPHYSRunAction_h 1

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UserRunAction.hh"

namespace MEMPHYS {

class RunAction;
  
class RunActionMessenger: public G4UImessenger {
public:
  RunActionMessenger(RunAction*) { 
    MEMPHYSIODir = new G4UIdirectory("/MEMPHYSIO/");
    MEMPHYSIODir->SetGuidance("Commands to select I/O options");
  }
  virtual ~RunActionMessenger() {
    delete MEMPHYSIODir;
  }
public:
  virtual void SetNewValue(G4UIcommand* command, G4String newValues) {}
private: //commands
  G4UIdirectory* MEMPHYSIODir;
};

class RunAction : public G4UserRunAction {
public:
  RunAction() {messenger = new RunActionMessenger(this);}

  virtual ~RunAction() {}

public:
  void BeginOfRunAction(const G4Run* ) {
    numberOfEventsGenerated = 0;
    numberOfTimesWaterTubeHit = 0;
    numberOfTimesCatcherHit = 0;
    numberOfTimesRockHit = 0;
  }
  void EndOfRunAction(const G4Run*) {
    G4cout << "(JEC)>>>>>>> End of RunAction" << G4endl;
    G4cout << "Number of Events Generated: "<< numberOfEventsGenerated << G4endl;
    G4cout<<"Number of times waterTube hit: " << numberOfTimesWaterTubeHit<<G4endl;
    G4cout << "Number of times Catcher hit: " << numberOfTimesCatcherHit<<G4endl;
    G4cout << "Number of times Rock hit: " << numberOfTimesRockHit<<G4endl;
    G4cout << (float(numberOfTimesCatcherHit)/float(numberOfEventsGenerated))*100.
           << "% through-going (hit Catcher)" << G4endl;
  }

  int  GetNumberOfEventsGenerated() { return numberOfEventsGenerated;}
  void incrementEventsGenerated() { numberOfEventsGenerated++;}
  void incrementWaterTubeHits()   { numberOfTimesWaterTubeHit++;}  
  void incrementCatcherHits()     { numberOfTimesCatcherHit++;}
  void incrementRockHits()        { numberOfTimesRockHit++;}

private:

  int numberOfEventsGenerated;
  int numberOfTimesWaterTubeHit;
  int numberOfTimesCatcherHit;
  int numberOfTimesRockHit;

  RunActionMessenger* messenger;
};
}
#endif
