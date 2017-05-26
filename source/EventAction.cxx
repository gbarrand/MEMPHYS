#include "../MEMPHYS/EventAction.hh"

//Geant4
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4UImanager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4TransportationManager.hh" 
#include "G4Navigator.hh" 
#include "G4SDManager.hh"
#include "G4DigiManager.hh"
#include "G4UnitsTable.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

//std
#include <set>
#include <iomanip>
#include <string>
#include <vector>

#ifdef APP_USE_AIDA
// AIDA :
#include <AIDA/ITree.h>
#include <AIDA/IManagedObject.h>
#include <AIDA/ITuple.h>
#endif

//MEMPHYS
#include "../MEMPHYS/Cast.hh"
#include "../MEMPHYS/Analysis.hh"
#include "../MEMPHYS/Trajectory.hh"
#include "../MEMPHYS/RunAction.hh"
#include "../MEMPHYS/PrimaryGeneratorAction.hh"
#include "../MEMPHYS/WCHit.hh"
#include "../MEMPHYS/WCDigi.hh"
#include "../MEMPHYS/WCDigitizer.hh"
#include "../MEMPHYS/DetectorConstruction.hh"


MEMPHYS::EventAction::EventAction(MEMPHYS::Analysis& aAnalysis,
				  MEMPHYS::RunAction* myRun, 
				  MEMPHYS::DetectorConstruction* myDetector, 
				  MEMPHYS::PrimaryGeneratorAction* myGenerator)
:fAnalysis(aAnalysis)
,runAction(myRun)
,generatorAction(myGenerator)
,detectorConstructor(myDetector)
#ifdef APP_USE_AIDA
,eventTuple(0)
,hitTimeTuple(0)
#endif
{
  
  G4DigiManager* DMman = G4DigiManager::GetDMpointer(); //JEC FIXME: be a data member
  WCDigitizer* WCDM = new WCDigitizer( "WCReadout");
  DMman->AddNewModule(WCDM);

#ifdef APP_USE_AIDA
  //JEC 10/11/05 introduce AIDA
  //Get User Histo pointers
  AIDA::ITree* usrTree = aAnalysis.tree(); //get the tree
  if (!usrTree) {
    G4cout << "MEMPHYS::EventAction: cannot get Analysis Tree" << G4endl;
    return;
  }
  //see AIDA::IManagedObject a comment on dynamic_cast
  AIDA::IManagedObject* obj; //generic Object managed by a Tree

  obj = usrTree->find("Event");
  if(!obj) {
    G4cout << "EventAction: WARNING: no tuple Event" << G4endl;
    usrTree->ls();
    //exit(0);
  } else {
    eventTuple =  CAST(obj,AIDA::ITuple);
    if (!eventTuple) {
      G4cout << "EventAction: FATAL: eventTuple not a Tuple" << G4endl;
      usrTree->ls();
      exit(0);
    }
  }

  //5/4/06 JEC access the Hit times Tuple
  obj = usrTree->find("HitTime");
  if(!obj) {
    G4cout << "EventAction: WARNING: no tuple HitTime" << G4endl;
    usrTree->ls();
    //exit(0);
  } else {
    hitTimeTuple =  CAST(obj,AIDA::ITuple);
    if (!hitTimeTuple) {
      G4cout << "EventAction: FATAL: hitTimeTuple not a Tuple" << G4endl;
      usrTree->ls();
      exit(0);
    }
  }
#endif //APP_USE_AIDA
  
}//Ctor

//-------------------------------------------------------------------------------------------

MEMPHYS::EventAction::~EventAction(){
}//Dtor

//-------------------------------------------------------------------------------------------

void MEMPHYS::EventAction::BeginOfEventAction(const G4Event* evt){

  G4cout << " (JEC) EventAction::Begin EventAction" << G4endl;
  if (!evt) evt->Print();

}//BeginOfEventAction

//-------------------------------------------------------------------------------------------

