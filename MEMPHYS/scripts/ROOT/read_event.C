void read_event() {

  TFile* f = new TFile("MEMPHYS.root");
  TTree* tEvent = (TTree*)f->Get("Event");

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


  //The Track Event:subTuple  part
  TTree* Event_track = new TTree();
  tEvent->SetBranchAddress("track",&Event_track);
  Int_t pId,parent;
  Float_t timeStart;
  Double_t mass, pTot, ETot;
  Int_t startVol, stopVol;

  //The Direction Track:subTuple
  TTree* Track_direction = new TTree();
  Double_t dx,dy,dz;

  //The Momentum Track:subTuple
  TTree* Track_momentum = new TTree();
  Double_t px,py,pz;
  
  //The StartPos Track:subTuple
  TTree* Track_startPos = new TTree();
  Double_t start_x,start_y,start_z;

  //The StopPos Track:subTuple
  TTree* Track_stopPos = new TTree();
  Double_t stop_x,stop_y,stop_z;

  //The Hit Event:subTuple
  TTree* Event_hit = new TTree();
  tEvent->SetBranchAddress("hit",&Event_hit);
  Int_t tubeId_hit; //JEC 16/1/06
  Int_t totalPE;
  
  //The Pe Hit::subTuple
  TTree* Hit_pe = new TTree();
  Float_t hit_time;

  //The Digit Event:subTuple   
  TTree* Event_digit = new TTree();
  tEvent->SetBranchAddress("digit",&Event_digit);
  Int_t tubeId;
  Double_t digit_pe, digit_time;

  Int_t nEvent = tEvent->GetEntries();

  for (Int_t i=0; i<nEvent; ++i){
    tEvent->GetEntry(i);
    std::cout << ">>>>>>>>>>>>> Event{" << i << "}: "
	      << " evt Id " << eventId 
	      << " evt Input Id " << inputEvtId
	      << "\n interaction mode " << interMode
	      << " start in volume " << vtxVol << "\n"
	      <<" #tracks: " << nPart
	      <<" #hits: " << nHits
	      <<" #digits: " << nDigits
	      << std::endl;

    Int_t nTracks = Event_track->GetEntries();
    Int_t nTubeHits = Event_hit->GetEntries();
    Int_t nTubeDigits = Event_digit->GetEntries();
    std::cout << "Verif: nTracks = " << nTracks 
	      << " nTube Hits = " << nTubeHits
	      << " nTube Digits = " << nTubeDigits
	      << std::endl;

    // Have a brand new overwritten track TTree ; we have
    // to rebind its user variables :
    Event_track->SetBranchAddress("pId",&pId);
    Event_track->SetBranchAddress("parent",&parent);
    Event_track->SetBranchAddress("timeStart",&timeStart);

    Event_track->SetBranchAddress("mass",&mass);
    Event_track->SetBranchAddress("pTot",&pTot);
    Event_track->SetBranchAddress("ETot",&ETot);

    Event_track->SetBranchAddress("startVol",&startVol);
    Event_track->SetBranchAddress("stopVol",&stopVol);

    Event_track->SetBranchAddress("direction",&Track_direction);
    Event_track->SetBranchAddress("momentum",&Track_momentum);
    Event_track->SetBranchAddress("startPos",&Track_startPos);
    Event_track->SetBranchAddress("stopPos",&Track_stopPos);

    // Have a brand new overwritten hit TTree ; we have
    // to rebind its user variables :
    Event_hit->SetBranchAddress("tubeId",&tubeId_hit); //JEC 16/1/06   
    Event_hit->SetBranchAddress("totalPE",&totalPE);
    Event_hit->SetBranchAddress("pe",&Hit_pe);
    
    // Have a brand new overwritten digit TTree ; we have
    // to rebind its user variables :
    Event_digit->SetBranchAddress("tubeId",&tubeId);
    Event_digit->SetBranchAddress("pe",&digit_pe);
    Event_digit->SetBranchAddress("time",&digit_time);


    for (Int_t j=0; j<nTracks; ++j) {

      Event_track->GetEntry(j);

      Track_direction->SetBranchAddress("dx",&dx);
      Track_direction->SetBranchAddress("dy",&dy);
      Track_direction->SetBranchAddress("dz",&dz);
      Track_direction->GetEntry(0);

      Track_momentum->SetBranchAddress("px",&px);
      Track_momentum->SetBranchAddress("py",&py);
      Track_momentum->SetBranchAddress("pz",&pz);
      Track_momentum->GetEntry(0);

      Track_startPos->SetBranchAddress("x",&start_x);
      Track_startPos->SetBranchAddress("y",&start_y);
      Track_startPos->SetBranchAddress("z",&start_z);
      Track_startPos->GetEntry(0);

      Track_stopPos->SetBranchAddress("x",&stop_x);
      Track_stopPos->SetBranchAddress("y",&stop_y);
      Track_stopPos->SetBranchAddress("z",&stop_z);
      Track_stopPos->GetEntry(0);
   
      std::cout << "----> Tk{"<<j<<"}: " 
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

    for (Int_t k=0; k<nTubeHits; ++k) {
      
      Event_hit->GetEntry(k);

      Hit_pe->SetBranchAddress("time",&hit_time);
      //JEC 16/1/06 add the tubeId_hit info
      std::cout << "----> Hit{"<<k<<"}: tube[" << tubeId_hit << "] total #PE " << totalPE << std::endl;
      for (Int_t ki=0; ki<Hit_pe->GetEntries(); ++ki) {
	Hit_pe->GetEntry(ki);
	std::cout << "<" << hit_time << ">";
      }
      std::cout << std::endl;
    }//Loop on Hits

    //--------
    // The Digits
    //--------
    for (Int_t l=0; l<nTubeDigits; ++l) {
      Event_digit->GetEntry(l);
      
      std::cout << "----> Digit{"<<l<<"}: " 
		<< "tube[" << tubeId << "] = " 
		<< " pe: " << digit_pe
		<< " time: " << digit_time
		<< std::endl;
      
    }//Loop on Digits
  }//loop on event
}//read_event
