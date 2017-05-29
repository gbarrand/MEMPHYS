//
//  This script shows how to read, with ROOT, a MEMPHYS.root 
// file having the "TTree within TTree" of the MEMPHYS_batch
// progrem (see Analysis.cpp).
//
//  Usage :
//    OS> <setup ROOT>
//  ( OS> root batch_dummy.C )
//    OS> root analyis.C
//
//  G.Barrand
//

//////////////////////////////////////////////////////////////////////////////
bool plot(
 TH1D& aHisto1D
,TH2D& aHisto2D
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  TCanvas* plotter = new TCanvas("canvas","",10,10,800,600);
  plotter->Divide(1,2);  

  plotter->cd(1);
  aHisto1D.Draw();

  plotter->cd(2);
  aHisto2D.Draw();

  plotter->Update();

  return true;
}
//////////////////////////////////////////////////////////////////////////////
void analysis() 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{

  ////////////////////////////////////////////////////////
  // Create histograms ///////////////////////////////////
  ////////////////////////////////////////////////////////
  TH1D* hits_times = new TH1D("hits_times","Hits times",100,0,3000);

  TH2D* digits_time_pe = new TH2D("digits_pe_time","Digits PE time",100,0,3000,100,0,10);

  int hit_count = 0;
  bool dump = true;
  bool dump_tracks = false;
  dump_tracks = true;
  bool dump_hits = false;
  //dump_hits = true;
  bool dump_digits = false;
  //dump_digits = true;
  ////////////////////////////////////////////////////////
  /// Read data and fill histos //////////////////////////
  ////////////////////////////////////////////////////////
  std::string root_file = "MEMPHYS.root";
  if(gApplication) {
    int argc = gApplication->Argc();
    char** argv = gApplication->Argv();
    for(int iarg=0;iarg<argc;iarg++) {
      if(!strncmp(argv[iarg],"-file=",6)) {
	root_file = argv[iarg];
	root_file = root_file.substr(6,root_file.size()-6+1);
      }
    }
  }
  TFile* file = new TFile(root_file.c_str());
  TTree* tEvent = (TTree*)file->Get("Event");

 {TObjArray* brs = tEvent->GetListOfBranches();
  for(int i=0;i<brs->GetEntries();i++) {
    TBranch* b = (TBranch*)brs->At(i);
    std::cout << " Event branch : " << b->GetName()
    	      << std::endl;    
  }}

  //declare Non subTuple part
  Int_t eventId,inputEvtId, interMode, vtxVol;
  tEvent->SetBranchAddress("eventId",&eventId);
  tEvent->SetBranchAddress("inputEvtId",&inputEvtId);
  tEvent->SetBranchAddress("interMode",&interMode);
  tEvent->SetBranchAddress("vtxVol",&vtxVol);

  Int_t nPart,leptonIndex, protonIndex;
  tEvent->SetBranchAddress("nPart",&nPart);
  tEvent->SetBranchAddress("leptonIndex",&leptonIndex);
  tEvent->SetBranchAddress("protonIndex",&protonIndex);

  Int_t nHits; 
  tEvent->SetBranchAddress("nHits",&nHits);

  Int_t nDigits;
  Double_t sumPE;
  tEvent->SetBranchAddress("nDigits",&nDigits);
  tEvent->SetBranchAddress("sumPE",&sumPE);

  std::vector<double>* vtxPos = 0;
  tEvent->SetBranchAddress("vtxPos",&vtxPos);
  
  Int_t nEvent = (Int_t)tEvent->GetEntries();  
  //nEvent = 1;
  //nEvent = 10000;
  std::cout << " nEvents = " << nEvent << std::endl;

  for (Int_t iev=0; iev<nEvent; ++iev) {

    TTree* Event_track = new TTree();
    tEvent->SetBranchAddress("track",&Event_track);

    TTree* Event_hit = new TTree();
    tEvent->SetBranchAddress("hit",&Event_hit);
    
    TTree* Event_digit = new TTree();
    tEvent->SetBranchAddress("digit",&Event_digit);

    tEvent->GetEntry(iev);

    //{TObjArray* brs = Event_track->GetListOfBranches();
    //for(int i=0;i<brs->GetEntries();i++) {
    //  TBranch* b = (TBranch*)brs->At(i);
    //  std::cout << " Event_track branch : " << b->GetName() << std::endl;    
    //  }}

    // Bind sub tuple variables 
    Double_t vx = (*vtxPos)[0];
    Double_t vy = (*vtxPos)[1];
    Double_t vz = (*vtxPos)[2];
    
    Int_t pId,parent;
    Float_t timeStart;
    Double_t mass, pTot, ETot;
    Int_t startVol, stopVol;
    Event_track->SetBranchAddress("pId",&pId);
    Event_track->SetBranchAddress("parent",&parent);
    Event_track->SetBranchAddress("timeStart",&timeStart);
    Event_track->SetBranchAddress("mass",&mass);
    Event_track->SetBranchAddress("pTot",&pTot);
    Event_track->SetBranchAddress("ETot",&ETot);
    Event_track->SetBranchAddress("startVol",&startVol);
    Event_track->SetBranchAddress("stopVol",&stopVol);

    std::vector<double>* direction = 0;
    Event_track->SetBranchAddress("direction",&direction);
    std::vector<double>* momentum = 0;
    Event_track->SetBranchAddress("momentum",&momentum);
    std::vector<double>* startPos = 0;
    Event_track->SetBranchAddress("startPos",&startPos);
    std::vector<double>* stopPos = 0;
    Event_track->SetBranchAddress("stopPos",&stopPos);
    
    Int_t tubeId_hit; //JEC 16/1/06
    Int_t totalPE;    
    Event_hit->SetBranchAddress("tubeId",&tubeId_hit); //JEC 16/1/06   
    Event_hit->SetBranchAddress("totalPE",&totalPE);

    Int_t tubeId_digit;
    Double_t digit_pe, digit_time;
    Event_digit->SetBranchAddress("tubeId",&tubeId_digit);
    Event_digit->SetBranchAddress("pe",&digit_pe);
    Event_digit->SetBranchAddress("time",&digit_time);
    
    if(dump)
    std::cout << ">>>>>>>>>>>>> Event{" << iev << "}: "
    	      << " evt Id " << eventId 
    //	      << " evt Input Id " << inputEvtId
    //	      << "\n interaction mode " << interMode
    //	      << " start in volume " << vtxVol << "\n"
    	      <<" #tracks: " << nPart
    	      <<" #hits: " << nHits
    	      <<" #digits: " << nDigits
	      << std::endl
	      << "vtxPos x: " << vx << ", y: " << vy << ", z:" << vz
    	      << std::endl;

    Int_t nTracks = (Int_t)Event_track->GetEntries();
    Int_t nTubeHits = (Int_t)Event_hit->GetEntries();
    Int_t nTubeDigits = (Int_t)Event_digit->GetEntries();

    if(dump)
    std::cout << "Verif :" 
              << " nTracks = " << nTracks 
    	      << " nTube Hits = " << nTubeHits
    	      << " nTube Digits = " << nTubeDigits
    	      << std::endl;

    
    
    for (Int_t jtk=0; jtk<nTracks; ++jtk) {

      Event_track->GetEntry(jtk);

      double dx = (*direction)[0];
      double dy = (*direction)[1];
      double dz = (*direction)[2];

      double px = (*momentum)[0];
      double py = (*momentum)[1];
      double pz = (*momentum)[2];
  
      double start_x = (*startPos)[0];
      double start_y = (*startPos)[1];
      double start_z = (*startPos)[2];
      
      double stop_x = (*stopPos)[0];
      double stop_y = (*stopPos)[1];
      double stop_z = (*stopPos)[2];
      
      if(dump_tracks)
      std::cout << "----> Tk{"<< jtk <<"}: " 
	   	<< " pId " << pId
		<< " parent " << parent
		<< " creation time " << timeStart 
		<< " Volumes " << startVol << " " << stopVol << "\n"
		<< " Start Pos (" << start_x << "," << start_y << "," << start_z << ")\n"
		<< " Stop Pos (" << stop_x << "," << stop_y << "," << stop_z << ")\n"
	        << " dx,dy,dz " << dx << " " << dy << " " << dz << "\n"
		<< " m " << mass
		<< " ETot " << ETot
		<< " pTot " << pTot
		<< " px,py,pz " << px << " " << py << " " << pz << "\n"
                << std::endl;

    }//loop on Tracks

    //--------
    // The Hits
    //--------
    std::vector<float>* pe = 0;
    Event_hit->SetBranchAddress("pe",&pe);

    for (Int_t khit=0; khit<nTubeHits; ++khit) {
      
      Event_hit->GetEntry(khit);

      //JEC 16/1/06 add the tubeId_hit info
      if(dump_hits)
      std::cout << "----> Hit{"<< khit <<"}: tube[" << tubeId_hit << "] total #PE " << totalPE << std::endl;

      for (size_t ki=0; ki<pe->size(); ++ki) {
        Float_t hit_time = (*pe)[ki];
        if(dump_hits) {
          std::cout << "<" << hit_time << ">";
	  //std::cout << "<" << trk_length << ">";
	}
        hits_times->Fill(hit_time);
        hit_count++;
      }
      if(dump_hits)
      std::cout << std::endl;
    }//Loop on Hits

    //--------
    // The Digits
    //--------
    for (Int_t l=0; l<nTubeDigits; ++l) {
      Event_digit->GetEntry(l);
      
      if(dump_digits)
      std::cout << "----> Digit{"<<l<<"}: " 
		<< "tube[" << tubeId_digit << "] = " 
		<< " pe: " << digit_pe
		<< " time: " << digit_time
		<< std::endl;
      
      digits_time_pe->Fill(digit_time,digit_pe);

    }//Loop on Digits

    delete Event_track;
    
    Event_hit->GetBranch("pe")->SetFile((TFile*)0);
    delete Event_hit;

    delete Event_digit;

  }//loop on event

  if(dump)
  std::cout << " nEvents = " << nEvent << " hits = " << hit_count << std::endl;

  ////////////////////////////////////////////////////////
  // Plot histograms /////////////////////////////////////
  ////////////////////////////////////////////////////////
  plot(*hits_times,*digits_time_pe);

}//read_event
