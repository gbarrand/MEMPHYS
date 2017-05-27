#include "../MEMPHYS/DetectorConstruction.hh"

// Geant 4
#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4EllipticalTube.hh"
#include "G4Hype.hh"
#include "G4Polyhedra.hh"

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4Vector3D.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"

#include "G4PVReplica.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"

#include "G4AssemblyVolume.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UserLimits.hh"
#include "G4UnitsTable.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpticalSurface.hh"

#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "globals.hh"

//std
#include <fstream>
#include <sstream>
#include <iomanip>

// AIDA :
#include <AIDA/ITree.h>
#include <AIDA/IManagedObject.h>
#include <AIDA/ITuple.h>


//MEMPHYS
#include "../MEMPHYS/WCSD.hh"
#include "../MEMPHYS/DetectorMessenger.hh"
#include "../MEMPHYS/Cast.hh"
#include "../MEMPHYS/Analysis.hh"

MEMPHYS::DetectorConstruction::DetectorConstruction(MEMPHYS::Analysis& aAnalysis) 
:fAnalysis(aAnalysis)
#ifdef APP_USE_AIDA
,geomTuple(0)
#endif
{

#ifdef APP_USE_AIDA
  //Get User Histo pointers
  AIDA::ITree* usrTree = aAnalysis.tree();
  if (!usrTree) {
    G4cout << "MEMPHYS::DetectorConstruction: FATAL: cannot get Analysis Tree" << G4endl;
    exit(0);
  }

  AIDA::IManagedObject* obj = usrTree->find("Geometry");
  if(!obj) {
    G4cout << "DetectorConstruction: WARNING: no tuple Geometry" << G4endl;
    usrTree->ls();
    //exit(0);   
  } else {
    geomTuple =  CAST(obj,AIDA::ITuple);
    if (!geomTuple) {
      G4cout << "DetectorConstruction: FATAL: geomtuple not a AIDA::ITuple" << G4endl;
      usrTree->ls();
      exit(0);
    }
  }
#endif
  
  //-----------------
  // Create Materials
  //-----------------
    
  ConstructMaterials();

  //-----------------
  // Initilize SD pointers
  //-----------------

  aWCPMT     = 0; 

  //-----------------
  // Initilize position paramaters
  //-----------------

  WCLength     = 0;
  RockLength   = 0;
  RockPosition = 0;
  WCPosition   = 0;

  //-----------------
  // Set rock choice (0 for no rock, 1 for rock inside the hall)
  //-----------------
  
  RockChoice = 1;

  //-----------------
  // Initilize things related to the tubeID
  //-----------------

  DetectorConstruction::tubeIDMap.clear();
  DetectorConstruction::tubeCylLocation.clear();
  DetectorConstruction::tubeLocationMap.clear();
  totalNumPMTs = 0;

  //-----------------
  // Set the default WC geometry.  This can be changed later.
  //-----------------

  Set12inchPMTGeometry(); //JEC for debug
  //Set8inchPMTGeometry();

  //-----------------
  // Make the detector messenger to allow changing geometry
  //-----------------

  messenger = new DetectorMessenger(this);

#ifdef APP_USE_AIDA
  obj = usrTree->find("Geometry");
  if(!obj) {
    G4cout << "DetectorConstruction: FATAL: no tuple Geometry" << G4endl;
    usrTree->ls();
    exit(0);
  }  
  geomTuple =  CAST(obj,AIDA::ITuple);
  if (!geomTuple) {
    G4cout << "DetectorConstruction: FATAL: geomtuple not a Tuple" << G4endl;
    usrTree->ls();
    exit(0);
  }
#endif
}//Ctor

//-----------------------------------------------------------------------------------------------

void MEMPHYS::DetectorConstruction::UpdateGeometry() {
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
} //UpdateGeometry

//----------------------------------------------------------------------------------------------
MEMPHYS::DetectorConstruction::~DetectorConstruction(){;}

//-----------------------------------------------------------------------------------------------
G4int MEMPHYS::DetectorConstruction::GetTubeID(std::string tubeTag){
  return tubeLocationMap[tubeTag];
} 
//-----------------------------------------------------------------------------------------------

G4VPhysicalVolume* MEMPHYS::DetectorConstruction::Construct() {
  //-----------------
  // Cleanup old geometry if it already exists 
  // This is needed if we are remaking the geometry.
  //-----------------

  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  totalNumPMTs = 0;

  //-----------------
  // Create Logical Volumes
  //-----------------

  // First create the logical volumes of the sub detectors.  After they are 
  // created their size will be used to make the world volume.
  // Note the order is important because they rearrange themselves depending
  // on their size and detector ordering.

  G4LogicalVolume* logicWCBox;
  logicWCBox = ConstructWC();

  if ( RockChoice == 1 )  SetRockGeometry();

  G4cout << " RockLength     = " << RockLength*0.001 << " m" << G4endl;
  G4cout << " WCLength       = " << WCLength*0.001 << " m"<< G4endl;

  // Now make the detector Hall.  The lengths of the subdectors 
  // were set above.

  G4double expHallLength;
  

  expHallLength = 1.5*(WCLength+RockLength); //JEC this is a guess from above original code

  G4cout << " expHallLength = " << expHallLength*0.001 << " m" << G4endl;
  G4double expHallHalfLength = 0.5*expHallLength;

  G4Box* solidExpHall = new G4Box("expHall",
				  expHallHalfLength,
				  expHallHalfLength,
				  expHallHalfLength);
  
  G4LogicalVolume* logicExpHall = 
    new G4LogicalVolume(solidExpHall,
			G4Material::GetMaterial("Air"),
			"expHall",
			0,0,0);

  // Now set the visualization attributes of the logical volumes.
  logicExpHall->SetVisAttributes(G4VisAttributes::Invisible);

  //-----------------
  // Create and place the physical Volumes
  //-----------------

  // Experimental Hall
  G4VPhysicalVolume* physiExpHall = 
    new G4PVPlacement(0,G4ThreeVector(),
		      logicExpHall,
		      "expHall",
		      0,false,0);

  // construct rock walls 
  if ( RockChoice == 1 ) ConstructRock(logicExpHall, true); //true : rock is visible, false : rock is not visible

  // Water Cherenkov Detector (WC) mother volume
  // WC Box, nice to turn on for x and y views to provide a frame:
  // JEC: Note that the center of WC is at 0,0,0

  G4VPhysicalVolume* physiWCBox = 
    new G4PVPlacement(0,
		      G4ThreeVector(0.,0.,WCPosition),
		      logicWCBox,
		      "WCBox",
		      logicExpHall,
		      false,
		      0);


  // Traverse and print the geometry Tree
  // G4cout << "JEC: Detector Construct Print GeometryTree " << G4endl;
  //TraverseReplicas(physiWCBox, 0, G4Transform3D(), 
  //	   &DetectorConstruction::PrintGeometryTree) ;

  TraverseReplicas(physiWCBox, 0, G4Transform3D(), 
		     &DetectorConstruction::DescribeAndRegisterPMT) ;

  TraverseReplicas(physiWCBox, 0, G4Transform3D(), 
		   &DetectorConstruction::GetWCGeom) ;
  
  //JEC 18/11/05 deprecated 
  //DumpGeometryTableToFile();
  FillGeometryTuple();
  
  // Return the pointer to the physical experimental hall
  return physiExpHall;
} //Construct

//---------------------------------------------------------------------------------------------
//JEC 17/11/05 Include the Rock Geometry Part
//---------------------------------------------------------------------------------------------
 
void MEMPHYS::DetectorConstruction::SetRockGeometry() {
  
  //JEC FIXME how to remove ?
  lArDLength =   8.5*m;       //JEC 17/11/05 JHF2kmConstructlArD.cc                           

  //------------------------------
  // Calculate Volume Dimensions
  //------------------------------
  G4double RockUnknownLength = 3.0*m;

  GapRockUpstreamlAr = 6.5*m;                      //JEC FIXME: how to remove
  GapRockDownstreamMRDScint = 0.*m;                //JEC FIXME: how to remove
  RockUpHallShift = 1.0*m;

  RockThickness = 14.0*m; // was 5.0*m
  RockUpstreamLength_z = RockThickness;
  RockDownstreamLength_z = RockThickness;


  RockUpHallinnerRadiusTube   = 0.;
  RockUpHallouterRadiusTube   = 7.5*m;
  RockUpHallhightTube         = 14.0*m + RockThickness;
  RockUpHallstartAngleTube    = 0.*deg; 
  RockUpHallspanningAngleTube = 360.*deg; 

  // To be fixed --------------
  RockMotherVolLength_x = 2.*RockUpHallouterRadiusTube + 2.*RockThickness;
  RockMotherVolLength_y = 14.0*m + RockThickness; // height of exphall
 
  RockMotherVolLength_z = 34.5*m + RockUpstreamLength_z + RockDownstreamLength_z;
  //---------------------------

  RockCentralHallLength_x = 10.0*m;
  RockCentralHallLength_y = (14.0+1.0)*m;
  RockCentralHallLength_z = 0.4*m + 13.8*m + 0.1*m + RockUnknownLength;

  //----------------------------

  RockTmpBox1Length_x     = RockUpHallouterRadiusTube * 2.;
  RockTmpBox1Length_y     = 5.*m; // > 3.*m  
  RockTmpBox1Length_z     = RockUpHallhightTube + 1.0*m;

  //----------------------------

  RockDownHallLength_x = 11.4*m;
  RockDownHallLength_y = (14.0+1.0)*m;
  RockDownHallLength_z = 5.1*m;

  RockCeilingLength_x = RockMotherVolLength_x;
  RockCeilingLength_y = RockThickness;
  RockCeilingLength_z = RockMotherVolLength_z;
 
  // no space between MRD and Rock for a moment -- 06/13/05 Naho
  RockLength = RockUpstreamLength_z +  RockDownstreamLength_z;

  G4cout << "Rock : RockLength = " << RockLength << G4endl; 

  // position --------------------------
  RockMotherPosition_y = RockMotherVolLength_y/2.-(WCRadius+RockThickness);
  //  if (myConfiguration==1){
  //  RockMotherPosition_z = -RockMotherVolLength_z/2.
  //      +(RockThickness+RockDownHallLength_z+0.4*m+lArDLength+0.1*m+WCLength/2.)+WCPosition;
  //  } else if (myConfiguration==2){ // default
  //JEC 18/11/05 use default as there is no myConfiguration
  RockMotherPosition_z = -RockMotherVolLength_z/2.
    +(RockThickness+RockDownHallLength_z+0.4*m+WCLength/2.)+WCPosition;
    //}
  RockMotherPosition = 
    G4ThreeVector (0, RockMotherPosition_y, RockMotherPosition_z);
  G4cout << " WC Position = " << WCPosition*0.001 << G4endl;

  //---------------

  RockCentralHallPosition_y
    = RockCentralHallLength_y/2.-WCRadius; // default

  //  if (myConfiguration==1){
  //  RockCentralHallPosition_z
  //    = -RockCentralHallLength_z/2.+(0.4*m+lArDLength+0.1*m+WCLength/2.)+WCPosition;
  //} else if (myConfiguration==2){ // default
  //JEC 18/11/05 use default as there is no myConfiguration
  RockCentralHallPosition_z = 
    -RockCentralHallLength_z/2.+(0.4*m+WCLength/2.)+WCPosition;
  //}

  RockCentralHallPosition = 
    G4ThreeVector(0, RockCentralHallPosition_y, RockCentralHallPosition_z);

  // center hall position relative to Mother volume
  RockCenterHallrelativeMomPosition = 
    G4ThreeVector(0, 
		  RockCentralHallPosition_y-RockMotherPosition_y,
		  RockCentralHallPosition_z-RockMotherPosition_z);

  G4double RockUpHallTubePosition_z =0;
  RockUpHallTubePosition_z = 
    RockCentralHallPosition_z 
    - RockCentralHallLength_z/2. 
    - RockUpHallouterRadiusTube 
    + RockUnknownLength
    - 0.1*m; //JEC FIXME what not to use a variable...  
    // 0.1*m is the gap between Up_tube and WC

  RockUpHallTubePosition_y = RockUpHallhightTube/2. - WCRadius;
  RockUpHallTubePosition = 
    G4ThreeVector (0, RockUpHallTubePosition_y, RockUpHallTubePosition_z);

  // TmpBox1 position relative to the UpHallTube position
  RockTmpBox1relativeUpHallTubePosition = 
    G4ThreeVector(0, 
		  -RockUpHallouterRadiusTube - RockTmpBox1Length_y/2.+RockUnknownLength-0.1*m,
		  0); 
  // UpTube position relative to the central Hall position
  RockUpHallTuberelativeCentralHallPosition = 
    G4ThreeVector(0, 
		  RockUpHallTubePosition_y - RockCentralHallPosition_y,
		  RockUpHallTubePosition_z - RockCentralHallPosition_z);
                             
  RockDownHallPosition_y = RockCentralHallPosition_y;
  RockDownHallPosition_z = 
    RockCentralHallPosition_z 
    + RockCentralHallLength_z/2.
    + RockDownHallLength_z/2.;

  RockDownHallPosition = 
    G4ThreeVector(0, RockDownHallPosition_y, RockDownHallPosition_z);

  RockDownHallrelativeCentralHallPosition = 
    G4ThreeVector(0, 
		  RockDownHallPosition_y - RockCentralHallPosition_y,
		  RockDownHallPosition_z - RockCentralHallPosition_z);

  RockDownrelativeMomPosition = 
    G4ThreeVector(0, 
		  RockDownHallPosition_y-RockMotherPosition_y,
		  RockDownHallPosition_z-RockMotherPosition_z);

  G4double RockCeilingPosition_y = 
    RockMotherPosition_y
    + RockMotherVolLength_y/2.
    + RockCeilingLength_y/2.;

  RockCeilingPosition  = G4ThreeVector(0, RockCeilingPosition_y, RockMotherPosition_z);

  RockUpHallrelativeCeilingPosition  = 
    G4ThreeVector(0, 
		  0,
		  RockUpHallTubePosition_z - RockMotherPosition_z);
}//SetRockGeometry

