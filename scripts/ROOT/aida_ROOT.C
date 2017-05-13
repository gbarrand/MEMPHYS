//
// Usage :
//   OS> cd MEMPHYS/<version>/cmt
//   OS> <source setup>
//   OS> <have a MEMPHYS.root file in current directory>
//   OS> root
//   root[] gSystem->Load("libMEMPHYSAIDADict");
//   root[] .x ../scripts/ROOT/aida_ROOT.C
//

//
// rootcint version of the applications/MEMPHYS_analysis.C program.
// The MEMPHYS.root data are read by using OpenScientist/AIDA (and 
// then Rio) and the histos are done by using ROOT/TH*.
// 
// WARNING : CINT can't handle the dynamic_cast, then we use
//  the cast methods found on the AIDA interfaces basic classes.
//

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
AIDA::ITuple* cast_Tuple(
 AIDA::ITupleEntry* aEntry
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef __CINT__
  //FIXME : CINT can't handle dynamic_cast :
  //AIDA-3.2.1 : ITupleEntry does not have the cast method.
  //return (AIDA::ITuple*)aEntry->cast("AIDA::ITuple");
  return cast_ITuple(aEntry);
#else
  return dynamic_cast<AIDA::ITuple*>(aEntry);
#endif
}
//////////////////////////////////////////////////////////////////////////////
bool get_XYZ(
 AIDA::ITuple& aParent
,int aColumn
,double& aX
,double& aY
,double& aZ
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(aColumn);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
  if(!tuple) return false;

  tuple->start();
  if(!tuple->next()) return false;

  aX = tuple->getDouble(0);
  aY = tuple->getDouble(1);
  aZ = tuple->getDouble(2);

  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool dump_tracks(
 AIDA::ITuple& aParent
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(8);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
  if(!tuple) return false;

  //if(nPart!=tracks->rows()) {
  //  std::cout << "read: nPart / tracks mismatch " 
  //              << nPart << " " << tracks->rows() << std::endl;
  //  return false;
  //}

  tuple->start();
  int irow = 0;
  while(tuple->next()) {

    int pId = tuple->getInt(0);
    int parent = tuple->getInt(1);
    float timeStart = tuple->getFloat(2);

    double dx,dy,dz;
    if(!get_XYZ(*tuple,3,dx,dy,dz)) return false;

    double mass = tuple->getDouble(4);
    double pTot = tuple->getDouble(5);
    double ETot = tuple->getDouble(6);

    double px,py,pz;
    if(!get_XYZ(*tuple,7,px,py,pz)) return false;

    double start_x,start_y,start_z;
    if(!get_XYZ(*tuple,8,start_x,start_y,start_z)) return false;

    double stop_x,stop_y,stop_z;
    if(!get_XYZ(*tuple,9,stop_x,stop_y,stop_z)) return false;

    int startVol = tuple->getInt(10);
    int stopVol = tuple->getInt(11);

    std::cout << "----> Tk{"<<irow<<"}: " 
              << " pId " << pId
              << " parent " << parent
              << " creation time " << timeStart 
              << " Volumes " << startVol << " " << stopVol << "\n"
              << " Start Pos (" << start_x 
              << "," << start_y << "," << start_z << ")\n"
              << " Stop Pos (" << stop_x 
              << "," << stop_y << "," << stop_z << ")\n"
              << " dx,dy,dz " << dx << " " << dy << " " << dz << "\n"
              << " m " << mass
              << " ETot " << ETot
              << " pTot " << pTot
              << " px,py,pz " << px << " " << py << " " << pz << "\n"
              << std::endl;

    irow++;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool process_hits_times(
 AIDA::ITuple& aParent
,TH1D& aHisto
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(2);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
  if(!tuple) return false;

  tuple->start();
  while(tuple->next()) {

    float time = tuple->getFloat(0);

    aHisto.Fill(time);
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool process_hits(
 AIDA::ITuple& aParent
,TH1D& aHisto
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(10);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
  if(!tuple) return false;

  tuple->start();
  while(tuple->next()) {

    //int tubeId = tuple->getInt(0);
    //int totalPE = tuple->getInt(1);

    if(!process_hits_times(*tuple,aHisto)) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool process_digits(
 AIDA::ITuple& aParent
,TH2D& aHisto
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  AIDA::ITupleEntry* entry = (AIDA::ITupleEntry*)aParent.getObject(13);
  if(!entry) return false;

  AIDA::ITuple* tuple = cast_Tuple(entry);
  if(!tuple) return false;

  tuple->start();
  while(tuple->next()) {

    //int tubeId = tuple->getInt(0);
    double pe = tuple->getDouble(1);
    double time = tuple->getDouble(2);
    //printf("debug : ++++ : %g %g\n",time,pe);

    aHisto.Fill(time,pe);
  }
  return true;
}
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
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int aida_ROOT()
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string program = "aida_ROOT";
  std::string header = program + " : ";

  std::cout << header << "begin..." << std::endl;

  ////////////////////////////////////////////////////////
  // Create histograms ///////////////////////////////////
  ////////////////////////////////////////////////////////
  TH1D* hits_times = new TH1D("hits_times","Hits times",100,0,3000);

  TH2D* digits_time_pe = 
    new TH2D("digits_pe_time","Digits PE time",100,0,3000,100,0,10);

  ////////////////////////////////////////////////////////
  /// Read data an file histos ///////////////////////////
  ////////////////////////////////////////////////////////

  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << header << "AIDA not found." << std::endl;
    return 1;
  }

  AIDA::ITreeFactory* treeFactory = aida->createTreeFactory();
  if(!treeFactory) {
    std::cout << header << "can't get a TreeFactory." << std::endl;
    return 1;
  }

  AIDA::ITree* tree = treeFactory->create("MEMPHYS.root","root",true,false);
  if(!tree) {
    std::cout << header << "can't open data file." << std::endl;
    return 1;
  }

  AIDA::IManagedObject* object = tree->find("Event");
  if(!object) {
    std::cout << header 
              << "object Event not found in tree." 
              << std::endl;
    return 1;
  }
#ifdef __CINT__
  //FIXME : CINT can't handle dynamic_cast :
  AIDA::ITuple* tuple = (AIDA::ITuple*)object->cast("AIDA::ITuple");
#else
  AIDA::ITuple* tuple = dynamic_cast<AIDA::ITuple*>(object);
#endif
  if(!tuple) {
    std::cout << header << "object not an AIDA::ITuple." << std::endl;
    return 1;
  }

  int coln = tuple->columns();
  for(int index=0;index<coln;index++) {
    std::cout << header 
              << " icol = " << index
              << ", label = " << tuple->columnName(index) 
              << ", type = " << tuple->columnType(index) 
              << std::endl;
  }

  std::cout << header << "rows = " << tuple->rows() << std::endl;

  int nentries = tuple->rows();
  //nentries = 100000;
  //nentries = 40;
  std::cout << header 
            << "traitements de " << nentries << " entrees..." 
            << std::endl;

  int irow = 0;
  tuple->start();
  while(tuple->next() && (irow<nentries)) {

    /*
    int eventId = tuple->getInt(0);
    //int inputEvtId = tuple->getInt(1);
    //int interMode = tuple->getInt(2);
    //int vtxVol = tuple->getInt(3);

    int nPart = tuple->getInt(5);
    //int leptonIndex = tuple->getInt(6);
    //int protonIndex = tuple->getInt(7);

    int nHits = tuple->getInt(9);
    int nDigits = tuple->getInt(11);
    double sumPE = tuple->getDouble(12);

    std::cout << ">>>>>>>>>>>>> Event{" << irow << "}: "
    	      << " evt Id " << eventId 
    //	      << " evt Input Id " << inputEvtId
    //	      << "\n interaction mode " << interMode
    //	      << " start in volume " << vtxVol << "\n"
    	      <<" #tracks: " << nPart
    	      <<" #hits: " << nHits
              <<" #digits: " << nDigits
              <<" sumPE " << sumPE
    	      << std::endl;

    if(!dump_tracks(*tuple)) return 1;
    */

    if(!process_hits(*tuple,*hits_times)) return 1;
    if(!process_digits(*tuple,*digits_time_pe)) return 1;

    irow++;
  }

  delete treeFactory;
  delete aida;

  ////////////////////////////////////////////////////////
  // Plot histograms /////////////////////////////////////
  ////////////////////////////////////////////////////////
  std::cout << header << "plot..." << std::endl;
  plot(*hits_times,*digits_time_pe);

  return 0;
}
