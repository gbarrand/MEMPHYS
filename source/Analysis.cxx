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
:fAIDA(aAIDA)
,fBatch(aBatch)
,fTree(0)
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
}//Dtor

//----------------------------------------------------------------------

void MEMPHYS::Analysis::closeTree(){
  if(!fTree) return;
  if(!fBatch) return;
  fTree->commit();
  delete fTree;
  fTree = 0;
}
