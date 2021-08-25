#ifndef MEMPHYSWCDigi_h
#define MEMPHYSWCDigi_h

#include "G4VDigi.hh"
#include "G4TDigiCollection.hh"
#include "G4Allocator.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"

#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"

#include <inlib/pointer>

namespace MEMPHYS {

class WCDigi : public G4VDigi {

public:
  
  WCDigi() : tubeID(0), pe(0.), time(0.) {}
  virtual ~WCDigi() {}

  int operator==(const WCDigi& right) const { 
    return ( (tubeID==right.tubeID) && (pe==right.pe) && (time==right.time) ); 
  }
  
  void* operator new(size_t) {return (void*)allocator()->MallocSingle();}

  void operator delete(void* aDigi) {allocator()->FreeSingle((WCDigi*) aDigi);}

  
  void Draw() {} //JEC FIXME what do we do with this?
  void Print() {
    G4cout << "TubeID: " << tubeID 
           << " PE: "    << pe
           << " Time:"   << time << G4endl;
  }
  
  //JEC 1/12/05 implement same mechanism as Trajectory
  virtual const std::map<G4String,G4AttDef>* GetAttDefs() const {
    G4bool isNew;
    std::map<G4String,G4AttDef>* store = G4AttDefStore::GetInstance("WCDigi",isNew);

    if (isNew) {    
      G4String LV("LV");
      (*store)[LV] = G4AttDef(LV,"Logical Volume","Display","","G4LogicalVolume");

      G4String TSF("TSF");
      (*store)[TSF] = G4AttDef(TSF,"3D Transformation","Display","","G4Transform3D");
   
      G4String Color("Color");
      (*store)[Color] = G4AttDef(Color,"Hit Colour","Display","","G4Colour");
  
      G4String TubeID("TubeID");
      (*store)[TubeID] = G4AttDef(TubeID,"Tube ID","Bookkeeping","","G4int");
      
      G4String Pe("Pe");
      (*store)[Pe] = G4AttDef(Pe,"Charge","Physics","","G4float");
      
      G4String Time("Time");
      (*store)[Time] = G4AttDef(Time,"Time","Physics","","G4float");    
    }
    return store;
  }
  
  virtual std::vector<G4AttValue>* CreateAttValues() const {

    std::vector<G4AttValue>* values = new std::vector<G4AttValue>;
  
    std::string sv;
    inlib::p2s(pLogV,sv); //pointer <=> string must be consistent with what is done in exlib::geant4::utils.
    values->push_back(G4AttValue("LV",sv,""));

    inlib::p2s(&transform,sv);
    values->push_back(G4AttValue("TSF",sv,""));

    std::ostringstream s;
    s.str("");
    s << colour.GetRed()   << " " 
      << colour.GetGreen() << " " 
      << colour.GetBlue()  << " "
      << colour.GetAlpha();
    values->push_back(G4AttValue("Color",s.str(),""));

    s.str("");
    s << tubeID;
    values->push_back(G4AttValue("TubeID",s.str(),""));
  
    s.str("");
    s << pe;
    values->push_back(G4AttValue("Pe",s.str(),""));
    
    s.str("");
    s << time;
    values->push_back(G4AttValue("Time",s.str(),""));

    return values;
  }

private:

  //JEC make Ctor and assignation private
  WCDigi(const WCDigi& right) : G4VDigi() {
    tubeID = right.tubeID; 
    pe     = right.pe;
    time   = right.time;
  }

  const WCDigi& operator=(const WCDigi& right) {
    if(&right!=this){
      tubeID = right.tubeID; 
      pe     = right.pe;
      time   = right.time;
    }
    return *this;
  }
  
  G4int   tubeID;
  G4float pe;   
  G4float time; 

  //JEC for vizu
  G4Colour         colour;
  G4Transform3D    transform;
  G4LogicalVolume* pLogV;
 
public:
  
  void SetTubeID(G4int tube) {tubeID = tube;}
  void SetPe(G4float Q)      {pe     = Q;}
  void SetTime(G4float T)    {time   = T;}

  //JEC 1/12/05 for vizu
  void SetColour(G4Colour aColour) { colour = aColour; }
  void UpdateColour() {/* not yet implemented */}
  void SetTransform(G4Transform3D aTrans) { transform = aTrans; }
  void SetLogicalVolume(G4LogicalVolume* logV) {pLogV = logV;}

  G4LogicalVolume* GetLogicalVolume() { return pLogV; }
  
  const G4Transform3D& GetTransform()  const { return transform; }
  const G4Colour& GetColour() const { return colour; }

  G4int   GetTubeID() {return tubeID;}
  G4float GetPe()     {return pe;}
  G4float GetTime()   {return time;}
  
  static G4Allocator<WCDigi>* allocator() {
    //warning : it can't be on the stack, since it is managed by the G4RunManager.
    static G4Allocator<WCDigi>* s_allocator = new G4Allocator<WCDigi>;
    return s_allocator;
  }
};
}

namespace MEMPHYS {
  typedef G4TDigiCollection<WCDigi> WCDigitsCollection;
}

#endif









