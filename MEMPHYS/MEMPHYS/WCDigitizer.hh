#ifndef MEMPHYSWCDigitizer_h
#define MEMPHYSWCDigitizer_h

#include "WCHit.hh"
#include "WCDigi.hh"

//Geant4
#include "G4VDigitizerModule.hh"
#include "Randomize.hh"
#include "G4DigiManager.hh"

namespace MEMPHYS {

class WCDigitizer : public G4VDigitizerModule {
public:
  
  WCDigitizer(G4String name) : G4VDigitizerModule(name) {
    G4String colName = "WCDigitizedCollection";
    collectionName.push_back(colName);  
  }

  virtual ~WCDigitizer() {}
  
  void SetPMTSize(G4float inputSize) {PMTSize = inputSize;}

  void Digitize() {
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
  }
  
private:
  template <class T> static T mn(const T& a,const T& b) {return (a<b?a:b);}    
  static void Threshold(double& pe,int& iflag){
    double x = pe+0.1; iflag=0;
    double thr; double RDUMMY,err;
    if ( x<1.1) {
      //thr = std::min(1.0, //problem with Windows/VC++.
      thr = mn(1.0,
	       -0.06374+x*(
	        3.748+x*(
	       -63.23+x*(
		452.0+x*(
	      -1449.0+x*(
	       2513.0+x*(
	       -2529.+x*(
               1472.0+x*(
               -452.2+x*(
                51.34+x*2.370))))))))));
    } else {
      thr = 1.0;
    }
    RDUMMY = G4UniformRand();
    if (thr < RDUMMY) {
      pe = 0.0;
      iflag = 1;
    } else {
      err = G4RandGauss::shoot(0.0,0.03);
      pe = pe+err;
    }
  }//Threshold

  //JEC 21/4/06 transfert the static variable here, but FIXME: should be put in a data file
  static constexpr G4double offset          = 950.0; // hit time offset. ns  
                                                            //JEC what is relevant is the difference of time...
  static constexpr G4double pmtgate         = 300.0; // time window to accept hit for a PMT. ns
  static constexpr G4double eventgateup     = 950.0; // time window for the event. ns
  //   constexpr G4double eventgatedown   = -400.0; ns JEC 20/4/06 not used ?
  static constexpr G4double LongTime        = 100000.0 ; // arbitrary long time. ns
  static constexpr G4int    GlobalThreshold = 10; //# of hits to trigger t0 determination.
  static constexpr G4int    TimeHistoStepBin = 40;   // time window to count the hits. 40*5ns = 200ns    : JEC 20/4/06 new var.
  static constexpr G4float  TimeHistoStep   = 5.0;   // time bin width of the trigger histo. ns                 : JEC 20/4/06 new var.

  static constexpr G4int    TimeHistoMaxBin = 20000; // 20,000*5ns = 100ms: JEC 20/4/06 new var.
                                                 // maximum number of bins of the trigger histo to determine the t0
  G4int triggerhisto[TimeHistoMaxBin]; // trigger histo for finding t0

  G4float RealOffset;  // t0 = offset corrected for trigger start
  //JEC not used  G4float MinTime;  // very first hit time

  WCDigitsCollection*  DigitsCollection;
  
  G4float PMTSize; //not yet used but may be in the future when OD + ID...
};

}
#endif








