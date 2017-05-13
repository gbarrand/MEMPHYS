//
// Compiled version of the interpreted
//   ../scripts/ROOT/aida_ROOT.C
// which uses OpenScienitst/AIDA to read a MEMPHYS.root
// file and the TH* for the histogramming and plotting.
//

#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/IManagedObject.h>
#include <AIDA/ITupleEntry.h>
#include <AIDA/ITuple.h>

#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TApplication.h>

#include <iostream>

#include "../scripts/ROOT/aida_ROOT.C"

int main(int aArgc,char* aArgv[]) {
  //ROOT::GetROOT();
  
  new TApplication("MEMPHYS_analysis_aida_ROOT",&aArgc,aArgv);

  aida_ROOT();

  gSystem->Run();

  return 1;
}
