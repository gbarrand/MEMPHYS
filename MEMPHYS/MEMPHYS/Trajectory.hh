//JEC FIXME: what a forward declaration like that!!!   class MEMPHYSTrajectory;
#ifndef MEMPHYSTrajectory_h
#define MEMPHYSTrajectory_h 1

#include "G4Version.hh"
#include "G4VTrajectory.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4TrajectoryPoint.hh"
#include "G4ParticleTable.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UnitsTable.hh"

#include <cstdlib>
#include <vector>

typedef std::vector<G4VTrajectoryPoint*>  TrajectoryPointContainer;

namespace MEMPHYS {

class Trajectory : public G4VTrajectory {

public:
  Trajectory()
  :positionRecord(0), 
  fTrackID(0), 
  fParentID(0),
  PDGEncoding(0), 
  PDGCharge(0.0), 
  ParticleName(""),
  initialMomentum( G4ThreeVector() ),
  SaveIt(false),
  creatorProcess(""),
  globalTime(0.0)
  {}

  Trajectory(const G4Track* aTrack) {
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
    stoppingDirection = aTrack->GetMomentumDirection();
    stoppingVolume = aTrack->GetVolume();

    SaveIt = false;  //JEC 15/12/05 SaveIt = false at creation time now
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
  }

  Trajectory(Trajectory & right):G4VTrajectory() {
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
    stoppingDirection = right.stoppingDirection;
    stoppingVolume   = right.stoppingVolume;
    SaveIt           = right.SaveIt;
    creatorProcess   = right.creatorProcess;
    globalTime       = right.globalTime;
  }

  virtual ~Trajectory() {
    //  positionRecord->clearAndDestroy();
    size_t i;
    for(i=0;i<positionRecord->size();i++){
      delete  (*positionRecord)[i];
    }
    positionRecord->clear();
    delete positionRecord;
  }

  // Operators
  void* operator new(size_t) {return (void*)allocator()->MallocSingle();}
  void operator delete(void* aTrajectory) {
    allocator()->FreeSingle((Trajectory*)aTrajectory);
  }

  int operator == (const Trajectory& right) const {return (this==&right);} 

  //inherited methods 
  virtual G4int GetTrackID() const { return fTrackID; }
  virtual G4int GetParentID() const { return fParentID; }
  virtual G4String GetParticleName() const { return ParticleName; }
  virtual G4double GetCharge() const { return PDGCharge; }
  virtual G4int GetPDGEncoding() const { return PDGEncoding; }
  virtual G4ThreeVector GetInitialMomentum() const { return initialMomentum; }
  virtual int GetPointEntries() const { return (int)positionRecord->size(); }
  virtual G4VTrajectoryPoint* GetPoint(G4int i) const { return (*positionRecord)[i]; }

  //JEC FIXME what to do with these Show/Draw routines...
  virtual void ShowTrajectory(std::ostream& os = G4cout) const {
    G4VTrajectory::ShowTrajectory(os);
    // ... or override with your own code here.
  }

#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=1031 //G.Barrand
  virtual void DrawTrajectory() const {
    G4VTrajectory::DrawTrajectory();
    // ... or override with your own code here.
  }
#else
  virtual void DrawTrajectory(G4int i_mode = 0) const {
    G4VTrajectory::DrawTrajectory(i_mode);
    // ... or override with your own code here.
  }
#endif

  //Used by G.Barrand (not yet 6/4/06)
  virtual const std::map<G4String,G4AttDef>* GetAttDefs() const {
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
  }

  virtual std::vector<G4AttValue>* CreateAttValues() const {
    std::vector<G4AttValue>* values = new std::vector<G4AttValue>;
  
    std::ostringstream s;
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
  }

  //Other Get/Set
  G4ParticleDefinition* GetParticleDefinition() {
    return (G4ParticleTable::GetParticleTable()->FindParticle(ParticleName));
  }

  G4String GetCreatorProcessName() const { return creatorProcess; }
  G4double GetGlobalTime() const { return globalTime; }

  G4bool GetSaveFlag() const { return SaveIt; }
  void SetSaveFlag(G4bool value) { SaveIt = value; }

  G4ThreeVector GetStoppingPoint() const { return stoppingPoint; }
  void SetStoppingPoint(const G4ThreeVector& currentPosition) { stoppingPoint = currentPosition;}

  G4ThreeVector GetStoppingDirection() const { return stoppingDirection; }
  void SetStoppingDirection(const G4ThreeVector& a_direction) { stoppingDirection = a_direction;}
  
  G4VPhysicalVolume* GetStoppingVolume() const { return stoppingVolume;}
  void SetStoppingVolume(G4VPhysicalVolume* currentVolume) { stoppingVolume = currentVolume;}


  // Other member functions
  virtual void AppendStep(const G4Step* aStep) {
    positionRecord->push_back( new G4TrajectoryPoint(aStep->GetPostStepPoint()->GetPosition() ));
  }

  virtual void MergeTrajectory(G4VTrajectory* secondTrajectory) {
    if(!secondTrajectory) return;
    Trajectory* seco         = (Trajectory*)secondTrajectory;
    stoppingPoint  = seco->stoppingPoint;
    stoppingDirection  = seco->stoppingDirection;
    stoppingVolume = seco->stoppingVolume;
    G4int ent = seco->GetPointEntries();
    for(G4int i=1;i<ent;i++) // initial point of the second trajectory should not be merged
    { 
      positionRecord->push_back((*(seco->positionRecord))[i]);
      //    positionRecord->push_back(seco->positionRecord->removeAt(1));
    }
    delete (*seco->positionRecord)[0];
    seco->positionRecord->clear();
  }

private:
  static G4Allocator<Trajectory>* allocator() {
    //warning : it can't be on the stack, since it is managed by the G4RunManager.
    static G4Allocator<Trajectory>* s_allocator = new G4Allocator<Trajectory>;
    return s_allocator;
  }
private:

  TrajectoryPointContainer* positionRecord;
  G4int                     fTrackID;
  G4int                     fParentID;
  G4int                     PDGEncoding;
  G4double                  PDGCharge;
  G4String                  ParticleName;
  G4ThreeVector             initialMomentum;

  G4ThreeVector             stoppingPoint;
  G4ThreeVector             stoppingDirection;
  G4VPhysicalVolume         *stoppingVolume;

  G4bool                    SaveIt;
  G4String                  creatorProcess;
  G4double                  globalTime;
};
}

#endif

