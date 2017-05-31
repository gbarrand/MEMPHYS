#ifndef MEMPHYSPhysicsList_h
#define MEMPHYSPhysicsList_h 1

//Geant4
#include "G4VUserPhysicsList.hh"
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

    G4bool gheishahad;
    G4bool bertinihad;
    G4bool binaryhad;

  protected:
    // Construct particle and physics process
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();

  protected:
    // these methods Construct particles 
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();

  protected:
    // these methods Construct physics processes and register them
    void ConstructGeneral();
    void ConstructEM();
    void ConstructOp();
    void ConstructHad();
};

}
#endif







