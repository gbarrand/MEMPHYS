#ifndef MEMPHYSDetectorConstruction_h
#define MEMPHYSDetectorConstruction_h

//Geant4
#include "G4Version.hh"
#include "G4Transform3D.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4OpticalSurface.hh"
#include "G4UnitsTable.hh"
#include "G4RunManager.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

//std
#include <fstream>
#include <map>
#include <string>

namespace MEMPHYS {

class Analysis; 
class WCSD;

enum cyl_location {endcap1,wall,endcap2}; //geographical position of the PMTs


class DetectorConstruction : public G4VUserDetectorConstruction {
public:
  DetectorConstruction(Analysis&);
  virtual ~DetectorConstruction() {}

public:  
  G4VPhysicalVolume* Construct();

  // Related to the WC geometry
  void Set20inchPMTGeometry(); //JEC is obsolete according to M.F but code is still valid(?)
  void Set8inchPMTGeometry();
  void Set12inchPMTGeometry(); //JEC 26/1/06 
  void SetRockGeometry();
  void UpdateGeometry() {G4RunManager::GetRunManager()->DefineWorldVolume(Construct());}

  G4double GetWaterTubeLength()   {return WCLength;}
  G4double GetWaterTubePosition() {return WCPosition;}
  G4double GetPMTSize()           {return WCPMTRadius;}
  G4double GetRockThickness()     {return RockThickness;}
  //JEC FIXME not used G4int    GetMyConfiguration()   {return myConfiguration;}

  // Related to the WC tube IDs
  //JEC 25/1/06 no more static functions + const char*
  G4int GetTubeID(const std::string& tubeTag) {return tubeLocationMap[tubeTag];} 

  G4Transform3D GetTubeTransform(int tubeNo){return tubeIDMap[tubeNo];}

private:

  // The Construction routines
  G4LogicalVolume*   ConstructWC();
  void               ConstructRock(G4LogicalVolume* logicExpHall, G4bool RockIsVisible);
  void               ConstructMaterials();

  // Code for traversing the geometry and assigning tubeIDs.
  // First make a typedef for the pointer to the member fcn.  The
  // syntax is too wacked out to be using all over.

  typedef void (DetectorConstruction::*DescriptionFcnPtr) (G4VPhysicalVolume*, 
							   int, int, const G4Transform3D&);

  // Now Funcs for traversing the geometry
  void TraverseReplicas(G4VPhysicalVolume*, int, const G4Transform3D&, DescriptionFcnPtr);

  void DescribeAndDescendGeometry(G4VPhysicalVolume*, int, int, const G4Transform3D&, DescriptionFcnPtr);

  // Functions that the traversal routines call or we use to manipulate the
  // data we accumulate.
  void DumpGeometryTableToFile(); //JEC FIXME deprecated use FillGeometryTuple
  void FillGeometryTuple();

  void PrintGeometryTree(G4VPhysicalVolume* aPV,int aDepth,int replicaNo,const G4Transform3D& aTransform) {
    for (int levels = 0; levels < aDepth; levels++) G4cout << " ";
    G4cout << aPV->GetName() << " Level:" << aDepth << " replica[" << replicaNo << "] "
           << " Pos:" << aTransform.getTranslation() 
           << " Rot:" << aTransform.getRotation().getTheta()/CLHEP::deg 
           << "," << aTransform.getRotation().getPhi()/CLHEP::deg 
           << "," << aTransform.getRotation().getPsi()/CLHEP::deg
           << G4endl;
  }
  
  void DescribeAndRegisterPMT(G4VPhysicalVolume*, int, int, const G4Transform3D&);
  void GetWCGeom(G4VPhysicalVolume*, int, int, const G4Transform3D&);

private:
  class DetectorMessenger: public G4UImessenger {
  public:
    DetectorMessenger(MEMPHYS::DetectorConstruction* MEMPHYSDet):MEMPHYSDetector(MEMPHYSDet) { 
      MEMPHYSDir = new G4UIdirectory("/MEMPHYS/");
      MEMPHYSDir->SetGuidance("Commands to change the geometry of the simulation");
    
      PMTConfig = new G4UIcmdWithAString("/MEMPHYS/WCgeom",this);
      PMTConfig->SetGuidance("Set the geometry configuration for the WC.");
      PMTConfig->SetGuidance("Available options 8inch");
      PMTConfig->SetParameterName("PMTConfig", false);
      PMTConfig->SetDefaultValue("8inch");
      PMTConfig->SetCandidates("8inch");
      PMTConfig->AvailableForStates(G4State_PreInit, G4State_Idle);
    }
    virtual ~DetectorMessenger() {
      delete PMTConfig;
      delete MEMPHYSDir;
    }
  public:
    virtual void SetNewValue(G4UIcommand* command, G4String newValue) {
      if( command == PMTConfig ) { 
        if (newValue == "8inch") {
          MEMPHYSDetector->Set8inchPMTGeometry();
        } else {
          G4cout << "That geometry choice not defined!" << G4endl;
        }
      }
      MEMPHYSDetector->UpdateGeometry();
    }
  private:
    DetectorConstruction* MEMPHYSDetector;
  private: //commands
    G4UIdirectory*      MEMPHYSDir;
    G4UIcmdWithAString* PMTConfig;
  };
  
private:  
  //JEC FIXME is it necessary to maintain fAnalysis?
  Analysis&  fAnalysis;            //the Analysis JEC 18/11/05
  
