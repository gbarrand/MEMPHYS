#ifndef MEMPHYSWCDigitizer_h
#define MEMPHYSWCDigitizer_h 1

//Geant4
#include "G4VDigitizerModule.hh"
#include "globals.hh"
#include "Randomize.hh"

//MEMPHYS
#include "WCDigi.hh"

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {

class WCDigitizer : public G4VDigitizerModule {
public:
  
  WCDigitizer(G4String name);
  virtual ~WCDigitizer();
  
  void SetPMTSize(G4float inputSize) {PMTSize = inputSize;}
  void Digitize();
  
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
  static const G4double offset; // hit time offset ns
  static const G4double pmtgate; // time window to accept hit for a PMT
  static const G4double eventgateup; //time window for the event
  static const G4double LongTime;   //arbitrary long time
  static const G4int    GlobalThreshold; //# of hits to trigger t0 determination
  
  static const G4int    TimeHistoMaxBin = 20000; // 20,000*5ns = 100ms: JEC 20/4/06 new var.
                                                 // maximum number of bins of the trigger histo to determine the t0
  G4int triggerhisto[TimeHistoMaxBin]; // trigger histo for finding t0
  static const G4int    TimeHistoStepBin; // time window to count the hits
  static const G4float  TimeHistoStep; // time bin width of the trigger histo

  G4float RealOffset;  // t0 = offset corrected for trigger start
  //JEC not used  G4float MinTime;  // very first hit time

  WCDigitsCollection*  DigitsCollection;
  
  G4float PMTSize; //not yet used but may be in the future when OD + ID...
};

}
#endif








