void geom() {

  //see Analysis AIDA Tuple definition

  bool dump = false;
  dump = true;
  
  TFile* file = new TFile("MEMPHYS.root");
  TTree* tGeom = (TTree*)file->Get("Geometry");
  
  Double_t wcRadius;
  tGeom->SetBranchAddress("wcRadius",&wcRadius);
  Double_t wcLength;
  tGeom->SetBranchAddress("wcLength",&wcLength);
  Double_t pmtRadius;
  tGeom->SetBranchAddress("pmtRadius",&pmtRadius);
  Int_t nPMTs;
  tGeom->SetBranchAddress("nPMTs",&nPMTs);
  std::vector<double>* wcOffset = 0;
  tGeom->SetBranchAddress("wcOffset",&wcOffset);

  TTree* tGeom_pmtInfos = new TTree();
  tGeom->SetBranchAddress("pmtInfos",&tGeom_pmtInfos);
  
  //--------------------------------
  //Start to extract the tuple values
  //--------------------------------
  if ( tGeom->GetEntries() !=1 ) {
    std::cout << "Very suspect, #entries in Geom Tuple = " << tGeom->GetEntries()
	      << std::endl;
    ::exit(1);
  }
  tGeom->GetEntry(0); //suppose that there is only 1 entry
  
  if(dump) std::cout << "WC Radius " << wcRadius << " Length " <<wcLength << std::endl;  
  
  Double_t xWC = (*wcOffset)[0];
  Double_t yWC = (*wcOffset)[1];
  Double_t zWC = (*wcOffset)[2];

  if(dump)
  std::cout << "WC center: (" 
	    << xWC << " , "
	    << yWC << " , "
	    << zWC << ")" << std::endl
            << "#PMTs " << nPMTs 
	    << " of radius " << pmtRadius
	    << std::endl;
  
  Int_t nPMTInfos = tGeom_pmtInfos->GetEntries();
  if ( nPMTInfos != nPMTs ) {
    std::cout << "Very suspect, #entries in pmtInfos Tuple = " <<  nPMTInfos
	      << " <> #PMts = " << nPMTs
	      << std::endl;
    ::exit(1);
  }
  
  Int_t pmtId;
  tGeom_pmtInfos->SetBranchAddress("pmtId",&pmtId);
  Int_t pmtLocation;
  tGeom_pmtInfos->SetBranchAddress("pmtLocation",&pmtLocation);
  std::vector<double>* pmtOrient = 0;
  tGeom_pmtInfos->SetBranchAddress("pmtOrient",&pmtOrient);
  std::vector<double>* pmtPosition = 0;
  tGeom_pmtInfos->SetBranchAddress("pmtPosition",&pmtPosition);

  tGeom_pmtInfos->GetBranch("pmtId")->SetFile(file);
  tGeom_pmtInfos->GetBranch("pmtLocation")->SetFile(file);
  
  for (Int_t i=0; i<nPMTInfos; ++i) {
    
    int nbytes = tGeom_pmtInfos->GetEntry(i);
    if(nbytes<0) {
      std::cout << "problem with IO. " << std::endl;
      ::exit(1);
    }
    
    Double_t dx = (*pmtOrient)[0];
    Double_t dy = (*pmtOrient)[1];
    Double_t dz = (*pmtOrient)[2];

    Double_t xPMT = (*pmtPosition)[0];
    Double_t yPMT = (*pmtPosition)[1];
    Double_t zPMT = (*pmtPosition)[2];

    if(dump)
    std::cout << "PMT [" << pmtId <<"]: loc. " <<  pmtLocation
              << " pos. (" 
	      << xPMT << " , "
	      << yPMT << " , "
	      << zPMT << ")"
	      << " dir. (" 
	      << dx << " , "
	      << dy << " , "
	      << dz << ")"
	      << std::endl;
    
  }//Loop on PMTs

  tGeom_pmtInfos->GetBranch("pmtId")->SetFile((TFile*)0);
  tGeom_pmtInfos->GetBranch("pmtLocation")->SetFile((TFile*)0);
  tGeom_pmtInfos->GetBranch("pmtOrient")->SetFile((TFile*)0);
  tGeom_pmtInfos->GetBranch("pmtPosition")->SetFile((TFile*)0);
  
  delete tGeom_pmtInfos;
  
}
