#ifndef MEMPHYSPrimaryGeneratorAction_h
#define MEMPHYSPrimaryGeneratorAction_h

//Geant4
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

//std
#include <fstream>

class G4ParticleGun;
class G4Event;

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {

class DetectorConstruction;
class PrimaryGeneratorMessenger;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
 public:
  PrimaryGeneratorAction(DetectorConstruction&,const std::string& a_res_dir = "");
  virtual ~PrimaryGeneratorAction();
  
public:
  void GeneratePrimaries(G4Event* anEvent);

  // Normal gun setting calls these functions to fill jhfNtuple and Root tree
  void SetVtx(G4ThreeVector i)     { vtx = i; };
  void SetBeamEnergy(G4double i)   { beamenergy = i; };
  void SetBeamDir(G4ThreeVector i) { beamdir = i; };
  void SetBeamPDG(G4int i)         { beampdg = i; };

  // These go with jhfNtuple
  G4int GetVecRecNumber(){return vecRecNumber;}
  G4int GetMode() {return mode;};
  G4ThreeVector GetVtx() {return vtx;}
  
  G4int GetBeamPDG() {return beampdg;};
  G4double GetBeamEnergy() {return beamenergy;};
  G4ThreeVector GetBeamDir() {return beamdir;};
  
  G4int GetTargetPDG() {return targetpdg;};
  G4double GetTargetEnergy() {return targetenergy;};
  G4ThreeVector GetTargetDir() {return targetdir;};

private:
  DetectorConstruction& fMyDetector;
  std::string m_res_dir; //GB.
  bool m_rewind; //GB.
  
  G4ParticleGun* particleGun;
  PrimaryGeneratorMessenger* messenger;

  // Variables set by the messenger
  G4bool   useMulineEvt;
  G4bool   useNormalEvt;
  std::fstream inputFile;
  G4bool   GenerateVertexInRock;

  // These go with jhfNtuple
  G4int mode;
  G4ThreeVector vtx;
  G4int vecRecNumber;
  
  G4int beampdg;
  G4ThreeVector beamdir;
  G4double beamenergy;

  G4int targetpdg;
  G4ThreeVector targetdir;
  G4double targetenergy;
  
  //G4int vecRecNumber;

  G4int    _counterRock; 
  G4int    _counterCublic;
public:

   void SetMulineEvtGenerator(G4bool choice) { useMulineEvt = choice; }
   G4bool IsUsingMulineEvtGenerator() { return useMulineEvt; }

   void SetNormalEvtGenerator(G4bool choice) { useNormalEvt = choice; }
   G4bool IsUsingNormalEvtGenerator()  { return useNormalEvt; }

  void OpenVectorFile(const G4String& fileName) {
    if ( inputFile.is_open() ) inputFile.close();
    inputFile.open(fileName, std::fstream::in);
    if ( !inputFile.is_open() && m_res_dir.size() ) { //GB : look in m_res_dir.
      std::string fname = m_res_dir+"/"+fileName;
      //G4cout << "PrimaryGeneratorAction::OpenVectorFile: " << fileName << " not found, try " << fname << "." << G4endl;
      inputFile.clear();
      inputFile.open(fname.c_str(), std::fstream::in);
    }
    if ( !inputFile.is_open() ) {
      G4cout << "PrimaryGeneratorAction::OpenVectorFile: FATAL: " << fileName << " not found." << G4endl;
      exit(0);
    }
  }
  G4bool IsGeneratingVertexInRock() { return GenerateVertexInRock; }
  void SetGenerateVertexInRock(G4bool choice) { GenerateVertexInRock = choice;}

  void set_rewind(bool a_value) {m_rewind = a_value;}
};


}

#endif


