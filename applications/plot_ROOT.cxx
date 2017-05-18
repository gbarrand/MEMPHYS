
// A MEMPHYS.root file is read with the AIDA API and plotting is done with CERN-ROOT.

#include "analysis.icc"

#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TApplication.h>

#ifdef APP_USE_ARCHIVE
#include <BatchLab/Core/Main.h>
extern "C" {
  void BatchLabRioInitialize(Slash::Core::ISession&);
}
#endif

#include <iostream>

int main(int aArgc,char* aArgv[]) {
  //ROOT::GetROOT();
  
  new TApplication("MEMPHYS_analysis_aida_ROOT",&aArgc,aArgv);

  ////////////////////////////////////////////////////////
  // Create histograms : /////////////////////////////////
  ////////////////////////////////////////////////////////
  TH1D* hits_times = new TH1D("hits_times","Hits times",100,0,3000);

  TH2D* digits_time_pe = new TH2D("digits_pe_time","Digits PE time",100,0,3000,100,0,10);

  ////////////////////////////////////////////////////////
  /// Read data : ////////////////////////////////////////
  ////////////////////////////////////////////////////////
#ifdef APP_USE_ARCHIVE
  BatchLab::Main* session = new BatchLab::Main(std::vector<std::string>());
  Slash::Core::ILibraryManager* libraryManager = Slash::libraryManager(*session);
  if(!libraryManager) {std::cout << "LibraryManager not found." << std::endl;return EXIT_FAILURE;}
  Slash::Core::ILibrary* library = libraryManager->addLibrary("Rio","main","");
  if(!library) {std::cout << "addLibrary() failed." << std::endl;return EXIT_FAILURE;}
  ::BatchLabRioInitialize(*session);
  AIDA::IAnalysisFactory* aida = static_cast<AIDA::IAnalysisFactory*>(session);
#else
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
#endif
  if(!aida) {
    std::cout << "AIDA not found." << std::endl;
    return EXIT_FAILURE;
  }

  if(!read_data(*aida,*hits_times,*digits_time_pe)) {
    std::cout << "can't read data file." << std::endl;
    delete aida;
    return EXIT_FAILURE;
  }

  delete aida;

  ////////////////////////////////////////////////////////
  /// plot histos : //////////////////////////////////////
  ////////////////////////////////////////////////////////
  TCanvas* plotter = new TCanvas("canvas","",10,10,800,600);
  plotter->Divide(1,2);  

  plotter->cd(1);
  hits_times->Draw();

  plotter->cd(2);
  digits_time_pe->Draw();

  plotter->Update();

  gSystem->Run();

  return EXIT_SUCCESS;
}
