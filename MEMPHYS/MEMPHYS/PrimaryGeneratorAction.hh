#ifndef MEMPHYSPrimaryGeneratorAction_h
#define MEMPHYSPrimaryGeneratorAction_h

#include "DetectorConstruction.hh"

//Geant4
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"

//std
#include <fstream>
#include <vector>

// Returns a vector with the tokens
inline void tokenize( std::string separators, std::string input,std::vector<std::string>& a_tokens)  {
  a_tokens.clear();
  std::string::size_type startToken = 0, endToken; // Pointers to the token pos
  if( separators.size() > 0 && input.size() > 0 )  {
    while( startToken < input.size() ) {
      // Find the start of token
      startToken = input.find_first_not_of( separators, startToken );
      // If found...
      if( startToken != input.npos ) {
        // Find end of token
        endToken = input.find_first_of( separators, startToken );
        if( endToken == input.npos )
          // If there was no end of token, assign it to the end of string
          endToken = input.size();
          // Extract token
          a_tokens.push_back( input.substr( startToken, endToken - startToken ) );
          // Update startToken
          startToken = endToken;
        }
      }
  }
}

inline void readInLine(std::fstream& inFile, int lineSize, char* inBuf,std::vector<std::string>& a_tokens) { // Read in line break it up into tokens
  inFile.getline(inBuf,lineSize);
  tokenize(" $", inBuf,a_tokens);
}

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {

inline float atof( const std::string& s ) {return std::atof( s.c_str() );}
inline int   atoi( const std::string& s ) {return std::atoi( s.c_str() );}

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  PrimaryGeneratorAction(DetectorConstruction& myDC,const std::string& a_res_dir = "")
  :fMyDetector(myDC)
  ,m_res_dir(a_res_dir),m_rewind(false)
  ,particleGun(0)
  ,messenger(0)
  ,useMulineEvt(false)
  ,useNormalEvt(true) //JEC 22/11/05 set to default
  ,inputFile()
  ,GenerateVertexInRock(false)
  ,mode(0)
  ,vtx(0,0,0)
  ,vecRecNumber(0)
  ,beampdg(0)
  ,beamdir(0,0,0)
  ,beamenergy(0)
  ,targetpdg(0)
  ,targetdir(0,0,0)
  ,targetenergy(0)
  ,_counterRock(0)
  ,_counterCublic(0)
  {
    G4int n_particle = 1;
    particleGun = new G4ParticleGun(n_particle);
    
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    particleGun->SetParticleDefinition(particleTable->FindParticle("mu+"));
    particleGun->SetParticlePosition(G4ThreeVector(0.*CLHEP::m,0.*CLHEP::m,0.*CLHEP::m));
    particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.0));
    particleGun->SetParticleEnergy(1.0*CLHEP::GeV);
  
    messenger = new PrimaryGeneratorMessenger(this);
  }

  virtual ~PrimaryGeneratorAction() {
    if (IsGeneratingVertexInRock()){
        G4cout << "Fraction of Rock volume is : " << G4endl;
        G4cout << " Random number generated in Rock / in Cublic = " 
               << _counterRock << "/" << _counterCublic 
               << " = " << _counterRock/(G4double)_counterCublic << G4endl;
    }
    inputFile.close();
    delete particleGun;
    delete messenger;
  }
  
