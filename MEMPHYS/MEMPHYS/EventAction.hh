#ifndef MEMPHYSEventAction_h
#define MEMPHYSEventAction_h 1

//Geant4
#include "G4UserEventAction.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"
#include "G4ios.hh"

//MEMPHYS
#include "DetectorConstruction.hh"
#include "WCHit.hh"
#include "WCDigi.hh"


class G4Event;

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {

class Analysis; 
class RunAction;
class PrimaryGeneratorAction;

class EventAction : public G4UserEventAction {
  
public:
  EventAction(Analysis&,
	      RunAction&, 
	      DetectorConstruction&,
	      PrimaryGeneratorAction&);
  virtual ~EventAction() {}
  
public:
  void BeginOfEventAction(const G4Event* /*evt*/){
    if(m_verbose==1) G4cout << " (JEC) EventAction::Begin EventAction" << G4endl;
    //if (evt) evt->Print();
  }

  void EndOfEventAction(const G4Event*);
  
  //RunAction& GetRunAction(){return runAction;}
  
  void set_verbose(unsigned int a_verbose) {m_verbose = a_verbose;} //GB
 private:
  G4int EventFindStartingVolume(G4ThreeVector vtx);
  G4int EventFindStoppingVolume(G4String stopVolumeName);
  
  void fill_track(int pId,int parent,float timeStart,
                  double dx,double dy,double dz,
                  double mass,double pTot, double ETot,double px,double py,double pz,
                  double startPos_x,double startPos_y,double startPos_z,
                  double stopPos_x,double stopPos_y,double stopPos_z,
                  int startVol,int stopVol);
  void fill_hit(int tubeID_hit,int totalPE);
  void fill_digit(int tubeID,double tubePhotoElectrons,double tubeTime);
  void fill_event(int event_id,int vecRecNumber,int mode,int vtxVol,
                  double vtx_x,double vtx_y,double vtx_z,int ntrack,
                  int leadingLeptonIndex,int outgoingProtonIndex,
                  int nHits,int nDigits,double sumPE);
  void fill_hit_time(float peArrivalTime) {}

 private:
  //JEC FIXME is it necessary to maintain fAnalysis?
  Analysis&               fAnalysis;            //the Analysis
  RunAction&              fRunAction;            //the Run
  PrimaryGeneratorAction& fGeneratorAction;      //the Generator 
  DetectorConstruction&   fDetectorConstructor;  //the Detector
  unsigned int m_verbose; //GB
};

}

#endif

    
