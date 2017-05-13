// Geant4:
#include "G4ios.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"

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


//JEC AIDA :
#include <AIDA/IAnalysisFactory.h>

#ifdef APP_USE_NO_PLUGIN
namespace Slash {namespace Core {class ISession;}}
extern "C" {
  void BatchLabRioInitialize(Slash::Core::ISession&);
  void BatchLabRioFinalize(Slash::Core::ISession&);
}
#endif //APP_USE_NO_PLUGIN


// std::
#include <iostream>

//#include <inlib/system>

//////////////////////////////////////////////////////////////////////////////
int main(
 int aArgc 
,char** aArgv
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef APP_USE_NO_PLUGIN
  if(aArgc<-10) {
    Slash::Core::ISession* session = 0;
    BatchLabRioInitialize(*session);
  }
#endif
  std::string arg1 = (aArgc>=2?aArgv[1]:"");

  //AIDA Analysis factory
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "MEMPHYS_batch :"
              << " AIDA not found." << std::endl;
  }
  //Book all the histo, tuple 
  MEMPHYS::Analysis* analysis = new MEMPHYS::Analysis(aida,arg1);

  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;
  

  runManager->SetGeometryToBeOptimized(false); //JEC 21/4/06 test to speed up the closing phase
                                               //note that G.B suspect memory leak with valgrind


  // UserInitialization classes (mandatory)



  //JEC 18/11/05 give the "analysis" to fill geometry ITuple
  MEMPHYS::DetectorConstruction* MEMPHYSdetector = 
    new MEMPHYS::DetectorConstruction(*analysis); 

  runManager->SetUserInitialization(MEMPHYSdetector);

  runManager->SetUserInitialization(new MEMPHYS::PhysicsList);

  //std::string MEMPHYSROOT; 
  //inlib::get_env("MEMPHYSROOT",MEMPHYSROOT);

  G4UImanager* UI = G4UImanager::GetUIpointer();
  if (UI) {
    //std::string file = MEMPHYSROOT+"/scripts/Geant4/jobOptions.mac"; //JEC FIXME this comes from JHF
    std::string file = "jobOptions.mac"; //JEC FIXME this comes from JHF
    UI->ApplyCommand("/control/execute "+file);  
  }



  // Set user action classes
  MEMPHYS::PrimaryGeneratorAction* myGeneratorAction = new MEMPHYS::PrimaryGeneratorAction(MEMPHYSdetector);
  runManager->SetUserAction(myGeneratorAction);

  MEMPHYS::RunAction* myRunAction = new MEMPHYS::RunAction();

  runManager->SetUserAction(myRunAction);


  runManager->SetUserAction(new MEMPHYS::EventAction(*analysis,
						   myRunAction, 
						   MEMPHYSdetector,
						   myGeneratorAction));
  runManager->SetUserAction(new MEMPHYS::TrackingAction);

  runManager->SetUserAction(new MEMPHYS::StackingAction);
  runManager->SetUserAction(new MEMPHYS::SteppingAction); //JEC 15/12/05 Add user SteppingAction

  // Initialize G4 kernel
  runManager->Initialize();

  if (UI) {
    //std::string file = MEMPHYSROOT+"/scripts/Geant4/init.g4m"; //JEC FIXME this comes from JHF
    std::string file = "init.g4m";
    UI->ApplyCommand("/control/execute "+file);  
  }

  runManager->BeamOn(10);

  //JEC interactive session  delete visManager;

  delete runManager;
  delete analysis;
  delete aida;
  return 0;
}


