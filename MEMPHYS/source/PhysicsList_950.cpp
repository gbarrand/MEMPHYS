// Physics List
//this
#include "../MEMPHYS/PhysicsList.hh"

//Geant 4
#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4HadronCaptureProcess.hh"
#include "G4Material.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include "globals.hh"
#include "G4Version.hh"

//MEMPHYS
#include "../MEMPHYS/PhysicsMessenger.hh"

using namespace CLHEP; //G.Barrand

MEMPHYS::PhysicsList::PhysicsList() 
: G4VUserPhysicsList()
,physicsMessenger(0)
,gheishahad(false)
,bertinihad(false)
,binaryhad(false)
{
  
  defaultCutValue = 1.0*mm;
  SetVerboseLevel(1);
  
  physicsMessenger = new PhysicsMessenger(this);
}//PhysicsList

//-----------------------------------------------------------------------------------------------

MEMPHYS::PhysicsList::~PhysicsList() {
  delete physicsMessenger;
  physicsMessenger = 0;
}//PhysicsList

//-----------------------------------------------------------------------------------------------
void MEMPHYS::PhysicsList::ConstructParticle() {
  G4LeptonConstructor leptonConstructor;
  G4MesonConstructor  mesonConstructor;
  G4BaryonConstructor baryonConstructor;
  G4BosonConstructor bosonConstructor;
  G4IonConstructor ionConstructor;
  leptonConstructor.ConstructParticle();
  mesonConstructor.ConstructParticle();
  baryonConstructor.ConstructParticle();
  bosonConstructor.ConstructParticle();
  ionConstructor.ConstructParticle();
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
  G4OpticalPhoton::OpticalPhotonDefinition();
#endif  
} //ConstructParticle

//-----------------------------------------------------------------------------------------------

void MEMPHYS::PhysicsList::ConstructProcess() {
  AddTransportation();
  ConstructEM();
  ConstructOp();
  ConstructGeneral();
  ConstructHad();
}//ConstructProcess

//-----------------------------------------------------------------------------------------------



#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#else  
#include "G4MultipleScattering.hh"
#include "G4MuonMinusCaptureAtRest.hh"
#endif
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4hIonisation.hh"


