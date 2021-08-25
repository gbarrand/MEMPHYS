#ifndef MEMPHYSSteppingAction_h
#define MEMPHYSSteppingAction_h

//MEMPHYS
#include "WCSD.hh"
#include "TrackInformation.hh"

#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4ParticleTypes.hh"
#include "G4OpBoundaryProcess.hh"

namespace MEMPHYS {

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction() {}
  virtual ~SteppingAction() {}

public:
  virtual void UserSteppingAction(const G4Step * theStep){
    G4Track* theTrack = theStep->GetTrack();
  
    //  G4cout << "(JEC) UserSteppingAction: INFO: BEGIN Step for Trk<"
    //	 <<theTrack->GetTrackID() << ">" << G4endl;
  
    TrackInformation* trackInformation = (TrackInformation*)theTrack->GetUserInformation();

    //JEC 25/1/06 not used  G4StepPoint* thePrePoint = theStep->GetPreStepPoint();
    //JEC 25/1/06 not used  G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();

    G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
    G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

    G4OpBoundaryProcessStatus boundaryStatus=Undefined;
    static G4OpBoundaryProcess* boundary=NULL;

    if(!thePostPV){//out of world
      //    G4cout << "(JEC) UserSteppingAction: WARNING: Out of World" << G4endl;
      return;
    }

    G4ParticleDefinition* particleType = theTrack->GetDefinition();
    if(particleType==G4OpticalPhoton::OpticalPhotonDefinition()){
      //Optical photon only
  
      //find the boundary process only once
      if( !boundary  ){
        //      G4cout << "(JEC) UserSteppingAction: INFO : boundary NOT defined " << G4endl;
        G4ProcessManager* pm = theStep->GetTrack()->GetDefinition()->GetProcessManager();
        //      G4cout << "(JEC) UserSteppingAction: INFO : particle < " << theStep->GetTrack()->GetDefinition()->GetParticleName(  ) << G4endl;
        G4int nprocesses = pm->GetProcessListLength();
        G4ProcessVector* pv = pm->GetProcessList();
        G4int i;
        for( i=0;i<nprocesses;i++){
  	//	G4cout << "(JEC) Us  erSteppingAction: INFO : process <"<<i << "/" << nprocesses << ">: " << (*pv)[i]->GetProcessName()  << G4endl;  
          if((*pv)[i]->GetProcessName()=="OpBoundary"){
            boundary = (G4OpBoundaryProcess*)(*pv)[i];
            break;
          }  
        }
      }//set boundary once
      if(!boundary) {
        G4cout << "(JEC) UserSteppingAction: FATAL: NO OpBoundary FOUND" << G4endl;
        exit(0);
      }
      
      //    G4cout << "(JEC) UserSteppingAction: INFO : PostPV [" << thePostPV->GetName() << "]" << G4endl;
      if(thePostPV->GetName()=="expHall") {
        //Kill photons entering expHall
        theTrack->SetTrackStatus(fStopAndKill);
      }
          
      //Was the photon absorbed by the absorption process    
  //     G4cout << "(JEC) UserSteppingAction: INFO : PostPointProcess [" 	   
  // 	   << thePostPoint->GetProcessDefinedStep()->GetProcessName() 
  // 	   << "]" << G4endl;
      if(thePostPoint->GetProcessDefinedStep()->GetProcessName() =="OpAbsorption"){
        trackInformation->AddTrackStatusFlag(absorbed);
      }
     
      boundaryStatus=boundary->GetStatus();
   
      //Check to see if the partcile was actually at a boundary
      //Otherwise the boundary status may not be valid
      //Prior to Geant4.6.0-p1 this would not have been enough to check
      if(thePostPoint->GetStepStatus()==fGeomBoundary){
        switch(boundaryStatus){
        case Absorption:
          trackInformation->AddTrackStatusFlag(boundaryAbsorbed);
          break;
        case Detection: //Note, this assumes that the volume causing detection
                        //is the photocathode because it is the only one with
                        //non-zero efficiency  
          {
  	  //Triger sensitive detector manually since photon is
	  //absorbed but status was Detection
            G4SDManager* SDman = G4SDManager::GetSDMpointer();
            G4String sdName="/MEMPHYS/WCPMT"; //see DetectorConstruction::ConstructWC 
            WCSD* wcSD = (WCSD*)SDman->FindSensitiveDetector(sdName);  
            if(wcSD) { 
              wcSD->ProcessHits_constStep(theStep,NULL);
            } else {
              G4cout << "(JEC) UserSteppingAction: FATAL: NO WC Sensitive Det. FOUND" << G4endl;
              exit(0);
            }
            trackInformation->AddTrackStatusFlag(hitPMT);
            //G4cout << "(JEC) UserSteppingAction: INFO : Track Flag include hitPMT"
            //       << G4endl;
            break;
          }//case Detection
        case FresnelReflection:
        case TotalInternalReflection:
        case SpikeReflection:
        default:
          break;
        }//eo switch on boundary values
      }//test on GeomBoundary
    }//Optical photon
  
    //  G4cout << "(JEC) UserSteppingAction: INFO: END" << G4endl;
  }

  //  void SetOneStepPrimaries(G4bool b){oneStepPrimaries=b;}
  // G4bool GetOneStepPrimaries(){return oneStepPrimaries;}
  
private:

  // G4bool oneStepPrimaries;
};
}
#endif