//---------------------------------------------------------------------------------------------

void MEMPHYS::DetectorConstruction::ConstructRock(G4LogicalVolume *logicExpHall, G4bool RockIsVisible) {

  G4Box* RockMotherVol_box
    = new G4Box("RockMotherVol_box",
                RockMotherVolLength_x/2.,
                RockMotherVolLength_y/2.,
                RockMotherVolLength_z/2.);

  G4Box* RockCentralHall_box
    = new G4Box("RockCentralHall_box",
		RockCentralHallLength_x/2.,
		RockCentralHallLength_y/2.,
		RockCentralHallLength_z/2. + 0.5*m);
 
  G4Tubs* RockUpHallTube
    = new G4Tubs("RockUpHallTube",
		 RockUpHallinnerRadiusTube,
		 RockUpHallouterRadiusTube,
		 RockUpHallhightTube/2.,
		 RockUpHallstartAngleTube,
		 RockUpHallspanningAngleTube);

  G4RotationMatrix* RockUpHallTubeRotation = new G4RotationMatrix;
  RockUpHallTubeRotation->rotateX(90.*deg);

  G4Box* RockTmpBox1
    = new G4Box("RockTmpBox1",
                RockTmpBox1Length_x/2.,
                RockTmpBox1Length_y/2.,
                RockTmpBox1Length_z/2.);

  G4SubtractionSolid * RockUpHallTube_TmpBox1
    = new G4SubtractionSolid("RockUpHallTube - TmpBox1", // name
                             RockUpHallTube,// mother
                             RockTmpBox1, // daughter
                             0, // rotate TmpBox1
                             RockTmpBox1relativeUpHallTubePosition);

  G4Box* RockDownHall_box
    = new G4Box("RockDownHall_box",
                RockDownHallLength_x/2.,
                RockDownHallLength_y/2.,
                RockDownHallLength_z/2.);


  G4UnionSolid * RockUpHallTubeUnionCentralHall
    = new G4UnionSolid("Central+(Tube-TmpBox1)",
		       RockCentralHall_box, 
		       RockUpHallTube_TmpBox1,
		       RockUpHallTubeRotation, // rotate tube
		       RockUpHallTuberelativeCentralHallPosition);

  G4SubtractionSolid * RockMomSubtractDownHall
    = new G4SubtractionSolid("Mom - (Down)",
			     RockMotherVol_box, // mother volume
			     RockDownHall_box,
			     0, // rotation
			     RockDownrelativeMomPosition);

  G4SubtractionSolid * RockMomMinusUnion
    = new G4SubtractionSolid("Mom - union",
			     RockMomSubtractDownHall,
			     RockUpHallTubeUnionCentralHall,
			     0,
			     RockCenterHallrelativeMomPosition);

  G4Box* RockCeiling_box
    = new G4Box("RockCeiling_box",
		RockCeilingLength_x/2.,
		RockCeilingLength_y/2.,
		RockCeilingLength_z/2.);

  G4SubtractionSolid * RockCeilingMinusTube
    = new G4SubtractionSolid("Ceiling - Tube",
			     RockCeiling_box,
			     RockUpHallTube_TmpBox1,
			     RockUpHallTubeRotation,
			     RockUpHallrelativeCeilingPosition);  
  //------------------------
  G4LogicalVolume *RockMother_log
    = new G4LogicalVolume(RockMomMinusUnion,
                          G4Material::GetMaterial("Rock"),
                          "RockMother_log");

  //JEC 18/11/05 not used  G4VPhysicalVolume* RockMother_phys =
  new G4PVPlacement(0,
		    RockMotherPosition,
		    RockMother_log,
		    "RockMother_phys",
		    logicExpHall, false, 0);  
                          
    G4VisAttributes* rockwallVisAtt= new G4VisAttributes(G4Colour(0.5,0.5,0.5));
  if (RockIsVisible) {
    rockwallVisAtt->SetVisibility(true);
  } else {
    rockwallVisAtt->SetVisibility(false);
  }
    RockMother_log->SetVisAttributes(rockwallVisAtt);
  // ----------------

  G4LogicalVolume * RockCeiling_log
    = new G4LogicalVolume(RockCeilingMinusTube,
			  G4Material::GetMaterial("Rock"),
			  "RockCeiling_log");

  //JEC 18/11/05 not used G4VPhysicalVolume* RockCeiling_phys = 
  new G4PVPlacement(0,
		    RockCeilingPosition,
		    RockCeiling_log,
		    "RockCeiling_phys",
		    logicExpHall, false, 0);
  
  RockCeiling_log->SetVisAttributes(rockwallVisAtt);
}//ConstructRock

//---------------------------------------------------------------------------------------------
//JEC 17/11/05 Include the Water Cerenkov Geometry Part
//---------------------------------------------------------------------------------------------

void MEMPHYS::DetectorConstruction::Set20inchPMTGeometry() { //JEC not used but maybe true MF
  // Set the paramaters for 20" tubes

  WCLength           = 16.0*m;
  WCPosition         = 0.0*m;
  WCPMTRadius           = .25*m;  // standard SK 50-cm PMTs
  WCDiameter            = 9.3*m;
  WCRadius              = WCDiameter/2;
  //  WCBarrelPMTBlackSheetDist = 5.0*cm; 
  // distance from PMT back to avoid overlap
  //WCCapPMTBlackSheetDist= 2.0*cm; // distance Cap black sheet to Cap PMTs
  WCCapLength           = 1.0*m;
  WCBackODLength        = 0.7*m;
  WCFrontODLength       = 0.0*m;
  WCBarrelLength        = WCLength - (WCCapLength+WCCapLength+WCBackODLength);
  WCBarrelRingRadius    = WCRadius - 1.0*m;
  WCBarrelPMTRadius     = WCRadius - 0.7*m;
  WCBarrelRingNPhi      = 35;
  WCBarrelRingdPhi      = (360./WCBarrelRingNPhi)*deg;
  WCBarrelNRings        = 19;
  WCBarrelCellLength    = WCBarrelLength/WCBarrelNRings;
  WCCapNCell            = 10;
  WCCapCellSize         = 0.7*m;
  WCCapEdgeWidth        = 1*0.7*m;
  WCCapEdgeLimit        = 4.2*m; // hand picked to fill cap nicely
  WCPMTGlassThickness   = .01*m;
  WCBlackSheetThickness = 0.1*cm;
}//Set20inchPMTGeometry

//---------------------------------------------------------------------------------------------

void MEMPHYS::DetectorConstruction::Set8inchPMTGeometry() { //JEC This is the Default
  // Set the paramaters for 8" tubes

  WCPMTRadius           = 0.10*m;  // 20-cm PMTs (8-inch)
  WCDiameter            = 9.3*m;
  WCRadius              = WCDiameter/2;  //4.65
  WCCapLength           = 0.44*m;
  WCBackODLength        = 0.18*m + .53*m; // .53m is to give proper total
  WCFrontODLength       = 0.00*m + .53*m; // WC length of 13.8m
  WCBarrelLength        = 12.3*m - WCCapLength;

  WCBarrelRingRadius    = WCRadius - 1.0*m; //3.65
  WCBarrelPMTRadius     = WCRadius - 0.4*m; //4.25
  WCBarrelRingNPhi      = 102;
  WCBarrelRingdPhi      = (360./WCBarrelRingNPhi)*deg;
  WCBarrelNRings        = 42;
  WCBarrelCellLength    = WCBarrelLength/WCBarrelNRings;
  WCCapNCell            = 32;
  WCCapCellSize         = 0.28*m;
  WCCapEdgeWidth        = 4*0.28*m;
  WCCapEdgeLimit        = 4.38*m; // hand picked to fill cap nicely

  WCPMTGlassThickness   = .5*cm;

  WCBlackSheetThickness = 2.0*cm; // limit the photon leaks...

  WCLength =  WCBarrelLength + 2.0*WCCapLength + WCBackODLength + WCFrontODLength;
  WCPosition = (WCBackODLength - WCFrontODLength)/2.0; //JEC This impose WC center at 0,0,0

}//Set8inchPMTGeometry

void MEMPHYS::DetectorConstruction::Set12inchPMTGeometry() { //JEC 26/1/06 MEMPHYS
  // Set the paramaters for 12" tubes 
  
  G4double WCVetoShieldLength = 2.0*m; //JEC 26/1/06 new variable to be put elsewhere

  WCDiameter            = 65.0*m;
  WCRadius              = WCDiameter/2;
  WCCapLength           = 0.44*m; //JEC FIXME
  WCBackODLength        = WCVetoShieldLength - WCCapLength/2; // JEC FIX this are the veto shield
  WCFrontODLength       = WCBackODLength;                     // JEC FIX   "
  WCLength              = 65.0*m; 
  WCBarrelLength        = WCLength - 2*WCCapLength - WCBackODLength - WCFrontODLength;

  WCPMTRadius           = 0.15*m;  //JEC FIXME for 12-inchs PMTs (PhotoCathodCoverageRadius = 14cm)

  WCBarrelPMTRadius     = WCRadius - WCVetoShieldLength;
  WCBarrelRingRadius    = WCBarrelPMTRadius - 0.6*m; //JEC FIX ME

  G4double WCInnerCoverage = 0.30; //JEC 26/1/06
  G4double WCPMTPhotoCathodeSurface = 615.0*cm2; 
  G4double WCCellSurface = WCPMTPhotoCathodeSurface/WCInnerCoverage;
  G4double cellApproxSideLength = sqrt(WCCellSurface);
  WCBarrelNRings        = int(floor(WCBarrelLength/cellApproxSideLength));
  WCBarrelCellLength    = WCBarrelLength/WCBarrelNRings; //JEC FIXME more precise name required
  WCBarrelRingNPhi      = int(floor(2.0 * pi * WCBarrelPMTRadius/cellApproxSideLength));
  WCBarrelRingdPhi      = (360./WCBarrelRingNPhi)*deg;

  G4cout << " (JEC) Set12inchPMTGeometry DUMP:\n"
	 << " WC Total Z (m) " <<  WCLength/m 
	 << " WC Total R (m) " << WCRadius/m
	 << " Cap Z  (m) " <<  WCCapLength/m
	 << " Veto Total (m) " << WCVetoShieldLength/m
	 << " Barrel Z (m) " << WCBarrelLength/m
	 << " Barrel PMT R (m) " << WCBarrelPMTRadius/m
	 << "\n Cov (%)= " << WCInnerCoverage*100.
	 << " Surf Cath (cm2) " << WCPMTPhotoCathodeSurface/cm2
	 << " Cell Surf (cm2) " << WCCellSurface/cm2
	 << "\n Cell Approx Length (m) " << cellApproxSideLength/m
	 << " Real Cell Z (m) " <<  WCBarrelCellLength/m
	 << " rPhi at PMTsurf (m) " << (2.0 * pi * WCBarrelPMTRadius/WCBarrelRingNPhi)/m
	 << "\n Nz " << WCBarrelNRings
	 << " Nphi " << WCBarrelRingNPhi
	 << " N Barrel PMTs = " << WCBarrelNRings*WCBarrelRingNPhi
	 << G4endl;

  WCBlackSheetThickness = 2.0*cm; // limit the photon leaks...
  WCCapCellSize         = cellApproxSideLength;
  WCCapNCell            = int(floor(2.0*WCBarrelPMTRadius/WCCapCellSize));

  //JEC NOT USED!!!  WCCapEdgeWidth        = 4*0.28*m;
  G4double extra        = 0.15*m; //JEC FIXME !!!
  WCCapEdgeLimit        = WCBarrelPMTRadius + WCBlackSheetThickness + extra; // hand picked to fill cap nicely

  G4cout << "WCCapEdgeLimit (m) " << WCCapEdgeLimit/m
	 << G4endl;

  WCPMTGlassThickness   = 0.5*cm; //JEC FIXME (ask Joel P)
  WCPosition = (WCBackODLength - WCFrontODLength)/2.0; //JEC This impose WC center at 0,0,0

}//Set8inchPMTGeometry

