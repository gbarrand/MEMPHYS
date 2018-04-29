#include "../MEMPHYS/WCDigitizer.hh"

//Geant4
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4DigiManager.hh"
#include "G4ios.hh"

//std
#include <vector>
#include <cstring>

//MEMPHYS
#include "../MEMPHYS/WCDigi.hh"
#include "../MEMPHYS/WCHit.hh"


const G4double MEMPHYS::WCDigitizer::offset          = 950.0; //ns  
                                                            //JEC what is relevant is the difference of time...
const G4double MEMPHYS::WCDigitizer::pmtgate         = 300.0; // ns
const G4double MEMPHYS::WCDigitizer::eventgateup     = 950.0; // ns
//   const G4double eventgatedown   = -400.0; ns JEC 20/4/06 not used ?
const G4double MEMPHYS::WCDigitizer::LongTime        = 100000.0 ; // ns
const G4int    MEMPHYS::WCDigitizer::GlobalThreshold = 10;
const G4int    MEMPHYS::WCDigitizer::TimeHistoStepBin = 40;   // 40*5ns = 200ns    : JEC 20/4/06 new var.
const G4float  MEMPHYS::WCDigitizer::TimeHistoStep   = 5.0;   //ns                 : JEC 20/4/06 new var.
                                                              


MEMPHYS::WCDigitizer::WCDigitizer(G4String name) : G4VDigitizerModule(name) {
  G4String colName = "WCDigitizedCollection";
  collectionName.push_back(colName);  
}//Ctor

//---------------------------------------------------------------------------

MEMPHYS::WCDigitizer::~WCDigitizer() {;}

//---------------------------------------------------------------------------

