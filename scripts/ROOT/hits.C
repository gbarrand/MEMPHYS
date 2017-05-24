void hits() {

  bool dump = false;
  dump = true;
  
  TH1D* hits_times = new TH1D("hits_times","Hits times",100,0,3000);
  
  TFile* file = new TFile("MEMPHYS.root");
  TTree* tEvent = (TTree*)file->Get("Event");

  Int_t eventId;
  tEvent->SetBranchAddress("eventId",&eventId);
  Int_t inputEvtId;
  tEvent->SetBranchAddress("inputEvtId",&inputEvtId);
  Int_t nHits; 
  tEvent->SetBranchAddress("nHits",&nHits);

  //TNtuple* hits_ntuple = new TNtuple("ntuple","MEMPHYS hit","i:tubeId:totalPE:time");

  Int_t nEvent = tEvent->GetEntries();
  
  for (Int_t i=0; i<nEvent; ++i){
    
    TTree* Event_hit = new TTree();
    tEvent->SetBranchAddress("hit",&Event_hit);
    
    tEvent->GetEntry(i);

    Int_t nTubeHits = Event_hit->GetEntries();
    if(nHits!=nTubeHits) {
      std::cout << ">>>>>>>>>>>>> Event{" << i << "}: "
                << " number of hits mismatch. "
                <<" nHits: " << nHits << ", nTubeHits: " << nTubeHits
                << std::endl;      
    }
    
    // Have a brand new overwritten hit TTree ; we have
    // to rebind its user variables :
    Int_t tubeId_hit;
    Event_hit->SetBranchAddress("tubeId",&tubeId_hit); //JEC 16/1/06   
    Int_t totalPE;  
    Event_hit->SetBranchAddress("totalPE",&totalPE);
    
    if(dump)
    std::cout << ">>>>>>>>>>>>> Event{" << i << "}: "
	      << " evt Id " << eventId 
	      << " evt Input Id " << inputEvtId
	      <<" nHits: " << nHits
	      << std::endl;
    
    //--------
    // The Hits
    //--------
    TBranch* br_Hit_pe = Event_hit->GetBranch("pe");
    br_Hit_pe->SetFile(file);

    for (Int_t k=0; k<nTubeHits; ++k) {
      
      TTree* Hit_pe = new TTree();
      Event_hit->SetBranchAddress("pe",&Hit_pe);
      
      int nbytes = Event_hit->GetEntry(k);
      if(nbytes<0) {
        std::cout << "----> Hit{"<<k<<"}: tube[" << tubeId_hit << "] total #PE " << totalPE << " problem with IO. " << std::endl;
	::exit(1);
      }

      if(dump) std::cout << "----> Hit{"<<k<<"}: tube[" << tubeId_hit << "] total #PE " << totalPE << std::endl;
      
      Float_t hit_time;
      if(Hit_pe->SetBranchAddress("time",&hit_time)==TTree::kMissingBranch) ::exit(1);	
	
      for (Int_t ki=0; ki<Hit_pe->GetEntries(); ++ki) {
	Hit_pe->GetEntry(ki);
	//std::cout << "<" << hit_time << ">";

	//hits_ntuple->Fill(i,tubeId_hit,totalPE,hit_time);
        hits_times->Fill(hit_time);
      }
      //      std::cout << std::endl;
      delete Hit_pe;
    }//Loop on Hits
    
    br_Hit_pe->SetFile((TFile*)0);
    delete Event_hit;
  }//loop on event

  TCanvas* plotter = new TCanvas("canvas","",10,10,800,600);
  plotter->Divide(1,1);  
  hits_times->Draw();
  plotter->Update();

}//MEMPHYS_hit
