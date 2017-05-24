void geom() {

  //see Analysis AIDA Tuple definition

  TFile* file = new TFile("MEMPHYS.root");
  TTree* tGeom = (TTree*)file->Get("Geometry");
  
  Double_t wcRadius, wcLenght;
  tGeom->SetBranchAddress("wcRadius",&wcRadius);
  tGeom->SetBranchAddress("wcLenght",&wcLenght);
  Double_t pmtRadius;
  tGeom->SetBranchAddress("pmtRadius",&pmtRadius);
  Int_t nPMTs;
  tGeom->SetBranchAddress("nPMTs",&nPMTs);
  
  TTree* tGeom_wcOffset = new TTree();
  tGeom->SetBranchAddress("wcOffset",&tGeom_wcOffset);
  Double_t xWC,yWC,zWC;

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
  
  std::cout << "WC Radius " << wcRadius << " Length " <<wcLenght << std::endl;  
  
  if ( tGeom_wcOffset->GetEntries() !=1 ) {
    std::cout << "Very suspect, #entries in wcOffset Tuple = " << tGeom_wcOffset->GetEntries()
	      << std::endl;
    ::exit(1);
  }
  
  tGeom_wcOffset->SetBranchAddress("x",&xWC);
  tGeom_wcOffset->SetBranchAddress("y",&yWC);
  tGeom_wcOffset->SetBranchAddress("z",&zWC);
  tGeom_wcOffset->GetEntry(0);

  std::cout << "WC center: (" 
	    << xWC << " , "
	    << yWC << " , "
	    << zWC << ")" << std::endl;

  std::cout << "#PMTs " << nPMTs 
	    << " of radius " << pmtRadius
	    << std::endl;

  Int_t nPMTInfos = tGeom_pmtInfos->GetEntries();
  if ( nPMTInfos != nPMTs ) {
    std::cout << "Very suspect, #entries in pmtInfos Tuple = " <<  nPMTInfos
	      << " <> #PMts = " << nPMTs
	      << std::endl;
    ::exit(1);
  }
  
  Int_t pmtId, pmtLocation;
  tGeom_pmtInfos->SetBranchAddress("pmtId",&pmtId);
  tGeom_pmtInfos->SetBranchAddress("pmtLocation",&pmtLocation);

  TBranch* br_pmtId = tGeom_pmtInfos->GetBranch("pmtId");
  br_pmtId->SetFile(file);
  TBranch* br_pmtLocation = tGeom_pmtInfos->GetBranch("pmtLocation");
  br_pmtLocation->SetFile(file);
  TBranch* br_pmtOrient = tGeom_pmtInfos->GetBranch("pmtOrient");
  br_pmtOrient->SetFile(file);
  TBranch* br_pmtPosition = tGeom_pmtInfos->GetBranch("pmtPosition");
  br_pmtPosition->SetFile(file);
  
  for (Int_t i=0; i<nPMTInfos; ++i) {

    std::cout << "PMT [" << pmtId <<"]: loc. " <<  pmtLocation << std::endl;
    
    TTree* tGeom_pmtInfos_pmtOrient = new TTree();
    tGeom_pmtInfos->SetBranchAddress("pmtOrient",&tGeom_pmtInfos_pmtOrient);

    TTree* tGeom_pmtInfos_pmtPosition = new TTree();
    tGeom_pmtInfos->SetBranchAddress("pmtPosition",&tGeom_pmtInfos_pmtPosition);
  
    int nbytes = tGeom_pmtInfos->GetEntry(i);
    if(nbytes<0) {
      std::cout << "problem with IO. " << std::endl;
      ::exit(1);
    }
    
    if ( tGeom_pmtInfos_pmtOrient->GetEntries() !=1 ) {
      std::cout << "Very suspect, #entries in tGeom_pmtInfos_pmtOrient Tuple = " << tGeom_pmtInfos_pmtOrient->GetEntries()
                << std::endl;
      ::exit(1);
      //continue;
    }
    if ( tGeom_pmtInfos_pmtPosition->GetEntries() !=1 ) {
      std::cout << "Very suspect, #entries in tGeom_pmtInfos_pmtPosition Tuple = " << tGeom_pmtInfos_pmtPosition->GetEntries()
                << std::endl;
      ::exit(1);
      //continue;
    }
  
    Double_t dx, dy, dz;
    tGeom_pmtInfos_pmtOrient->SetBranchAddress("dx",&dx);
    tGeom_pmtInfos_pmtOrient->SetBranchAddress("dy",&dy);
    tGeom_pmtInfos_pmtOrient->SetBranchAddress("dz",&dz);
    tGeom_pmtInfos_pmtOrient->GetEntry(0);

    Double_t xPMT, yPMT, zPMT;
    tGeom_pmtInfos_pmtPosition->SetBranchAddress("x",&xPMT);
    tGeom_pmtInfos_pmtPosition->SetBranchAddress("y",&yPMT);
    tGeom_pmtInfos_pmtPosition->SetBranchAddress("z",&zPMT);
    tGeom_pmtInfos_pmtPosition->GetEntry(0);

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
    
    delete tGeom_pmtInfos_pmtOrient;
    delete tGeom_pmtInfos_pmtPosition;
    
  }//Loop on PMTs

  br_pmtId->SetFile((TFile*)0);
  br_pmtLocation->SetFile((TFile*)0);
  br_pmtOrient->SetFile((TFile*)0);
  br_pmtPosition->SetFile((TFile*)0);
  
  delete tGeom_wcOffset;
  delete tGeom_pmtInfos;
  
}
