#ifndef MEMPHYSPhysicsList_h
#define MEMPHYSPhysicsList_h 1

//Geant4
#include "G4VUserPhysicsList.hh"
#include "G4Version.hh"
#include "globals.hh"

//MEMPHYS
#include "PhysicsMessenger.hh"

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {


class PhysicsList: public G4VUserPhysicsList {
  public:
    PhysicsList();
    virtual ~PhysicsList();

  //=================================
  // Added by JLR 2005-07-05
  //=================================
  // This method sets the model for 
  // hadronic secondary interactions
  void SetSecondaryHad(G4String hadval);
  private:
    PhysicsMessenger* physicsMessenger;

    G4String SecondaryHadModel;

#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER<950 //G.Barrand
    G4bool gheishahad;
    G4bool bertinihad;
    G4bool binaryhad;
#endif
  
  protected:
    // Construct particle and physics process
    virtual void ConstructParticle();
    virtual void ConstructProcess();
  
  protected:
    void ConstructEM();
    void ConstructOp();
    void ConstructGeneral();
    virtual void SetCuts();

  protected:
    // these methods Construct physics processes and register them
    void ConstructHad();
};

}
#endif