void MEMPHYS::PhysicsList::ConstructEM() {
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=1031 //G.Barrand
  auto theParticleIterator=GetParticleIterator();
#endif  
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
     
    if (particleName == "gamma") {
    // gamma
      pmanager->AddDiscreteProcess(new G4GammaConversion());
      pmanager->AddDiscreteProcess(new G4ComptonScattering());      
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());

    } else if (particleName == "e-") {
    //electron
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
      G4VProcess* theeminusMultipleScattering = new G4eMultipleScattering();
#else  
      G4VProcess* theeminusMultipleScattering = new G4MultipleScattering();
#endif      
      G4VProcess* theeminusIonisation         = new G4eIonisation();
      G4VProcess* theeminusBremsstrahlung     = new G4eBremsstrahlung();
      //
      // add processes
      pmanager->AddProcess(theeminusMultipleScattering);
      pmanager->AddProcess(theeminusIonisation);
      pmanager->AddProcess(theeminusBremsstrahlung);
      //      
      // set ordering for AlongStepDoIt
      pmanager->SetProcessOrdering(theeminusMultipleScattering,idxAlongStep,1);
      pmanager->SetProcessOrdering(theeminusIonisation,        idxAlongStep,2);
      //
      // set ordering for PostStepDoIt
      pmanager->SetProcessOrdering(theeminusMultipleScattering, idxPostStep,1);
      pmanager->SetProcessOrdering(theeminusIonisation,         idxPostStep,2);
      pmanager->SetProcessOrdering(theeminusBremsstrahlung,     idxPostStep,3);

    } else if (particleName == "e+") {
    //positron
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
      G4VProcess* theeplusMultipleScattering = new G4eMultipleScattering();
#else  
      G4VProcess* theeplusMultipleScattering = new G4MultipleScattering();
#endif      
      G4VProcess* theeplusIonisation         = new G4eIonisation();
      G4VProcess* theeplusBremsstrahlung     = new G4eBremsstrahlung();
      G4VProcess* theeplusAnnihilation       = new G4eplusAnnihilation();
      //
      // add processes
      pmanager->AddProcess(theeplusMultipleScattering);
      pmanager->AddProcess(theeplusIonisation);
      pmanager->AddProcess(theeplusBremsstrahlung);
      pmanager->AddProcess(theeplusAnnihilation);
      //
      // set ordering for AtRestDoIt
      pmanager->SetProcessOrderingToFirst(theeplusAnnihilation, idxAtRest);
      //
      // set ordering for AlongStepDoIt
      pmanager->SetProcessOrdering(theeplusMultipleScattering, idxAlongStep,1);
      pmanager->SetProcessOrdering(theeplusIonisation,         idxAlongStep,2);
      //
      // set ordering for PostStepDoIt
      pmanager->SetProcessOrdering(theeplusMultipleScattering, idxPostStep,1);
      pmanager->SetProcessOrdering(theeplusIonisation,         idxPostStep,2);
      pmanager->SetProcessOrdering(theeplusBremsstrahlung,     idxPostStep,3);
      pmanager->SetProcessOrdering(theeplusAnnihilation,       idxPostStep,4);

    } else if( particleName == "mu+" ||
               particleName == "mu-"    ) {
    //muon
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
      G4VProcess* aMultipleScattering = new G4MuMultipleScattering();
#else  
      G4VProcess* aMultipleScattering = new G4MultipleScattering();
#endif      
      G4VProcess* aBremsstrahlung     = new G4MuBremsstrahlung();
      G4VProcess* aPairProduction     = new G4MuPairProduction();
      G4VProcess* anIonisation        = new G4MuIonisation();
      //
      // add processes
      pmanager->AddProcess(anIonisation);
      pmanager->AddProcess(aMultipleScattering);
      pmanager->AddProcess(aBremsstrahlung);
      pmanager->AddProcess(aPairProduction);
      //
      // set ordering for AlongStepDoIt
      pmanager->SetProcessOrdering(aMultipleScattering, idxAlongStep,1);
      pmanager->SetProcessOrdering(anIonisation,        idxAlongStep,2);
      //
      // set ordering for PostStepDoIt
      pmanager->SetProcessOrdering(aMultipleScattering, idxPostStep,1);
      pmanager->SetProcessOrdering(anIonisation,        idxPostStep,2);
      pmanager->SetProcessOrdering(aBremsstrahlung,     idxPostStep,3);
      pmanager->SetProcessOrdering(aPairProduction,     idxPostStep,4);

#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else  
      // MF , stolen from CWW, april 2005
      if (particleName == "mu-")
        {
          G4VProcess* aG4MuonMinusCaptureAtRest =
            new G4MuonMinusCaptureAtRest();
          pmanager->AddProcess(aG4MuonMinusCaptureAtRest);
          pmanager->SetProcessOrdering(aG4MuonMinusCaptureAtRest,idxAtRest);
        }


    } else if ((!particle->IsShortLived()) &&
	       (particle->GetPDGCharge() != 0.0)&&
               (particle->GetParticleName() != "chargedgeantino")) {
     G4VProcess* aMultipleScattering = new G4MultipleScattering();
     G4VProcess* anIonisation        = new G4hIonisation();
     //
     // add processes
     pmanager->AddProcess(anIonisation);
     pmanager->AddProcess(aMultipleScattering);
     //
     // set ordering for AlongStepDoIt
     pmanager->SetProcessOrdering(aMultipleScattering, idxAlongStep,1);
     pmanager->SetProcessOrdering(anIonisation,        idxAlongStep,2);
     //
     // set ordering for PostStepDoIt
     pmanager->SetProcessOrdering(aMultipleScattering, idxPostStep,1);
     pmanager->SetProcessOrdering(anIonisation,        idxPostStep,2);
#endif  
    }
  }
}//ConstructEM

//-----------------------------------------------------------------------------------------------


#include "G4Cerenkov.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#include "G4Scintillation.hh"
#include "G4OpMieHG.hh"
#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"
#endif

