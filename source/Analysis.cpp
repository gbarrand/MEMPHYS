#include "../MEMPHYS/Analysis.hh"

#ifdef APP_USE_AIDA
//AIDA
#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/ITupleFactory.h>
#endif

#ifdef APP_USE_INLIB_WROOT
#include <exlib/zlib>
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
#include "Event_tree_zero.icc"
#include "Geometry_tree_zero.icc"
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
  m_file.add_ziper('Z',exlib::compress_buffer);
  m_file.set_compression(1); //max is 9.
#include "Event_tree_create.icc"
#include "Geometry_tree_create.icc"
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
  m_file.close(); // m_file dstor will delete m_Event_tree, m_Geometry_tree.

  delete m_Event_vtxPos_tree;
  delete m_Event_track_tree;
  delete m_Event_track_direction_tree;
  delete m_Event_track_momentum_tree;
  delete m_Event_track_startPos_tree;
  delete m_Event_track_stopPos_tree;
  delete m_Event_hit_tree;
  delete m_Event_hit_pe_tree;
  delete m_Event_digit_tree;
  
  delete m_Geometry_wcOffset_tree;
  delete m_Geometry_pmtInfos_tree;
  delete m_Geometry_pmtInfos_pmtOrient_tree;
  delete m_Geometry_pmtInfos_pmtPosition_tree;
  
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
