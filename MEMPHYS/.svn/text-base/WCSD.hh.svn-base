#ifndef MEMPHYSWCSD_h
#define MEMPHYSWCSD_h 1

//Geant4
#include "G4VSensitiveDetector.hh"

//MEMPHYS
#include "MEMPHYS/WCHit.hh"

class G4Step;
class G4HCofThisEvent;

namespace MEMPHYS {

class DetectorConstruction; //JEC 25/1/06


class WCSD : public G4VSensitiveDetector {
 public:
  WCSD(G4String, DetectorConstruction*); //JEC 25/1/06
  virtual ~WCSD();
  
  void   Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  G4bool ProcessHits_constStep(const G4Step*, G4TouchableHistory*);
  void   EndOfEvent(G4HCofThisEvent*);
  
 private:

  WCHitsCollection* hitsCollection;
  std::map<int,int> PMTHitMap;   // Whether a PMT was hit already
  G4int HCID;

  
  DetectorConstruction* MEMPHYSDetConstruction; //JEC 25/1/06

};

}
#endif

