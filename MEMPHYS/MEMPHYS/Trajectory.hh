//JEC FIXME: what a forward declaration like that!!!   class MEMPHYSTrajectory;
#ifndef MEMPHYSTrajectory_h
#define MEMPHYSTrajectory_h 1

//Geant4 
#include "G4VTrajectory.hh"
#include "G4Allocator.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4TrajectoryPoint.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Version.hh"

//std
#include <stdlib.h>
#include <vector>


class G4Polyline;

typedef std::vector<G4VTrajectoryPoint*>  TrajectoryPointContainer;

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {


class Trajectory : public G4VTrajectory {

 public:

  
  // Constructor/Destrcutor
   Trajectory();
   Trajectory(const G4Track* aTrack);
   Trajectory(Trajectory &);

   virtual ~Trajectory();

  // Operators
  inline void* operator new(size_t);
  inline void  operator delete(void*);
  inline int operator == (const Trajectory& right) const
  {return (this==&right);} 

  //inherited methods 
  virtual G4int GetTrackID() const { return fTrackID; }
  virtual G4int GetParentID() const { return fParentID; }
  virtual G4String GetParticleName() const { return ParticleName; }
  virtual G4double GetCharge() const { return PDGCharge; }
  virtual G4int GetPDGEncoding() const { return PDGEncoding; }
  virtual G4ThreeVector GetInitialMomentum() const { return initialMomentum; }
  virtual int GetPointEntries() const { return positionRecord->size(); }
  virtual G4VTrajectoryPoint* GetPoint(G4int i) const 
  { return (*positionRecord)[i]; }
  //JEC FIXME what to do with these Show/Draw routines...
  virtual void ShowTrajectory(std::ostream& os=G4cout) const;
#if defined(G4VERSION_NUMBER) && G4VERSION_NUMBER>=1031 //G.Barrand
  virtual void DrawTrajectory() const;
#else
  virtual void DrawTrajectory(G4int i_mode=0) const;
#endif

  //Used by G.Barrand (not yet 6/4/06)
  virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
  virtual std::vector<G4AttValue>* CreateAttValues() const;


  //Other Get/Set
  G4ParticleDefinition* GetParticleDefinition();

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
  virtual void AppendStep(const G4Step* aStep);
  virtual void MergeTrajectory(G4VTrajectory* secondTrajectory);



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

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {
  extern G4Allocator<Trajectory> myTrajectoryAllocator;
}

inline void* MEMPHYS::Trajectory::operator new(size_t)
{
  void* aTrajectory;
  aTrajectory = (void*)MEMPHYS::myTrajectoryAllocator.MallocSingle();
  return aTrajectory;
}

inline void MEMPHYS::Trajectory::operator delete(void* aTrajectory)
{
  MEMPHYS::myTrajectoryAllocator.FreeSingle((MEMPHYS::Trajectory*)aTrajectory);
}

#endif