void MEMPHYS::PhysicsList::ConstructOp(){

  G4Cerenkov*          theCerenkovProcess           = new G4Cerenkov("Cerenkov");
  G4OpAbsorption*      theAbsorptionProcess         = new G4OpAbsorption();
  G4OpRayleigh*        theRayleighScatteringProcess = new G4OpRayleigh();
  G4OpBoundaryProcess* theBoundaryProcess           = new G4OpBoundaryProcess();

//   theCerenkovProcess->DumpPhysicsTable();
//   theAbsorptionProcess->DumpPhysicsTable();
//   theRayleighScatteringProcess->DumpPhysicsTable();

  theCerenkovProcess->SetVerboseLevel(0);
  theAbsorptionProcess->SetVerboseLevel(0);
  theRayleighScatteringProcess->SetVerboseLevel(0);
  theBoundaryProcess->SetVerboseLevel(0);

#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
  theCerenkovProcess->SetMaxNumPhotonsPerStep(20);
  theCerenkovProcess->SetMaxBetaChangePerStep(10.0);
  
  G4Scintillation* theScintillationProcess = new G4Scintillation("Scintillation");
  theScintillationProcess->SetScintillationYieldFactor(1.);
  theScintillationProcess->SetTrackSecondariesFirst(true);
  
  // Use Birks Correction in the Scintillation process
  //if(G4Threading::IsMasterThread()) {
  //  G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
  //  theScintillationProcess->AddSaturation(emSaturation);
  //}
  
  G4OpMieHG* theMieHGScatteringProcess = new G4OpMieHG();
#else
  G4int MaxNumPhotons = 300; //JEC: to be FIXED ?
  theCerenkovProcess->SetMaxNumPhotonsPerStep(MaxNumPhotons);
#endif  
  theCerenkovProcess->SetTrackSecondariesFirst(true);

#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=1031 //G.Barrand
  auto theParticleIterator=GetParticleIterator();
#else  
  G4OpticalSurfaceModel themodel = unified; //JEC where does it come from?
  theBoundaryProcess->SetModel(themodel);
#endif
  theParticleIterator->reset();
  while( (*theParticleIterator)() )
  {
    G4ParticleDefinition* particle     = theParticleIterator->value();
    G4ProcessManager*     pmanager     = particle->GetProcessManager();
    G4String              particleName = particle->GetParticleName();

    if (theCerenkovProcess->IsApplicable(*particle)) 
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
      pmanager->AddProcess(theCerenkovProcess);
      pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
#else    
      pmanager->AddContinuousProcess(theCerenkovProcess);
#endif

    if (particleName == "opticalphoton") 
    {
      pmanager->AddDiscreteProcess(theAbsorptionProcess);
      //     G4cout << "warning direct light only\n";
      pmanager->AddDiscreteProcess(theRayleighScatteringProcess);
      pmanager->AddDiscreteProcess(theBoundaryProcess);
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
      pmanager->AddDiscreteProcess(theMieHGScatteringProcess);
#endif  
    }
  }
}//ConstructOp

//-----------------------------------------------------------------------------------------------

#include "G4Decay.hh"

void MEMPHYS::PhysicsList::ConstructGeneral() {
  // Add Decay Process
  G4Decay* theDecayProcess = new G4Decay();
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=1031 //G.Barrand
  auto theParticleIterator=GetParticleIterator();
#endif
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (theDecayProcess->IsApplicable(*particle)) { 
      pmanager ->AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }
}//ConstructGeneral

//-----------------------------------------------------------------------------------------------
//--- Hadronic ----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
// G.Barrand : take the code of 
//   geant4-09-05-ref-00 examples/advanced/underground_physics/src/DMXPhysicsList.cc


// Elastic processes:
#include "G4HadronElasticProcess.hh"

// Inelastic processes:
#include "G4PionPlusInelasticProcess.hh"
#include "G4PionMinusInelasticProcess.hh"
#include "G4KaonPlusInelasticProcess.hh"
#include "G4KaonZeroSInelasticProcess.hh"
#include "G4KaonZeroLInelasticProcess.hh"
#include "G4KaonMinusInelasticProcess.hh"
#include "G4ProtonInelasticProcess.hh"
#include "G4AntiProtonInelasticProcess.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4AntiNeutronInelasticProcess.hh"
#include "G4DeuteronInelasticProcess.hh"
#include "G4TritonInelasticProcess.hh"
#include "G4AlphaInelasticProcess.hh"

// Low-energy Models: < 20GeV
#include "G4LElastic.hh"
#include "G4LEPionPlusInelastic.hh"
#include "G4LEPionMinusInelastic.hh"
#include "G4LEKaonPlusInelastic.hh"
#include "G4LEKaonZeroSInelastic.hh"
#include "G4LEKaonZeroLInelastic.hh"
#include "G4LEKaonMinusInelastic.hh"
#include "G4LEProtonInelastic.hh"
#include "G4LEAntiProtonInelastic.hh"
#include "G4LENeutronInelastic.hh"
#include "G4LEAntiNeutronInelastic.hh"
#include "G4LEDeuteronInelastic.hh"
#include "G4LETritonInelastic.hh"
#include "G4LEAlphaInelastic.hh"
#include "G4HadronCaptureProcess.hh"
// High-energy Models: >20 GeV
#include "G4HEPionPlusInelastic.hh"
#include "G4HEPionMinusInelastic.hh"
#include "G4HEKaonPlusInelastic.hh"
#include "G4HEKaonZeroInelastic.hh"
#include "G4HEKaonZeroInelastic.hh"
#include "G4HEKaonMinusInelastic.hh"
#include "G4HEProtonInelastic.hh"
#include "G4HEAntiProtonInelastic.hh"
#include "G4HENeutronInelastic.hh"
#include "G4HEAntiNeutronInelastic.hh"

// Neutron high-precision models: <20 MeV
#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPElasticData.hh"
#include "G4NeutronHPCapture.hh"
#include "G4NeutronHPCaptureData.hh"
#include "G4NeutronHPInelastic.hh"
#include "G4NeutronHPInelasticData.hh"
#include "G4LCapture.hh"

