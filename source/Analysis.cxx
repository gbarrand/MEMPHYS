#include "../MEMPHYS/Analysis.hh"

//AIDA
#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/ITupleFactory.h>

//Std
#include <iostream>
#include <vector>
#include <string>

MEMPHYS::Analysis::Analysis(
 AIDA::IAnalysisFactory* aAIDA
,bool aBatch)
:fBatch(aBatch)
,fAIDA(aAIDA)
,fTree(0)
#ifdef APP_USE_INLIB_WROOT
,m_file(std::cout,"MEMPHYS_inlib.root")
,m_geom_tree(0)
,m_leaf_wcRadius(0)
,m_leaf_wcLength(0)
  ,m_wcOffset_tree(0)
  ,m_wcOffset_leaf_x(0)
  ,m_wcOffset_leaf_y(0)
  ,m_wcOffset_leaf_z(0)
,m_leaf_wcOffset(0)
,m_leaf_pmtRadius(0)
,m_leaf_nPMTs(0)
  ,m_pmtInfos_tree(0)
  ,m_pmtInfos_leaf_pmtId(0)
  ,m_pmtInfos_leaf_pmtLocation(0)
    ,m_pmtOrient_tree(0)
    ,m_pmtOrient_leaf_dx(0)
    ,m_pmtOrient_leaf_dy(0)
    ,m_pmtOrient_leaf_dz(0)
  ,m_pmtInfos_leaf_Orient(0)
    ,m_pmtPosition_tree(0)
    ,m_pmtPosition_leaf_x(0)
    ,m_pmtPosition_leaf_y(0)
    ,m_pmtPosition_leaf_z(0)
  ,m_pmtInfos_leaf_Position(0)
