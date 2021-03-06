void digits() {

  bool dump = false;
  
  TCanvas* c1 = new TCanvas("c1","MEMPHYS Digit",100,100,600,600);
  c1->SetGridx();
  c1->SetGridy();
 
  TH1F* htime = new TH1F("htile","Time distribution",100,900.,1100.);



  TFile* f = new TFile("MEMPHYS.root");
  TTree* tEvent = (TTree*)f->Get("Event");

  //declare Non subTuple part
  Int_t eventId;
  tEvent->SetBranchAddress("eventId",&eventId);
  Int_t inputEvtId;
  tEvent->SetBranchAddress("inputEvtId",&inputEvtId);

  Int_t nDigits;
  Double_t sumPE;
  tEvent->SetBranchAddress("nDigits",&nDigits);
  tEvent->SetBranchAddress("sumPE",&sumPE);

  Int_t nEvent = tEvent->GetEntries();

  for (Int_t i=0; i<nEvent; ++i){
    TTree* Event_digit = new TTree();
    tEvent->SetBranchAddress("digit",&Event_digit);

    tEvent->GetEntry(i);
//     std::cout << ">>>>>>>>>>>>> Event{" << i << "}: "
// 	      << " evt Id " << eventId 
// 	      <<" #digits: " << nDigits
// 	      << std::endl;

    Int_t nTubeDigits = Event_digit->GetEntries();
//     std::cout << " nTube Digits = " << nTubeDigits
// 	      << std::endl;

    
    // Have a brand new overwritten digit TTree ; we have
    // to rebind its user variables :
    Int_t tubeId;
    Event_digit->SetBranchAddress("tubeId",&tubeId);
    Double_t digit_pe;
    Event_digit->SetBranchAddress("pe",&digit_pe);
    Double_t digit_time;
    Event_digit->SetBranchAddress("time",&digit_time);


    //--------
    // The Digits
    //--------
    for (Int_t l=0; l<nTubeDigits; ++l) {
      Event_digit->GetEntry(l);

      if(dump)
      std::cout << "----> Digit{"<<l<<"}: " 
                << "tube[" << tubeId << "] = " 
                << " pe: " << digit_pe
                << " time: " << digit_time
                << std::endl;
      
      htime->Fill(digit_time);
      
    }//Loop on Digits
    
    delete Event_digit;
  }//loop on event

  htime->Draw();

}//read_event