void MEMPHYS::WCDigitizer::Digitize() {
  DigitsCollection = new WCDigitsCollection("/MEMPHYS/WCPMT",collectionName[0]); 
  
  G4DigiManager* DigiMan = G4DigiManager::GetDMpointer();
  
  // Get the Associated Hit collection IDs
  G4int WCHCID = DigiMan->GetHitsCollectionID("WCPMT");

  // The Hits collection
  WCHitsCollection* WCHC = (WCHitsCollection*)(DigiMan->GetHitsCollection(WCHCID));
  if(0 == WCHC) {
    StoreDigiCollection(DigitsCollection);
    return;
  }
  
    
  // JEC 19/1/06 the definition of t0 may be more appropriatly defined as
  // t0 = "offset" - (time after which "GlobalThreshold" hits have been observed) = RealOffset
  
  std::memset(triggerhisto,0,WCDigitizer::TimeHistoMaxBin*sizeof(G4int)); //JEC FIXME not very smart but efficient...
  RealOffset = WCDigitizer::offset;    
  //JEC 20/4/06 not realy used  MinTime = WCDigitizer::LongTime;  
  G4float tc;
  for (G4int i = 0 ; i < WCHC->entries() ; i++) {
    
    (*WCHC)[i]->SortHitTimes(); //JEC FIXME this is mandatory (see WCHit.cpp)

    tc = (*WCHC)[i]->GetTime(0);
    //JEC  20/4/06  if ( tc < MinTime ) MinTime = tc;
    for ( G4int ip = 0 ; ip < (*WCHC)[i]->GetTotalPe() ; ip++) {
      tc = (*WCHC)[i]->GetTime(ip);
      if ( tc < WCDigitizer::LongTime) 
	triggerhisto[int(floor(tc/WCDigitizer::TimeHistoStep))]++;     //JEC FIXME step of 5ns?
    }//loop on time registered in a PMT
  }//Loop on hit (=PMT)    
  
  G4int acc = 0; // accumulated # hits within time window...
  //G4int accold = 0;
  G4int beginning;
  
  for ( G4int j = 1 ; j <= WCDigitizer::TimeHistoMaxBin ; j++) {
    beginning = ( (j+1-WCDigitizer::TimeHistoStepBin>1) ? (j+1-WCDigitizer::TimeHistoStepBin) : 1 );
    acc = 0;
    for ( G4int k = beginning ; k <= j; k++) { 
      acc += triggerhisto[k-1];
    }
    if ( acc > WCDigitizer::GlobalThreshold ) {
      RealOffset -= WCDigitizer::TimeHistoStep*float(j);
      break;
    }
    //accold = acc; // check if it works, will be removed soon	
  }    

//   G4cout << "(JEC) WCDigitizer: ";
//   G4cout << " acc = " << acc << " old : " << accold <<" ; RealOffset = " << RealOffset << std::endl;

  
  G4int oldtotal = 0;
  G4float newtotal = 0;
  
  //JEC 19/1/06 only 8" cte for the moment
  G4float timingConstant = 1.890; // M Fechner : JEC FIXME for 12"
  G4double efficiency = 0.985; // with rn1pe   : JEC FIXME ?
  G4double bound1;
  G4double bound2;
  G4double peSmeared;
  G4int iflag;
  G4float Q;
  G4float timingResolution;    
  G4double digihittime;
  G4int   tube;
  G4float firstHitTime;
  G4float totalPe;
  
  for (G4int i=0; i < WCHC->entries(); i++) {
    
    // Get the information from the hit
    tube         = (*WCHC)[i]->GetTubeID();
    // august 2004 : sorting is necessary before doing this !!!     
    firstHitTime = (*WCHC)[i]->GetTime(0);
    
    bound1 = -RealOffset  + WCDigitizer::offset + WCDigitizer::eventgateup;
    bound2 = firstHitTime + WCDigitizer::pmtgate;
    
    totalPe = (*WCHC)[i]->GetPeInGate(bound1,bound2);
    newtotal += totalPe;
    oldtotal += (*WCHC)[i]->GetTotalPe();
    
//     G4cout << "(JEC) WCDigitizer hit(" << i << ")"
// 	   << " b1: " << bound1
// 	   << " b2: " << bound2
// 	   << " totalPeInGate: " << totalPe; 

    // Now digitize this hit    
    do {
      peSmeared  = (G4RandGauss::shoot(double(totalPe),sqrt(totalPe))); //confirmed by M.F (truncated Gauss fnt)
    } while (peSmeared < 0.0 );
    
//     G4cout << " peSmear: " << peSmeared;
    
    WCDigitizer::Threshold(peSmeared,iflag);

//     G4cout << " peThres " <<  peSmeared << " flag " << iflag;

    peSmeared *= efficiency; // MC tuning correction from data
    
//     G4cout << " pe Eff " <<  peSmeared << G4endl;

    if ( 0 == iflag ) {
      // MF : reorganized the algorithm. Because of the offset, very early
      // hits can appear to be negative (if the t0 is triggered much later)
      // they have to be removed otherwise nothing will work.
            
      // Add cutoffs
      //JEC FIXME constante 0.5, 0.33, 0.58 ???
      Q = (peSmeared > 0.5) ? peSmeared : 0.5;      
      timingResolution = 0.33 + sqrt(timingConstant/Q);
      if (timingResolution < 0.58) timingResolution=0.58;
      
      digihittime = RealOffset + firstHitTime + G4RandGauss::shoot(0.0,timingResolution);
      
//       G4cout << " digitTime " << digihittime << " pe " << peSmeared << G4endl;

      if ( digihittime > 0.0 && peSmeared>0.0) {	  
	WCDigi* Digi = new WCDigi();
	
	Digi->SetLogicalVolume((*WCHC)[i]->GetLogicalVolume());
	Digi->SetTransform((*WCHC)[i]->GetTransform());
	
	Digi->SetTubeID(tube);
	Digi->SetPe(peSmeared);
	Digi->SetTime(digihittime);
	DigitsCollection->insert(Digi);	
      }
    }//Threshold Ok (iflag=0)      
  } // Loop over hits
  
  //Save the collection
  StoreDigiCollection(DigitsCollection);
}//Digitize

