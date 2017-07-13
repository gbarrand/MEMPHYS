#ifndef MEMPHYSRunAction_h
#define MEMPHYSRunAction_h 1

//Geant4
#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4String.hh"

class G4Run;

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {

class RunActionMessenger;

class RunAction : public G4UserRunAction {
public:
  RunAction();
  virtual ~RunAction();

public:
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);

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
