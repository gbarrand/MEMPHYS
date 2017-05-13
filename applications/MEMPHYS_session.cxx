
// Geant4:
#include <G4ios.hh>
#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <G4SDManager.hh>

//AIDA :
#include <AIDA/IAnalysisFactory.h>

// std::
#include <iostream>

// Slash :
#include <Slash/Core/IManager.h>
#include <Slash/Core/ISession.h>
#include <Slash/Data/IProcessor.h>

// Lib :
#include <Lib/Manager.h>
#include <Lib/System.h>
#include <Lib/Out.h>
#include <Lib/Cast.h>

// G4Lab :
#include <G4Lab/UIOnX.h>
#include <G4Lab/TrackingAction.h>
#include <G4Lab/Interfaces/IGeant4Manager.h>
//#include <G4Lab/DigitsCollectionAccessor.h>

//MEMPHYS:
#include "MEMPHYS/Analysis.hh"
#include "MEMPHYS/DetectorConstruction.hh"
#include "MEMPHYS/PhysicsList.hh"
#include "MEMPHYS/PhysicsMessenger.hh"
#include "MEMPHYS/PrimaryGeneratorAction.hh"
#include "MEMPHYS/EventAction.hh"
#include "MEMPHYS/RunAction.hh"
#include "MEMPHYS/StackingAction.hh"
#include "MEMPHYS/TrackingAction.hh"
#include "MEMPHYS/SteppingAction.hh"
#include "MEMPHYS/WCDigi.hh"
#include <MEMPHYS/HitsCollectionAccessor.h>
#include <MEMPHYS/DigitsCollectionAccessor.h>

// Create a manager to access the MEMPHYSAnalysis from the callbacks :

//JEC 10/1/06 introduce MEMPHYS namespace
namespace MEMPHYS {

class AppManager 
  :public virtual Slash::Core::IManager
  ,public Analysis  {
 public: //Slash::Core::IManager
  virtual std::string name() const { return fName;}
  virtual void* cast(const std::string& aClass) const {
    if_Lib_SCast(MEMPHYS::AppManager)
    else if_Lib_SCast(MEMPHYS::IAppManager)
    else if_Lib_SCast(MEMPHYS::Analysis)
    else if_Lib_SCast(Slash::Core::IManager)
    else return 0;
  }
 public://IAppManager
  virtual bool initialize() {
    //GB :
    // Used in the MEMPHYS_Initialize callback.
    // Do all the below here because of platforms having 
    // not Geant4 shared libs.
    IGeant4Manager* g4Manager = 
      Lib_findManager(fSession,"Geant4Manager",IGeant4Manager);
    if(!g4Manager) {
      Lib::Out out(fSession.printer());
      out << "MEMPHYS_Initialize :"
          << " Geant4Manager not found."
          << Lib::endl;
      return false;
    }

    // To initialize G4Lab Types (PV, Trajectory, HitsCollection, etc.. types).
    // To initialize G4Lab SoNodes.
    g4Manager->initialize();

    // Overload the HitsCollectionAccessor of G4Lab :
    Slash::Data::IProcessor* accessorManager = 
      Lib_findManager(fSession,"AccessorManager",Slash::Data::IProcessor);
    if(accessorManager) {
      G4SDManager* sdManager = g4Manager->sdManager();
      if(sdManager) {
        G4HCtable* hcTable  = sdManager->GetHCtable();
        if(hcTable) {
          int number = hcTable->entries();
          for(int index=0;index<number;index++) {
            G4String hcName = hcTable->GetHCname(index);
            accessorManager->removeAccessor(hcName);
            accessorManager->addAccessor
              (new MEMPHYS::HitsCollectionAccessor(fSession,g4Manager->runManager(),hcName));
          }
        }
      }

      /*
      accessorManager->addAccessor
        (new G4Lab::DigitsCollectionAccessor<MEMPHYS::WCDigi>
          (fSession,"WCDigitizedCollection"));
      */
      accessorManager->addAccessor
        (new MEMPHYS::DigitsCollectionAccessor<MEMPHYS::WCDigi>
      	 (fSession,g4Manager->runManager(),"WCDigitizedCollection"));
    }
    return true;
  }
 public:
  AppManager(Slash::Core::ISession& aSession,AIDA::IAnalysisFactory* aAIDA,bool aBatch)
    :Analysis(aAIDA,"",aBatch)
    ,fSession(aSession)
    ,fName("MEMPHYS::AppManager") {}
  virtual ~AppManager(){}
 private:
  Slash::Core::ISession& fSession;
  std::string fName;
};
}

