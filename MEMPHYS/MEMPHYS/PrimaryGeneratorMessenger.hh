#ifndef MEMPHYSPrimaryGeneratorMessenger_h
#define MEMPHYSPrimaryGeneratorMessenger_h 1

//Geant4
#include "G4UImessenger.hh"
#include "globals.hh"

class G4UIdirectory;
class G4UIcmdWithAString;


//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {

class PrimaryGeneratorAction;


class PrimaryGeneratorMessenger: public G4UImessenger {
 public:
  PrimaryGeneratorMessenger(PrimaryGeneratorAction* mpga);
  virtual ~PrimaryGeneratorMessenger();
  
 public:
  void     SetNewValue(G4UIcommand* command, G4String newValues);
  G4String GetCurrentValue(G4UIcommand* command);
  
 private:
  PrimaryGeneratorAction* myAction;
  
 private: //commands
  G4UIdirectory*      mydetDirectory;
  G4UIcmdWithAString* genCmd;
  G4UIcmdWithAString* fileNameCmd;
  
};
}
#endif


