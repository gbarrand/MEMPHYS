// Geant4:
#include "G4ios.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4Version.hh"

//MEMPHYS:
#include "../MEMPHYS/Analysis.hh"
#include "../MEMPHYS/DetectorConstruction.hh"
#include "../MEMPHYS/PrimaryGeneratorAction.hh"
#include "../MEMPHYS/EventAction.hh"
#include "../MEMPHYS/RunAction.hh"
#include "../MEMPHYS/StackingAction.hh"
#include "../MEMPHYS/TrackingAction.hh"
#include "../MEMPHYS/SteppingAction.hh"
#include "../MEMPHYS/PhysicsList.hh"

#ifdef INLIB_MEM
#include <inlib/mem>
#endif

#include <inlib/system>
#include <inlib/args>
#include <inlib/snpf>
#include <inlib/sys/process>
#include <inlib/net/base_socket>

#include <iostream>

#define CHECK_ENV(a__name) \
  if(!inlib::is_env(#a__name)) { \
    std::cout << "env variable " << #a__name << " not defined." << std::endl;\
    return EXIT_FAILURE;\
  }

int main(int a_argc,char** a_argv) {
#ifdef INLIB_MEM
  inlib::mem::set_check_by_class(true);{
#endif //INLIB_MEM

  inlib::args args(a_argc,a_argv);

  //////////////////////////////////////////////////////////////////////////////
  /// args /////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////
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
    std::cout << "output file is " << inlib::sout(root_file) << std::endl;  
  } else {
    if(!args.file(root_file)) root_file = "MEMPHYS.root";
  }

  unsigned int nevent;
  args.find<unsigned int>("-events",nevent,10);
  
#ifdef INLIB_MEM
#else
  bool verbose = args.is_arg("-verbose");
#endif  
  //////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////
  
 {std::string _G4VERSION_NUMBER;
  inlib::num2s(G4VERSION_NUMBER,_G4VERSION_NUMBER);
  std::cout << "G4VERSION_NUMBER : " << _G4VERSION_NUMBER << std::endl;}
  
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=1030
  CHECK_ENV(G4NEUTRONHPDATA)
  CHECK_ENV(G4LEDATA)
  CHECK_ENV(G4LEVELGAMMADATA)
  CHECK_ENV(G4ENSDFSTATEDATA)
  CHECK_ENV(G4SAIDXSDATA)
#elif defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=950
  CHECK_ENV(G4NEUTRONHPDATA)
  CHECK_ENV(G4LEDATA)
  CHECK_ENV(G4LEVELGAMMADATA)
#elif defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=820
  CHECK_ENV(NeutronHPCrossSections)
  CHECK_ENV(G4LEDATA)
  CHECK_ENV(G4LEVELGAMMADATA)
#endif

  //Book all the histo, tuple 
  MEMPHYS::Analysis* analysis = new MEMPHYS::Analysis(root_file,true); //true=handle MEMPHYS.root file.

  G4Random::setTheEngine(new CLHEP::RanecuEngine);
    
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
    std::string file = "job_options.mac"; //JEC FIXME this comes from JHF
    UI->ApplyCommand("/control/execute "+file);  
  }

  // Set user action classes
  MEMPHYS::PrimaryGeneratorAction* myGeneratorAction =
    new MEMPHYS::PrimaryGeneratorAction(*MEMPHYSdetector);
  runManager->SetUserAction(myGeneratorAction);

  MEMPHYS::RunAction* myRunAction = new MEMPHYS::RunAction();

  runManager->SetUserAction(myRunAction);

  runManager->SetUserAction(new MEMPHYS::EventAction(*analysis,
						     *myRunAction, 
						     *MEMPHYSdetector,
						     *myGeneratorAction));
  runManager->SetUserAction(new MEMPHYS::TrackingAction);

  runManager->SetUserAction(new MEMPHYS::StackingAction);
  runManager->SetUserAction(new MEMPHYS::SteppingAction); //JEC 15/12/05 Add user SteppingAction

  // Initialize G4 kernel
  runManager->Initialize();

  if (UI) {
    std::string file = "init.g4m";
    UI->ApplyCommand("/control/execute "+file);  
  }

  runManager->BeamOn(nevent);

  //JEC interactive session  delete visManager;

  delete runManager;
  delete analysis;
  
#ifdef INLIB_MEM
  }inlib::mem::balance(std::cout);
  std::cout << "MEMPHYS_sim : exit(mem) ..." << std::endl;
#else
  if(verbose) std::cout << "MEMPHYS_sim : exit ..." << std::endl;
#endif //INLIB_MEM
  
  return EXIT_SUCCESS;
}


