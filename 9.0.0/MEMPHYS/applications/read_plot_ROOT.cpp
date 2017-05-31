
// A MEMPHYS.root file is read with the inlib/rroot API and plotting is done with CERN-ROOT.

#include "read.icc"

#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TApplication.h>

#include <inlib/args>
#include <iostream>

int main(int a_argc,char* a_argv[]) {

#ifdef INLIB_MEM
  inlib::mem::set_check_by_class(true);{
#endif //INLIB_MEM

  inlib::args args(a_argc,a_argv);
  std::string file;
  if(!args.file(file)) file = "MEMPHYS.root";
  
  new TApplication("MEMPHYS_analysis_aida_ROOT",&a_argc,a_argv);

  ////////////////////////////////////////////////////////
  // Create histograms : /////////////////////////////////
  ////////////////////////////////////////////////////////
  TH1D* hits_times = new TH1D("hits_times","Hits times",100,0,3000);

  TH2D* digits_time_pe = new TH2D("digits_pe_time","Digits PE time",100,0,3000,100,0,10);

  ////////////////////////////////////////////////////////
  /// Read data : ////////////////////////////////////////
  ////////////////////////////////////////////////////////

  if(!read_data(std::cout,file,*hits_times,*digits_time_pe)) {
    std::cout << "can't read data file." << std::endl;
    return EXIT_FAILURE;
  }

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

#ifdef INLIB_MEM
  }inlib::mem::balance(std::cout);
#endif //INLIB_MEM

  return EXIT_SUCCESS;
}
