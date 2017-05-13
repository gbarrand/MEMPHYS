// Usage :
//   OS> cd <some path>/MEMPHYS/<version>/cmt
//   OS> source setup.csh
//   OS> ../<platform>/MEMPHYS_batch.exe
//   OS> root ../scripts/ROOT/MEMPHYS.C
//
// Note :
//   The MEMPHYS_batch.exe program produces a MEMPHYS.root file.
//   This file sould have been produced with the "export=root"
//   option when creating the AIDA::ITree for this file
//   (see MEMPHYS::Analysis constructor).
//     By default the Lab package does not stream its histograms
//   as THs of ROOT (because the AIDA histos are more rich). 
//   In ordet that ROOT plot the saved histos, the Lab::Histograms
//   can be saved as THs by using the "export=root" option.

{
  TFile* file = new TFile("MEMPHYS.root");
  file->ls();

  //TH1F* histoDE = file->Get("histo_DE");
  TH1F* histo = file->Get("histo_HitTotalE");

  TCanvas* canvas = new TCanvas("canvas","TH1D",10,10,800,600);

  histo->Draw();                                 

  canvas->Update();

  file->Close();
  
}
