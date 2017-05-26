//
//  This script shows how to read, with ROOT, a MEMPHYS.root 
// file having the "TTree within TTree" of the MEMPHYS_batch
// progrem (see Analysis.cxx).
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
void analysis_inlib() 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{

  ////////////////////////////////////////////////////////
  // Create histograms ///////////////////////////////////
  ////////////////////////////////////////////////////////
  TH1D* hits_times = new TH1D("hits_times","Hits times",100,0,3000);

  TH2D* digits_time_pe = 
    new TH2D("digits_pe_time","Digits PE time",100,0,3000,100,0,10);

  int hit_count = 0;
  bool dump = false;
  //dump = true;

  ////////////////////////////////////////////////////////
  /// Read data and fill histos //////////////////////////
  ////////////////////////////////////////////////////////
  TFile* file = new TFile("MEMPHYS_inlib.root");
  TTree* tEvent = (TTree*)file->Get("event");

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

  /*
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
  */
  
  Int_t nEvent = (Int_t)tEvent->GetEntries();  
  //nEvent = 1;
  //nEvent = 10000;
  std::cout << " nEvents = " << nEvent << std::endl;
  /*
  for (Int_t iev=0; iev<nEvent; ++iev) {

    TTree* Event_vtxPos = new TTree();
    tEvent->SetBranchAddress("vtxPos",&Event_vtxPos);

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
    Double_t vx,vy,vz;
    Event_vtxPos->SetBranchAddress("x",&vx);
    Event_vtxPos->SetBranchAddress("y",&vy);
    Event_vtxPos->SetBranchAddress("z",&vz);

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
    	      << std::endl;

    Int_t nVtx = (Int_t)Event_vtxPos->GetEntries();
    Int_t nTracks = (Int_t)Event_track->GetEntries();
    Int_t nTubeHits = (Int_t)Event_hit->GetEntries();
    Int_t nTubeDigits = (Int_t)Event_digit->GetEntries();

    if(dump)
    std::cout << "Verif :" 
              << " nVtx = " << nVtx 
              << " nTracks = " << nTracks 
    	      << " nTube Hits = " << nTubeHits
    	      << " nTube Digits = " << nTubeDigits
    	      << std::endl;

    for (Int_t jtk=0; jtk<nTracks; ++jtk) {

      //Sub tuples of Event_tracks :
      TTree* Track_direction = new TTree();
      Event_track->SetBranchAddress("direction",&Track_direction);
      TTree* Track_momentum = new TTree();
      Event_track->SetBranchAddress("momentum",&Track_momentum);
      TTree* Track_startPos = new TTree();
      Event_track->SetBranchAddress("startPos",&Track_startPos);
      TTree* Track_stopPos = new TTree();
      Event_track->SetBranchAddress("stopPos",&Track_stopPos);

      Event_track->GetEntry(jtk);

      Double_t dx,dy,dz;
      Track_direction->SetBranchAddress("dx",&dx);
      Track_direction->SetBranchAddress("dy",&dy);
      Track_direction->SetBranchAddress("dz",&dz);

      Double_t px,py,pz;
      Track_momentum->SetBranchAddress("px",&px);
      Track_momentum->SetBranchAddress("py",&py);
      Track_momentum->SetBranchAddress("pz",&pz);

      Double_t start_x,start_y,start_z;
      Track_startPos->SetBranchAddress("x",&start_x);
      Track_startPos->SetBranchAddress("y",&start_y);
      Track_startPos->SetBranchAddress("z",&start_z);

      Double_t stop_x,stop_y,stop_z;
      Track_stopPos->SetBranchAddress("x",&stop_x);
      Track_stopPos->SetBranchAddress("y",&stop_y);
      Track_stopPos->SetBranchAddress("z",&stop_z);

      // One entry only :
      //std::cout << "debug : " 
      //          << " " << Track_direction->GetEntries()
      //          << " " << Track_momentum->GetEntries()
      //          << " " << Track_startPos->GetEntries()
      //          << " " << Track_stopPos->GetEntries()
      //          << std::endl;
      if(Track_direction->GetEntries()==1) Track_direction->GetEntry(0);
      if(Track_momentum->GetEntries()==1)   Track_momentum->GetEntry(0);
      if(Track_startPos->GetEntries()==1) Track_startPos->GetEntry(0);
      if(Track_stopPos->GetEntries()==1) Track_stopPos->GetEntry(0);

      if(dump)
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

      delete Track_direction;
      delete Track_momentum;
      delete Track_startPos;
      delete Track_stopPos;

    }//loop on Tracks

    //--------
    // The Hits
    //--------
    Event_hit->GetBranch("pe")->SetFile(file);

    for (Int_t khit=0; khit<nTubeHits; ++khit) {
      
      TTree* Hit_pe = new TTree();
      Event_hit->SetBranchAddress("pe",&Hit_pe);
  
      Event_hit->GetEntry(khit);

      Float_t hit_time;
      //Float_t trk_length; //NV 13/6/06
      Hit_pe->SetBranchAddress("time",&hit_time);
      //Hit_pe->SetBranchAddress("length",&trk_length);

      //JEC 16/1/06 add the tubeId_hit info
      if(dump)
      std::cout << "----> Hit{"<< khit <<"}: tube[" << tubeId_hit << "] total #PE " << totalPE << std::endl;

      for (Int_t ki=0; ki<Hit_pe->GetEntries(); ++ki) {
	Hit_pe->GetEntry(ki);

        if(dump) {
          std::cout << "<" << hit_time << ">";
	  //std::cout << "<" << trk_length << ">";
	}

        hits_times->Fill(hit_time);

        hit_count++;
      }
      if(dump)
      std::cout << std::endl;

      delete Hit_pe;
    }//Loop on Hits

    //--------
    // The Digits
    //--------
    for (Int_t l=0; l<nTubeDigits; ++l) {
      Event_digit->GetEntry(l);
      
      if(dump)
      std::cout << "----> Digit{"<<l<<"}: " 
		<< "tube[" << tubeId_digit << "] = " 
		<< " pe: " << digit_pe
		<< " time: " << digit_time
		<< std::endl;
      
      digits_time_pe->Fill(digit_time,digit_pe);

    }//Loop on Digits

    delete Event_vtxPos;
    delete Event_track;
    
    Event_hit->GetBranch("pe")->SetFile((TFile*)0);
    delete Event_hit;

    delete Event_digit;

  }//loop on event
*/
  if(dump)
  std::cout << " nEvents = " << nEvent << " hits = " << hit_count << std::endl;

  ////////////////////////////////////////////////////////
  // Plot histograms /////////////////////////////////////
  ////////////////////////////////////////////////////////
  plot(*hits_times,*digits_time_pe);

}//read_event
