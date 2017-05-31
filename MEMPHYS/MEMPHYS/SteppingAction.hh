#ifndef MEMPHYSSteppingAction_H
#define MEMPHYSSteppingACtion_H 1

//Geant 4
#include "globals.hh"
#include "G4UserSteppingAction.hh"

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction();
  virtual ~SteppingAction();
  virtual void UserSteppingAction(const G4Step*);

  //  void SetOneStepPrimaries(G4bool b){oneStepPrimaries=b;}
  // G4bool GetOneStepPrimaries(){return oneStepPrimaries;}
  
private:

  // G4bool oneStepPrimaries;
};
}
#endif
