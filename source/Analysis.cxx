#include "../MEMPHYS/Analysis.hh"

#ifdef APP_USE_AIDA
//AIDA
#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/ITupleFactory.h>
#endif

//Std
#include <iostream>
#include <vector>
#include <string>

MEMPHYS::Analysis::Analysis(
#ifdef APP_USE_AIDA
 AIDA::IAnalysisFactory* aAIDA,
#endif 
 bool aBatch)
:fBatch(aBatch)
#ifdef APP_USE_AIDA
,fAIDA(aAIDA)
,fTree(0)
#endif
#ifdef APP_USE_INLIB_WROOT
,m_file(std::cout,"MEMPHYS_inlib.root")

#include "event_tree_zero.icc"
 
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
#ifdef APP_USE_AIDA
  if(fAIDA) {

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
  std::string column =  "double wcRadius, wcLength, ITuple wcOffset = {double x, y, z}, ";
  column += "double pmtRadius, int nPMTs,  ";
  column += "ITuple pmtInfos = { ";
  column +=                      "int pmtId, pmtLocation, ";
  column +=                      "ITuple pmtOrient   = { double dx, dy, dz }, ";
  column +=                      "ITuple pmtPosition = { double  x,  y,  z } ";
  column +=                    "}";
  
  tf->create("Geometry","MEMPHYS WC Geometry",column,"");

  delete tf;
  } //fAIDA
#endif //APP_USE_AIDA

#ifdef APP_USE_INLIB_WROOT
  
#include "event_tree_create.icc"

  
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
#ifdef APP_USE_AIDA
  if(fTree) {
  if(fBatch) {
    fTree->commit();
    delete fTree;
    fTree = 0;
  }}
#endif
  
#ifdef APP_USE_INLIB_WROOT
 {unsigned int n;
  if(!m_file.write(n)) {
    std::cout << "file write failed." << std::endl;
  }}  
  m_file.close(); // m_file dstor will delete m_geom_tree.

  delete m_event_vtxPos_tree;
  delete m_event_track_tree;
  delete m_event_track_direction_tree;
  delete m_event_track_momentum_tree;
  delete m_event_track_startPos_tree;
  delete m_event_track_stopPos_tree;
  delete m_event_hit_tree;
  delete m_event_hit_pe_tree;
  delete m_event_digit_tree;
  
  delete m_wcOffset_tree;
  delete m_pmtInfos_tree;
  delete m_pmtOrient_tree;
  delete m_pmtPosition_tree;
  
#endif
}//Dtor

//----------------------------------------------------------------------

void MEMPHYS::Analysis::closeTree(){
#ifdef APP_USE_AIDA
  if(fTree) {
  if(fBatch) {
    fTree->commit();
    delete fTree;
    fTree = 0;
  }}
#endif
}

void MEMPHYS::Analysis::fill_track(int pId,int parent,float timeStart,
		                   double dx,double dy,double dz,
                                   double mass,double pTot, double ETot,double px,double py,double pz,
                                   double startPos_x,double startPos_y,double startPos_z,
                                   double stopPos_x,double stopPos_y,double stopPos_z,
		                   int startVol,int stopVol) {
#ifdef APP_USE_INLIB_WROOT
  m_event_track_leaf_pId->fill(pId);
  m_event_track_leaf_parent->fill(parent);  
  m_event_track_leaf_timeStart->fill(timeStart);

    m_event_track_direction_leaf_dx->fill(dx);
    m_event_track_direction_leaf_dy->fill(dy);
    m_event_track_direction_leaf_dz->fill(dz);
   {inlib::uint32 nbytes;
    if(!m_event_track_direction_tree->fill(nbytes)) {
      std::cout << "m_event_track_direction_tree fill failed." << std::endl;
    }}
   
  m_event_track_leaf_mass->fill(mass);
  m_event_track_leaf_pTot->fill(pTot);
  m_event_track_leaf_ETot->fill(ETot);

    m_event_track_momentum_leaf_px->fill(px);
    m_event_track_momentum_leaf_py->fill(py);
    m_event_track_momentum_leaf_pz->fill(pz);
   {inlib::uint32 nbytes;
    if(!m_event_track_momentum_tree->fill(nbytes)) {
      std::cout << "m_event_track_momentum_tree fill failed." << std::endl;
    }}

    m_event_track_startPos_leaf_x->fill(startPos_x);
    m_event_track_startPos_leaf_y->fill(startPos_y);
    m_event_track_startPos_leaf_z->fill(startPos_z);
   {inlib::uint32 nbytes;
    if(!m_event_track_startPos_tree->fill(nbytes)) {
      std::cout << "m_event_track_startPos_tree fill failed." << std::endl;
    }}

    m_event_track_stopPos_leaf_x->fill(stopPos_x);
    m_event_track_stopPos_leaf_y->fill(stopPos_y);
    m_event_track_stopPos_leaf_z->fill(stopPos_z);
   {inlib::uint32 nbytes;
    if(!m_event_track_stopPos_tree->fill(nbytes)) {
      std::cout << "m_event_track_stopPos_tree fill failed." << std::endl;
    }}
   
  m_event_track_leaf_startVol->fill(startVol);
  m_event_track_leaf_stopVol->fill(stopVol);
   
  //add the Beam track to tuple
 {inlib::uint32 nbytes;
  if(!m_event_track_tree->fill(nbytes)) {
    std::cout << "m_event_track_tree fill failed." << std::endl;
  }}
#endif 
}
 
