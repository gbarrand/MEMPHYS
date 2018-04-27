#ifndef MEMPHYSRunActionMessenger_h
#define MEMPHYSRunActionMessenger_h 1

//Geant4
#include "G4UImessenger.hh"
#include "globals.hh"

class G4UIdirectory;
class G4UIcmdWithAString;


//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {

class RunAction;

class RunActionMessenger: public G4UImessenger {
 public:
  RunActionMessenger(RunAction* mpga);
  virtual ~RunActionMessenger();
  
 public:
  void     SetNewValue(G4UIcommand* command, G4String newValues);
  
//private:
//RunAction* MEMPHYSRun;
  
 private: //commands
  G4UIdirectory*      MEMPHYSIODir;
};
}
#endif


