#ifndef MEMPHYSEventAction_h
#define MEMPHYSEventAction_h 1

//Geant4
#include "G4UserEventAction.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"
#include "G4ios.hh"

//MEMPHYS
#include "MEMPHYS/DetectorConstruction.hh"
#include "MEMPHYS/WCHit.hh"
#include "MEMPHYS/WCDigi.hh"


class G4Event;

//JEC 10/11/05 introduce AIDA
namespace AIDA {
  class ITuple;
}
//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {

class Analysis; 
class RunAction;
class PrimaryGeneratorAction;

class EventAction : public G4UserEventAction {
  
 public:
  EventAction(Analysis&,
	      RunAction*, 
	      DetectorConstruction*,
	      PrimaryGeneratorAction*);
  virtual ~EventAction();
  
 public:
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);
  
  RunAction* GetRunAction(){return runAction;}
  
 private:
  G4int EventFindStartingVolume(G4ThreeVector vtx);
  G4int EventFindStoppingVolume(G4String stopVolumeName);
  
 private:
  //JEC FIXME is it necessary to maintain fAnalysis?
  Analysis&               fAnalysis;            //the Analysis
  RunAction*              runAction;            //the Run
  PrimaryGeneratorAction* generatorAction;      //the Generator 
  DetectorConstruction*   detectorConstructor;  //the Detector

  AIDA::ITuple*           eventTuple;           //the Event tuple
  AIDA::ITuple*           hitTimeTuple;         //the Hit Time tuple JEC 5/4/06

};

}

#endif

    
