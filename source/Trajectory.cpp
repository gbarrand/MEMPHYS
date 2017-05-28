#include "../MEMPHYS/Trajectory.hh"

//Geant4
#include "G4TrajectoryPoint.hh"
#include "G4ParticleTable.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UnitsTable.hh"
#include "G4VProcess.hh"

//std
#include <sstream>

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {
  G4Allocator<Trajectory> myTrajectoryAllocator;
}

MEMPHYS::Trajectory::Trajectory()
  :  positionRecord(0), 
     fTrackID(0), 
     fParentID(0),
     PDGEncoding(0), 
     PDGCharge(0.0), 
     ParticleName(""),
     initialMomentum( G4ThreeVector() ),
     SaveIt(false),
     creatorProcess(""),
     globalTime(0.0)
{;}//Ctor

//----------------------------------------------------------------------------------------------------

MEMPHYS::Trajectory::Trajectory(const G4Track* aTrack) {

  // Following is for the first trajectory point
  positionRecord = new TrajectoryPointContainer();
  positionRecord->push_back(new G4TrajectoryPoint(aTrack->GetPosition()));

  fTrackID = aTrack->GetTrackID();
  fParentID = aTrack->GetParentID();


  G4ParticleDefinition * fpParticleDefinition = aTrack->GetDefinition();
  PDGEncoding = fpParticleDefinition->GetPDGEncoding();
  PDGCharge = fpParticleDefinition->GetPDGCharge();
  ParticleName = fpParticleDefinition->GetParticleName();
  initialMomentum = aTrack->GetMomentum();

  stoppingPoint  = aTrack->GetPosition();
  stoppingVolume = aTrack->GetVolume();

  //JEC 15/12/05 SaveIt = false at creation time now
  SaveIt = false;
//   if ( aTrack->GetUserInformation() != 0 ) {
//     SaveIt = true;
//   } else { 
//     SaveIt = false;
//   }

  if (aTrack->GetCreatorProcess() != 0 ) {
    const G4VProcess* tempproc = aTrack->GetCreatorProcess();
    creatorProcess = tempproc->GetProcessName();
  } else {
    creatorProcess = "";
  }

  globalTime = aTrack->GetGlobalTime();
} //Ctor

//----------------------------------------------------------------------------------------------------

MEMPHYS::Trajectory::Trajectory(MEMPHYS::Trajectory & right):G4VTrajectory() {

  positionRecord = new TrajectoryPointContainer();
  for(size_t i=0;i<right.positionRecord->size();i++) {
    G4TrajectoryPoint* rightPoint = (G4TrajectoryPoint*)((*(right.positionRecord))[i]);
    positionRecord->push_back(new G4TrajectoryPoint(*rightPoint));
  }

  fTrackID         = right.fTrackID;
  fParentID        = right.fParentID;
  PDGEncoding      = right.PDGEncoding;
  PDGCharge        = right.PDGCharge;
  ParticleName     = right.ParticleName;
  initialMomentum  = right.initialMomentum;
  stoppingPoint    = right.stoppingPoint;
  stoppingVolume   = right.stoppingVolume;
  SaveIt           = right.SaveIt;
  creatorProcess   = right.creatorProcess;
  globalTime       = right.globalTime;
}//Ctor

//----------------------------------------------------------------------------------------------------

MEMPHYS::Trajectory::~Trajectory() {
  //  positionRecord->clearAndDestroy();
  size_t i;
  for(i=0;i<positionRecord->size();i++){
    delete  (*positionRecord)[i];
  }
  positionRecord->clear();
  
  delete positionRecord;
} //Dtor

//----------------------------------------------------------------------------------------------------


void MEMPHYS::Trajectory::ShowTrajectory(std::ostream& os) const {
  // Invoke the default implementation in G4VTrajectory...
  G4VTrajectory::ShowTrajectory(os);
  // ... or override with your own code here.
}//ShowTrajectory

//----------------------------------------------------------------------------------------------------

