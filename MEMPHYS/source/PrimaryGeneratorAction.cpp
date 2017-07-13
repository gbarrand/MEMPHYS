#include "../MEMPHYS/PrimaryGeneratorAction.hh"

//Geant4
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"

//std
#include <fstream>
#include <vector>
#include <string>

//MEMPHYS
#include "../MEMPHYS/DetectorConstruction.hh"
#include "../MEMPHYS/PrimaryGeneratorMessenger.hh"

// Returns a vector with the tokens
inline void tokenize( std::string separators, std::string input,std::vector<std::string>& a_tokens)  {
  a_tokens.clear();
  
  std::string::size_type startToken = 0, endToken; // Pointers to the token pos
  
  if( separators.size() > 0 && input.size() > 0 ) 
    {
    
      while( startToken < input.size() )
	{
	  // Find the start of token
	  startToken = input.find_first_not_of( separators, startToken );
      
	  // If found...
	  if( startToken != input.npos ) 
	    {
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
}//tokenize

inline void readInLine(std::fstream& inFile, int lineSize, char* inBuf,std::vector<std::string>& a_tokens) {
  // Read in line break it up into tokens
  inFile.getline(inBuf,lineSize);
  tokenize(" $", inBuf,a_tokens);
}//readInLine

inline float atof( const std::string& s ) {return std::atof( s.c_str() );}
inline int   atoi( const std::string& s ) {return std::atoi( s.c_str() );}



MEMPHYS::PrimaryGeneratorAction::PrimaryGeneratorAction(MEMPHYS::DetectorConstruction& myDC,const std::string& a_res_dir)
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
}//Ctor

//-------------------------------------------------------------------------------------------------

MEMPHYS::PrimaryGeneratorAction::~PrimaryGeneratorAction() {
  if (IsGeneratingVertexInRock()){
      G4cout << "Fraction of Rock volume is : " << G4endl;
      G4cout << " Random number generated in Rock / in Cublic = " 
             << _counterRock << "/" << _counterCublic 
             << " = " << _counterRock/(G4double)_counterCublic << G4endl;
  }
  inputFile.close();
  delete particleGun;
  delete messenger;
}//Dtor

//-------------------------------------------------------------------------------------------------

void MEMPHYS::PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

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
	if(m_rewind) { //GB : used in vis.
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
	      G4cout << "vertex set particle : " << pdgid << ", name " << partDef->GetParticleName() << ", KE " << ekin << G4endl;
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
      
      G4ThreeVector P  =anEvent->GetPrimaryVertex()->GetPrimary()->GetMomentum();
      G4ThreeVector vtx=anEvent->GetPrimaryVertex()->GetPosition();
      G4double m       =anEvent->GetPrimaryVertex()->GetPrimary()->GetMass();
      G4int pdg        =anEvent->GetPrimaryVertex()->GetPrimary()->GetPDGcode();
      
      G4ThreeVector dir  = P.unit();
      G4double E         = std::sqrt((P.dot(P))+(m*m));
      
      //     particleGun->SetParticleEnergy(E);
      //     particleGun->SetParticlePosition(vtx);
      //     particleGun->SetParticleMomentumDirection(dir);
      
      SetVtx(vtx);
      SetBeamEnergy(E);
      SetBeamDir(dir);
      SetBeamPDG(pdg);
  }//eo Particle Gun choice
  
}//GeneratePrimaries

//-------------------------------------------------------------------------------------------------

