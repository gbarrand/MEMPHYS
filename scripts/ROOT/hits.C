void hits() {

  bool dump = false;
  //dump = true;
  
  TFile* f = new TFile("MEMPHYS.root");
  TTree* tEvent = (TTree*)f->Get("Event");

  //declare Non subTuple part
  Int_t eventId,inputEvtId, interMode, vtxVol;
  tEvent->SetBranchAddress("eventId",&eventId);
  tEvent->SetBranchAddress("inputEvtId",&inputEvtId);

  Int_t nHits; 
  tEvent->SetBranchAddress("nHits",&nHits);


  //The Hit Event:subTuple
  TTree* Event_hit = new TTree();
  tEvent->SetBranchAddress("hit",&Event_hit);
  Int_t tubeId_hit; //JEC 16/1/06
  Int_t totalPE;
  
  //The Pe Hit::subTuple
  TTree* Hit_pe = new TTree();
  Float_t hit_time;


  Int_t nEvent = tEvent->GetEntries();

  
  //  TList* aListOfH = new TList();
  TNtuple* newTuple = new TNtuple("ntuple","MEMPHYS hit","i:tubeId:totalPE:time");

  for (Int_t i=0; i<nEvent; ++i){
    tEvent->GetEntry(i);

    Int_t nTubeHits = Event_hit->GetEntries();
    if(nHits!=nTubeHits) {
      std::cout << ">>>>>>>>>>>>> Event{" << i << "}: "
                << " number of hits mismatch. "
                <<" nHits: " << nHits << ", nTubeHits: " << nTubeHits
                << std::endl;      
    }
    
    if(dump)
    std::cout << ">>>>>>>>>>>>> Event{" << i << "}: "
	      << " evt Id " << eventId 
	      << " evt Input Id " << inputEvtId
	      <<" #hits: " << nHits
	      << std::endl;

    // Have a brand new overwritten hit TTree ; we have
    // to rebind its user variables :
    Event_hit->SetBranchAddress("tubeId",&tubeId_hit); //JEC 16/1/06   
    Event_hit->SetBranchAddress("totalPE",&totalPE);
    Event_hit->SetBranchAddress("pe",&Hit_pe);
    
    //--------
    // The Hits
    //--------
    for (Int_t k=0; k<nTubeHits; ++k) {
      
      Event_hit->GetEntry(k);

      if(Hit_pe->SetBranchAddress("time",&hit_time)==TTree::kMissingBranch) ::exit(0);	
	
      //JEC 16/1/06 add the tubeId_hit info
      //std::cout << "----> Hit{"<<k<<"}: tube[" << tubeId_hit << "] total #PE " << totalPE << std::endl;
      for (Int_t ki=0; ki<Hit_pe->GetEntries(); ++ki) {
	Hit_pe->GetEntry(ki);
	//std::cout << "<" << hit_time << ">";

	ntuple->Fill(i,tubeId_hit,totalPE,hit_time);
      }
      //      std::cout << std::endl;
    }//Loop on Hits

  }//loop on event


}//MEMPHYS_hit