void MEMPHYS::Trajectory::DrawTrajectory(G4int i_mode) const {
  // Invoke the default implementation in G4VTrajectory...
  G4VTrajectory::DrawTrajectory(i_mode);
  // ... or override with your own code here.
}//DrawTrajectory

//----------------------------------------------------------------------------------------------------

const std::map<G4String,G4AttDef>* MEMPHYS::Trajectory::GetAttDefs() const {
  G4bool isNew;
  std::map<G4String,G4AttDef>* store = G4AttDefStore::GetInstance("Trajectory",isNew);

  if (isNew) {    
    G4String ID("ID");
    (*store)[ID] = G4AttDef(ID,"Track ID","Bookkeeping","","G4int");
    
    G4String PID("PID");
    (*store)[PID] = G4AttDef(PID,"Parent ID","Bookkeeping","","G4int");
    
    G4String PN("PN");
    (*store)[PN] = G4AttDef(PN,"Particle Name","Physics","","G4String");
    
    G4String Ch("Ch");
    (*store)[Ch] = G4AttDef(Ch,"Charge","Physics","","G4double");
    
    G4String PDG("PDG");
    (*store)[PDG] = G4AttDef(PDG,"PDG Encoding","Physics","","G4int");
    
    G4String IMom("IMom");
    (*store)[IMom] = G4AttDef(IMom, "Momentum of track at start of trajectory",
			      "Physics","","G4ThreeVector");
    
    G4String NTP("NTP");
    (*store)[NTP] = G4AttDef(NTP,"No. of points","Physics","","G4int");
    
  }
  return store;
}//GetAttDefs

//----------------------------------------------------------------------------------------------------

std::vector<G4AttValue>* MEMPHYS::Trajectory::CreateAttValues() const {

  //JEC 1/12/05 use sstream std library
  std::ostringstream s;
  
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;
  
  s.str("");
  s << fTrackID;
  values->push_back(G4AttValue("ID",s.str(),""));
  
  s.str("");
  s << fParentID;
  values->push_back(G4AttValue("PID",s.str(),""));
  
  values->push_back(G4AttValue("PN",ParticleName,""));
  
  s.str("");
  s << PDGCharge;
  values->push_back(G4AttValue("Ch",s.str(),""));

  s.str("");
  s << PDGEncoding;
  values->push_back(G4AttValue("PDG",s.str(),""));

  s.str("");
  s << G4BestUnit(initialMomentum,"Energy");
  values->push_back(G4AttValue("IMom",s.str(),""));

  s.str("");
  s << GetPointEntries();
  values->push_back(G4AttValue("NTP",s.str(),""));

  return values;
}//CreateAttValues

//----------------------------------------------------------------------------------------------------

void MEMPHYS::Trajectory::AppendStep(const G4Step* aStep) {
  positionRecord->push_back( new G4TrajectoryPoint(aStep->GetPostStepPoint()->GetPosition() ));
}//AppendStep

//----------------------------------------------------------------------------------------------------


G4ParticleDefinition* MEMPHYS::Trajectory::GetParticleDefinition() {
  return (G4ParticleTable::GetParticleTable()->FindParticle(ParticleName));
}//GetParticleDefinition

//----------------------------------------------------------------------------------------------------


void MEMPHYS::Trajectory::MergeTrajectory(G4VTrajectory* secondTrajectory) {
  if(!secondTrajectory) return;

  Trajectory* seco         = (Trajectory*)secondTrajectory;

  stoppingPoint  = seco->stoppingPoint;
  stoppingVolume = seco->stoppingVolume;
  
  G4int ent = seco->GetPointEntries();
  for(G4int i=1;i<ent;i++) // initial point of the second trajectory should not be merged
  { 
    positionRecord->push_back((*(seco->positionRecord))[i]);
    //    positionRecord->push_back(seco->positionRecord->removeAt(1));
  }
  delete (*seco->positionRecord)[0];
  seco->positionRecord->clear();
}//MergeTrajectory