  // Sensitive Detectors. We declare the pointers here because we need
  // to check their state if we change the geometry.
  WCSD*  aWCPMT;

  //Water, Blacksheet surface
  G4OpticalSurface * OpWaterBSSurface;

  //Glass, Cathode surface in PMTs
  G4OpticalSurface * OpGlassCathodeSurface;

  // The messenger we use to change the geometry.
  DetectorMessenger* messenger;

  //---Volume lengths

  // These are shared between the different member functions 
  // constructWC

  G4int RockChoice;

  G4double WCLength;
  G4double RockLength;

  G4double WCPosition;
  G4double RockPosition;

  // WC geometry parameters

  G4double WCPMTRadius;
  G4double WCDiameter;
  G4double WCRadius;
//G4double WCBarrelPMTBlackSheetDist;
//G4double WCCapPMTBlackSheetDist;
  G4double WCCapLength;
  G4double WCBackODLength;
  G4double WCFrontODLength;
  G4double WCBarrelLength;
  G4double WCBarrelRingRadius;
  G4double WCBarrelPMTRadius;
  G4int    WCBarrelRingNPhi;
  G4double WCBarrelRingdPhi;
  G4int    WCBarrelNRings;
  G4double WCBarrelCellLength;
  G4int    WCCapNCell;
  G4double WCCapCellSize;
//G4double WCCapSquareSide;
//G4double WCCapSquareDiag;
  G4double WCCapEdgeWidth;
  G4double WCCapEdgeLimit;
  G4double WCPMTGlassThickness;
  G4double WCBlackSheetThickness;

  // Variables related to the geometry

  //  std::ofstream geoFile;   // File for text output
  G4int totalNumPMTs;      // The number of PMTs for this configuration     
  G4double WCCylInfo[2];    // Info for the geometry tree: radius & length
  G4double WCPMTSize;       // Info for the geometry tree: pmt size
  G4ThreeVector WCOffset;   // Info for the geometry tree: WC center offset

  // Tube map information
  //JEC 25/1/06 no more static variables
  std::map<int, G4Transform3D> tubeIDMap;
  std::map<int, cyl_location> tubeCylLocation;
  std::map<std::string, int>  tubeLocationMap; //JEC 25/1/06 use a map standard

  // Variable related to the rock
  G4double RockThickness;
  G4double RockUpstreamLength_z;
  G4double RockDownstreamLength_z;
 
  G4double RockMotherVolLength_x;
  G4double RockMotherVolLength_y;
  G4double RockMotherVolLength_z;

//G4double RockUpstreamHallRadius;

  G4double RockCentralHallLength_x;
  G4double RockCentralHallLength_y;
  G4double RockCentralHallLength_z;

  G4double RockUpHallouterRadiusTube;
  G4double RockUpHallinnerRadiusTube;
  G4double RockUpHallhightTube;
  G4double RockUpHallstartAngleTube;
  G4double RockUpHallspanningAngleTube;
 
  G4double RockDownHallLength_x;
  G4double RockDownHallLength_y;
  G4double RockDownHallLength_z;

  G4double RockUpHallTubePosition_y;
//G4double RockUpHallTubePosition_z;

  G4double RockTmpBox1Length_x;
  G4double RockTmpBox1Length_y;
  G4double RockTmpBox1Length_z;

  G4double RockCeilingLength_x;
  G4double RockCeilingLength_y;
  G4double RockCeilingLength_z;


  G4ThreeVector RockMotherPosition;
  G4ThreeVector RockCentralHallPosition;
  G4ThreeVector RockDownHallPosition;
  G4ThreeVector RockCenterHallrelativeMomPosition;
  G4ThreeVector RockCeilingPosition;

  G4double RockCentralHallPosition_y;
  G4double RockCentralHallPosition_z;
  G4double RockMotherPosition_y;
  G4double RockMotherPosition_z;
  G4double RockDownHallPosition_y;
  G4double RockDownHallPosition_z;

  G4ThreeVector RockUpHallTubePosition;
  G4ThreeVector RockTmpBox1relativeUpHallTubePosition;
  G4ThreeVector RockUpHallTuberelativeCentralHallPosition;
  G4ThreeVector RockDownHallrelativeCentralHallPosition;
  G4ThreeVector RockDownrelativeMomPosition;
  G4ThreeVector RockUpHallrelativeCeilingPosition;

  G4double RockUpHallShift;


  G4double GapRockUpstreamlAr;                 //JEC FIXME 
  G4double GapRockDownstreamMRDScint;          //JEC FIXME
  G4double lArDLength;                         //JEC FIXME

  // Variables related to configuration

  //JEC FIXME not used G4int myConfiguration;   // Detector Config Parameter
 
  // Rock related variable
//G4double RockUpstreamPosition; // RockUpstream z-position from WC center
//G4double RockDownstreamPosition; // RockDownstream z-position from WC center
  
};

}


#endif