void MEMPHYS::EventAction::EndOfEventAction(const G4Event* evt) {
  G4cout << " (JEC) EventAction::End EventAction" << G4endl;
  
  // --------------------
  //  Get Particle Table
  // --------------------

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

  
  //JEC 10/11/05 see Analysis.cxx to get the Tuple variables

  // --------------------
  //  Get Event Information
  // --------------------
  //JEC FIXME: difference between event_id &  vecRecNumber
  G4int         event_id = evt->GetEventID();                      
  G4int         vecRecNumber = generatorAction->GetVecRecNumber(); 
  G4int         mode     = generatorAction->GetMode();             


  G4ThreeVector vtx = generatorAction->GetVtx();                   
  G4int         vtxvol   = EventFindStartingVolume(vtx);    //JEC FIXME change to string saving


  G4cout << ">>>>>>>>>>>>> New Event ";
  G4cout << " evt Id " << event_id 
	 << " evt Input Id " << vecRecNumber
	 << "\n interaction mode " << mode
	 << " start in volume " << vtxvol
	 << G4endl;
  G4cout << "Vertex (" 
	 << vtx.x()/cm << " , "
	 << vtx.y()/cm << " , "
	 << vtx.z()/cm << " , "
	 << ")"
	 << G4endl;

  //JEC FIXME: introduce enumeration for the column shared by Analysis/EventAction &  namespace protected

#ifdef APP_USE_AIDA
  if(!eventTuple) return;
  if(!hitTimeTuple) return;

  eventTuple->fill(0, event_id);                                    //eventId
  eventTuple->fill(1, vecRecNumber);                                //inputEvtId
  eventTuple->fill(2, mode);                                        //interMode
  eventTuple->fill(3, vtxvol);                                      //vtxVol

  AIDA::ITuple* vtxPos = eventTuple->getTuple( 4 );
  vtxPos->fill(0, vtx.x()/cm);                                       //vtxPos
  vtxPos->fill(1, vtx.y()/cm);
  vtxPos->fill(2, vtx.z()/cm);
  vtxPos->addRow();
  

  // --------------------
  //  Get Track Information
  // --------------------

  //variables used to fill the tuple (track part)
  //JEC FIXME how to set flag? G4int flag; 
  G4int pId, parent; 
  G4float timeStart;
  G4double dx, dy, dz;
  G4double mass, pTot, ETot, px, py, pz;
  G4int startVol, stopVol;

  // mustop, pstop, npar will be filled later

  // Next in the ntuple is an array of tracks.
  // We will keep count with npar

  // First two tracks are special: beam and target

  //----------------
  // The beam
  //----------------
  AIDA::ITuple* track = eventTuple->getTuple(8);

  G4int  beampdg = generatorAction->GetBeamPDG();
  pId =  beampdg;                                                  //pId
  track->fill(0, pId);

  parent = 0;                                                      //parent (none)
  track->fill(1, parent);  

  timeStart = 0;                                                   //creation time
  track->fill(2, timeStart);
  
  G4ThreeVector beamdir    = generatorAction->GetBeamDir();        //direction
  AIDA::ITuple* direction = track->getTuple( 3 );
  dx = beamdir.x();
  dy = beamdir.y();
  dz = beamdir.z();

  direction->fill(0, dx);
  direction->fill(1, dy);
  direction->fill(2, dz);
  direction->addRow();

  mass = 0;                                                       //mass
  track->fill(4, mass);
  
  G4double      beamenergy = generatorAction->GetBeamEnergy();
  pTot =  beamenergy;                                            //ptot
  track->fill(5, pTot);

  ETot = pTot;                                                   //ETot (= pTot for neutrino)
  track->fill(6, ETot);
  
  AIDA::ITuple* momentum = track->getTuple( 7 );                           //momentum
  px = pTot * dx;
  py = pTot * dy;
  pz = pTot * dz;
  momentum->fill(0, px);
  momentum->fill(1, py);
  momentum->fill(2, pz);
  momentum->addRow();

  AIDA::ITuple* startPos = track->getTuple( 8 );                          //start position
  startPos->fill(0, vtx.x()/cm);
  startPos->fill(1, vtx.y()/cm);
  startPos->fill(2, vtx.z()/cm);
  startPos->addRow(); 

  AIDA::ITuple* stopPos = track->getTuple( 9 );                          //stop position
  stopPos->fill(0, vtx.x()/cm); 
  stopPos->fill(1, vtx.y()/cm);
  stopPos->fill(2, vtx.z()/cm);
  stopPos->addRow(); 

  startVol = -1;                                               //startVol
  track->fill(10, startVol);

  stopVol  = -1;                                               //stopVol
  track->fill(11, stopVol);
  
  //add the Beam track to tuple
  track->addRow();

  G4cout << "----> Tk{Beam}: " 
	 << " pId " << pId
	 << " parent " << parent
	 << " creation time " << timeStart 
	 << " Volumes " << startVol << " " << stopVol << "\n"
	 << " dx,dy,dz " << dx << " " << dy << " " << dz << "\n"
	 << " m " << mass
	 << " ETot " << ETot
	 << " pTot " << pTot
	 << " px,py,pz " << px << " " << py << " " << pz << "\n"
	 << G4endl;
  
  //----------------
  // The target
  //----------------
  G4int  targetpdg    = generatorAction->GetTargetPDG();
  pId =  targetpdg;                                                  //pId
  track->fill(0, pId);

  parent = 0;                                                      //parent (none)
  track->fill(1, parent);  

  timeStart = 0;                                                   //creation time
  track->fill(2, timeStart);
  
  G4ThreeVector targetdir    = generatorAction->GetTargetDir();
  direction = track->getTuple( 3 );
  dx = targetdir.x();
  dy = targetdir.y();
  dz = targetdir.z();
  
  direction->fill(0, dx);
  direction->fill(1, dy);
  direction->fill(2, dz);
  direction->addRow();
  
  G4double      targetenergy = generatorAction->GetTargetEnergy();
  G4double      targetpmag = 0.0, targetmass = 0.0;
  
  //JEC FIXME: keep original code for the moment
  if (targetpdg!=0) {            // protects against seg-fault
    if ( 8016 == targetpdg ) {   // JEC 25/11/05 Oxygen 16 in Nuance PID = Z*1000+A = 8016
      targetmass = targetenergy; // supposed at Rest
    } else {
      targetmass = particleTable->FindParticle(targetpdg)->GetPDGMass();
    }
    if (targetenergy > targetmass) {
      targetpmag = sqrt(targetenergy*targetenergy - targetmass*targetmass);
    } else { // protect against NaN
      targetpmag = 0.0;
    }
  }

  mass = targetmass;                                             //mass
  track->fill(4, mass);
  
  pTot =  targetpmag;                                            //ptot
  track->fill(5, pTot);

  ETot = targetenergy;                                           //ETot
  track->fill(6, ETot);
  
  momentum = track->getTuple( 7 );                           //momentum
  px = pTot * dx;
  py = pTot * dy;
  pz = pTot * dz;
  momentum->fill(0, px);
  momentum->fill(1, py);
  momentum->fill(2, pz);
  momentum->addRow();

  startPos = track->getTuple( 8 );                          //start position
  startPos->fill(0, vtx.x()/cm);
  startPos->fill(1, vtx.y()/cm);
  startPos->fill(2, vtx.z()/cm);
  startPos->addRow(); 

  stopPos = track->getTuple( 9 );                          //stop position
  stopPos->fill(0, vtx.x()/cm);
  stopPos->fill(1, vtx.y()/cm);
  stopPos->fill(2, vtx.z()/cm);
  stopPos->addRow(); 

  startVol = -1;                                               //startVol
  track->fill(10, startVol);

  stopVol  = -1;                                               //stopVol
  track->fill(11, stopVol);
  
  //add the Target track to tuple
  track->addRow();


  G4cout << "----> Tk{Target}: " 
	 << " pId " << pId
	 << " parent " << parent
	 << " creation time " << timeStart 
	 << " Volumes " << startVol << " " << stopVol << "\n"
	 << " dx,dy,dz " << dx << " " << dy << " " << dz << "\n"
	 << " m " << mass
	 << " ETot " << ETot
	 << " pTot " << pTot
	 << " px,py,pz " << px << " " << py << " " << pz << "\n"
	 << G4endl;


  // --------------------------
  //  Loop over Trajectories
  // --------------------------

  G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();

  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

  std::set<int> pizeroList;
  std::set<int> muonList;
  std::set<int> antimuonList;
  std::set<int> pionList;
  std::set<int> antipionList;

  G4int trjIndex;    //JEC 14/11/05 this is the trajectory Index
  G4int parentIndex; //JEC 14/11/05 this the parent trajectory index

  //JEC 14/11/05 factorize declarations...
  G4ThreeVector mom; //px,py,pz
  G4double      mommag; //p = sqrt(px*px + ...)
  G4ThreeVector stop; //stopping point
  G4ThreeVector start;//starting point
  G4String stopVolumeName; //Volume name of the stopping point for retreive
  G4int ntrack = 2; //count tracks that are stored in Tuple (start at 2 for the Beam + Target)

  //JEC 21/11/05
  G4int leadingLeptonIndex = -1;    //Index of the most energetic lepton issued from the neutrino interaction
  G4double tmpEnergyleadingLepton = -1.0; //temproary variable to trigger the update of leadingLeptonIndex
  G4int outgoingProtonIndex= -1;    //Index of the most energetic proton issued from the neutrino interaction
  G4double tmpEnergyoutgoingProton = -1.0; //temproary variable to trigger the update of outgoingProtonIndex
  

  for (G4int i=0; i < n_trajectories; i++) {
    Trajectory* trj = (Trajectory*)((*(evt->GetTrajectoryContainer()))[i]);
    
    // Draw Charged Tracks
    //JEC FIWME: display if (trj->GetCharge() != 0.) trj->DrawTrajectory(50);
    
    // If this track has a special id remember it for later
    
    pId      =  trj->GetPDGEncoding();                                   //pId
    trjIndex =  trj->GetTrackID();
    switch ( pId ) {
    case 111: //pi0
      pizeroList.insert( trjIndex );
      break;
    case 13: //muon
      muonList.insert( trjIndex );
      break;
    case -13: //anti-mu
      antimuonList.insert( trjIndex );
      break;
    case 211: //pion
      pionList.insert( trjIndex );
      break;
    case -211: //anti-pion
      antipionList.insert( trjIndex );
      break;
    }//eosw

      
    // Process primary tracks or the secondaries from pizero or muons, pions...
    if ( ! trj->GetSaveFlag() ) continue;

    // initial point of the trajectory
    G4TrajectoryPoint* aa =   (G4TrajectoryPoint*)trj->GetPoint(0) ;   
    runAction->incrementEventsGenerated(); //JEC FIXME: what is the need of that?
    
    track->fill(0, pId);                                         //pId
    
    //JEC FIXME should be META info G4int         flag   = 0;    // will be set later
    
    //JEC 14/11/06 code adapeted from M.F
    parentIndex = trj->GetParentID();
    if (parentIndex == 0){
      parent = 0;
    } else if ( pizeroList.count(parentIndex) ) {
      parent = 111;
    } else if ( muonList.count(parentIndex) ) {
      parent = 13;
    } else if ( antimuonList.count(parentIndex) ) {
      parent = -13;
    } else if ( antipionList.count(parentIndex) ) {
      parent = -211;
    } else if ( pionList.count(parentIndex) ) {
      parent = 211;
    } else {  // no identified parent, but not a primary
      parent = 999;
    }//eoif
    track->fill(1,parent);                                       //parent
    
    timeStart =  trj->GetGlobalTime()/ns;
    track->fill(2, timeStart);                                   //creation time
    
    mom    = trj->GetInitialMomentum();
    mommag = mom.mag();
    direction = track->getTuple( 3 );                            //direction
    dx = mom.x()/mommag;
    dy = mom.y()/mommag;
    dz = mom.z()/mommag;
    direction->fill(0, dx);
    direction->fill(1, dy);
    direction->fill(2, dz);
    direction->addRow();
    
    mass   = trj->GetParticleDefinition()->GetPDGMass();
    track->fill(4, mass);                                        //mass
    
    pTot = mommag;
    track->fill(5, pTot);                                        //p Total
    
    ETot = sqrt(mom.mag2() + mass*mass);
    track->fill(6, ETot);                                        //Total energy


    //Save index of the most energetic primary lepton and proton
    if ( 0 == parent ) {
      if ( ( (13 == pId) || (-13 == pId) ) && 
	   ( ETot > tmpEnergyleadingLepton ) ) {
	//a mu+ or mu- from the neutrino interaction and with a greater energy than previous case, update
	tmpEnergyleadingLepton = ETot;
	//flag the particle Index
	leadingLeptonIndex = i; //JEC FIXME it may be 'ntrack' directly
      } else if ( ( (11 == pId) || (-11 == pId) ) && 
		  ( ETot > tmpEnergyoutgoingProton ) ) {
	tmpEnergyoutgoingProton = ETot;
	outgoingProtonIndex = i; //JEC FIXME it may be 'ntrack' directly
      }//eo update special particle index saving
    }//eo a primary particle
    
    momentum = track->getTuple( 7 );                             //3-momentum
    px = mom.x();
    py = mom.y();
    pz = mom.z();
    momentum->fill(0, px);
    momentum->fill(1, py);
    momentum->fill(2, pz);
    momentum->addRow();
    
    start  = aa->GetPosition();
    startPos = track->getTuple( 8 );                             //start position
    startPos->fill(0, start.x()/cm);
    startPos->fill(1, start.y()/cm);
    startPos->fill(2, start.z()/cm);
    startPos->addRow(); 
    
    stop   = trj->GetStoppingPoint();
    stopPos = track->getTuple( 9 );                              //stop position
    stopPos->fill(0, stop.x()/cm);
    stopPos->fill(1, stop.y()/cm);
    stopPos->fill(2, stop.z()/cm);
    stopPos->addRow(); 
    
    startVol = EventFindStartingVolume(start);
    track->fill(10, startVol);                                   //Starting Volume
    
    
    stopVolumeName = trj->GetStoppingVolume()->GetName();
    stopVol  = EventFindStoppingVolume(stopVolumeName);
    track->fill(11, stopVol);                                    //Stopping Volume
    
    //add the new track to tuple
    track->addRow(); 

    G4cout << "----> Tk{"<<ntrack<<"}: " 
	   << " pId " << pId
	   << " parent " << parent
	   << " creation time " << timeStart 
	   << " Volumes " << startVol << " " << stopVol << "\n"
	   << " Start Pos (" << start.x()/cm << "," << start.y() << "," << start.z() << ")\n"
	   << " Stop Pos (" << stop.x()/cm << "," << stop.y() << "," << stop.z() << ")\n"
	   << " dx,dy,dz " << dx << " " << dy << " " << dz << "\n"
	   << " m " << mass
	   << " ETot " << ETot
	   << " pTot " << pTot
	   << " px,py,pz " << px << " " << py << " " << pz << "\n"
	   << G4endl;
       
    ntrack++;

  }//end of loop on trajectories
  
  eventTuple->fill(5, ntrack);                                           //nPart
  G4cout << "Final # of tracks : " << ntrack << G4endl;

  if (leadingLeptonIndex != -1 ) {
    //most energetic primary lepton found
    leadingLeptonIndex += 2; //add the "beam" + "target" index, JEC FIXME see comment above
  }
  eventTuple->fill(6,leadingLeptonIndex);                               //leptonIndex

  if (outgoingProtonIndex !=-1 ) {
    outgoingProtonIndex += 2; //add the "beam" + "target" index, JEC FIXME see comment above
  }
  eventTuple->fill(7, outgoingProtonIndex);                            //protonIndex

  
  // --------------------
  //  Get WC Hit Collection
  // --------------------
    
  G4SDManager* SDman = G4SDManager::GetSDMpointer(); //JEC FIXME: use data member
    
  // Get Hit collection of this event
  G4HCofThisEvent* HCE         = evt->GetHCofThisEvent();
  WCHitsCollection* WCHC = 0;

  AIDA::ITuple* hit = eventTuple->getTuple(10);                         //hit
  G4int nHits=0;
  G4int tubeID_hit; //JEC 16/1/06
  G4int totalPE;
  
  G4float peArrivalTime;

  if (HCE) { 
    G4String name = "WCPMT";
    G4int collectionID = SDman->GetCollectionID(name);
    WCHC = (WCHitsCollection*)HCE->GetHC(collectionID);
  }

  
  //JEC 14/6/06 START the Digitization should be done before manipulation of hit (ie. sorting the time!)
  // Get a pointer to the Digitizing Module Manager
  G4DigiManager* DMman = G4DigiManager::GetDMpointer(); //JEC FIXME: use data member
  
  // Get a pointer to the WC Digitizer module
  WCDigitizer* WCDM =
    (WCDigitizer*)DMman->FindDigitizerModule("WCReadout");
  if (!WCDM) {
    G4cout << "(JEC:EndOfEventAction): FATAL no WC Digitizer found" << G4endl;
    exit(0);
  }
  //JEC 14/6/06 END

  if (WCHC) {

    //JEC 14/6/06 START   
    // Figure out what size PMTs we are using in the WC detector.
    G4float PMTSize = detectorConstructor->GetPMTSize();
    WCDM->SetPMTSize(PMTSize);
    // Digitize the hits
    WCDM->Digitize();
    //JEC 14/6/06 END



    //nHits = std::min(500,WCHC->entries());                              //JEC: limit the number of hits

    nHits = WCHC->entries();
 
    //JEC FIWME save the time information also later
    for (G4int i=0; i<nHits  ;i++) {
      
      (*WCHC)[i]->UpdateColour();
      
      tubeID_hit = (*WCHC)[i]->GetTubeID(); //JEC 16/1/06
      hit->fill(0,tubeID_hit);              //   "

      totalPE = (*WCHC)[i]->GetTotalPe();
      hit->fill(1, totalPE);                                            //totalPE (JEC 16/1/06 tupleid=1)
      AIDA::ITuple* pe = hit->getTuple(2);                              //(JEC 16/1/06 tupleid=2)
      for (G4int j=0; j<std::min(100,totalPE) ; j++) {                  //JEC: limit the number of "impacts"
	peArrivalTime = (*WCHC)[i]->GetTime(j); 
	pe->fill(0,peArrivalTime);

	//JEC 5/4/06 fill the Hit time tuple
	hitTimeTuple->fill(0,peArrivalTime);
	hitTimeTuple->addRow();

	pe->addRow();
      }
      hit->addRow();
    }
  }//Hit container

  eventTuple->fill(9, nHits);                                          //nHits

  // --------------------
  //  Get Digitized Hit Collection
  // --------------------

  //JEC 14/6/06 this part should be done before storing the Hits as the Digitization sort the times for instance
//   // Get a pointer to the Digitizing Module Manager
//   G4DigiManager* DMman = G4DigiManager::GetDMpointer(); //JEC FIXME: use data member

//   // Get a pointer to the WC Digitizer module
//   WCDigitizer* WCDM =
//     (WCDigitizer*)DMman->FindDigitizerModule("WCReadout");
//   if (!WCDM) {
//     G4cout << "(JEC:EndOfEventAction): FATAL no WC Digitizer found" << G4endl;
//     exit(0);
//   }

//   // Figure out what size PMTs we are using in the WC detector.
//   G4float PMTSize = detectorConstructor->GetPMTSize();
//   WCDM->SetPMTSize(PMTSize);

//   // Digitize the hits
//   WCDM->Digitize();
  
  // Get the digitized collection for the WC
  G4int WCDCID = DMman->GetDigiCollectionID("WCDigitizedCollection");
  WCDigitsCollection * WCDC = 
    (WCDigitsCollection*)DMman->GetDigiCollection(WCDCID);
    

  AIDA::ITuple* digit = eventTuple->getTuple(13);
  G4int nDigits=0;
  G4double sumPE=0;
  G4int   tubeID;
  G4double tubePhotoElectrons;
  G4double tubeTime;
  if(WCDC) {
    nDigits = WCDC->entries();
    for (G4int i=0; i < nDigits; i++) {
      tubeID             = (*WCDC)[i]->GetTubeID();
      digit->fill(0, tubeID);                                         //tubeId

      tubePhotoElectrons = (*WCDC)[i]->GetPe();
      sumPE += tubePhotoElectrons;
      digit->fill(1, tubePhotoElectrons);                             //pe

      tubeTime           = (*WCDC)[i]->GetTime();
      digit->fill(2, tubeTime);                                       //time
      digit->addRow();
      //	(*WCDC)[i]->Print();
    }//loop on digits
  } else {
    G4cout << "(JEC) EventAction: No Digits for Event: "  << event_id << G4endl;
  }//digits collection
  
  eventTuple->fill(11, nDigits);                                        //nDigits
  eventTuple->fill(12, sumPE);                                          //sumPE
  
  //Save the Event
  eventTuple->addRow();
#endif //APP_USE_AIDA

#ifdef APP_USE_INLIB_WROOT
  ///////////////////////////////////////////////////////
  /// fill event trees : ////////////////////////////////
  ///////////////////////////////////////////////////////
#endif
    
}//EndOfEventAction

