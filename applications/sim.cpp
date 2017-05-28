// Geant4:
#include "G4ios.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"

//MEMPHYS:
#include "../MEMPHYS/Analysis.hh"
#include "../MEMPHYS/DetectorConstruction.hh"
#include "../MEMPHYS/PhysicsList.hh"
#include "../MEMPHYS/PhysicsMessenger.hh"
#include "../MEMPHYS/PrimaryGeneratorAction.hh"
#include "../MEMPHYS/EventAction.hh"
#include "../MEMPHYS/RunAction.hh"
#include "../MEMPHYS/StackingAction.hh"
#include "../MEMPHYS/TrackingAction.hh"
#include "../MEMPHYS/SteppingAction.hh"

#ifdef APP_USE_AIDA
#include <AIDA/IAnalysisFactory.h>

#ifdef APP_USE_ARCHIVE
#ifdef INLIB_MEM
#undef INLIB_MEM
#include <BatchLab/Core/Main.h>
#define INLIB_MEM
#else
#include <BatchLab/Core/Main.h>
#endif
extern "C" {
  void BatchLabRioInitialize(Slash::Core::ISession&);
}
#endif
#endif //APP_USE_AIDA

#ifdef APP_USE_INLIB_WROOT
#ifdef INLIB_MEM
#include <inlib/mem>
#endif
#endif

#include <iostream>

int main(int aArgc,char** aArgv) {
#ifdef APP_USE_INLIB_WROOT
#ifdef INLIB_MEM
  inlib::mem::set_check_by_class(true);{
#endif //INLIB_MEM
#endif
    
#ifdef APP_USE_AIDA
#ifdef APP_USE_ARCHIVE
  BatchLab::Main* session = new BatchLab::Main(std::vector<std::string>());
  Slash::Core::ILibraryManager* libraryManager = Slash::libraryManager(*session);
  if(!libraryManager) {std::cout << "LibraryManager not found." << std::endl;return EXIT_FAILURE;}
  Slash::Core::ILibrary* library = libraryManager->addLibrary("Rio","main","");
  if(!library) {std::cout << "addLibrary() failed." << std::endl;return EXIT_FAILURE;}
  ::BatchLabRioInitialize(*session);
  AIDA::IAnalysisFactory* aida = static_cast<AIDA::IAnalysisFactory*>(session);
#else
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
#endif
  if(!aida) {
    std::cout << "MEMPHYS_batch : AIDA not found." << std::endl;
  }  
  //Book all the histo, tuple 
  MEMPHYS::Analysis* analysis = new MEMPHYS::Analysis(aida);
#else  
  //Book all the histo, tuple 
  MEMPHYS::Analysis* analysis = new MEMPHYS::Analysis();  
#endif //APP_USE_AIDA

  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;
  
  runManager->SetGeometryToBeOptimized(false); //JEC 21/4/06 test to speed up the closing phase
                                               //note that G.B suspect memory leak with valgrind

  // UserInitialization classes (mandatory)

  //JEC 18/11/05 give the "analysis" to fill geometry ITuple
  MEMPHYS::DetectorConstruction* MEMPHYSdetector = new MEMPHYS::DetectorConstruction(*analysis); 

  runManager->SetUserInitialization(MEMPHYSdetector);

  runManager->SetUserInitialization(new MEMPHYS::PhysicsList);

  G4UImanager* UI = G4UImanager::GetUIpointer();
  if (UI) {
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
    std::string file = "init.g4m";
    UI->ApplyCommand("/control/execute "+file);  
  }

  runManager->BeamOn(10);

  //JEC interactive session  delete visManager;

  delete runManager;
  delete analysis;
  
#ifdef APP_USE_AIDA
  delete aida;
#endif
  
#ifdef APP_USE_INLIB_WROOT
#ifdef INLIB_MEM
  }inlib::mem::balance(std::cout);
#endif //INLIB_MEM
#endif
  
  return EXIT_SUCCESS;
}