//---------------------------------------------------------------------------------------------

G4LogicalVolume* MEMPHYS::DetectorConstruction::ConstructWC() {

  //--------------
  // Positions
  //--------------

  G4ThreeVector WCFrontODPosition =
    G4ThreeVector(0.,0., - WCLength/2 + WCFrontODLength/2);

  G4ThreeVector WCFrontCapPosition =
    G4ThreeVector(0.,
		  0.,
		  WCFrontODPosition[2] + WCFrontODLength/2 + WCCapLength/2);

  G4ThreeVector WCBarrelPosition =
    G4ThreeVector(0.,
		  0.,
		  WCFrontCapPosition[2] + WCCapLength/2 +  WCBarrelLength/2);

  G4ThreeVector WCBackCapPosition =
    G4ThreeVector(0.,
		  0.,
		  WCBarrelPosition[2] + WCBarrelLength/2 + WCCapLength/2);

  G4ThreeVector WCBackODPosition =
    G4ThreeVector(0.,
		  0.,
		  WCBackCapPosition[2] + WCCapLength/2 + WCBackODLength/2);


  //G4double GAP = 20.0e-3*mm ; //set to 20 um
  G4double GAP = 0.0000000000000000*mm ; //set to 0 um JEC FIXME 0 or not 0?
  //--------------
  // Volumes
  //--------------

  //--------------
  // Water Cherenkov Detector (WC) mother volume
  //--------------

  // WC Box, nice to turn on for x and y views to provide a frame:

  G4Box* solidWCBox = new G4Box("solidWCBox",
				WCRadius,
				WCRadius,
				WCLength/2);
  
  G4LogicalVolume* logicWCBox = 
    new G4LogicalVolume(solidWCBox,
			G4Material::GetMaterial("Air"),
			"WCBox",
			0,0,0);
  
  //   logicWCBox->SetVisAttributes(G4VisAttributes::Invisible);

  // Cylindrical water Cherenkov tank

  G4Tubs* solidWC = new G4Tubs("WC",
			       0.0*m,
			       WCRadius,
			       .5*WCLength,
			       0.*deg,
			       360.*deg);
  
  G4LogicalVolume* logicWC = 
    new G4LogicalVolume(solidWC,
			G4Material::GetMaterial("Water"),
			"WC",
			0,0,0);
  
  //JEC 22/11/05 G4VPhysicalVolume* physiWC = 
  new G4PVPlacement(0,
		    G4ThreeVector(0.,0.,0.),
		    logicWC,
		    "WC",
		    logicWCBox,
		    false,
		    0);
  
  //JEC FIXME logicWC->SetVisAttributes(G4VisAttributes::Invisible);
  
  //-----------------------------------------------------
  // z-Subdivsions filling Water Cherenkov Detector
  //-----------------------------------------------------

  G4Tubs* solidWCCap = new G4Tubs("WCCap",
				  0.0*m,
				  WCRadius,
				  .5*WCCapLength,
				  0.*deg,
				  360.*deg);
  
  G4LogicalVolume* logicWCCap = 
    new G4LogicalVolume(solidWCCap,
			G4Material::GetMaterial("Water"),
			"WCCap",
			0,0,0);
  
  //JEC 22/11/05 G4VPhysicalVolume* physiWCFrontCap = 
  new G4PVPlacement(0,
		    WCFrontCapPosition,
		    logicWCCap,
		    "WCFrontCap",
		    logicWC,
		    false,
		    0);
  

  // The back cap is duplicate and a rotation of the front cap,
  // however it was defined.

  G4RotationMatrix* WCBackCapRotation = new G4RotationMatrix;
  WCBackCapRotation->rotateX(180.0*deg);

  //JEC 22/11/05 G4VPhysicalVolume* physiWCBackCap = 
  new G4PVPlacement(WCBackCapRotation,
		    WCBackCapPosition,
		    logicWCCap,
		    "WCBackCap",
		    logicWC,
		    false,
		    1);
  
  //JECFIXME  logicWCCap->SetVisAttributes(G4VisAttributes::Invisible);

  G4Tubs* solidWCBarrel = new G4Tubs("WCBarrel",
				     0.0*m,
				     WCRadius,
				     .5*WCBarrelLength,
				     0.*deg,
				     360.*deg);
  
  G4LogicalVolume* logicWCBarrel = 
    new G4LogicalVolume(solidWCBarrel,
			G4Material::GetMaterial("Water"),
			"WCBarrel",
			0,0,0);
  
  //JEC 22/11/05 G4VPhysicalVolume* physiWCBarrel = 
  new G4PVPlacement(0,
		    WCBarrelPosition,
		    logicWCBarrel,
		    "WCBarrel",
		    logicWC,
		    false,
		    0);
  
  //   logicWCBarrel->SetVisAttributes(G4VisAttributes::Invisible);

  if (WCBackODLength != 0.0*m) {
    G4Tubs* solidWCBackOD = new G4Tubs("WCBackOD",
				       0.0*m,
				       WCRadius,
				       .5*WCBackODLength,
				       0.*deg,
				       360.*deg);
    
    G4LogicalVolume* logicWCBackOD = 
      new G4LogicalVolume(solidWCBackOD,
			  G4Material::GetMaterial("Water"),
			  "WCBackOD",
			  0,0,0);
    
    //JEC 22/11/05    G4VPhysicalVolume* physiWCBackOD =
    new G4PVPlacement(0,
		      WCBackODPosition,
		      logicWCBackOD,
		      "WCBackOD",
		      logicWC,
		      false,
		      0);
    
    //logicWCBackOD->SetVisAttributes(G4VisAttributes::Invisible);
    G4VisAttributes* greenColor = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
    logicWCBackOD->SetVisAttributes(greenColor);
  }//Back OD

  if (WCFrontODLength != 0.0*m) {
    G4Tubs* solidWCFrontOD = new G4Tubs("WCFrontOD",
					0.0*m,
					WCRadius,
					.5*WCFrontODLength,
					0.*deg,
					360.*deg);
    
    G4LogicalVolume* logicWCFrontOD = 
      new G4LogicalVolume(solidWCFrontOD,
			  G4Material::GetMaterial("Water"),
			  "WCFrontOD",
			  0,0,0);
    
    //JEC 22/11/05  G4VPhysicalVolume* physiWCFrontOD = 
    new G4PVPlacement(0,
		      WCFrontODPosition,
		      logicWCFrontOD,
		      "WCFrontOD",
		      logicWC,
		      false,
		      0);
    
    //JEC logicWCFrontOD->SetVisAttributes(G4VisAttributes::Invisible);
    G4VisAttributes* greenColor = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
    logicWCFrontOD->SetVisAttributes(greenColor);
  }//Front OD


  //-----------------------------------------------------
  // Form annular section of barrel to hold PMTs 
  // Center remains a solid cylinder of water.
  //-----------------------------------------------------

  G4Tubs* solidWCBarrelAnnulus = new G4Tubs("WCBarrelAnnulus",
					    WCBarrelRingRadius,
					    WCRadius,
					    .5*WCBarrelLength,
					    0.*deg,
					    360.*deg);
  
  G4LogicalVolume* logicWCBarrelAnnulus = 
    new G4LogicalVolume(solidWCBarrelAnnulus,
			G4Material::GetMaterial("Water"),
			"WCBarrelAnnulus",
			0,0,0);
  
  //JEC 22/11/05 G4VPhysicalVolume* physiWCBarrelAnnulus = 
  new G4PVPlacement(0,
		    G4ThreeVector(0.,0.,0.),
		    logicWCBarrelAnnulus,
		    "WCBarrelAnnulus",
		    logicWCBarrel,
		    false,
		    0);
  
  logicWCBarrelAnnulus->SetVisAttributes(G4VisAttributes::Invisible);

  //-----------------------------------------------------
  // Subdivide the BarrelAnnulus into rings
  //-----------------------------------------------------

  G4Tubs* solidWCBarrelRing = new G4Tubs("WCBarrelRing",
					 WCBarrelRingRadius,
					 WCRadius,
					 .5*WCBarrelCellLength,
					 0.*deg,
					 360.*deg);
  
  G4LogicalVolume* logicWCBarrelRing = 
    new G4LogicalVolume(solidWCBarrelRing,
			G4Material::GetMaterial("Water"),
			"WCBarrelRing",
			0,0,0);
  
  //JEC 22/11/05 G4VPhysicalVolume* physiWCBarrelRing = 
  new G4PVReplica("WCBarrelRing",
		  logicWCBarrelRing,
		  logicWCBarrelAnnulus,
		  kZAxis,
		  WCBarrelNRings,
		  WCBarrelCellLength);
  
  logicWCBarrelRing->SetVisAttributes(G4VisAttributes::Invisible);

  //-----------------------------------------------------
  // Subdivisions of the BarrelRing are cells
  //-----------------------------------------------------

  G4Tubs* solidWCBarrelCell = new G4Tubs("WCBarrelCell",
					 WCBarrelRingRadius,
					 WCRadius,
					 .5*WCBarrelCellLength,
					 -WCBarrelRingdPhi/2.,
					 WCBarrelRingdPhi);
  
  G4LogicalVolume* logicWCBarrelCell = 
    new G4LogicalVolume(solidWCBarrelCell,
			G4Material::GetMaterial("Water"),
			"WCBarrelCell",
			0,0,0);
  
  G4VPhysicalVolume* physiWCBarrelCell = 
    new G4PVReplica("WCBarrelCell",
		    logicWCBarrelCell,
		    logicWCBarrelRing,
		    kPhi,
		    WCBarrelRingNPhi,
		    WCBarrelRingdPhi);
  
//   //JEC logicWCBarrelCell->SetVisAttributes(G4VisAttributes::Invisible);
//   G4VisAttributes* tmpVisAtt = new G4VisAttributes(G4Colour(1.,0.5,0.5));
//   tmpVisAtt->SetForceWireframe(true);
//   logicWCBarrelCell->SetVisAttributes(tmpVisAtt);
  
  //---------------------------------------------------------
  // CapPolygon is a volume that contains CapCells.
  // This volume is  now an Nphi-gon
  // CAUTION : WCCapEdgeLimit must be > WCBarrelPMTRadius + WCBlackSheetThickness 
  //        M Fechner
  //---------------------------------------------------------

  
  double tmpZ[2] = { -0.5*WCCapLength  ,  0.5*WCCapLength} ;
  double tmpRmin[2] = {  0. , 0.} ;
  double tmpRmax[2] = { WCBarrelPMTRadius + WCBlackSheetThickness +GAP  ,  WCBarrelPMTRadius + WCBlackSheetThickness +GAP} ;

  G4Polyhedra* solidWCCapPolygon
    = new G4Polyhedra("WCCapPolygon",
		      0.*deg, // phi start
		      360.*deg, //phi end
		      WCBarrelRingNPhi, //NPhi-gon
		      2, // 2 z-planes
		      tmpZ, //position of the Z planes
		      tmpRmin, // min radius at the z planes
		      tmpRmax// max radius at the Z planes
		      );
  
  
  G4LogicalVolume* logicWCCapPolygon = 
    new G4LogicalVolume(solidWCCapPolygon,
			G4Material::GetMaterial("Water"),
			"WCCapPolygon",
			0,0,0);

//JEC no more visAtt necessary
//   G4VisAttributes* tmpVisAtt2 = new G4VisAttributes(G4Colour(.6,0.5,0.5));
//   tmpVisAtt2->SetForceWireframe(true);
//   logicWCCapPolygon->SetVisAttributes(tmpVisAtt2);
// logicWCCapPolygon->SetVisAttributes(G4VisAttributes::Invisible);

  G4VPhysicalVolume* physiWCCapPolygon =
    new G4PVPlacement(0,                           // no rotation
		      G4ThreeVector(0.,0.,0.),     // its position
		      logicWCCapPolygon,          // its logical volume
		      "WCCapPolygon",             // its name
		      logicWCCap,                  // its mother volume
		      false,                       // no boolean operations
		      0);                          // Copy #


  //---------
  // The PMT
  //---------

  // TODO : Add a cathode

  G4Sphere* solidWCPMT = new G4Sphere("WCPMT",
				      0.0*m,WCPMTRadius-WCPMTGlassThickness,
				      0.0*deg,360.0*deg,
				      0.0*deg,90.0*deg);

  G4Sphere* glassWCPMT = new G4Sphere("glassFaceWCPMT",
                                      (WCPMTRadius-WCPMTGlassThickness),
				      WCPMTRadius,
                                      0.0*deg,360.0*deg,
                                      0.0*deg,90.0*deg);

  G4LogicalVolume* logicWCPMT = 
    new G4LogicalVolume(solidWCPMT,
			G4Material::GetMaterial("Air"),
			"WCPMT",
			0,0,0);

  G4LogicalVolume* logicGlassFaceWCPMT =
    new G4LogicalVolume(glassWCPMT,
                        G4Material::GetMaterial("Glass"),
                        "glassFaceWCPMT",
                        0,0,0);


  //-----------------------------------------------------------
  // The Blacksheet, a daughter of the cells containing PMTs,
  // and also some other volumes to make the edges light tight
  //-----------------------------------------------------------


  // M Fechner : change to put the BS flush behind the tubes.
  G4ThreeVector capBSpos = 
    G4ThreeVector(0.0, 
		  0.0, 
		  -WCBlackSheetThickness*0.5-GAP);

  //-----------------------------------------------------
  // PMT placement within the CapQuadrant volume.
  // Start in lower lh corner (0,0) and step out
  // in x and y checking against radial boundary
  // WCCapEdgeLimit (which is the number to tweak
  // for a new geometry). The other quadrants
  // will be made by duplication and rotation. (ETK)
  //-----------------------------------------------------

  G4double xoffset;
  G4double yoffset;
  G4int    icopy = 0;

  // loop over the cells
  for ( int i = -WCCapNCell/2 ; i <  WCCapNCell/2; i++) {
    for (int j = -WCCapNCell/2; j <  WCCapNCell/2; j++)   {


      // check all the corners to see if they are within proper radius
       
      xoffset = i*WCCapCellSize + WCCapCellSize*0.5;
      yoffset = j*WCCapCellSize + WCCapCellSize*0.5;

      G4double rmax = 0;
      for ( int k = 0 ; k <= 1 ; k++) {
	for (int l = 0 ; l <= 1 ; l++) {
	  G4double xhi = xoffset + (2*k-1)*WCCapCellSize/2;
	  G4double yhi = yoffset + (2*l-1) *WCCapCellSize/2;
	  G4double rhi = sqrt(xhi*xhi + yhi*yhi);
	  if ( rhi > rmax )  rmax = rhi;
	}
      }
      
      G4ThreeVector cellpos = G4ThreeVector(xoffset, yoffset, 0.0);      
      double comp = xoffset*xoffset + yoffset*yoffset 
	- 2.0 * WCBarrelPMTRadius * sqrt(xoffset*xoffset+yoffset*yoffset)
	+ WCBarrelPMTRadius*WCBarrelPMTRadius;
      if ( (comp > WCPMTRadius*WCPMTRadius) && (rmax < WCCapEdgeLimit) ) {

	G4VPhysicalVolume* physiCapPMT =
	  new G4PVPlacement(0,                         // no rotation
			    cellpos,                   // its position
			    logicWCPMT,                // its logical volume
			    "WCCapPMT",                // its name 
			    logicWCCapPolygon,         // its mother volume
			    false,                     // no boolean os
			    icopy);                    // no particular field
	
	G4VPhysicalVolume* physiCapPMTGlass =
	  new G4PVPlacement(0,                         // no rotation
			    cellpos,                   // its position
			    logicGlassFaceWCPMT,       // its logical volume
			    "WCCapPMTGlass",           // its name 
			    logicWCCapPolygon,         // its mother volume
			    false,                     // no boolean os
			    icopy);                    // no particular field
	//JEC 22/11/05 G4LogicalBorderSurface *  GlassCathodeCapSurface =
	new G4LogicalBorderSurface("GlassCathodeCapSurface",
				   physiCapPMTGlass,physiCapPMT,
				   OpGlassCathodeSurface);

	icopy++;
      }//valid cell
    }//end 1st loop on cells 
  }//end 2nd loop on cells
  G4cout << "(JEC) ConstructWC Nber of Cap Cells = " << icopy << G4endl;
  //  exit(0);


  G4double WCBS_Radius = WCBarrelPMTRadius + 0.5*WCBlackSheetThickness + GAP;
   
  double gap = 0.00*mm;
  G4Trd* solidWCBarrelCellBlackSheet = new 
    G4Trd("WCBarrelCellBlackSheet",
	  (WCBarrelPMTRadius+gap)*tan(WCBarrelRingdPhi/2.), // x/2 at -dz
	  (WCBarrelPMTRadius+gap+WCBlackSheetThickness)*tan(WCBarrelRingdPhi/2.), // x/2 at +dz
	  0.5*WCBarrelCellLength,  // y/2 at -dz
	  0.5*WCBarrelCellLength,  // y/2 at +dz
	  WCBlackSheetThickness*0.5 //dz
	  );
  
  G4RotationMatrix* WCBSRotM = new G4RotationMatrix;
  WCBSRotM->rotateY(-90.*deg);
  WCBSRotM->rotateZ(-90.*deg);
  
  G4LogicalVolume* logicWCBarrelCellBlackSheet =
    new G4LogicalVolume(solidWCBarrelCellBlackSheet,
			G4Material::GetMaterial("Blacksheet"),
			"WCBarrelCellBlackSheet",
			  0,0,0);
  
  
  
  G4VPhysicalVolume* physiWCBarrelCellBlackSheet =
    new G4PVPlacement(WCBSRotM,
		      G4ThreeVector(WCBS_Radius,0.,0.),
		      logicWCBarrelCellBlackSheet,
		      "WCBarrelCellBlackSheet",
		      logicWCBarrelCell,
		      false,
		      0);
   

  //JEC 22/11/05 G4LogicalBorderSurface * WaterBSBarrelCellSurface =
  new G4LogicalBorderSurface("WaterBSBarrelCellSurface",
			     physiWCBarrelCell,
			     physiWCBarrelCellBlackSheet, 
			     OpWaterBSSurface);
  
  // CapBarrel - union of  the cap and the top of the barrel
  // M Fechner : the BS has to be light tight.
  // let's make it a N-gon to match the barrel cell structure
  // Using the G4 polyhedra it probably could be done using only one solid
    
  G4double WCBSradmin = WCBarrelPMTRadius + GAP;
  G4double WCBSradmax = WCBSradmin + WCBlackSheetThickness ;
  
  
  G4double WCC_BBS_Z[2] = {  0.0 ,  0.5*WCCapLength  };
  G4double WCC_BBS_RMIN[2] = {WCBSradmin,WCBSradmin };
  G4double WCC_BBS_RMAX[2] = { WCBSradmax,WCBSradmax };
  
  G4double WCCBS_Z[2] = {  -WCBlackSheetThickness ,  0.0 };
  G4double WCCBS_RMIN[2] = {0.0,0.0 };
  G4double WCCBS_RMAX[2] = { WCBSradmax,WCBSradmax };
  
 // Cap overall (superseded by cells, but this fills in the rest)
  
  G4Polyhedra* solidWCCapBlackSheet
    = new G4Polyhedra("WCCapBlackSheet",
		      0.*deg, // phi start
		      360.*deg, //phi end
		      WCBarrelRingNPhi, //NPhi-gon
		      2, // 2 z-planes
		      WCCBS_Z, //position of the Z planes
		      WCCBS_RMIN, // min radius at the z planes
		      WCCBS_RMAX // max radus at the Z planes
		      );
  
  
  G4LogicalVolume* logicWCCapBlackSheet =
    new G4LogicalVolume(solidWCCapBlackSheet,
			G4Material::GetMaterial("Blacksheet"),
			"WCCapBlackSheet",
			0,0,0);
  
  G4VPhysicalVolume* physiWCCapBlackSheet =
    new G4PVPlacement(0,
		      G4ThreeVector(0.,0.,0.),
		      logicWCCapBlackSheet,
		      "WCCapBlackSheet",
		      logicWCCapPolygon,
		      false,
		      0);
  
  //JEC 22/11/05 G4LogicalBorderSurface * WaterBSCapPolySurface =
  new G4LogicalBorderSurface("WaterBSCapPolySurface",
			     physiWCCapPolygon,physiWCCapBlackSheet,
			     OpWaterBSSurface);
  
    //JEC    G4cout << *solidWCCapBlackSheet << "\n";
    
    // Cap Barrel : defined as an intersection
    // There may be something simpler 
    

  G4Polyhedra* solidWCCapBarrelBlackSheet
    = new G4Polyhedra("WCCapBarrelBlackSheet",
		      0.*deg, // phi start
		      360.*deg, //phi end
		      WCBarrelRingNPhi, //NPhi-gon
		      2, // 2 z-planes
		      WCC_BBS_Z, //position of the Z planes
		      WCC_BBS_RMIN, // min radius at the z planes
		      WCC_BBS_RMAX // max radus at the Z planes
		      );
  
  //JEC G4cout << *solidWCCapBarrelBlackSheet << "\n";
  
  
  G4LogicalVolume* logicWCCapBarrelBlackSheet =
    new G4LogicalVolume(solidWCCapBarrelBlackSheet,
			G4Material::GetMaterial("Blacksheet"),
			"WCCapBarrelBlackSheet",
			  0,0,0);
    
  G4VPhysicalVolume* physiWCCapBarrelBlackSheet =
    new G4PVPlacement(0,
		      G4ThreeVector(0.,0.,0.),
		      logicWCCapBarrelBlackSheet,
		      "WCCapBarrelBlackSheet",
		      logicWCCapPolygon,  // placed in the Cap, not the Quadrant !!
		      false,                     // no boolean operations
		      0);                        // no particular field
  

  //JEC 22/11/05 G4LogicalBorderSurface * WaterBSCapBarrelPolySurface =
  new G4LogicalBorderSurface("WaterBSCapBarrelPolySurface",
			     physiWCCapPolygon,physiWCCapBarrelBlackSheet,
			     OpWaterBSSurface);
  
    // this code section can be uncommented to check geometries:
   
//     G4VisAttributes* WCCapBlackSheetVisAtt = new G4VisAttributes(G4Colour(0.9,0.2,0.2));
//     WCCapBlackSheetVisAtt->SetForceWireframe(true);
//     logicWCCapBlackSheet->SetVisAttributes(WCCapBlackSheetVisAtt);
    //JEC logicWCCapBlackSheet->SetVisAttributes(G4VisAttributes::Invisible);
    

    
//     G4VisAttributes* WCBarrelBlackSheetCellVisAtt = new G4VisAttributes(G4Colour(0.2,0.9,0.2));
//     WCBarrelBlackSheetCellVisAtt->SetForceWireframe(true);
//     logicWCBarrelCellBlackSheet->SetVisAttributes(WCBarrelBlackSheetCellVisAtt);
    //JEC logicWCBarrelCellBlackSheet->SetVisAttributes(G4VisAttributes::Invisible);
      
//     G4VisAttributes* WCCapBarrelBlackSheetVisAtt = new G4VisAttributes(G4Colour(0.1,0.1,0.9));
//     WCCapBarrelBlackSheetVisAtt->SetForceWireframe(true);
//     logicWCCapBarrelBlackSheet->SetVisAttributes(WCCapBarrelBlackSheetVisAtt);
    //JEC logicWCCapBarrelBlackSheet->SetVisAttributes(G4VisAttributes::Invisible);
    

    //JEC logicWCCapBlackSheet->SetVisAttributes(G4VisAttributes::Invisible);
    //JEC logicWCBarrelCellBlackSheet->SetVisAttributes(G4VisAttributes::Invisible);
    //JEC logicWCCapBarrelBlackSheet->SetVisAttributes(G4VisAttributes::Invisible);
    
    
    // M Fechner : usual Barrel PMT placement
      
    G4RotationMatrix* WCPMTRotation = new G4RotationMatrix;
    WCPMTRotation->rotateY(90*deg);


    G4VPhysicalVolume* physiWCBarrelPMT =
      new G4PVPlacement(WCPMTRotation,                          // its rotation
			G4ThreeVector(WCBarrelPMTRadius,0.,0.), // its position
			logicWCPMT,                // its logical volume
			"WCBarrelPMT",             // its name
			logicWCBarrelCell,         // its mother volume
			false,                     // no boolean operations
			0);                        // no particular field
  
  G4VPhysicalVolume* physiWCBarrelPMTGlass =
    new G4PVPlacement(WCPMTRotation,                          // its rotation
		      G4ThreeVector(WCBarrelPMTRadius,0.,0.), // its position
		      logicGlassFaceWCPMT,                // its logical volume
		      "WCBarrelPMTGlass",             // its name
		      logicWCBarrelCell,         // its mother volume
		      false,                     // no boolean operations
		      0);                        // no particular field

  //JEC 22/11/05 G4LogicalBorderSurface *  GlassCathodeBarrelSurface =
  new G4LogicalBorderSurface("GlassCathodeBarrelSurface",
			     physiWCBarrelPMTGlass,physiWCBarrelPMT,
			     OpGlassCathodeSurface);



//   G4VisAttributes* PMTVisAtt = new G4VisAttributes(G4Colour(0.2,0.2,0.2)); 
//   PMTVisAtt->SetForceSolid(true);
//   logicWCPMT->SetVisAttributes(PMTVisAtt);
//   logicGlassFaceWCPMT->SetVisAttributes(PMTVisAtt);


  //JEC logicWCPMT->SetVisAttributes(G4VisAttributes::Invisible);
  //JEC logicGlassFaceWCPMT->SetVisAttributes(G4VisAttributes::Invisible); 

  // Make sensitive detectors

  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  if (!aWCPMT) {
    //JEC 25/1/06 add the "this" pointer, so WCSD can access to the PMT location wo static functions
    aWCPMT = new WCSD( "/MEMPHYS/WCPMT", this ); 
    SDman->AddNewDetector( aWCPMT );
  }
  logicWCPMT->SetSensitiveDetector( aWCPMT );

  return logicWCBox;
}//ConstructWC


