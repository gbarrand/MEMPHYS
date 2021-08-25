#ifndef MEMPHYSWCSD_h
#define MEMPHYSWCSD_h

#include "WCHit.hh"
#include "DetectorConstruction.hh"

//Geant4
#include "G4VSensitiveDetector.hh"
#include "G4ParticleTypes.hh"

namespace MEMPHYS {

class WCSD : public G4VSensitiveDetector {
 public:
  WCSD(G4String name, DetectorConstruction* aDetConstruction)
 :G4VSensitiveDetector(name),MEMPHYSDetConstruction(aDetConstruction) {
    G4String HCname;
    collectionName.insert(HCname="WCPMT");
    HCID = -1;
  }

  virtual ~WCSD() {}
  
  void Initialize(G4HCofThisEvent* HCE) {
    // Make a new hits collection.  With the name we set in the constructor
    hitsCollection = new WCHitsCollection(SensitiveDetectorName,collectionName[0]); 
  
    // This is a trick.  We only want to do this once.  When the program
    // starts HCID will equal -1.  Then it will be set to the pointer to
    // this collection.
    
    //JEC 16/11/05 static int HCID = -1;
    
    // Get the Id of the "0th" collection
    if (HCID<0) HCID =  GetCollectionID(0); //JEC FIXME: is it necessary?
    
    // Add it to the Hit collection of this event.
    HCE->AddHitsCollection( HCID, hitsCollection );  //JEC FIXME: is it necessary?
  
    // Initilize the Hit map to all tubes not hit.
    PMTHitMap.clear();
  
    // Trick to access the static maxPE variable.  This will go away with the 
    // variable.
    //JEC FIXME this maxPe is used for drawing purposes (keep it?)
  
    WCHit* newHit = new WCHit();
    newHit->SetMaxPe(0);
    delete newHit;
  }

  G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* aTH) {return ProcessHits_constStep(aStep,aTH);}

  G4bool ProcessHits_constStep(const G4Step* aStep, G4TouchableHistory*) {
    G4StepPoint*       preStepPoint = aStep->GetPreStepPoint();
    G4TouchableHandle  theTouchable = preStepPoint->GetTouchableHandle();
    G4VPhysicalVolume* thePhysical  = theTouchable->GetVolume();
  
    G4int    trackID           = aStep->GetTrack()->GetTrackID();
    G4String volumeName        = aStep->GetTrack()->GetVolume()->GetName();
    G4double energyDeposition  = aStep->GetTotalEnergyDeposit();
    G4float hitTime            = aStep->GetPreStepPoint()->GetGlobalTime();
  
    G4ParticleDefinition *particleDefinition = aStep->GetTrack()->GetDefinition();
  
    //JEC 16/11/05 Take MF conditional argument
    // MF : I don't see why other particles should register hits
    // they don't in skdetsim. 
    if ( particleDefinition != G4OpticalPhoton::OpticalPhotonDefinition()) return false;
  
    //JEC FIXME the test on OpticalPhoton is not necessary considering the previous test
    if ( (aStep->GetTrack()->GetTrackStatus() == fAlive ) && 
         (particleDefinition == G4OpticalPhoton::OpticalPhotonDefinition()) ) return false;
  
  
    std::string tubeTag(theTouchable->GetVolume()->GetName());
    std::ostringstream copyNo;
    for (G4int i = theTouchable->GetHistoryDepth()-1 ; i >= 0; i--) {
      tubeTag += ":";
      copyNo.str("");
      copyNo << theTouchable->GetVolume(i)->GetCopyNo();
      tubeTag += copyNo.str(); 
    }
  
    G4int replicaNumber = MEMPHYSDetConstruction->GetTubeID(tubeTag);
    G4int pmtIdentifier;
    
    //   G4cout <<  "(JEC): ProcssHits: tubeTag <" << tubeTag.c_str() << ">" 
    //  	 << " -> PMT[" << replicaNumber << "]"
    //	 << G4endl;
  
  
    // If this tube hasn't been hit add it to the collection
    if (PMTHitMap[replicaNumber] == 0) {
      WCHit* newHit = new WCHit();
      newHit->SetTubeID(replicaNumber);
      //JEC 6/4/06 the PMTs may be hit by several Tracks so store the infos in vector as for the time
  //     newHit->SetTrackID(trackID);
  //     newHit->SetEdep(energyDeposition); 
      newHit->SetLogicalVolume(thePhysical->GetLogicalVolume());
  
      G4AffineTransform aTrans = theTouchable->GetHistory()->GetTopTransform();
      G4RotationMatrix  aRot   = aTrans.NetRotation();
      //JEC 5/4/06 confirmation of the Inversion after getting the Rotation and before the Translation.
      aTrans.Invert();
      G4ThreeVector    aPos    = aTrans.NetTranslation();
      newHit->SetTransform(G4Transform3D(aRot,aPos));
  
      //JEC 20/4/06 fill the newHit before insert !
      newHit->AddPe(hitTime);
      newHit->AddTrk(trackID);
      newHit->AddEdep(energyDeposition);
      
      PMTHitMap[replicaNumber] = hitsCollection->insert( newHit ); 
      
      //JEC 20/4/06 too complicated...
  //     (*hitsCollection)[PMTHitMap[replicaNumber]-1]->AddPe(hitTime);
  //     (*hitsCollection)[PMTHitMap[replicaNumber]-1]->AddTrk(trackID);
  //     (*hitsCollection)[PMTHitMap[replicaNumber]-1]->AddEdep(energyDeposition);
      
  //     G4cout << "(JEC) ProcessHits: new Hit " << G4endl; newHit->Print();
    } else {
      //JEC 20/4/06 simplify the code
      pmtIdentifier = PMTHitMap[replicaNumber]-1;
      (*hitsCollection)[pmtIdentifier]->AddPe(hitTime);
      (*hitsCollection)[pmtIdentifier]->AddTrk(trackID);
      (*hitsCollection)[pmtIdentifier]->AddEdep(energyDeposition);
      //G4cout << "(JEC) ProcessHits: old Hit[" << PMTHitMap[replicaNumber]-1 << "]:" << hitTime << G4endl;
    }
  
    return true;
  }

  void EndOfEvent(G4HCofThisEvent*) {
    if (verboseLevel>0) { 
      G4int numHits = hitsCollection->entries();
  
      G4cout << "There are " << numHits << " hits in the WC: " << G4endl;
      for (G4int i=0; i < numHits; i++) 
        (*hitsCollection)[i]->Print();
    } 
  }
   
 private:

  WCHitsCollection* hitsCollection;
  std::map<int,int> PMTHitMap;   // Whether a PMT was hit already
  G4int HCID;

  
  DetectorConstruction* MEMPHYSDetConstruction; //JEC 25/1/06

};

}
#endif

