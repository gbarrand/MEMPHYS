#ifndef MEMPHYSPhysicsMessenger_h
#define MEMPHYSPhysicsMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;

namespace MEMPHYS {

class PhysicsList;

class PhysicsMessenger: public G4UImessenger
{
public:
  PhysicsMessenger(PhysicsList*);
  virtual ~PhysicsMessenger();

  void SetNewValue(G4UIcommand* command, G4String newValue);

private:
  PhysicsList* MEMPHYSPhysics;

  G4UIdirectory*      MEMPHYSDir;
  G4UIcmdWithAString* hadmodelCmd;

};

}
#endif