,m_leaf_pmtInfos(0)
#endif
{
  if(!fAIDA) return;

  AIDA::ITreeFactory* treeFactory = fAIDA->createTreeFactory();
  if(fBatch) {
    fTree = treeFactory->create("MEMPHYS.root","root",false,true);
  } else {
    fTree = treeFactory->create(); // Memory tree :
  }
  delete treeFactory;
  if(!fTree) {
    std::cout << "Analysis : FATAL"
              << " tree not created." << std::endl;
    return;
  }
  
  //Booking Event Tuple
  AIDA::ITupleFactory* tf = fAIDA->createTupleFactory(*fTree);
  if(fBatch) { //GB 8/7/06 : Book tuple and histos only if batch.
  //JEC 16/11/05: add tubeId in the hit structure
  std::string column = "int eventId, inputEvtId, interMode, vtxVol, "; 
  column += "ITuple vtxPos = { double x, y, z }, ";
  column += "int nPart, leptonIndex, protonIndex, ";
  column += "ITuple track = { int pId, parent, float timeStart, ";
  column +=                  "ITuple direction = { double dx, dy, dz }, ";
  column +=                  "double mass, pTot, ETot, ";
  column +=                  "ITuple momentum  = { double px, py, pz }, ";
  column +=                  "ITuple startPos  = { double  x,  y,  z }, ";
  column +=                  "ITuple stopPos   = { double  x,  y,  z }, ";
  column +=                  "int startVol, stopVol ";
  column +=                "}, ";
  column += "int nHits, ITuple hit = { int tubeId, int totalPE, ITuple pe = { float time} }, ";
  column += "int nDigits, double sumPE, ";
  column += "ITuple digit = { int tubeId, double pe, time }";

  tf->create("Event","MEMPHYS WC Event",column,"");

  //Time tuple for Interactive session needed for the time beeing (G.Barrand does'nt yet manage the Tuple into Tuple in his Tuple Explorer (JEC 5/4/06)
  column = "float time";
  tf->create("HitTime","MEMPHYS WC hit time info",column,"");

  }

  //Booking Geometry Tuple (JEC 18/11/05 replace the RunEVent ROOT IO)
  //Needed for the visualization program.
  std::string column =  "double wcRadius, wcLenght, ITuple wcOffset = {double x, y, z}, ";
  column += "double pmtRadius, int nPMTs,  ";
  column += "ITuple pmtInfos = { ";
  column +=                      "int pmtId, pmtLocation, ";
  column +=                      "ITuple pmtOrient   = { double dx, dy, dz }, ";
  column +=                      "ITuple pmtPosition = { double  x,  y,  z } ";
  column +=                    "}";
  
  tf->create("Geometry","MEMPHYS WC Geometry",column,"");

  delete tf;

#ifdef APP_USE_INLIB_WROOT
  m_geom_tree = new inlib::wroot::tree(m_file.dir(),"Geometry","MEMPHYS WC Geometry");
  m_leaf_wcRadius = m_geom_tree->create_leaf<double>("wcRadius");
  m_leaf_wcLength = m_geom_tree->create_leaf<double>("wcLength");
  
    // Do not let the file directory manage a sub tree, else its going to be saved in the file.
    // Only the main geom tree should appear in the file.
    m_wcOffset_tree = new inlib::wroot::tree(m_file.dir(),"wcOffset","wcOffset",false); //false = not managed.
    m_wcOffset_leaf_x = m_wcOffset_tree->create_leaf<double>("x");
    m_wcOffset_leaf_y = m_wcOffset_tree->create_leaf<double>("y");
    m_wcOffset_leaf_z = m_wcOffset_tree->create_leaf<double>("z");  
  m_leaf_wcOffset = m_geom_tree->create_leaf("wcOffset",*m_wcOffset_tree);
  
  m_leaf_pmtRadius = m_geom_tree->create_leaf<double>("pmtRadius");
  m_leaf_nPMTs = m_geom_tree->create_leaf<int>("nPMTs");

    m_pmtInfos_tree = new inlib::wroot::tree(m_file.dir(),"pmtInfos","pmtInfos",false); //false = not managed.
    m_pmtInfos_leaf_pmtId = m_pmtInfos_tree->create_leaf<int>("pmtId");
    m_pmtInfos_leaf_pmtLocation = m_pmtInfos_tree->create_leaf<int>("pmtLocation");
    //column +=                      "ITuple pmtPosition = { double  x,  y,  z } ";

      m_pmtOrient_tree = new inlib::wroot::tree(m_file.dir(),"pmtOrient","pmtOrient",false); //false = not managed.
      m_pmtOrient_leaf_dx = m_pmtOrient_tree->create_leaf<double>("dx");
      m_pmtOrient_leaf_dy = m_pmtOrient_tree->create_leaf<double>("dy");
      m_pmtOrient_leaf_dz = m_pmtOrient_tree->create_leaf<double>("dz");
    m_pmtInfos_leaf_Orient = m_pmtInfos_tree->create_leaf("pmtOrient",*m_pmtOrient_tree);

      m_pmtPosition_tree = new inlib::wroot::tree(m_file.dir(),"pmtPosition","pmtPosition",false); //false = not managed.
      m_pmtPosition_leaf_x = m_pmtPosition_tree->create_leaf<double>("x");
      m_pmtPosition_leaf_y = m_pmtPosition_tree->create_leaf<double>("y");
      m_pmtPosition_leaf_z = m_pmtPosition_tree->create_leaf<double>("z");
    m_pmtInfos_leaf_Position = m_pmtInfos_tree->create_leaf("pmtPosition",*m_pmtPosition_tree);


  m_leaf_pmtInfos = m_geom_tree->create_leaf("pmtInfos",*m_pmtInfos_tree);

  
#endif
  
}//Ctor

//----------------------------------------------------------------------
MEMPHYS::Analysis::~Analysis(){
  //Not to be changed
  if(!fTree) return;
  if(fBatch) {
    fTree->commit();
    delete fTree;
    fTree = 0;
  }
#ifdef APP_USE_INLIB_WROOT
 {unsigned int n;
  if(!m_file.write(n)) {
    std::cout << "file write failed." << std::endl;
  }}  
  m_file.close(); // m_file dstor will delete m_geom_tree.
  delete m_wcOffset_tree;
  delete m_pmtInfos_tree;
  delete m_pmtOrient_tree;
  delete m_pmtPosition_tree;
#endif
}//Dtor

//----------------------------------------------------------------------

void MEMPHYS::Analysis::closeTree(){
  if(!fTree) return;
  if(!fBatch) return;
  fTree->commit();
  delete fTree;
  fTree = 0;
}
