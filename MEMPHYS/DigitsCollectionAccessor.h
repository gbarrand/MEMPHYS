#ifndef MEMPHYS_DigitsCollectionAccessor_h
#define MEMPHYS_DigitsCollectionAccessor_h

// Inheritance :
#include <G4Lab/DigitsCollectionAccessor.h>

namespace MEMPHYS {

template <class T> 
class DigitsCollectionAccessor : public G4Lab::DigitsCollectionAccessor<T> {
public: //Slash::Data::IVisualizer
  virtual void visualize(Slash::Data::IAccessor::Data,void*);
public:
  DigitsCollectionAccessor(Slash::Core::ISession& aSession,G4RunManager* aRunManager,const std::string& aName = "")
    :G4Lab::DigitsCollectionAccessor<T>(aSession,aRunManager,aName){}
  virtual ~DigitsCollectionAccessor(){}
};

}

// Inventor :
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoCube.h> //FIXME

// HEPVis :
#include <HEPVis/misc/SoStyleCache.h>
#include <HEPVis/nodes/SoHighlightMaterial.h>

// Geant4 :
#include <G4LogicalVolume.hh>
#include <G4Transform3D.hh>
#include <G4VDigi.hh>

// G4Lab :
#include <G4Lab/Transform3D.h>

namespace MEMPHYS {

//////////////////////////////////////////////////////////////////////////////
template <class T> 
void DigitsCollectionAccessor<T>::visualize(
 Slash::Data::IAccessor::Data aData
,void*
) 
//////////////////////////////////////////////////////////////////////////////
// The digi must have the "LV" and "TSF" AttDef.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  G4VDigi* obj = (G4VDigi*)aData;  
  G4LogicalVolume* lv = 
    G4Lab::DigitsCollectionBaseAccessor::digiLogicalVolume(*obj);
  if(!lv) return;
  G4Transform3D* tsf = 
    G4Lab::DigitsCollectionBaseAccessor::digiTransform3D(*obj);
  if(!tsf) return;
  G4VSolid* solid = lv->GetSolid();
  if(!solid) return;
  //G4Material* material = lv->GetMaterial();

  SoSeparator* separator = new SoSeparator;
  separator->setName("sceneGraph");
      
 {G4Colour color;
  SoGC& f_SoGC = this->soGC();
  if(G4Lab::DigitsCollectionBaseAccessor::digiColor(*obj,color)) {
    SbColor sbColor((float)color.GetRed(),
                    (float)color.GetGreen(),
                    (float)color.GetBlue());
    float transp = 1.0F - (float)color.GetAlpha();
    SoStyleCache* styleCache = f_SoGC.getStyleCache();
    separator->addChild(
      styleCache->getHighlightMaterial
      (sbColor,f_SoGC.getHighlightColor(),transp));
  } else {
    separator->addChild(f_SoGC.getHighlightMaterial());
  }
  separator->addChild(f_SoGC.getDrawStyle());
  separator->addChild(f_SoGC.getLightModel());}

 {SoTransform* transform = new SoTransform;
  G4Lab::Transform3D* t = new G4Lab::Transform3D(*tsf);
  SbMatrix* matrix = t->getMatrix();
  transform->setMatrix(*matrix);
  delete matrix;
  delete t;
  separator->addChild(transform);}
          
  // Build name (for picking) :
  std::string s;
  Lib::smanip::printf(s,128,"%s/0x%lx",
		      this->DCName().c_str(),(unsigned long)obj);
  SbName name(s.c_str());
        
  double WCPMTRadius           = 0.10*m;  // 20-cm PMTs (8-inch)

  SoCube* soCube = new SoCube;
  soCube->width.setValue(WCPMTRadius);
  soCube->height.setValue(WCPMTRadius);
  soCube->depth.setValue(WCPMTRadius);
  soCube->setName(name);
        
  separator->addChild(soCube);
          
  this->soRegion()->doIt(SbAddNode(separator,"dynamicScene"));
}

}


#endif