//---------------------------------------------------------------------------------------------

// These routines are object registration routines that you can pass
// to the traversal code.

void MEMPHYS::DetectorConstruction::PrintGeometryTree(G4VPhysicalVolume* aPV ,
						    int aDepth, 
						    int replicaNo, 
						    const G4Transform3D& aTransform) {
  for (int levels = 0; levels < aDepth; levels++) G4cout << " ";
  G4cout << aPV->GetName() << " Level:" << aDepth << " replica[" << replicaNo << "] "
	 << " Pos:" << aTransform.getTranslation() 
	 << " Rot:" << aTransform.getRotation().getTheta()/deg 
	 << "," << aTransform.getRotation().getPhi()/deg 
	 << "," << aTransform.getRotation().getPsi()/deg
	 << G4endl;
}//PrintGeometryTree

//---------------------------------------------------------------------------------------------

void MEMPHYS::DetectorConstruction::GetWCGeom(G4VPhysicalVolume* aPV ,
					    int aDepth, 
					    int /*replicaNo*/, 
					    const G4Transform3D& aTransform) {
  
  //JEC 17/11/05 keep the phylosophy (and the comments) 
  //             but use  "if/else-if"  structure and not independant "if" tests

  // Grab mishmash of useful information from the tree while traversing it
  // This information will later be written to the geometry file
  // (Alternatively one might define accessible constants)

  // Note WC can be off-center... get both extremities
  static G4float zmin=100000,zmax=-100000.;
  if (aDepth == 0) { // Reset for this traversal
    zmin=100000,zmax=-100000.; 
  }

  std::string physVolumeName = aPV->GetName();  //JEC 17/11/05 call GetName Once

  if ((physVolumeName == "WCBarrel")) {
    // Stash info in data member
    WCOffset = G4ThreeVector(aTransform.getTranslation().getX()/cm,
			     aTransform.getTranslation().getY()/cm,
			     aTransform.getTranslation().getZ()/cm);
  } else  if ( (physVolumeName == "WCCapPMTGlass") || (physVolumeName == "WCBarrelPMTGlass") ) {
    // Assume all the PMTs are the same size
    G4Sphere* solidWCPMT = (G4Sphere *)aPV->GetLogicalVolume()->GetSolid();
    G4double outerRadius = solidWCPMT->GetOuterRadius()/cm;

    // Stash info in data member
    WCPMTSize = outerRadius;
  } else if ( (physVolumeName == "WCBarrelCellBlackSheet") ) {
    
    // M Fechner : it is no longer a cylinder
    G4double innerRadius = sqrt(  pow(aTransform.getTranslation().getX()/cm,2) 
				+ pow(aTransform.getTranslation().getY()/cm,2));
    // Stash info in data member
    WCCylInfo[0] = innerRadius;
  } else if ( (physVolumeName == "WCCapBlackSheet") ) {
    
    G4float z =  aTransform.getTranslation().getZ()/cm;
    if ( z < zmin ) {
      zmin = z;
    } else if ( z > zmax ) {  //JEC FIXME original code:  if (z>zmin){zmax=z;} 
      zmax = z;
    }
   
    WCCylInfo[1] = zmax-zmin;
    G4cout << "DetectorConstruction::GetWCGeom: "  
	   << zmin << "  " << zmax << G4endl; //JEC FIXME: too verbose ?
  }//eoif  physVolumeName test

}//GetWCGeom