public:
  void GeneratePrimaries(G4Event* anEvent) {
    // Do for every event
  
    if (useMulineEvt) { 
  
      if ( !inputFile.is_open() ) {
        G4cout << "Set a vector file using the command /mygen/vecfile name"
  	     << G4endl;  
        return;
      }

      // We will need a particle table
      G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

      // Temporary kludge to turn on/off vector text format 
      G4bool useNuanceTextFormat = true;

      if (useNuanceTextFormat) {
        const int lineSize=100;
        char      inBuf[lineSize];
        std::vector<std::string> token(1);
      
        readInLine(inputFile, lineSize, inBuf,token);
      
        if (token.size() == 0) {
          if(m_rewind) { //GB : us  ed in vis.
            G4cout << "end of nuance vector file! We rewind it." << G4endl;
            inputFile.clear();
            inputFile.seekg(0);
            readInLine(inputFile, lineSize, inBuf,token);
            if (token.size() == 0) {
              G4cout << "Can't read nuance vector file anymore." << G4endl;
              return;
            }
          } else {
            G4cout << "end of nuance vector file!" << G4endl;
            return;
          }
        }
	
        if (token[0] != "begin") {
          G4cout << "unexpected line begins with " << token[0] << G4endl;
        } else {  // normal parsing begins here 
	// Read the nuance line (ignore value now)
	
	readInLine(inputFile, lineSize, inBuf,token);
	mode = atoi(token[1]);
	
	// Read the Vertex line
	
	readInLine(inputFile, lineSize, inBuf,token);
	vtx = G4ThreeVector(atof(token[1])*CLHEP::cm,
			    atof(token[2])*CLHEP::cm,
			    atof(token[3])*CLHEP::cm);
	
	// true : Generate vertex in Rock , false : Generate vertex in WC tank
	SetGenerateVertexInRock(false);
	//true : if you use +-18m wide beam and z position is anywhere in rock
	G4bool beamprofile_is_18m = true;
	
	// DON'T DO ANYTHING
	// Randomize the vertex if it is not set
	G4double wclen;
	G4double wczpos;
	G4double rockthickness = 0.;
	rockthickness = fMyDetector.GetRockThickness();
	//JEC 18/11/05 not used G4int _myconfiguration = fMyDetector.GetMyConfiguration();
	//JEC 18/11/05 not used G4int _detectorchoice = fMyDetector.GetDetectorChoice();
	
	if (IsGeneratingVertexInRock()){
	  
	  if (beamprofile_is_18m){  //  beam size(xy) is 36 x 36 m (+-18m)
	    wclen = rockthickness*2. + 34.5*CLHEP::m; 
	  }  else { //  beam size(xy) is 8 x 8 m (+-4m)
	    wclen = rockthickness+1.48*CLHEP::m; //vertex is inside the upstream rock
	  }//eo if beamprofile
	  
	  //JEC 18/11/05 use a default value FIXME
	  wczpos = -21.92*CLHEP::m - rockthickness + wclen/2.; // curved rock wall -23.68*m
	  //	      if (_myconfiguration==2){
	  //	      } else { // _myconfiguration==1
	  //	      wczpos = -21.92*m - rockthickness + wclen/2.; // curved rock wall -23.68*m
	  //                 if (_detectorchoice==1){ //lAr
	  // 		  wczpos = -13.32*m - rockthickness + wclen/2.; 
	  //                 } else {// FGD
	  // 		  wczpos = -21.82*m - rockthickness + wclen/2.;
	  // 		}
	  //} 
	} else { // vertex is inside Water tank
	  wclen = 16.0*CLHEP::m; // TO DO this should be retrieved!
	  wczpos = 0.*CLHEP::m;  // TO DO this should be retrieved!
	}//eo if Vertex in Rock
	
	if (fabs(vtx[2]) < 0.000001) {
	  if (IsGeneratingVertexInRock()) {
	    G4double random_z = wczpos - wclen/2 + wclen*G4UniformRand();
	    vtx[2] = random_z;
	    _counterRock++;
	    _counterCublic++;
	    G4Navigator* tmpNavigator = 
	      G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
	    G4VPhysicalVolume* tmpVolume = 
	      tmpNavigator->LocateGlobalPointAndSetup(vtx);
	    G4String vtxVolumeName = tmpVolume->GetName(); 
	    while (!vtxVolumeName.contains("Rock")){
	      random_z = wczpos - wclen/2 + wclen*G4UniformRand();
	      vtx[2] = random_z;
	      _counterCublic++;
	      tmpVolume = tmpNavigator->LocateGlobalPointAndSetup(vtx);
	      vtxVolumeName = tmpVolume->GetName();
	    }//eo while in the Rock
	  } else { // Default
	    G4cout << "warning : using WClen= 16m for random vertex generation\n";
	    G4double random_z = wczpos - wclen/2 + wclen*G4UniformRand();
	    vtx[2] = random_z;
	    _counterRock++; _counterCublic++;
	  }//eo Vertex in Rock
	}//eo vtx[2] close to 0
	
	// Next we read the incoming neutrino and target
	
	// First, the neutrino line
	readInLine(inputFile, lineSize, inBuf,token);
	beampdg = atoi(token[1]);
	beamenergy = atof(token[2])*CLHEP::MeV;
	beamdir = G4ThreeVector(atof(token[3]),
				atof(token[4]),
				atof(token[5]));
	
	// Now read the target line
	
	readInLine(inputFile, lineSize, inBuf,token);
	targetpdg = atoi(token[1]);
	targetenergy = atof(token[2])*CLHEP::MeV;
	targetdir = G4ThreeVector(atof(token[3]),
				  atof(token[4]),
				  atof(token[5]));
	
	// Read the info line, basically a dummy
	readInLine(inputFile, lineSize, inBuf,token);
	//	G4cout << "Vector File Record Number " << token[2] << G4endl;
	vecRecNumber = atoi(token[2]);
	
	// Now read the outgoing particles
	// These we will simulate.
	
	while (true) {
	  readInLine(inputFile, lineSize, inBuf,token);
	  if(token[0] != "track") break;
	  
	  // We are only interested in the particles
	  // that leave the nucleus, tagged by "0"
	  if ( token[6] == "0") {
	    G4int pdgid = atoi(token[1]);
	    G4double energy = atof(token[2])*CLHEP::MeV;
	    G4ThreeVector dir = G4ThreeVector(atof(token[3]),
					      atof(token[4]),
					      atof(token[5]));
	    //JEC 16/1/06 take care of exhautic pdgid
	    G4ParticleDefinition* partDef = particleTable->FindParticle(pdgid);
	    if(partDef){
	      particleGun->SetParticleDefinition(partDef);
	      //JEC 16/1/06 protect against Non-PDG code used in NUANCE: eg. 8016=Oxygen
	      G4double mass = particleGun->GetParticleDefinition()->GetPDGMass();
	      
	      G4double ekin = energy - mass;
	      
	      particleGun->SetParticleEnergy(ekin);
	      G4cout << "vertex set particle :"
		     << " pdg " << pdgid
		     << ", name " << partDef->GetParticleName()
		     << ", KE " << ekin
		     << ", vtx " << vtx[0] << " " << vtx[1] << " " << vtx[2]
		     << ", dir " << dir[0] << " " << dir[1] << " " << dir[2]
		     << G4endl;
	      particleGun->SetParticlePosition(vtx);
	      particleGun->SetParticleMomentumDirection(dir);
	      particleGun->GeneratePrimaryVertex(anEvent);
	    }//Good particle (JEC 16/1/06)
	  }//eo Final particle
	}//eo while track scan
      }
    }//eo Nuance Style

    } else if (useNormalEvt) {      // manual gun operation
      particleGun->GeneratePrimaryVertex(anEvent);
      
      G4ThreeVector P   =anEvent->GetPrimaryVertex()->GetPrimary()->GetMomentum();
      G4ThreeVector _vtx=anEvent->GetPrimaryVertex()->GetPosition();
      G4double m        =anEvent->GetPrimaryVertex()->GetPrimary()->GetMass();
      G4int pdg         =anEvent->GetPrimaryVertex()->GetPrimary()->GetPDGcode();
      
      G4ThreeVector dir  = P.unit();
      G4double E         = std::sqrt((P.dot(P))+(m*m));
      
      //     particleGun->SetParticleEnergy(E);
      //     particleGun->SetParticlePosition(_vtx);
      //     particleGun->SetParticleMomentumDirection(dir);
      
      SetVtx(_vtx);
      SetBeamEnergy(E);
      SetBeamDir(dir);
      SetBeamPDG(pdg);
    }//eo Particle Gun choice
  
  }

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
  class PrimaryGeneratorMessenger: public G4UImessenger {
  public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction* pointerToAction):myAction(pointerToAction) {
      mydetDirectory = new G4UIdirectory("/mygen/");
      mydetDirectory->SetGuidance("MEMPHYS detector control commands.");
  
      genCmd = new G4UIcmdWithAString("/mygen/generator",this);
      genCmd->SetGuidance("Select primary generator.");
      genCmd->SetGuidance(" Available generators : muline, normal");
      genCmd->SetParameterName("generator",true);
      genCmd->SetDefaultValue("normal");
      genCmd->SetCandidates("muline normal");
  
      fileNameCmd = new G4UIcmdWithAString("/mygen/vecfile",this);
      fileNameCmd->SetGuidance("Select the file of vectors.");
      fileNameCmd->SetGuidance(" Enter the file name of the vector file");
      fileNameCmd->SetParameterName("fileName",true);
      fileNameCmd->SetDefaultValue("inputvectorfile");
    }
    virtual ~PrimaryGeneratorMessenger() {
      delete genCmd;
      delete fileNameCmd; //JEC 24/1/06
      delete mydetDirectory;
    }
  public:
    virtual void SetNewValue(G4UIcommand* command, G4String newValue) {
      if ( command == genCmd ) {
        if ( newValue == "muline" ) {
          myAction->SetMulineEvtGenerator(true);
          myAction->SetNormalEvtGenerator(false);
        } else if ( newValue == "normal" ) {
          myAction->SetMulineEvtGenerator(false);
          myAction->SetNormalEvtGenerator(true);
        }
      }
      if ( command == fileNameCmd ) {
        myAction->OpenVectorFile(newValue);
        G4cout << "Input vector file set to " << newValue << G4endl;
      }
    }

    virtual G4String GetCurrentValue(G4UIcommand* command) {
      G4String cv;
      if( command == genCmd ) {
        if( myAction->IsUsingMulineEvtGenerator() ) { cv = "muline"; }
        if( myAction->IsUsingNormalEvtGenerator() ) { cv = "normal"; }
      }
      return cv;
    }
    
  private:
    PrimaryGeneratorAction* myAction;
  private: //commands
    G4UIdirectory*      mydetDirectory;
    G4UIcmdWithAString* genCmd;
    G4UIcmdWithAString* fileNameCmd;
  };

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