// Stopping processes
#include "G4PiMinusAbsorptionAtRest.hh"
#include "G4KaonMinusAbsorptionAtRest.hh"
#include "G4AntiProtonAnnihilationAtRest.hh"
#include "G4AntiNeutronAnnihilationAtRest.hh"


// ConstructHad()
// Makes discrete physics processes for the hadrons, at present limited
// to those particles with GHEISHA interactions (INTRC > 0).
// The processes are: Elastic scattering and Inelastic scattering.
// F.W.Jones  09-JUL-1998
//void DMXPhysicsList::ConstructHad() 
void MEMPHYS::PhysicsList::ConstructHad() 
{
  G4HadronElasticProcess* theElasticProcess = new G4HadronElasticProcess;
  G4LElastic* theElasticModel = new G4LElastic;
  theElasticProcess->RegisterMe(theElasticModel);
  
  theParticleIterator->reset();
  while ((*theParticleIterator)()) 
    {
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();
      G4String particleName = particle->GetParticleName();

      if (particleName == "pi+") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4PionPlusInelasticProcess* theInelasticProcess = 
	    new G4PionPlusInelasticProcess("inelastic");
	  G4LEPionPlusInelastic* theLEInelasticModel = 
	    new G4LEPionPlusInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  G4HEPionPlusInelastic* theHEInelasticModel = 
	    new G4HEPionPlusInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	} 

      else if (particleName == "pi-") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4PionMinusInelasticProcess* theInelasticProcess = 
	    new G4PionMinusInelasticProcess("inelastic");
	  G4LEPionMinusInelastic* theLEInelasticModel = 
	    new G4LEPionMinusInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  G4HEPionMinusInelastic* theHEInelasticModel = 
	    new G4HEPionMinusInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	  G4String prcNam;
	  pmanager->AddRestProcess(new G4PiMinusAbsorptionAtRest, ordDefault);
	}
      
      else if (particleName == "kaon+") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4KaonPlusInelasticProcess* theInelasticProcess = 
	    new G4KaonPlusInelasticProcess("inelastic");
	  G4LEKaonPlusInelastic* theLEInelasticModel = 
	    new G4LEKaonPlusInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  G4HEKaonPlusInelastic* theHEInelasticModel = 
	    new G4HEKaonPlusInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}
      
      else if (particleName == "kaon0S") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4KaonZeroSInelasticProcess* theInelasticProcess = 
	    new G4KaonZeroSInelasticProcess("inelastic");
	  G4LEKaonZeroSInelastic* theLEInelasticModel = 
	    new G4LEKaonZeroSInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  G4HEKaonZeroInelastic* theHEInelasticModel = 
	    new G4HEKaonZeroInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}

      else if (particleName == "kaon0L") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4KaonZeroLInelasticProcess* theInelasticProcess = 
	    new G4KaonZeroLInelasticProcess("inelastic");
	  G4LEKaonZeroLInelastic* theLEInelasticModel = 
	    new G4LEKaonZeroLInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  G4HEKaonZeroInelastic* theHEInelasticModel = 
	    new G4HEKaonZeroInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}

      else if (particleName == "kaon-") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4KaonMinusInelasticProcess* theInelasticProcess = 
	    new G4KaonMinusInelasticProcess("inelastic");
	  G4LEKaonMinusInelastic* theLEInelasticModel = 
	    new G4LEKaonMinusInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  G4HEKaonMinusInelastic* theHEInelasticModel = 
	    new G4HEKaonMinusInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	  pmanager->AddRestProcess(new G4KaonMinusAbsorptionAtRest, ordDefault);
	}

      else if (particleName == "proton") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4ProtonInelasticProcess* theInelasticProcess = 
	    new G4ProtonInelasticProcess("inelastic");
	  G4LEProtonInelastic* theLEInelasticModel = new G4LEProtonInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  G4HEProtonInelastic* theHEInelasticModel = new G4HEProtonInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}

      else if (particleName == "anti_proton") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4AntiProtonInelasticProcess* theInelasticProcess = 
	    new G4AntiProtonInelasticProcess("inelastic");
	  G4LEAntiProtonInelastic* theLEInelasticModel = 
	    new G4LEAntiProtonInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  G4HEAntiProtonInelastic* theHEInelasticModel = 
	    new G4HEAntiProtonInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}

      else if (particleName == "neutron") {
	// elastic scattering
	G4HadronElasticProcess* theNeutronElasticProcess = 
	  new G4HadronElasticProcess;
	G4LElastic* theElasticModel1 = new G4LElastic;
	G4NeutronHPElastic * theElasticNeutron = new G4NeutronHPElastic;
	theNeutronElasticProcess->RegisterMe(theElasticModel1);
	theElasticModel1->SetMinEnergy(19*MeV);
	theNeutronElasticProcess->RegisterMe(theElasticNeutron);
	G4NeutronHPElasticData * theNeutronData = new G4NeutronHPElasticData;
	theNeutronElasticProcess->AddDataSet(theNeutronData);
	pmanager->AddDiscreteProcess(theNeutronElasticProcess);
	// inelastic scattering
	G4NeutronInelasticProcess* theInelasticProcess =
	  new G4NeutronInelasticProcess("inelastic");
	G4LENeutronInelastic* theInelasticModel = new G4LENeutronInelastic;
	theInelasticModel->SetMinEnergy(19*MeV);
	theInelasticProcess->RegisterMe(theInelasticModel);
	G4NeutronHPInelastic * theLENeutronInelasticModel =
	  new G4NeutronHPInelastic;
	theInelasticProcess->RegisterMe(theLENeutronInelasticModel);
	G4NeutronHPInelasticData * theNeutronData1 = 
	  new G4NeutronHPInelasticData;
	theInelasticProcess->AddDataSet(theNeutronData1);
	pmanager->AddDiscreteProcess(theInelasticProcess);
	// capture
	G4HadronCaptureProcess* theCaptureProcess =
	  new G4HadronCaptureProcess;
	G4LCapture* theCaptureModel = new G4LCapture;
	theCaptureModel->SetMinEnergy(19*MeV);
	theCaptureProcess->RegisterMe(theCaptureModel);
	G4NeutronHPCapture * theLENeutronCaptureModel = new G4NeutronHPCapture;
	theCaptureProcess->RegisterMe(theLENeutronCaptureModel);
	G4NeutronHPCaptureData * theNeutronData3 = new G4NeutronHPCaptureData;
	theCaptureProcess->AddDataSet(theNeutronData3);
	pmanager->AddDiscreteProcess(theCaptureProcess);
	//  G4ProcessManager* pmanager = G4Neutron::Neutron->GetProcessManager();
	//  pmanager->AddProcess(new G4UserSpecialCuts(),-1,-1,1);
      }
      else if (particleName == "anti_neutron") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4AntiNeutronInelasticProcess* theInelasticProcess = 
	    new G4AntiNeutronInelasticProcess("inelastic");
	  G4LEAntiNeutronInelastic* theLEInelasticModel = 
	    new G4LEAntiNeutronInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  G4HEAntiNeutronInelastic* theHEInelasticModel = 
	    new G4HEAntiNeutronInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}

      else if (particleName == "deuteron") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4DeuteronInelasticProcess* theInelasticProcess = 
	    new G4DeuteronInelasticProcess("inelastic");
	  G4LEDeuteronInelastic* theLEInelasticModel = 
	    new G4LEDeuteronInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}
      
      else if (particleName == "triton") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4TritonInelasticProcess* theInelasticProcess = 
	    new G4TritonInelasticProcess("inelastic");
	  G4LETritonInelastic* theLEInelasticModel = 
	    new G4LETritonInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}

      else if (particleName == "alpha") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4AlphaInelasticProcess* theInelasticProcess = 
	    new G4AlphaInelasticProcess("inelastic");
	  G4LEAlphaInelastic* theLEInelasticModel = 
	    new G4LEAlphaInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}

    }
}