//---------------------------------------------------------------------------------------------

void MEMPHYS::DetectorConstruction::DescribeAndRegisterPMT(G4VPhysicalVolume* aPV ,int aDepth, int replicaNo,
							 const G4Transform3D& aTransform) {
  static std::string replicaNoString[10] = {"0","0","0",
					    "0","0","0",
					    "0","0","0","0" };

  std::ostringstream depth;  //JEC 1/12/05 use ostringstream
  depth << replicaNo;
  replicaNoString[aDepth] = depth.str();

  std::string physVolumeName = aPV->GetName();

  //  if ((physVolumeName == "WCCapPMT") || (physVolumeName == "WCBarrelPMT")){
  if ((physVolumeName == "WCCapPMTGlass") || (physVolumeName == "WCBarrelPMTGlass")){ //JEC 15/12/05
    // First increment the number of PMTs in the tank.
    totalNumPMTs++;  
    
    // Put the location of this tube into the location map so we can find
    // its ID later.  It is coded by its tubeTag string.    
    std::string tubeTag(physVolumeName);
    for (int i=0; i <= aDepth; i++) {
      tubeTag += ":";
      tubeTag += replicaNoString[i];
    }
    tubeLocationMap[tubeTag] = totalNumPMTs; //JEC 25/1/06

   
    // Record where tube is in the cylinder
    //    if (physVolumeName == "WCBarrelPMT"){    
    if (physVolumeName == "WCBarrelPMTGlass"){    //JEC 15/12/05
      tubeCylLocation[totalNumPMTs] = wall;
    } else {   // It's a cap pmt
      // Get which endcap 
      G4int capdepth = 2;
      G4int endcap = std::atoi(replicaNoString[capdepth].c_str());
      // If the cap is replica 1, it's endcap1.  If replica 0, it's endcap2
      tubeCylLocation[totalNumPMTs] = (endcap ? endcap1: endcap2);
    }
    
    // Put the transform for this tube into the map keyed by its ID
    tubeIDMap[totalNumPMTs] = aTransform;
    
    // Print
    //     G4cout << "(JEC) Describe Register Input: " << aDepth << " replica # " << replicaNo 
    // 	   << " size of the map " << tubeLocationMap.size() 
// 	   << " max_size  " << tubeLocationMap.max_size() 
// 	   << G4endl;
     //       G4cout <<  "depth " << depth.str().c_str() << std::endl;
//     std::map<std::string, int>::iterator iter; 
//     for (iter = tubeLocationMap.begin(); iter != tubeLocationMap.end() ; ++iter) {
//       G4cout << "Tube [" << (*iter).second << "] has tag <" << (*iter).first << ">"
// 	     << G4endl;
//     }
//     G4cout << "tubeLocationmap[" << tubeTag.c_str()  << "]= " << tubeLocationMap[tubeTag]
// 	   << G4endl;
//     G4int verif = GetTubeID(tubeTag);
//     G4cout << "DConst Verif " << verif << G4endl;

//    G4cout << "tubeCylLocation[" << totalNumPMTs  << "]= " << tubeCylLocation[totalNumPMTs] 
//	   << G4endl;
    
//     G4cout << "(JEC) Register Tube: "<<std::setw(6) << totalNumPMTs << " " << tubeTag.c_str()
//     	   << " Pos:" << aTransform.getTranslation()/cm 
//     	   << " Rot:" << aTransform.getRotation().getTheta()/deg 
//     	   << "," << aTransform.getRotation().getPhi()/deg 
//     	   << "," << aTransform.getRotation().getPsi()/deg
//     	   << G4endl; 
  }//eo if physVolumeName

}//DescribeAndRegisterPMT


//---------------------------------------------------------------------------------------------

// Utilities to do stuff with the info we have found.
// Output to WC geometry text file
void MEMPHYS::DetectorConstruction::DumpGeometryTableToFile() {
  // Open a file
  std::ofstream geoFile;  
  geoFile.open("geofile.txt", std::ios::out);

  geoFile.precision(2);
  geoFile.setf(std::ios::fixed);

  geoFile << std::setw(8)<< WCCylInfo[0]<<" "<<std::setw(8)<<WCCylInfo[1]
	  <<std::setw(6)<<totalNumPMTs<< std::setw(8)<<WCPMTSize<<G4endl;

  geoFile << std::setw(8)<< WCOffset(0)<< std::setw(8)<<WCOffset(1)<<
    std::setw(8) << WCOffset(2)<<G4endl;


  // Grab the tube information from the tubeID Map and dump to file.
  
  G4Transform3D newTransform;
  G4Vector3D nullOrient; 
  G4Vector3D pmtOrientation;
  cyl_location cylLocation;

  for ( int tubeID = 1; tubeID <= totalNumPMTs; tubeID++){
    newTransform = tubeIDMap[tubeID];
    // Get tube orientation vector
    nullOrient     = G4Vector3D(0,0,1);
    pmtOrientation = newTransform * nullOrient;
    cylLocation    = tubeCylLocation[tubeID];

// M Fechner : otherwise we have unnormalized vectors in dxyzpm
// => geometry problems in polfit
    geoFile.precision(9);
    geoFile << std::setw(4) << tubeID 
 	    << " " << std::setw(8) << newTransform.getTranslation().getX()/cm
 	    << " " << std::setw(8) << newTransform.getTranslation().getY()/cm
 	    << " " << std::setw(8) << newTransform.getTranslation().getZ()/cm
	    << " " << std::setw(7) << pmtOrientation.x()
	    << " " << std::setw(7) << pmtOrientation.y()
	    << " " << std::setw(7) << pmtOrientation.z()
 	    << " " << std::setw(3) << cylLocation
 	    << G4endl;
  }
  geoFile.close();
}// DumpGeometryTableToFile

//---------------------------------------------------------------------------------------------

