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

#ifdef APP_USE_INLIB_WROOT
#ifdef INLIB_MEM
#include <inlib/mem>
#endif
#endif

#include <inlib/system>
#include <inlib/args>
#include <inlib/snpf>
#include <inlib/sys/process>
#include <inlib/net/base_socket>

#include <iostream>

int main(int a_argc,char** a_argv) {
#ifdef APP_USE_INLIB_WROOT
#ifdef INLIB_MEM
  inlib::mem::set_check_by_class(true);{
#endif //INLIB_MEM
#endif

  inlib::args args(a_argc,a_argv);
    
  if(!inlib::is_env("NeutronHPCrossSections")) {
    std::cout << "env variable NeutronHPCrossSections not defined." << std::endl;
    return EXIT_FAILURE;
  }
  if(!inlib::is_env("G4LEVELGAMMADATA")) {
    std::cout << "env variable G4LEVELGAMMADATA not defined." << std::endl;
    return EXIT_FAILURE;
  }
  if(!inlib::is_env("G4RADIOACTIVEDATA")) {
    std::cout << "env variable G4RADIOACTIVEDATA not defined." << std::endl;
    return EXIT_FAILURE;
  }
  if(!inlib::is_env("G4LEDATA")) {
    std::cout << "env variable G4LEDATA not defined." << std::endl;
    return EXIT_FAILURE;
  }

  std::string root_file;
  
  if(args.is_arg("-pid_root")) {
    char spid[128];
    inlib::snpf(spid,sizeof(spid),"%d",inlib::process_id());
    std::string host;
    if(!inlib::net::host_name(std::cout,host)) {
      std::cout << "inlib::host_name failed." << std::endl;
      return EXIT_FAILURE;
    }
    root_file = "MEMPHYS_"+host+"_"+spid+".root";
  } else {
    if(!args.file(root_file)) root_file = "MEMPHYS.root";
  }
 
  //Book all the histo, tuple 
  MEMPHYS::Analysis* analysis = new MEMPHYS::Analysis(root_file);  

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
  
#ifdef APP_USE_INLIB_WROOT
#ifdef INLIB_MEM
  }inlib::mem::balance(std::cout);
#endif //INLIB_MEM
#endif
  
  return EXIT_SUCCESS;
}


