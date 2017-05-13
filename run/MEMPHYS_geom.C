void MEMPHYS_geom() {

  //see Analysis AIDA Tuple definition

  TFile* f = new TFile("MEMPHYS.root");
  TTree* tGeom = (TTree*)f->Get("Geometry");
  
  //declare Non sibTuple part
  Double_t wcRadius, wcLenght;
  tGeom->SetBranchAddress("wcRadius",&wcRadius);
  tGeom->SetBranchAddress("wcLenght",&wcLenght);
  Double_t pmtRadius;
  tGeom->SetBranchAddress("pmtRadius",&pmtRadius);
  Int_t nPMTs;
  tGeom->SetBranchAddress("nPMTs",&nPMTs);
  
  //The wcOffset subTuple
  TTree* tGeom_wcOffset = new TTree();
  tGeom->SetBranchAddress("wcOffset",&tGeom_wcOffset);
  Double_t xWC,yWC,zWC;

  //The pmtInfos subTuple
  TTree* tGeom_pmtInfos = new TTree();
  tGeom->SetBranchAddress("pmtInfos",&tGeom_pmtInfos);
  Int_t pmtId, pmtLocation;

  //The pmtOrient(ation) subsubTuple
  TTree* tGeom_pmtInfos_pmtOrient = new TTree();
  Double_t dx, dy, dz;

  //The pmtPosition subsubTuple
  TTree* tGeom_pmtInfos_pmtPosition = new TTree();
  Double_t xPMT, yPMT, zPMT;
  
  //--------------------------------
  //Start to extract the tuple values
  //--------------------------------
  Int_t nEntries = tGeom->GetEntries(); //should be 1
  if ( nEntries !=1 ) {
    std::cout << "Very suspect, #entries in Geom Tuple = " << nEntries 
	      << std::endl;
  }
  tGeom->GetEntry(0); //suppose that there is only 1 entry
  
  std::cout << "WC Radius " << wcRadius << " Length " <<wcLenght << std::endl;  

  
  nEntries = tGeom_wcOffset->GetEntries(); //should be 1 
  if ( nEntries !=1 ) {
    std::cout << "Very suspect, #entries in wcOffset Tuple = " << nEntries 
	      << std::endl;
  }
  
  // Have a brand new overwritten hit TTree ; we have
  // to rebind its user variables :
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
  }
  // Have a brand new overwritten hit TTree ; we have
  // to rebind its user variables :
  tGeom_pmtInfos->SetBranchAddress("pmtId",&pmtId);
  tGeom_pmtInfos->SetBranchAddress("pmtLocation",&pmtLocation);
  tGeom_pmtInfos->SetBranchAddress("pmtOrient",&tGeom_pmtInfos_pmtOrient);
  tGeom_pmtInfos->SetBranchAddress("pmtPosition",&tGeom_pmtInfos_pmtPosition);

  for (Int_t i=0; i<nPMTInfos; ++i) {
    tGeom_pmtInfos->GetEntry(i);
    
    tGeom_pmtInfos_pmtOrient->SetBranchAddress("dx",&dx);
    tGeom_pmtInfos_pmtOrient->SetBranchAddress("dy",&dy);
    tGeom_pmtInfos_pmtOrient->SetBranchAddress("dz",&dz);
    tGeom_pmtInfos_pmtOrient->GetEntry(0);


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
  }//Loop on PMTs
}