void MEMPHYS::DetectorConstruction::FillGeometryTuple() {
  G4cout << "DetectorConstruction::FillGeometryTuple : begin" << G4endl;

  G4Transform3D newTransform;
  G4Vector3D nullOrient = G4Vector3D(0,0,1); 
  G4Vector3D pmtOrientation;
  cyl_location cylLocation;

#ifdef APP_USE_AIDA
  if(geomTuple) {

  //JEC Have a look at MEMPHYS::Analysis for the description of the Tuple variables
  geomTuple->fill(0, WCCylInfo[0]);                                   //wcRadius
  
  geomTuple->fill(1, WCCylInfo[1]);                                   //wcLength
  
  AIDA::ITuple* wcOffset = geomTuple->getTuple( 2 );
  wcOffset->fill(0, WCOffset.x()/cm);                                //wcOffset
  wcOffset->fill(1, WCOffset.y()/cm);
  wcOffset->fill(2, WCOffset.z()/cm);
  wcOffset->addRow();

  geomTuple->fill(3, WCPMTSize);                                     //pmtRadius
  
  geomTuple->fill(4, totalNumPMTs);                                  //nPMTs

  AIDA::ITuple* pmtInfos = geomTuple->getTuple( 5 );                //pmtInfos (JEC 21/4/06 fix: should be placed here)

  G4cout << "DetectorConstruction::FillGeometryTuple : " 
         << " totalNumPMTs = " << totalNumPMTs 
         << G4endl;

  for ( int tubeID = 1; tubeID <= totalNumPMTs; tubeID++){
    cylLocation    = tubeCylLocation[tubeID];
    newTransform   = tubeIDMap[tubeID];
    pmtOrientation = newTransform * nullOrient;
    
    pmtInfos->fill(0, tubeID);                                          //pmtId 
    pmtInfos->fill(1, cylLocation);                                     //pmtLocation
    
    //std::cout << "PMT [" << tubeID <<"]: loc. " <<  cylLocation << std::endl;
    
    AIDA::ITuple* pmtOrient = pmtInfos->getTuple( 2 );                   //pmtOrient
    // Get tube orientation vector
    pmtOrient->fill(0, pmtOrientation.x());                                 //dx
    pmtOrient->fill(1, pmtOrientation.y());                                 //dy
    pmtOrient->fill(2, pmtOrientation.z());                                 //dz
    pmtOrient->addRow();

    AIDA::ITuple* pmtPosition = pmtInfos->getTuple( 3 );                //pmtPosition
    pmtPosition->fill(0,  newTransform.getTranslation().getX()/cm);        //x
    pmtPosition->fill(1,  newTransform.getTranslation().getY()/cm);        //y
    pmtPosition->fill(2,  newTransform.getTranslation().getZ()/cm);        //z
    pmtPosition->addRow();

    //add the new PMT
    pmtInfos->addRow();
  }//eo loop on PMTs 

  //Save the geom
  geomTuple->addRow();}
#endif //APP_USE_AIDA
  
#ifdef APP_USE_INLIB_WROOT

  fAnalysis.m_Geometry_leaf_wcRadius->fill(WCCylInfo[0]);
  fAnalysis.m_Geometry_leaf_wcLength->fill(WCCylInfo[1]);

    fAnalysis.m_Geometry_wcOffset_tree->reset();
    fAnalysis.m_Geometry_wcOffset_leaf_x->fill(WCOffset.x()/cm);
    fAnalysis.m_Geometry_wcOffset_leaf_y->fill(WCOffset.y()/cm);
    fAnalysis.m_Geometry_wcOffset_leaf_z->fill(WCOffset.z()/cm);
   {inlib::uint32 nbytes;
    if(!fAnalysis.m_Geometry_wcOffset_tree->fill(nbytes)) {
      std::cout << "wcOffset tree fill failed." << std::endl;
    }}
  
  fAnalysis.m_Geometry_leaf_pmtRadius->fill(WCPMTSize);
  fAnalysis.m_Geometry_leaf_nPMTs->fill(totalNumPMTs);

  fAnalysis.m_Geometry_pmtInfos_tree->reset();
  for ( int tubeID = 1; tubeID <= totalNumPMTs; tubeID++){
    cylLocation    = tubeCylLocation[tubeID];
    newTransform   = tubeIDMap[tubeID];
    pmtOrientation = newTransform * nullOrient;
      
    fAnalysis.m_Geometry_pmtInfos_leaf_pmtId->fill(tubeID);
    fAnalysis.m_Geometry_pmtInfos_leaf_pmtLocation->fill(cylLocation);

    fAnalysis.m_Geometry_pmtInfos_pmtOrient_tree->reset();
    fAnalysis.m_Geometry_pmtInfos_pmtOrient_leaf_dx->fill(pmtOrientation.x());
    fAnalysis.m_Geometry_pmtInfos_pmtOrient_leaf_dy->fill(pmtOrientation.y());
    fAnalysis.m_Geometry_pmtInfos_pmtOrient_leaf_dz->fill(pmtOrientation.z());
   {inlib::uint32 nbytes;
    if(!fAnalysis.m_Geometry_pmtInfos_pmtOrient_tree->fill(nbytes)) {
      std::cout << "pmtOrient tree fill failed." << std::endl;
    }}

    fAnalysis.m_Geometry_pmtInfos_pmtPosition_tree->reset();
    fAnalysis.m_Geometry_pmtInfos_pmtPosition_leaf_x->fill(newTransform.getTranslation().getX()/cm);
    fAnalysis.m_Geometry_pmtInfos_pmtPosition_leaf_y->fill(newTransform.getTranslation().getY()/cm);
    fAnalysis.m_Geometry_pmtInfos_pmtPosition_leaf_z->fill(newTransform.getTranslation().getZ()/cm);
   {inlib::uint32 nbytes;
    if(!fAnalysis.m_Geometry_pmtInfos_pmtPosition_tree->fill(nbytes)) {
      std::cout << "pmtPosition tree fill failed." << std::endl;
    }}
      
   {inlib::uint32 nbytes;
    if(!fAnalysis.m_Geometry_pmtInfos_tree->fill(nbytes)) {
      std::cout << "pmtInfos tree fill failed." << std::endl;
    }}
  }
  
 {inlib::uint32 nbytes;
  if(!fAnalysis.m_Geometry_tree->fill(nbytes)) {
    std::cout << "geom tree fill failed." << std::endl;
  }}
#endif
  
  G4cout << "DetectorConstruction::FillGeometryTuple : end" << G4endl;

}//FillGeometryTuple

//---------------------------------------------------------------------------------------------

// Code for traversing the geometry tree.  This code is very general you pass
// it a function and it will call the function with the information on each
// object it finds.
//
// The traversal code comes from a combination of me/G4Lab project &
// from source/visualization/modeling/src/G4PhysicalVolumeModel.cc
//
// If you are trying to understand how passing the function works you need
// to understand pointers to member functions...
//
// Also notice that DescriptionFcnPtr is a (complicated) typedef.
//

void MEMPHYS::DetectorConstruction::TraverseReplicas(G4VPhysicalVolume* aPV, int aDepth, 
						     const G4Transform3D& aTransform,
						     DescriptionFcnPtr registrationRoutine) {
  // Recursively visit all of the geometry below the physical volume
  // pointed to by aPV including replicas.
  
  G4ThreeVector     originalTranslation = aPV->GetTranslation();
  G4RotationMatrix* pOriginalRotation   = aPV->GetRotation();
  
  if (aPV->IsReplicated() ) {
    EAxis    axis;
    G4int    nReplicas;
    G4double width, offset;
    G4bool   consuming;
    
    aPV->GetReplicationData(axis,nReplicas,width,offset,consuming);
    
    for (int n = 0; n < nReplicas; n++) {
      switch(axis) {
      default:
      case kXAxis:
	aPV->SetTranslation(G4ThreeVector
			    (-width*(nReplicas-1)*0.5+n*width,0,0));
	aPV->SetRotation(0);
	break;
      case kYAxis:
	aPV->SetTranslation(G4ThreeVector
			    (0,-width*(nReplicas-1)*0.5+n*width,0));
	aPV->SetRotation(0);
	break;
      case kZAxis:
	aPV->SetTranslation(G4ThreeVector
			    (0,0,-width*(nReplicas-1)*0.5+n*width));
	aPV->SetRotation(0);
	break;
      case kRho:
	//Lib::Out::putL("GeometryVisitor::visit: WARNING:");
	//Lib::Out::putL(" built-in replicated volumes replicated");
	//Lib::Out::putL(" in radius are not yet properly visualizable.");
	aPV->SetTranslation(G4ThreeVector(0,0,0));
	aPV->SetRotation(0);
	break;
      case kPhi:
	{
	  G4RotationMatrix rotation;
          rotation.rotateZ(-(offset+(n+0.5)*width));
          // Minus Sign because for the physical volume we need the
          // coordinate system rotation.
          aPV->SetTranslation(G4ThreeVector(0,0,0));
          aPV->SetRotation(&rotation);
	}
	break;

      } // axis switch

      //      G4cout << "(JEC) TraverseReplicas: Replica Volmume <" << aPV->GetName() << "> at depth: " << aDepth
      //     << G4endl;

      DescribeAndDescendGeometry(aPV, aDepth, n, aTransform, registrationRoutine);

    }   // num replicas for loop
  } else {

    //G4cout << "(JEC) TraverseReplicas: Not-Replica Volmume <" << aPV->GetName() << "> at depth: " << aDepth
    //   << G4endl;
    DescribeAndDescendGeometry(aPV, aDepth, aPV->GetCopyNo(), aTransform, registrationRoutine);
    
  } //eo replicat
  
  // Restore original transformation...
  aPV->SetTranslation(originalTranslation);
  aPV->SetRotation(pOriginalRotation);
}//TraverseReplicas

//---------------------------------------------------------------------------------------------

void MEMPHYS::DetectorConstruction::DescribeAndDescendGeometry(G4VPhysicalVolume* aPV ,
							     int aDepth, int replicaNo, 
							     const G4Transform3D& aTransform,  
							     DescriptionFcnPtr registrationRoutine) {
  // Calculate the new transform relative to the old transform
  G4Transform3D* transform = 
    new G4Transform3D(*(aPV->GetObjectRotation()), aPV->GetTranslation());

  G4Transform3D newTransform = aTransform * (*transform);
  delete transform; 

  // Call the routine we use to print out geometry descriptions, make
  // tables, etc.  The routine was passed here as a paramater.  It needs to
  // be a memeber function of the class

  (this->*registrationRoutine)(aPV, aDepth, replicaNo, newTransform);

  int nDaughters = aPV->GetLogicalVolume()->GetNoDaughters();
  
  for (int iDaughter = 0; iDaughter < nDaughters; iDaughter++) {
    TraverseReplicas(aPV->GetLogicalVolume()->GetDaughter(iDaughter),
		     aDepth+1, newTransform, registrationRoutine);
  }
}//DescribeAndDescendGeometry

//---------------------------------------------------------------------------------------------
//JEC 17/11/05 Include the Material Part: Let comments as it is as it can be instructive...
//---------------------------------------------------------------------------------------------