//--------------------------------------------------------------------------------------------------

G4int MEMPHYS::EventAction::EventFindStartingVolume(G4ThreeVector vtx) {
  // Get volume of starting point (see GEANT4 FAQ)

  G4int vtxvol = -1;

  G4Navigator* tmpNavigator = 
    G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();

  G4VPhysicalVolume* tmpVolume = tmpNavigator->LocateGlobalPointAndSetup(vtx);
  G4String       vtxVolumeName = tmpVolume->GetName();


  //JEC 15/11/05 FIXME: code to be clean and keep only WC detector elements
  G4cout << "(EventFindStartingVolume): vtxVolumeName: <"
	 << vtxVolumeName
	 << ">" << G4endl;
  
  if (vtxVolumeName.contains("MRD") ){
    vtxvol = 30;
  } else if ( vtxVolumeName.contains("FGD") ){
    vtxvol = 20;
  } else if ( vtxVolumeName.contains("lAr") ){
    vtxvol = 20;
  } else if ( vtxVolumeName == "outerTube" ||
	    vtxVolumeName == "innerTube" ||
	    vtxVolumeName == "rearEndCap"|| 
	    vtxVolumeName == "frontEndCap" ){
      vtxvol = 10;
  } else if ( vtxVolumeName.contains("WC") ){
    if (vtxVolumeName == "WCBarrel")
      vtxvol = 10;
    else if (vtxVolumeName == "WCBox")
      vtxvol = -2;
    else 
      {
	if (vtxVolumeName.contains("PMT") ||
	    vtxVolumeName.contains("Cap") ||
	    vtxVolumeName.contains("Cell"))
	  vtxvol = 11;
	else if (vtxVolumeName.contains("OD"))
	  vtxvol = 12;
	else
	  {
	    G4cout << vtxVolumeName << " unkown vtxVolumeName " << G4endl;
	    vtxvol = -3;
	  }
      }
  } else if ( vtxVolumeName == "expHall" ){
    vtxvol = 0;
  } else if ( vtxVolumeName == "catcher" ){
    vtxvol = 40;
  } else if (vtxVolumeName.contains("Rock") ){
    vtxvol = 50;
  }
  
  return vtxvol;
}//EventFindStartingVolume