#else //G4VERSION_NUMBER<950 //G.Barrand


// Elastic processes:
#include "G4HadronElasticProcess.hh"

// Inelastic processes:
#include "G4PionPlusInelasticProcess.hh"
#include "G4PionMinusInelasticProcess.hh"
#include "G4KaonPlusInelasticProcess.hh"
#include "G4KaonZeroSInelasticProcess.hh"
#include "G4KaonZeroLInelasticProcess.hh"
#include "G4KaonMinusInelasticProcess.hh"
#include "G4ProtonInelasticProcess.hh"
#include "G4AntiProtonInelasticProcess.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4AntiNeutronInelasticProcess.hh"
#include "G4DeuteronInelasticProcess.hh"
#include "G4TritonInelasticProcess.hh"
#include "G4AlphaInelasticProcess.hh"

#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
// Low-energy Models: < 20GeV
#include "G4LElastic.hh"
#include "G4LEPionPlusInelastic.hh"
#include "G4LEPionMinusInelastic.hh"
#include "G4LEKaonPlusInelastic.hh"
#include "G4LEKaonZeroSInelastic.hh"
#include "G4LEKaonZeroLInelastic.hh"
#include "G4LEKaonMinusInelastic.hh"
#include "G4LEProtonInelastic.hh"
#include "G4LEAntiProtonInelastic.hh"
#include "G4LENeutronInelastic.hh"
#include "G4LEAntiNeutronInelastic.hh"
#include "G4LEDeuteronInelastic.hh"
#include "G4LETritonInelastic.hh"
#include "G4LEAlphaInelastic.hh"

