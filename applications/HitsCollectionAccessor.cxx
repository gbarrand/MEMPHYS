// this :
#include <MEMPHYS/HitsCollectionAccessor.h>

// Inventor :
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoDrawStyle.h>

//#include <Inventor/nodes/SoCube.h> //FIXME
//#include <Inventor/nodes/SoCylinder.h> //JEC 27/1/06 FIXME

// HEPVis :
#include <HEPVis/SbPolyhedron.h>
#include <HEPVis/misc/SoStyleCache.h>
#include <HEPVis/nodes/SoHighlightMaterial.h>
#include <HEPVis/nodes/SoPolyhedron.h>

#ifdef WIN32
#undef pascal // Clash between windef.h and Geant4/SystemOfUnits.hh
#endif

// Geant4 :
#include <G4LogicalVolume.hh>
#include <G4Colour.hh>

// Lib :
#include <Lib/smanip.h>

// G4Lab :
#include <G4Lab/Transform3D.h>

//////////////////////////////////////////////////////////////////////////////
MEMPHYS::HitsCollectionAccessor::HitsCollectionAccessor(
 Slash::Core::ISession& aSession
,G4RunManager* aRunManager
,const std::string& aHC
)
:G4Lab::HitsCollectionAccessor(aSession,aRunManager,aHC)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
MEMPHYS::HitsCollectionAccessor::~HitsCollectionAccessor(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void MEMPHYS::HitsCollectionAccessor::visualize(
 Slash::Data::IAccessor::Data aData
,void*
) 
//////////////////////////////////////////////////////////////////////////////
// The hit must have the "LV" and "TSF" AttDef.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  G4VHit* obj = (G4VHit*)aData;  
  G4LogicalVolume* lv = hitLogicalVolume(*obj);
  if(!lv) return;
  G4Transform3D* tsf = hitTransform3D(*obj);
  if(!tsf) return;
  G4VSolid* solid = lv->GetSolid();
  if(!solid) return;
  //G4Material* material = lv->GetMaterial();

  SoSeparator* separator = new SoSeparator;
  separator->setName("sceneGraph");
      
 {G4Colour color;
  if(hitColor(*obj,color)) {
    SbColor sbColor((float)color.GetRed(),
                    (float)color.GetGreen(),
                    (float)color.GetBlue());
    float transp = 1.0F - (float)color.GetAlpha();
    SoStyleCache* styleCache = fSoGC.getStyleCache();
    separator->addChild(
      styleCache->getHighlightMaterial
        (sbColor,fSoGC.getHighlightColor(),transp));
  } else {
    separator->addChild(fSoGC.getHighlightMaterial());
  }
  separator->addChild(fSoGC.getDrawStyle());
  separator->addChild(fSoGC.getLightModel());}

 {SoTransform* transform = new SoTransform;
  G4Lab::Transform3D* t = new G4Lab::Transform3D(*tsf);
  SbMatrix* matrix = t->getMatrix();
  transform->setMatrix(*matrix);
  delete matrix;
  delete t;
  separator->addChild(transform);}
          
  // Build name (for picking) :
  std::string s;
  Lib::smanip::printf(s,128,"%s/0x%lx",HCName().c_str(),(unsigned long)obj);
  SbName name(s.c_str());

  // Representation :

  //FIXME

  //FIXME : PM base size :
//   double WCPMTRadius           = 0.10*m;  // 20-cm PMTs (8-inch)
  //JEC 27/1/06 double WCPMTRadius           = 0.10*m;  // 20-cm PMTs (8-inch)
  double WCPMTRadius           = 0.15*m;  // PMTs (12-inch)

  /*
  SoCylinder* soPMT = new SoCylinder;
  soPMT->radius.setValue(WCPMTRadius);
  soPMT->height.setValue(WCPMTRadius);
  //JEC what's SoCylinder parts?
  separator->addChild(soPMT);
  */

  SbPolyhedronTube sbTube(0,WCPMTRadius,WCPMTRadius/2);
  SoPolyhedron* soPMT = new SoPolyhedron(sbTube);
  soPMT->setName(name);
  //soPMT->radius.setValue(WCPMTRadius);
  //soPMT->height.setValue(WCPMTRadius);
  //JEC what's SoCylinder parts?
  soPMT->setName(name);
  separator->addChild(soPMT);


//   SoCube* soCube = new SoCube;
//   soCube->width.setValue(WCPMTRadius);
//   soCube->height.setValue(WCPMTRadius);
//   soCube->depth.setValue(WCPMTRadius);
//   soCube->setName(name);
        
//   separator->addChild(soCube);
          
  fSoRegion->doIt(SbAddNode(separator,"dynamicScene"));
}