//--------------------------------------------------------------------------------------------------

G4int MEMPHYS::EventAction::EventFindStoppingVolume(G4String stopVolumeName) {
  G4int stopvol = -1;

  //JEC 15/11/05 FIXME: code to be clean and keep only WC detector elements
  if ( stopVolumeName.contains("MRD") )
    {
      //JEC 18/11/05 not used      runAction->incrementMRDHits();
      stopvol = 30;
    }	
  else if ( stopVolumeName.contains("FGD") )
    {
      //JEC 18/11/05 not used runAction->incrementFGDHits();
      stopvol = 20;  }	
  else if (stopVolumeName.contains("lAr"))
    {
      //JEC 18/11/05 not used runAction->incrementlArDHits();
      stopvol = 20;
    }
  else if ( stopVolumeName == "outerTube" ||
	    stopVolumeName == "innerTube" ||
	    stopVolumeName == "rearEndCap"|| 
	    stopVolumeName == "frontEndCap" )
    {
      runAction->incrementWaterTubeHits();
      stopvol = 10;
    }	
  else if ( stopVolumeName.contains("WC") )
    {
      if (stopVolumeName == "WCBarrel")
	stopvol = 10;
      else if (stopVolumeName == "WCBox")
	stopvol = 0; // same as expHall
      else 
	{
	  if (stopVolumeName.contains("PMT") ||
	      stopVolumeName.contains("Cap") ||
	      stopVolumeName.contains("Cell"))
	    stopvol = 11;
	  else if (stopVolumeName.contains("OD"))
	    stopvol = 12;
	  else
	    {
	      G4cout << stopVolumeName 
		     << " unknown stopVolumeName " << G4endl;
	      stopvol = -3;
	    }
	}
      runAction->incrementWaterTubeHits();
    }	
  else if ( stopVolumeName == "expHall" )
    {
      stopvol = 0;
    }
  else if ( stopVolumeName == "catcher" )
    {
      runAction->incrementCatcherHits();
      stopvol = 40;
    }
  else if (stopVolumeName.contains("Rock") ){
    stopvol = 50;
    runAction->incrementRockHits();
  }
  
  return stopvol;
}//EventFindStoppingVolume