// High-energy Models: >20 GeV
#include "G4HEPionPlusInelastic.hh"
#include "G4HEPionMinusInelastic.hh"
#include "G4HEKaonPlusInelastic.hh"
#include "G4HEKaonZeroInelastic.hh"
#include "G4HEKaonZeroInelastic.hh"
#include "G4HEKaonMinusInelastic.hh"
#include "G4HEProtonInelastic.hh"
#include "G4HEAntiProtonInelastic.hh"
#include "G4HENeutronInelastic.hh"
#include "G4HEAntiNeutronInelastic.hh"
#endif

// Neutron high-precision models: <20 MeV
#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPElasticData.hh"
#include "G4NeutronHPCapture.hh"
#include "G4NeutronHPCaptureData.hh"
#include "G4NeutronHPInelastic.hh"
#include "G4NeutronHPInelasticData.hh"
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
#include "G4LCapture.hh"
#endif

//=================================
// Added by JLR 2005-07-05
//=================================
// Secondary hadronic interaction models
#include "G4CascadeInterface.hh"
#include "G4BinaryCascade.hh"


#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
// Stopping processes
#include "G4PiMinusAbsorptionAtRest.hh"
#include "G4KaonMinusAbsorptionAtRest.hh"
#include "G4AntiProtonAnnihilationAtRest.hh"
#include "G4AntiNeutronAnnihilationAtRest.hh"
#endif

void MEMPHYS::PhysicsList::ConstructHad() {

// Makes discrete physics processes for the hadrons, at present limited
// to those particles with GHEISHA interactions (INTRC > 0).
// The processes are: Elastic scattering and Inelastic scattering.
// F.W.Jones  09-JUL-1998
// 
// This code stolen from:
// examples/advanced/underground_physics/src/DMXPhysicsList.cc
// CWW 2/23/05
//
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
  G4cout << "PhysicsList::ConstructHad : +++++++++++++++++ WARNING +++++++++++++++++++++++++++++ :"
	 << G4endl;
  G4cout << "PhysicsList::ConstructHad :"
         << " G.Barrand : it is disconnected. It must be revisited for this version of Geant4."
	 << G4endl;
  return;
#endif
  
  G4HadronElasticProcess* theElasticProcess = new G4HadronElasticProcess;
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=1031 //G.Barrand
  auto theParticleIterator=GetParticleIterator();
#endif
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else  
  G4LElastic* theElasticModel = new G4LElastic;
  theElasticProcess->RegisterMe(theElasticModel);
#endif
  
  theParticleIterator->reset();
  while ((*theParticleIterator)()) 
    {
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();
      G4String particleName = particle->GetParticleName();

      if (particleName == "pi+") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4PionPlusInelasticProcess* theInelasticProcess = new G4PionPlusInelasticProcess();
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
	  G4LEPionPlusInelastic* theLEInelasticModel = new G4LEPionPlusInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  G4HEPionPlusInelastic* theHEInelasticModel = new G4HEPionPlusInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
#endif	  
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	} 

      else if (particleName == "pi-") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4PionMinusInelasticProcess* theInelasticProcess = new G4PionMinusInelasticProcess();
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
	  G4LEPionMinusInelastic* theLEInelasticModel = new G4LEPionMinusInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  G4HEPionMinusInelastic* theHEInelasticModel = new G4HEPionMinusInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
	  G4String prcNam;
	  pmanager->AddRestProcess(new G4PiMinusAbsorptionAtRest, ordDefault);
#endif	  
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}
      
      else if (particleName == "kaon+") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4KaonPlusInelasticProcess* theInelasticProcess = new G4KaonPlusInelasticProcess();
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
	  G4LEKaonPlusInelastic* theLEInelasticModel = new G4LEKaonPlusInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  G4HEKaonPlusInelastic* theHEInelasticModel = new G4HEKaonPlusInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
#endif	  
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}
      
      else if (particleName == "kaon0S") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4KaonZeroSInelasticProcess* theInelasticProcess = new G4KaonZeroSInelasticProcess();
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
	  G4LEKaonZeroSInelastic* theLEInelasticModel = new G4LEKaonZeroSInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  G4HEKaonZeroInelastic* theHEInelasticModel = new G4HEKaonZeroInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
#endif	  
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}

      else if (particleName == "kaon0L") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4KaonZeroLInelasticProcess* theInelasticProcess = new G4KaonZeroLInelasticProcess();
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
	  G4LEKaonZeroLInelastic* theLEInelasticModel = new G4LEKaonZeroLInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  G4HEKaonZeroInelastic* theHEInelasticModel = new G4HEKaonZeroInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
#endif	  
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}

      else if (particleName == "kaon-") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4KaonMinusInelasticProcess* theInelasticProcess = new G4KaonMinusInelasticProcess();
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
	  G4LEKaonMinusInelastic* theLEInelasticModel = new G4LEKaonMinusInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  G4HEKaonMinusInelastic* theHEInelasticModel = new G4HEKaonMinusInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
	  pmanager->AddRestProcess(new G4KaonMinusAbsorptionAtRest, ordDefault);
