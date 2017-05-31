#ifndef MEMPHYSTrackingActionMessenger_h
#define MEMPHYSTrackingActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithABool;

namespace MEMPHYS {

class TrackingAction;

class TrackingActionMessenger:  public G4UImessenger {
 public:
  TrackingActionMessenger(TrackingAction*);
  virtual ~TrackingActionMessenger();

  void SetNewValue(G4UIcommand* command, G4String newValue);

private:
  TrackingAction* MEMPHYSTrackingAction;

  G4UIdirectory*      MEMPHYSDir;
  G4UIcmdWithABool*   drawOpticalPhotonCmd;

};
}



#endif