//////////////////////////////////////////////////////////////////////////////
int main(
 int aArgc
,char** aArgv
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //Lib::Debug::checkByClass(true);

  G4RunManager* runManager = new G4RunManager;

  //JEC 27/1/06 test to speed up the closing phase
  // GB : in the below :
  // - if passing false, it induces a memory leak on Replicas 
  //   at exit (seen with valgrind and MacOSX).
  // - if passing true (default), it takes a lot of time 
  //   to "close the geometry".
  // !!!!
  runManager->SetGeometryToBeOptimized(false);

  // Need to pass the G4RunManager at UIOnX creation
  std::string gui = "$MEMPHYSROOT/scripts/OnX/MEMPHYS_session.onx";
  G4Lab::UIOnX* session = new G4Lab::UIOnX(*runManager,gui,aArgc,aArgv);
  if(!session->isValid()) {
    std::cout << "MEMPHYS_session : problem starting OnX." << std::endl;
    return EXIT_FAILURE;
  }

  MEMPHYS::AppManager* appManager = 0;

  AIDA::IAnalysisFactory* aida = session->findAIDA();
  if(!aida) {
    std::cout << "MEMPHYS_session : AIDA not found." << std::endl;
  } 

  appManager = new MEMPHYS::AppManager(*session->session(),aida,false);
  session->addManager(appManager);

  //JEC 18/11/05 give the appManager to fill geometry ITuple
  MEMPHYS::DetectorConstruction* MEMPHYSdetector = 
    new MEMPHYS::DetectorConstruction(*appManager); 

  runManager->SetUserInitialization(MEMPHYSdetector);

  runManager->SetUserInitialization(new MEMPHYS::PhysicsList);

  G4UImanager* UI = G4UImanager::GetUIpointer();
  if (UI) {
    std::string file = 
      Lib::System::getenv("MEMPHYSROOT")+"/scripts/Geant4/jobOptions.mac";
    UI->ApplyCommand("/control/execute "+file);  
  }

  
  // Set user action classes
  MEMPHYS::PrimaryGeneratorAction* myGeneratorAction = 
    new MEMPHYS::PrimaryGeneratorAction(MEMPHYSdetector);
  runManager->SetUserAction(myGeneratorAction);

  MEMPHYS::RunAction* myRunAction = new MEMPHYS::RunAction();

  runManager->SetUserAction(myRunAction);


  runManager->SetUserAction(new MEMPHYS::EventAction(*appManager,
						   myRunAction, 
						   MEMPHYSdetector,
						   myGeneratorAction));
  runManager->SetUserAction(new MEMPHYS::TrackingAction);

  runManager->SetUserAction(new MEMPHYS::StackingAction);

  runManager->SetUserAction(new MEMPHYS::SteppingAction); //JEC 15/12/05 Add user SteppingAction

  // Initialize G4 kernel
  runManager->Initialize();

  if (UI) {
    std::string file = 
      Lib::System::getenv("MEMPHYSROOT")+"/scripts/Geant4/init.g4m";
    UI->ApplyCommand("/control/execute "+file);  
  }


  // Main UI loop :
  // GUI is really constructed here. 
  // Create callbacks are executed here too.
  session->SessionStart();

  delete session; //It will delete the appManager.

  delete runManager;

  return EXIT_SUCCESS;
}

