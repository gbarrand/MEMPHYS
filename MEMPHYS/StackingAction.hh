#ifndef MEMPHYSStackingAction_H
#define MEMPHYSStackingAction_H 1

//Geant4
#include "globals.hh"
#include "G4UserStackingAction.hh"

class G4Track;

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {

class StackingAction : public G4UserStackingAction {

  public:
    StackingAction();
    virtual ~StackingAction();

  public:
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    virtual void NewStage();
    virtual void PrepareNewEvent();
};
}
#endif