#endif	  
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}

      else if (particleName == "proton") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4ProtonInelasticProcess* theInelasticProcess = new G4ProtonInelasticProcess();
	  
	  //=================================
	  // Added by JLR 2005-07-05
	  //=================================
	  // Options for secondary interaction models
	  // Choice defined in jobOptions.mac, which is
	  // read in before initialization of the run manager.
	  // In the absence of this file, BINARY will be used.
	  // Gheisha = Original Geant4 default 
	  // Bertini = Bertini intra-nuclear cascade model
	  // Binary  = Binary intra-nuclear cascade model
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
	  if (gheishahad) {
	    G4LEProtonInelastic* theLEInelasticModel = new G4LEProtonInelastic;
	    theInelasticProcess->RegisterMe(theLEInelasticModel);
	  }
	  else
#endif	    
          if (bertinihad) {
	    G4CascadeInterface* theBertiniModel = new G4CascadeInterface;
	    theInelasticProcess->RegisterMe(theBertiniModel);
	  }
	  else if (binaryhad) {
	    G4BinaryCascade* theBinaryModel = new G4BinaryCascade();
	    theInelasticProcess->RegisterMe(theBinaryModel);
	  }
	  else {
	    G4cout << "No secondary interaction model chosen! Using G4 BINARY." << G4endl;
	    G4BinaryCascade* theBinaryModel = new G4BinaryCascade();
	    theInelasticProcess->RegisterMe(theBinaryModel);
	  }
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
	  G4HEProtonInelastic* theHEInelasticModel = new G4HEProtonInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
#endif	  
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}

      else if (particleName == "anti_proton") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4AntiProtonInelasticProcess* theInelasticProcess = new G4AntiProtonInelasticProcess();
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
	  G4LEAntiProtonInelastic* theLEInelasticModel = new G4LEAntiProtonInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  G4HEAntiProtonInelastic* theHEInelasticModel = new G4HEAntiProtonInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
#endif	  
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}

      else if (particleName == "neutron") 
	{
	  // inelastic scattering
	  G4NeutronInelasticProcess* theInelasticProcess = new G4NeutronInelasticProcess();
	  
	  // elastic scattering
	  G4HadronElasticProcess* theNeutronElasticProcess = new G4HadronElasticProcess;
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
	  G4LElastic* theElasticModel1 = new G4LElastic;
	  theNeutronElasticProcess->RegisterMe(theElasticModel1);
	  theElasticModel1->SetMinEnergy(19*MeV);
#endif	  
	  G4NeutronHPElastic * theElasticNeutron = new G4NeutronHPElastic;
	  theNeutronElasticProcess->RegisterMe(theElasticNeutron);
	  G4NeutronHPElasticData * theNeutronData = new G4NeutronHPElasticData;
	  theNeutronElasticProcess->AddDataSet(theNeutronData);
	  pmanager->AddDiscreteProcess(theNeutronElasticProcess);
	  //=================================
	  // Added by JLR 2005-07-05
	  //=================================
	  // Options for secondary interaction models
	  // Choice defined in jobOptions.mac, which is
	  // read in before initialization of the run manager.
	  // In the absence of this file, BINARY will be used.
	  // GHEISHA = Original Geant4 default model 
	  // BERTINI = Bertini intra-nuclear cascade model
	  // BINARY  = Binary intra-nuclear cascade model
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
	  if (gheishahad) {
	    G4LENeutronInelastic* theInelasticModel = new G4LENeutronInelastic;
	    theInelasticModel->SetMinEnergy(19*MeV);
	    theInelasticProcess->RegisterMe(theInelasticModel);
	  }
	  else
#endif	    
	  if (bertinihad) {
	    G4CascadeInterface* theBertiniModel = new G4CascadeInterface;
	    theBertiniModel->SetMinEnergy(19*MeV);
	    theInelasticProcess->RegisterMe(theBertiniModel);
	  }
	  else if (binaryhad) {
	    G4BinaryCascade* theBinaryModel = new G4BinaryCascade();
	    theBinaryModel->SetMinEnergy(19*MeV);
	    theInelasticProcess->RegisterMe(theBinaryModel);
	  }
	  else {
	    G4cout << "No secondary interaction model chosen! Using G4 BINARY." << G4endl;
	    G4BinaryCascade* theBinaryModel = new G4BinaryCascade();
	    theBinaryModel->SetMinEnergy(19*MeV);
	    theInelasticProcess->RegisterMe(theBinaryModel);
	  }
	  
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
	  G4HENeutronInelastic* theHEInelasticModel = new G4HENeutronInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
	  
	  G4NeutronHPInelastic * theLENeutronInelasticModel = new G4NeutronHPInelastic;
	  theInelasticProcess->RegisterMe(theLENeutronInelasticModel);
	  
	  G4NeutronHPInelasticData * theNeutronData1 = new G4NeutronHPInelasticData;
	  theInelasticProcess->AddDataSet(theNeutronData1);
#endif	  
	  pmanager->AddDiscreteProcess(theInelasticProcess);

	  // capture
	  G4HadronCaptureProcess* theCaptureProcess = new G4HadronCaptureProcess;
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
	  G4LCapture* theCaptureModel = new G4LCapture;
	  theCaptureModel->SetMinEnergy(19*MeV);
	  theCaptureProcess->RegisterMe(theCaptureModel);
#endif	  
	  G4NeutronHPCapture * theLENeutronCaptureModel = new G4NeutronHPCapture;
	  theCaptureProcess->RegisterMe(theLENeutronCaptureModel);
	  G4NeutronHPCaptureData * theNeutronData3 = new G4NeutronHPCaptureData;
	  theCaptureProcess->AddDataSet(theNeutronData3);
	  pmanager->AddDiscreteProcess(theCaptureProcess);
	  //  G4ProcessManager* pmanager = G4Neutron::Neutron->GetProcessManager();
	  //  pmanager->AddProcess(new G4UserSpecialCuts(),-1,-1,1);
	}

      else if (particleName == "anti_neutron") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4AntiNeutronInelasticProcess* theInelasticProcess = new G4AntiNeutronInelasticProcess();
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
	  G4LEAntiNeutronInelastic* theLEInelasticModel = new G4LEAntiNeutronInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
	  G4HEAntiNeutronInelastic* theHEInelasticModel = new G4HEAntiNeutronInelastic;
	  theInelasticProcess->RegisterMe(theHEInelasticModel);
