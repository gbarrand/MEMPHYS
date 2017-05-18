//
//  To help debugging.
//
//  This script produces a MEMPHYS.root file
// with dummy data but with the same "TTree within TTree"
// structure than the file produced by the MEMPHYS_batch 
// program (see Analyis.cxx file).
//
//  Usage :
//    OS> <setup ROOT>
//    OS> root batch_dummy.C
//
//  G.Barrand
//

#include <cstdlib>

void batch_dummy() {

  int compress = 1;
  TFile f("MEMPHYS.root","recreate","JEC",compress);

  TTree* event = new TTree("Event","");

  int eventId,inputEvtId, interMode, vtxVol,nPart,leptonIndex,protonIndex;
  event->Branch("eventId",&eventId,"eventId/I");
  event->Branch("inputEvtId",&inputEvtId,"inputEvtId/I");
  event->Branch("interMode",&interMode,"interMode/I");
  event->Branch("vtxVol",&vtxVol,"vtxVol/I");
  event->Branch("nPart",&nPart,"nPart/I");
  event->Branch("leptonIndex",&leptonIndex,"leptonIndex/I");
  event->Branch("protonIndex",&protonIndex,"protonIndex/I");
  int nHits,nDigits; 
  event->Branch("nHits",&nHits,"nHits/I");
  event->Branch("nDigits",&nDigits,"nDigits/I");
  double sumPE;
  event->Branch("sumPE",&sumPE,"sumPE/D");

  TTree* vtxPos = new TTree("vtxPos","");
  event->Branch("vtxPos",&vtxPos);
  double v_x,v_y,v_z;
  vtxPos->Branch("x",&v_x,"x/D");
  vtxPos->Branch("y",&v_y,"y/D");
  vtxPos->Branch("z",&v_z,"z/D");

  TTree* tracks = new TTree("Tracks","");
  event->Branch("track",&tracks);
  int pId, parent;
  tracks->Branch("pId",&pId,"pId/I");
  tracks->Branch("parent",&parent,"parent/I");
  float timeStart;
  tracks->Branch("timeStart",&timeStart,"timeStart/F");
  double mass,pTot,ETot;
  tracks->Branch("mass",&mass,"mass/D");
  tracks->Branch("pTot",&pTot,"pTot/D");
  tracks->Branch("ETot",&ETot,"ETot/D");
  int startVol,stopVol;
  tracks->Branch("startVol",&startVol,"startVol/I");
  tracks->Branch("stopVol",&stopVol,"stopVol/I");

  TTree* dir = new TTree("Direction","");
  tracks->Branch("direction",&dir);
  double dx,dy,dz;
  dir->Branch("dx",&dx,"dx/D");
  dir->Branch("dy",&dy,"dy/D");
  dir->Branch("dz",&dz,"dz/D");

  TTree* mom = new TTree("Momentum","");
  tracks->Branch("momentum",&mom);
  double px,py,pz;
  mom->Branch("px",&px,"px/D");
  mom->Branch("py",&py,"py/D");
  mom->Branch("pz",&pz,"pz/D");

  TTree* startPos = new TTree("startPos","");
  tracks->Branch("startPos",&startPos);
  double start_x,start_y,start_z;
  startPos->Branch("x",&start_x,"x/D");
  startPos->Branch("y",&start_y,"y/D");
  startPos->Branch("z",&start_z,"z/D");

  TTree* stopPos = new TTree("stopPos","");
  tracks->Branch("stopPos",&stopPos);
  double stop_x,stop_y,stop_z;
  stopPos->Branch("x",&stop_x,"x/D");
  stopPos->Branch("y",&stop_y,"y/D");
  stopPos->Branch("z",&stop_z,"z/D");

  TTree* hits = new TTree("hits","");
  event->Branch("hit",&hits);
  int tubeId,totalPE;
  hits->Branch("tubeId",&tubeId,"tubeId/I");
  hits->Branch("totalPE",&totalPE,"totalPE/I");

  TTree* pe = new TTree("pe","");
  hits->Branch("pe",&pe);
  float pe_time;
  pe->Branch("time",&time,"time/F");
  //float trk_length;
  //pe->Branch("length",&trk_length,"length/F");

  TTree* digits = new TTree("digits","");
  event->Branch("digit",&digits);
  int digit_tubeId;
  digits->Branch("tubeId",&digit_tubeId,"tubeId/I");
  double digit_pe,time;
  digits->Branch("pe",&digit_pe,"pe/D");
  digits->Branch("time",&time,"time/D");

  //int nrows = 10000000;
  int nrows = 10000;
  //int nrows = 10;
  for (int index=0;index<nrows;index++) {
    eventId = index+1;

    interMode = 0;
    vtxVol = 0;
    leptonIndex = 0;
    protonIndex = 0;

    vtxPos->Reset();
    // once :
      v_x = 0;
      v_y = 0;
      v_z = 0;
      vtxPos->Fill();

    nPart = 1+(int) (10.0*rand()/(RAND_MAX+1.0)); //[1,10]

    tracks->Reset();
    for (int ipart=1; ipart<=nPart; ++ipart) {
      pId = -10+(int) (20.0*rand()/(RAND_MAX+1.0)); //[-10,10]
      parent = 1+(int) (ipart*rand()/(RAND_MAX+1.0)); //[1,ipart+1]
      timeStart = 0;
      mass = 0;
      pTot = 0;
      ETot = 10.;
      startVol = 0;
      stopVol = 0;

      dir->Reset();
      // once :
        dx = 1;
        dy = 0;
        dz = 0;
        dir->Fill();

      mom->Reset();
      // once :
        px = 1;
        py = 2;
        pz = 3;
        mom->Fill();

      startPos->Reset();
      // once :
        start_x = 1;
        start_y = 2;
        start_z = 3;
        startPos->Fill();

      stopPos->Reset();
      // once :
        stop_x = 1;
        stop_y = 2;
        stop_z = 3;
        stopPos->Fill();

      tracks->Fill();
    }

    //nHits = 0;
    nHits = (int) (10.0*rand()/(RAND_MAX+1.0));
    hits->Reset();
    for (int ihit=1; ihit<=nHits; ++ihit) {
      tubeId = 0;
      totalPE = 0;

      pe->Reset();
      // once :
        pe_time = 0;
        //trk_length = 3;
        pe->Fill();

      hits->Fill();
    }

    //nDigits = 0;
    nDigits = (int) (10.0*rand()/(RAND_MAX+1.0));
    digits->Reset();
    for (int idigit=1; idigit<=nDigits; ++idigit) {
      digit_tubeId = 0;
      digit_pe = 0;
      time = 0;
      digits->Fill();
    }

    sumPE = 314;

    event->Fill();
  }

  event->AutoSave();
  f.Close();

}