void MEMPHYS::DetectorConstruction::ConstructMaterials() {
    
  //---Vaccuum
  G4double density     = universe_mean_density;              //from PhysicalConstants.h
  G4double pressure    = 1.e-19*pascal;
  G4double temperature = 0.1*kelvin;
  G4double a = 1.01*g/mole;
  new G4Material("Vaccuum", 1., a, density,
                   kStateGas,temperature,pressure);

  //---Water
  
  a = 1.01*g/mole;
  G4Element* elH 
    = new G4Element("Hydrgen","H", 1,a);
  
  a = 16.00*g/mole;
  G4Element* elO 
    = new G4Element("Oxygen","O", 8,a);
  
  density = 1.00*g/cm3;
  G4Material* Water 
    = new G4Material("Water",density,2);
  Water->AddElement(elH, 2);
  Water->AddElement(elO, 1);

 //---Ice 
 
 density = 0.92*g/cm3;//Ice
 G4Material* Ice = new G4Material("Ice",density,2);
 Ice->AddElement(elH, 2);
 Ice->AddElement(elO, 1);

 //---Steel
  
  a= 12.01*g/mole;
  G4Element* elC 
    = new G4Element("Carbon","C", 6,a);
  
  a = 55.85*g/mole;
  G4Element* elFe
    = new G4Element("Iron","Fe", 26,a);
  
  density = 7.8*g/cm3;
  G4Material* Steel
    = new G4Material("Steel",density,2);
  Steel->AddElement(elC, 1.*perCent);
  Steel->AddElement(elFe, 99.*perCent);
  
  //---Stainless steel 304L (simple example, particular alloy can be different)
  
  a = 51.9961*g/mole;
  G4Element* elCr = new G4Element("Chromium", "Cr", 24., a);
  
  a = 58.6934*g/mole;
  G4Element* elNi = new G4Element("Nickel", "Ni", 28., a);

  a = 54.938*g/mole;
  G4Element* elMn = new G4Element("Manganese", "Mn", 25., a); 

  a = 30.974*g/mole;
  G4Element* elP = new G4Element("Phosphore", "P", 15., a);

  a = 28.09*g/mole;
  G4Element* elSi = new G4Element("Silicon", "Si", 14., a); 

  a = 32.065*g/mole;
  G4Element* elS = new G4Element("Sulphur", "S", 16., a);

  density = 7.81*g/cm3;
  G4Material* StainlessSteel = new G4Material("StainlessSteel", density, 8);
 
  StainlessSteel->AddElement(elFe, 70.44*perCent);
  StainlessSteel->AddElement(elCr, 18*perCent);
  StainlessSteel->AddElement(elC,  0.08*perCent);
  StainlessSteel->AddElement(elNi, 8*perCent); 
  StainlessSteel->AddElement(elP, 0.45*perCent);
  StainlessSteel->AddElement(elSi,  1*perCent);
  StainlessSteel->AddElement(elMn, 2*perCent);
  StainlessSteel->AddElement(elS, 0.03*perCent);
  
  G4MaterialPropertiesTable *mpt = new G4MaterialPropertiesTable();
   
  const G4int nEntries = 2;
  G4double photonEnergy[nEntries] = {1.*eV , 7.*eV};
  
  //G4double rindex_Steel[nEntries] = {1.462 , 1.462}; // No I haven't gone mad
  G4double abslength_Steel[nEntries] = {.001*mm , .001*mm};
  //mpt->AddProperty("RINDEX", photonEnergy, rindex_Steel, nEntries);
  mpt->AddProperty("ABSLENGTH", photonEnergy, abslength_Steel, nEntries);
  
  StainlessSteel->SetMaterialPropertiesTable(mpt);


  //---Solid Dry Ice

  density = 1.563*g/cm3;
  G4Material* DryIce = new G4Material("SolidDryIce", density, 2);
  DryIce->AddElement(elC, 1);
  DryIce->AddElement(elO, 2);

  //---Air
  
  a = 14.01*g/mole;
  G4Element* elN 
    = new G4Element("Nitrogen","N", 7,a);
  
  density = 1.290*mg/cm3;
  G4Material* Air 
    = new G4Material("Air",density,2);
  Air->AddElement(elN, 70.*perCent);
  Air->AddElement(elO, 30.*perCent);
  
  //---Plastic
  
  density = 0.95*g/cm3;
  G4Material* Plastic
    = new G4Material("Plastic",density,2);
  Plastic->AddElement(elC, 1);
  Plastic->AddElement(elH, 2);

  //---Aluminum (element and material)

  a = 26.98*g/mole;
  G4Element* elAl = new G4Element("Aluminum", "Al", 13, a);  

  density = 2.7*g/cm3;
  G4Material* Aluminum
    = new G4Material("Aluminum",density,1);
  Aluminum->AddElement(elAl, 1);

  //---Black sheet

  density = 0.95*g/cm3;
  G4Material* Blacksheet
    = new G4Material("Blacksheet",density,2);
  Blacksheet->AddElement(elC, 1);
  Blacksheet->AddElement(elH, 2);

  //---Glass
 
  density = 2.20*g/cm3;
  G4Material* SiO2 = new G4Material("SiO2",density,2);
  SiO2->AddElement(elSi, 1);
  SiO2->AddElement(elO , 2);

  a = 10.81*g/mole;
  G4Element* elB = new G4Element("Boron", "B", 5, a);  

  density = 2.46*g/cm3;
  G4Material* B2O3 = new G4Material("B2O3",density,2);
  B2O3->AddElement(elB, 2);
  B2O3->AddElement(elO, 3);

  a = 22.99*g/mole;
  G4Element* elNa = new G4Element("Sodium", "Na", 11, a);  

  density = 2.27*g/cm3;
  G4Material* Na2O = new G4Material("Na2O",density,2);
  Na2O->AddElement(elNa, 2);
  Na2O->AddElement(elO, 1);

  density = 4.00*g/cm3;
  G4Material* Al2O3 = new G4Material("Al2O3",density,2);
  Al2O3->AddElement(elAl, 2);
  Al2O3->AddElement(elO, 3);

//   G4Material* blackAcryl
//     = new G4Material("blackAcryl", density, 3);
//   blackAcryl -> AddElement(elH, 6);
//   blackAcryl -> AddElement(elC, 4);
//   blackAcryl -> AddElement(elO, 2);

  density = 2.23*g/cm3;
  G4Material* Glass = new G4Material("Glass",density,4);
  //G4Material* Glass
  //= new G4Material("Glass",density,8);  //Put in 8 materials later
  
  Glass->AddMaterial(SiO2, 80.6*perCent);
  Glass->AddMaterial(B2O3, 13.0*perCent);
  Glass->AddMaterial(Na2O, 4.0*perCent);
  Glass->AddMaterial(Al2O3, 2.4*perCent);
  //Glass->AddMaterial(Al2O3, 2.3*perCent);  
  //Put in 2.3 percent if the other 4 materials = 0.1 percent

  // --Rock (SiO2)
  density = 2.7*g/cm3;
  G4Material* Rock = new G4Material("Rock",density,2);
  Rock->AddElement(elSi, 1);
  Rock->AddElement(elO, 2);

  // -------------------------------------------------------------
  // Generate & Add Material Properties Table
  // -------------------------------------------------------------
  
  /* JEC 18/11/05 not used
     const G4int NUMENTRIES = 32;
     G4double PPCKOV[NUMENTRIES] =
     { 2.034E-9*GeV, 2.068E-9*GeV, 2.103E-9*GeV, 2.139E-9*GeV,
     2.177E-9*GeV, 2.216E-9*GeV, 2.256E-9*GeV, 2.298E-9*GeV,
     2.341E-9*GeV, 2.386E-9*GeV, 2.433E-9*GeV, 2.481E-9*GeV,
     2.532E-9*GeV, 2.585E-9*GeV, 2.640E-9*GeV, 2.697E-9*GeV,
     2.757E-9*GeV, 2.820E-9*GeV, 2.885E-9*GeV, 2.954E-9*GeV,
     3.026E-9*GeV, 3.102E-9*GeV, 3.181E-9*GeV, 3.265E-9*GeV,
     3.353E-9*GeV, 3.446E-9*GeV, 3.545E-9*GeV, 3.649E-9*GeV,
     3.760E-9*GeV, 3.877E-9*GeV, 4.002E-9*GeV, 4.136E-9*GeV };
  */

  // default values
  /* G4double RINDEX1[NUMENTRIES] =
     { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
     1.346,  1.3465, 1.347,  1.3475, 1.348,
     1.3485, 1.3492, 1.35,   1.3505, 1.351,
     1.3518, 1.3522, 1.3530, 1.3535, 1.354,
     1.3545, 1.355,  1.3555, 1.356,  1.3568,
     1.3572, 1.358,  1.3585, 1.359,  1.3595,
     1.36,   1.3608};
  */
  
  // M Fechner,  values from refsg.F in apdetsim
  /*  G4double RINDEX1[NUMENTRIES] = 
      { 1.33332, 1.333364, 1.333396, 1.3343, 1.33465,
      1.33502, 1.3354, 1.33579, 1.3362, 1.33663, 1.33709,
      1.33756, 1.33806, 1.3386, 1.33915, 1.33974,
      1.34037, 1.34105, 1.34176, 1.34253, 1.34336,
      1.34425, 1.34521, 1.34626, 1.3474, 1.34864,
      1.35002, 1.35153, 1.35321, 1.35507, 1.35717, 1.35955 };
  */
  
   //From SFDETSIM water absorption
  const G4int NUMENTRIES_water=60;
  
  G4double ENERGY_water[NUMENTRIES_water] =
    { 1.56962e-09*GeV, 1.58974e-09*GeV, 1.61039e-09*GeV, 1.63157e-09*GeV, 
      1.65333e-09*GeV, 1.67567e-09*GeV, 1.69863e-09*GeV, 1.72222e-09*GeV, 
      1.74647e-09*GeV, 1.77142e-09*GeV,1.7971e-09*GeV, 1.82352e-09*GeV, 
      1.85074e-09*GeV, 1.87878e-09*GeV, 1.90769e-09*GeV, 1.93749e-09*GeV, 
      1.96825e-09*GeV, 1.99999e-09*GeV, 2.03278e-09*GeV, 2.06666e-09*GeV,
      2.10169e-09*GeV, 2.13793e-09*GeV, 2.17543e-09*GeV, 2.21428e-09*GeV, 
      2.25454e-09*GeV, 2.29629e-09*GeV, 2.33962e-09*GeV, 2.38461e-09*GeV, 
      2.43137e-09*GeV, 2.47999e-09*GeV, 2.53061e-09*GeV, 2.58333e-09*GeV, 
      2.63829e-09*GeV, 2.69565e-09*GeV, 2.75555e-09*GeV, 2.81817e-09*GeV, 
      2.88371e-09*GeV, 2.95237e-09*GeV, 3.02438e-09*GeV, 3.09999e-09*GeV,
      3.17948e-09*GeV, 3.26315e-09*GeV, 3.35134e-09*GeV, 3.44444e-09*GeV, 
      3.54285e-09*GeV, 3.64705e-09*GeV, 3.75757e-09*GeV, 3.87499e-09*GeV, 
      3.99999e-09*GeV, 4.13332e-09*GeV, 4.27585e-09*GeV, 4.42856e-09*GeV, 
      4.59258e-09*GeV, 4.76922e-09*GeV, 4.95999e-09*GeV, 5.16665e-09*GeV, 
      5.39129e-09*GeV, 5.63635e-09*GeV, 5.90475e-09*GeV, 6.19998e-09*GeV };

  // Air
  G4double RINDEX_air[NUMENTRIES_water] = 
    { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
       
  
  // M Fechner : new ; define the water refraction index using refsg.F 
  //from skdetsim using the whole range.   
  G4double RINDEX1[NUMENTRIES_water] =  
    { 1.32885, 1.32906, 1.32927, 1.32948, 1.3297, 1.32992, 1.33014, 
      1.33037, 1.3306, 1.33084, 1.33109, 1.33134, 1.3316, 1.33186, 1.33213,
      1.33241, 1.3327, 1.33299, 1.33329, 1.33361, 1.33393, 1.33427, 1.33462,
      1.33498, 1.33536, 1.33576, 1.33617, 1.3366, 1.33705, 1.33753, 1.33803,
      1.33855, 1.33911, 1.3397, 1.34033, 1.341, 1.34172, 1.34248, 1.34331,
      1.34419, 1.34515, 1.3462, 1.34733, 1.34858, 1.34994, 1.35145, 1.35312,
      1.35498, 1.35707, 1.35943, 1.36211, 1.36518, 1.36872, 1.37287, 1.37776,
      1.38362, 1.39074, 1.39956, 1.41075, 1.42535 };
  
   /* G4double ABSORPTION_water[NUMENTRIES_water] =
      { 22.8154*cm, 28.6144*cm, 35.9923*cm, 45.4086*cm, 57.4650*cm,
      72.9526*cm, 92.9156*cm, 118.737*cm, 152.255*cm, 195.925*cm,
      202.429*cm, 224.719*cm, 236.407*cm, 245.700*cm, 289.017*cm,
      305.810*cm, 316.456*cm, 326.797*cm, 347.222*cm, 414.938*cm,
      636.943*cm, 934.579*cm, 1245.33*cm, 1402.52*cm, 1550.39*cm,
      1745.20*cm, 1883.24*cm, 2016.13*cm, 2442.18*cm, 3831.28*cm,
      4652.89*cm, 5577.04*cm, 6567.08*cm, 7559.88*cm, 8470.06*cm,
      9205.54*cm, 9690.95*cm, 9888.36*cm, 9804.50*cm, 9482.17*cm,
      8982.77*cm, 8369.39*cm, 7680.31*cm, 6902.11*cm, 6183.84*cm,
      5522.27*cm, 4914.33*cm, 4357.09*cm, 3847.72*cm, 3383.51*cm,
      2961.81*cm, 2580.08*cm, 2235.83*cm, 1926.66*cm, 1650.21*cm,
      1404.21*cm, 1186.44*cm, 994.742*cm, 827.027*cm, 681.278*cm};
   */
  G4double ABSORPTION_water[NUMENTRIES_water] =
    {25.3504*cm, 31.7938*cm, 39.9915*cm, 50.454*cm, 63.85*cm, 
     81.0584*cm, 103.24*cm, 131.93*cm, 169.172*cm, 217.694*cm, 
     224.921*cm, 249.688*cm, 262.674*cm, 273*cm, 321.13*cm, 339.789*cm,
     351.617*cm, 363.108*cm, 385.802*cm, 461.042*cm, 707.714*cm, 
     1038.42*cm, 1383.7*cm, 1558.36*cm, 1722.65*cm, 1939.11*cm, 
     2092.49*cm, 2240.14*cm, 2962.96*cm, 4967.03*cm, 6368.58*cm, 
     8207.56*cm, 10634.2*cm, 13855.3*cm, 18157.3*cm, 23940.2*cm, 
     31766*cm, 42431.6*cm, 57074.9*cm, 77335.9*cm, 105598*cm, 
     145361*cm, 192434*cm, 183898*cm, 176087*cm, 168913*cm, 162301*cm, 
     156187*cm, 150516*cm, 145243*cm, 140327*cm, 135733*cm, 131430*cm, 
     127392*cm, 123594*cm, 120016*cm, 116640*cm, 113448*cm, 110426*cm, 
     107562*cm};
  
  // M Fechner: Rayleigh scattering -- as of version 4.6.2 of GEANT,
  // one may use one's own Rayleigh scattering lengths (the buffer is no
  // longer overwritten for "water", see 4.6.2 release notes)
  
  // RAYFF = 1/ARAS, for those of you who know SKdetsim...
  // actually that's not quite right because the scattering models
  // are different; in G4 there is no scattering depolarization
  // std value at SK = 0.6. But Mie scattering is implemented
  // in SKdetsim and not in G4
  
  // april 2005 : reduced reflections, let's increase scattering...
  //   G4double RAYFF = 1.0/1.65;
  G4double RAYFF = 1.0/1.5;
  
  G4double RAYLEIGH_water[NUMENTRIES_water] = {
    167024.4*cm*RAYFF, 158726.7*cm*RAYFF, 150742*cm*RAYFF,
    143062.5*cm*RAYFF, 135680.2*cm*RAYFF, 128587.4*cm*RAYFF,
    121776.3*cm*RAYFF, 115239.5*cm*RAYFF, 108969.5*cm*RAYFF,
    102958.8*cm*RAYFF, 97200.35*cm*RAYFF, 91686.86*cm*RAYFF,
    86411.33*cm*RAYFF, 81366.79*cm*RAYFF, 76546.42*cm*RAYFF,
    71943.46*cm*RAYFF, 67551.29*cm*RAYFF, 63363.36*cm*RAYFF,
    59373.25*cm*RAYFF, 55574.61*cm*RAYFF, 51961.24*cm*RAYFF,
    48527.00*cm*RAYFF, 45265.87*cm*RAYFF, 42171.94*cm*RAYFF,
    39239.39*cm*RAYFF, 36462.50*cm*RAYFF, 33835.68*cm*RAYFF,
    31353.41*cm*RAYFF, 29010.30*cm*RAYFF, 26801.03*cm*RAYFF,
    24720.42*cm*RAYFF, 22763.36*cm*RAYFF, 20924.88*cm*RAYFF,
    19200.07*cm*RAYFF, 17584.16*cm*RAYFF, 16072.45*cm*RAYFF,
    14660.38*cm*RAYFF, 13343.46*cm*RAYFF, 12117.33*cm*RAYFF,
    10977.70*cm*RAYFF, 9920.416*cm*RAYFF, 8941.407*cm*RAYFF,
    8036.711*cm*RAYFF, 7202.470*cm*RAYFF, 6434.927*cm*RAYFF,
    5730.429*cm*RAYFF, 5085.425*cm*RAYFF, 4496.467*cm*RAYFF,
    3960.210*cm*RAYFF, 3473.413*cm*RAYFF, 3032.937*cm*RAYFF,
    2635.746*cm*RAYFF, 2278.907*cm*RAYFF, 1959.588*cm*RAYFF,
    1675.064*cm*RAYFF, 1422.710*cm*RAYFF, 1200.004*cm*RAYFF,
    1004.528*cm*RAYFF, 833.9666*cm*RAYFF, 686.1063*cm*RAYFF
  };

  //From SFDETSIM
  /*  G4double RINDEX_glass[NUMENTRIES] =
      { 1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
      1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
      1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
      1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
      1.600, 1.600, 1.600, 1.600 };
  */
  // M Fechner : unphysical, I want to reduce reflections
  // back to the old value 1.55

  G4double RINDEX_glass[NUMENTRIES_water] =
    { 1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
      1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
      1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
      1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
      1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
      1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
      1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
      1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
      1.600, 1.600 }; 
  
  //G4double RINDEX_blacksheet[NUMENTRIES] =
  //{ 2.500, 2.500, 2.500, 2.500, 2.500, 2.500, 2.500,
  //  2.500, 2.500, 2.500, 2.500, 2.500, 2.500, 2.500,
  //  2.500, 2.500, 2.500, 2.500, 2.500, 2.500, 2.500,
  //  2.500, 2.500, 2.500, 2.500, 2.500, 2.500, 2.500,
  //  2.500, 2.500, 2.500, 2.500 };
  
   
  //G4double ABSORPTION1[NUMENTRIES] =
  //{344.8*cm,  408.2*cm,  632.9*cm,  917.4*cm, 1234.6*cm, 1388.9*cm,
  // 1515.2*cm, 1724.1*cm, 1886.8*cm, 2000.0*cm, 2631.6*cm, 3571.4*cm,
  // 4545.5*cm, 4761.9*cm, 5263.2*cm, 5263.2*cm, 5555.6*cm, 5263.2*cm,
  // 5263.2*cm, 4761.9*cm, 4545.5*cm, 4166.7*cm, 3703.7*cm, 3333.3*cm,
  // 3000.0*cm, 2850.0*cm, 2700.0*cm, 2450.0*cm, 2200.0*cm, 1950.0*cm,
  // 1750.0*cm, 1450.0*cm };
  
  /* G4double ABSORPTION_glass[NUMENTRIES] = 
     { 100.0*cm, 110.0*cm, 120.0*cm, 130.0*cm, 140.0*cm, 150.0*cm, 160.0*cm,
     165.0*cm, 170.0*cm, 175.0*cm, 180.0*cm, 185.0*cm, 190.0*cm, 195.0*cm,
     200.0*cm, 200.0*cm, 200.0*cm, 200.0*cm, 200.0*cm, 195.0*cm, 190.0*cm,
     185.0*cm, 180.0*cm, 175.0*cm, 170.0*cm, 160.0*cm, 150.0*cm, 140.0*cm,
     130.0*cm, 120.0*cm, 110.0*cm, 100.0*cm };
  */
  // M Fechner : the quantum efficiency already takes glass abs into account

   G4double ABSORPTION_glass[NUMENTRIES_water]= 
     { 1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm, 1.0e9*cm,
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm, 1.0e9*cm };
   
   G4double BLACKABS_blacksheet[NUMENTRIES_water] =
     { 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 
       1.0e-9*cm,
       1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm,
       1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm,
       1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm,
       1.0e-9*cm, 1.0e-9*cm,
       1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 
       1.0e-9*cm,
       1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm,
       1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm,
       1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm,
       1.0e-9*cm, 1.0e-9*cm};
   
   
   //The following reflectivity for blacksheet is obtained from skdetsim
   //There is a fudge factor of 2.7 for the reflectivity of blacksheet
   //depending on whether SK1 or SK2 simulation is used.  
   //The BlackSheetFudgeFactor is set to true if you want to use the 
   //SK2 values, false if not.
   G4double SK1SK2FF = 1.0;
   //G4bool BlackSheetFudgeFactor=false;
   G4bool BlackSheetFudgeFactor=true;
   //   if (BlackSheetFudgeFactor) SK1SK2FF=SK1SK2FF*2.7;
   if (BlackSheetFudgeFactor) SK1SK2FF=SK1SK2FF*1.55;
   
   /*
     G4double REFLECTIVITY_blacksheet[NUMENTRIES] =
     { 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
     0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
     0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
     0.055*SK1SK2FF, 0.057*SK1SK2FF, 0.059*SK1SK2FF, 0.060*SK1SK2FF, 
     0.059*SK1SK2FF, 0.058*SK1SK2FF, 0.057*SK1SK2FF, 0.055*SK1SK2FF, 
     0.050*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 
     0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF,
     0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF };
   */
   
   G4double REFLECTIVITY_blacksheet[NUMENTRIES_water] =
     { 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.057*SK1SK2FF, 0.059*SK1SK2FF, 0.060*SK1SK2FF, 
       0.059*SK1SK2FF, 0.058*SK1SK2FF, 0.057*SK1SK2FF, 0.055*SK1SK2FF, 
       0.050*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 
       0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF,
       0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF,
       0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF,
       0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF ,
       0.045*SK1SK2FF, 0.045*SK1SK2FF };


   //utter fiction at this stage
   G4double EFFICIENCY[NUMENTRIES_water] =
     { 0.001*m };
   
   //utter fiction at this stage, does not matter
   G4double RAYLEIGH_air[NUMENTRIES_water] =
     { 0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
       0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
       0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
       0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
       0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
       0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
       0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
       0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
       0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
       0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m};
      
   //Not used yet, fictional values
   //G4double SPECULARLOBECONSTANT1[NUMENTRIES] =
   //{ 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001 };
   
   //Not used yet, fictional values
   //G4double SPECULARSPIKECONSTANT1[NUMENTRIES] =
   //{ 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001 };
   
   //Not used yet, fictional values
   //G4double BACKSCATTERCONSTANT1[NUMENTRIES] =
   //{ 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001 };
   
   G4double EFFICIENCY_blacksheet[NUMENTRIES_water] =
     { 0.0 };
   
   //	------------- Surfaces --------------
   
   OpWaterBSSurface = new G4OpticalSurface("WaterBSCellSurface");
   OpWaterBSSurface->SetType(dielectric_dielectric);
   OpWaterBSSurface->SetModel(unified);
   OpWaterBSSurface->SetFinish(groundfrontpainted);
   OpWaterBSSurface->SetSigmaAlpha(0.1);
   
   const G4int NUM = 2;
   //   G4double PP[NUM] =
   //{ 2.038E-9*GeV, 4.144E-9*GeV };

   G4double PP[NUM] = { 1.4E-9*GeV,6.2E-9*GeV};
   G4double RINDEX_blacksheet[NUM] =
     { 1.6, 1.6 };
   
   G4double SPECULARLOBECONSTANT[NUM] =
     { 0.3, 0.3 };
   G4double SPECULARSPIKECONSTANT[NUM] =
     { 0.2, 0.2 };
   G4double BACKSCATTERCONSTANT[NUM] =
     { 0.2, 0.2 };
   
   OpGlassCathodeSurface = new G4OpticalSurface("GlassCathodeSurface");
   //JEC 14/12/05 OpGlassCathodeSurface->SetType(dielectric_dielectric);
   //JEC 14/12/05 OpGlassCathodeSurface->SetModel(unified);
   OpGlassCathodeSurface->SetType(dielectric_metal); //as for LXe G4 example
   OpGlassCathodeSurface->SetModel(glisur);          //as for LXe G4 example
   OpGlassCathodeSurface->SetFinish(polished);
   //JEC 14/12/05 G4double RINDEX_cathode[NUM]         = { 1.0, 1.0 };
   //JEC 14/12/05   G4double EFFICIENCY_glasscath[NUM]   = { 0.0, 0.0 };
   G4double REFLECTIVITY_glasscath[NUM] = { 0.0, 0.0 };
   G4double EFFICIENCY_glasscath[NUM]   = { 1.0, 1.0 };
   
   G4MaterialPropertiesTable *myMPT1 = new G4MaterialPropertiesTable();
   // M Fechner : new   ; wider range for lambda
   myMPT1->AddProperty("RINDEX", ENERGY_water, RINDEX1, NUMENTRIES_water);
   myMPT1->AddProperty("ABSLENGTH",ENERGY_water, ABSORPTION_water, NUMENTRIES_water);
   // M Fechner: new, don't let G4 compute it.
   myMPT1->AddProperty("RAYLEIGH",ENERGY_water,RAYLEIGH_water,NUMENTRIES_water);
   Water->SetMaterialPropertiesTable(myMPT1);
   
   G4cout << "ConstructMaterial: Dump Water properties" << G4endl;
   //myMPT1->DumpTable();
   
   G4MaterialPropertiesTable *myMPT2 = new G4MaterialPropertiesTable();
   myMPT2->AddProperty("RINDEX", ENERGY_water, RINDEX_air, NUMENTRIES_water);
   // M Fechner : what is that ?????
   myMPT2->AddProperty("ABSLENGTH", ENERGY_water, BLACKABS_blacksheet, NUMENTRIES_water);
   myMPT2->AddProperty("RAYLEIGH",ENERGY_water, RAYLEIGH_air, NUMENTRIES_water);
   Air->SetMaterialPropertiesTable(myMPT2);
   
   G4MaterialPropertiesTable *myMPT3 = new G4MaterialPropertiesTable();
   myMPT3->AddProperty("ABSLENGTH", ENERGY_water, BLACKABS_blacksheet, NUMENTRIES_water);
   myMPT3->AddProperty("REFLECTIVITY", ENERGY_water, REFLECTIVITY_blacksheet, NUMENTRIES_water);
   myMPT3->AddProperty("EFFICIENCY",   ENERGY_water, EFFICIENCY, NUMENTRIES_water);
   Plastic->SetMaterialPropertiesTable(myMPT3);
   
   G4MaterialPropertiesTable *myMPT4 = new G4MaterialPropertiesTable();
   myMPT4->AddProperty("ABSLENGTH", ENERGY_water, BLACKABS_blacksheet, NUMENTRIES_water);
   Blacksheet->SetMaterialPropertiesTable(myMPT4);
   
   G4MaterialPropertiesTable *myMPT5 = new G4MaterialPropertiesTable();
   myMPT5->AddProperty("RINDEX", ENERGY_water, RINDEX_glass, NUMENTRIES_water);
   myMPT5->AddProperty("ABSLENGTH",ENERGY_water, ABSORPTION_glass, NUMENTRIES_water);
   Glass->SetMaterialPropertiesTable(myMPT5);
    
   //	------------- Surfaces --------------

   // Blacksheet
   G4MaterialPropertiesTable *myST1 = new G4MaterialPropertiesTable();
   myST1->AddProperty("RINDEX", ENERGY_water, RINDEX_blacksheet, NUMENTRIES_water);
   myST1->AddProperty("SPECULARLOBECONSTANT", PP, SPECULARLOBECONSTANT, NUM);
   myST1->AddProperty("SPECULARSPIKECONSTANT", PP, SPECULARSPIKECONSTANT, NUM);
   myST1->AddProperty("BACKSCATTERCONSTANT", PP, BACKSCATTERCONSTANT, NUM);
   myST1->AddProperty("REFLECTIVITY", ENERGY_water, REFLECTIVITY_blacksheet, NUMENTRIES_water);
   myST1->AddProperty("EFFICIENCY", ENERGY_water, EFFICIENCY_blacksheet, NUMENTRIES_water);
   OpWaterBSSurface->SetMaterialPropertiesTable(myST1);

   //Glass to Cathode surface inside PMTs
   G4MaterialPropertiesTable *myST2 = new G4MaterialPropertiesTable();
   //JEC 14/12/05   myST2->AddProperty("RINDEX", PP, RINDEX_cathode, NUM);
   myST2->AddProperty("REFLECTIVITY", PP, REFLECTIVITY_glasscath, NUM);
   myST2->AddProperty("EFFICIENCY", PP, EFFICIENCY_glasscath, NUM);
   OpGlassCathodeSurface->SetMaterialPropertiesTable(myST2);
}//ConstructMaterials


