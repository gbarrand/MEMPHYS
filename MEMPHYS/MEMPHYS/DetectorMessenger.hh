#ifndef MEMPHYSDetectorMessenger_h
#define MEMPHYSDetectorMessenger_h 1


#include "G4UImessenger.hh"
#include "globals.hh"
#include "G4ios.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"


class G4UIdirectory;
class G4UIcmdWithAString;

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {

class DetectorConstruction;


class DetectorMessenger: public G4UImessenger {
 public:
  DetectorMessenger(DetectorConstruction* mpga);
  virtual ~DetectorMessenger();
  
 public:
  void         SetNewValue(G4UIcommand* command, G4String newValues);
 
 private:
  DetectorConstruction* MEMPHYSDetector;

 private: //commands
  
  G4UIdirectory*      MEMPHYSDir;
  G4UIcmdWithAString* PMTConfig;
};

}
#endif


