#include "MEMPHYS/WCDigi.hh"

//Geant4
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"

//std
#include <sstream>

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {
  G4Allocator<WCDigi> WCDigiAllocator;
}

MEMPHYS::WCDigi::WCDigi() : tubeID(0), pe(0.), time(0.) {
}//Ctor

//-----------------------------------------------------------------------------------------------

MEMPHYS::WCDigi::~WCDigi(){;}

//-----------------------------------------------------------------------------------------------

MEMPHYS::WCDigi::WCDigi(const MEMPHYS::WCDigi& right) : G4VDigi() {
  tubeID = right.tubeID; 
  pe     = right.pe;
  time   = right.time;
}//Ctor

//-----------------------------------------------------------------------------------------------

const MEMPHYS::WCDigi& MEMPHYS::WCDigi::operator=(const MEMPHYS::WCDigi& right) {
  if(&right!=this){
    tubeID = right.tubeID; 
    pe     = right.pe;
    time   = right.time;
  }
  return *this;
}//op=

//-----------------------------------------------------------------------------------------------

int MEMPHYS::WCDigi::operator==(const MEMPHYS::WCDigi& right) const { 
  return ( (tubeID==right.tubeID) && (pe==right.pe) && (time==right.time) ); 
}//op==

//-----------------------------------------------------------------------------------------------

void MEMPHYS::WCDigi::Draw() {;}

//-----------------------------------------------------------------------------------------------

void MEMPHYS::WCDigi::Print() {
  G4cout << "TubeID: " << tubeID 
	 << " PE: "    << pe
	 << " Time:"   << time << G4endl;
}//Print

//-----------------------------------------------------------------------------------------------
const std::map<G4String,G4AttDef>* MEMPHYS::WCDigi::GetAttDefs() const {
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
}//GetAttDefs

//----------------------------------------------------------------------------------------------------

std::vector<G4AttValue>* MEMPHYS::WCDigi::CreateAttValues() const {

  std::ostringstream s;
  
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;
  
  s.str("");
  s << pLogV;
  values->push_back(G4AttValue("LV",s.str(),""));

  s.str("");
  s << &transform;
  values->push_back(G4AttValue("TSF",s.str(),""));

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
}//CreateAttValues