#endif	  
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}

      else if (particleName == "deuteron") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4DeuteronInelasticProcess* theInelasticProcess = new G4DeuteronInelasticProcess();
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
	  G4LEDeuteronInelastic* theLEInelasticModel = new G4LEDeuteronInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
#endif
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}
      
      else if (particleName == "triton") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4TritonInelasticProcess* theInelasticProcess = new G4TritonInelasticProcess();
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
	  G4LETritonInelastic* theLEInelasticModel = new G4LETritonInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
#endif
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}

      else if (particleName == "alpha") 
	{
	  pmanager->AddDiscreteProcess(theElasticProcess);
	  G4AlphaInelasticProcess* theInelasticProcess = new G4AlphaInelasticProcess();
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
	  G4LEAlphaInelastic* theLEInelasticModel = new G4LEAlphaInelastic;
	  theInelasticProcess->RegisterMe(theLEInelasticModel);
#endif
	  pmanager->AddDiscreteProcess(theInelasticProcess);
	}

    }
}//ConstructHad

#endif //G4VERSION_NUMBER

//-----------------------------------------------------------------------------------------------

//=================================
// Added by JLR 2005-07-05
//=================================
// Sets secondary hadronic interaction model 
// Note: this is currently only implemented for
// protons and neutrons -- not pions.
// Gheisha = Original Geant4 default 
// Bertini = Bertini intra-nuclear cascade model
// Binary  = Binary intra-nuclear cascade model
void MEMPHYS::PhysicsList::SetSecondaryHad(G4String hadval) {
  SecondaryHadModel = hadval;

#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else  
  if (SecondaryHadModel == "GHEISHA") {
    G4cout << "Secondary interaction model set to GHEISHA" << G4endl;
    gheishahad = true;
    bertinihad = false;
    binaryhad  = false;
  }
  else
#endif    
  if (SecondaryHadModel == "BERTINI") {
    G4cout << "Secondary interaction model set to BERTINI" << G4endl;
    gheishahad = false;
    bertinihad = true;
    binaryhad  = false;
  }
  else if (SecondaryHadModel == "BINARY") {
    G4cout << "Secondary interaction model set to BINARY" << G4endl;
    gheishahad = false;
    bertinihad = false;
    binaryhad  = true;
  }
  else {
    G4cout << "Secondary interaction model " << SecondaryHadModel
	   << " is not a valid choice. BINARY model will be used." << G4endl;
    gheishahad = false;
    bertinihad = false;
    binaryhad  = true;
  }
}//SetSecondaryHad

//-----------------------------------------------------------------------------------------------

void MEMPHYS::PhysicsList::SetCuts() {
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950 //G.Barrand
#else
  if (verboseLevel >0){
    G4cout << "PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  //
  SetCutValue(defaultCutValue, "gamma");
  SetCutValue(defaultCutValue, "e-");
  SetCutValue(defaultCutValue, "e+");

  if (verboseLevel>0) DumpCutValuesTable();
#endif
}//SetCuts
