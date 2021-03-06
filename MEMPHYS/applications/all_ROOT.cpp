//
// Compiled version of the interpreted
//   ../scripts/ROOT/analysis.C
//

// To read data :
#include <TFile.h>
#include <TTree.h>

// To have histos :
#include <TH1D.h>
#include <TH2D.h>

// To plot histos :
#include <TCanvas.h>
#include <TSystem.h>
#include <TApplication.h>

#include <iostream>

#include "../scripts/ROOT/analysis.C"

int main(int aArgc,char* aArgv[]) {
  
  new TApplication("MEMPHYS_all_ROOT",&aArgc,aArgv);

  analysis();

  gSystem->Run();

  return 1;
}
