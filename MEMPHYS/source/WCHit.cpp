#include "../MEMPHYS/WCHit.hh"

//Geant4
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4ParticleTypes.hh"
#include "G4Circle.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"

#include <inlib/pointer>

//std
#include <sstream>
#include <iomanip>

G4int MEMPHYS::WCHit::maxPe = 0; //JEC FIXME is it necessary (only use for Colour definition)

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {
  G4Allocator<WCHit> WCHitAllocator;
}

MEMPHYS::WCHit::WCHit()
:tubeID(0),pLogV(0),totalPe(0),total_edep(0),totalPeInGate(0)
{}


//-----------------------------------------------------------------------------------------

MEMPHYS::WCHit::~WCHit() {}

//-----------------------------------------------------------------------------------------

G4int MEMPHYS::WCHit::operator==(const MEMPHYS::WCHit& right) const {
  return (this==&right) ? 1 : 0;
}//op==

//-----------------------------------------------------------------------------------------
void MEMPHYS::WCHit::UpdateColour() {
  if ( totalPe > maxPe*.05 ) {      
    G4Colour aColour(1.,1.-(float(totalPe-.05*maxPe)/float(.95*maxPe)),0.0);
    colour = aColour;
  }
}//UpdateColour
//-----------------------------------------------------------------------------------------

void MEMPHYS::WCHit::Draw() {
  
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(!pVVisManager) return; //JEC 16/11/05 no visualisation manager => return
  
  G4String volumeName  = pLogV->GetName();
  if ( volumeName != "WCPMT" ) return; //JEC 16/11/05 not a WC PMT => return
  
  //JEC 1/12/05 use transfomation defined by WCSD  G4Transform3D trans(rot, pos);
  G4VisAttributes attribs;
  
  //       G4cout << "PE: " << pe << " Max Pe " << maxPe << G4endl;
  
  if ( totalPe > maxPe*.05 ) {      
    G4Colour _colour(1.,1.-(float(totalPe-.05*maxPe)/float(.95*maxPe)),0.0);
    attribs.SetColour(_colour);
    attribs.SetForceSolid(true);
    pVVisManager->Draw(*pLogV,attribs,transform);
  }

}//Draw

//-----------------------------------------------------------------------------------------

const std::map<G4String,G4AttDef>* MEMPHYS::WCHit::GetAttDefs() const {
  G4bool isNew;
  std::map<G4String,G4AttDef>* store = G4AttDefStore::GetInstance("WCHit",isNew);

  if (isNew) {    
    G4String LV("LV");
    (*store)[LV] = G4AttDef(LV,"Logical Volume","Display","","G4LogicalVolume");

    G4String TSF("TSF");
    (*store)[TSF] = G4AttDef(TSF,"3D Transformation","Display","","G4Transform3D");
 
    G4String Color("Color");
    (*store)[Color] = G4AttDef(Color,"Hit Colour","Display","","G4Colour");

    G4String TubeID("TubeID");
    (*store)[TubeID] = G4AttDef(TubeID,"Tube ID","Bookkeeping","","G4int");
    
    G4String TotalPe("TotalPe");
    (*store)[TotalPe] = G4AttDef(TotalPe,"Number of Arrival Times","Physics","","G4int");

    G4String Times("Times");
    (*store)[Times] = G4AttDef(Times,"Vector of Arrival Times","Physics","","std::vector<G4float>");

    //JEC 6/4/06 add vector of particle ids
    G4String TrkID("TrkID");
    (*store)[TrkID] = G4AttDef(TrkID,"Vector of Track Id","Physics","","std::vector<G4int>");

  }
  return store;
}//GetAttDefs

//----------------------------------------------------------------------------------------------------

std::vector<G4AttValue>* MEMPHYS::WCHit::CreateAttValues() const {

  std::ostringstream s;
  
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

  //s.str("");
  //s << pLogV;
  //values->push_back(G4AttValue("LV",s.str(),""));
  std::string sv;
  inlib::p2s(pLogV,sv); //pointer <=> string must be consistent with what is done in exlib::geant4::utils.
  values->push_back(G4AttValue("LV",sv,""));

  //s.str("");
  //s << &transform;
  //values->push_back(G4AttValue("TSF",s.str(),""));
  inlib::p2s(&transform,sv);
  values->push_back(G4AttValue("TSF",sv,""));

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
  s << totalPe;
  values->push_back(G4AttValue("TotalPe",s.str(),""));

  //s.str("");
  //s << &time;
  //values->push_back(G4AttValue("Times",s.str(),""));
  inlib::p2s(&time,sv);
  values->push_back(G4AttValue("Times",sv,""));

  //JEC 6/4/06 add vector of particle ids
  //s.str("");
  //s << &trkId;
  //values->push_back(G4AttValue("TrkID",s.str(),""));
  inlib::p2s(&trkId,sv);
  values->push_back(G4AttValue("TrkID",sv,""));
    
  return values;
}//CreateAttValues

//-----------------------------------------------------------------------------------------

void MEMPHYS::WCHit::Print() {

  G4cout.setf(std::ios::fixed);
  G4cout.precision(1);

  G4cout << " Tube:"  << std::setw(4) << tubeID 
	 << " Pe:"    << totalPe
	 << "\tTime: "; 

  for (int i = 0; i < totalPe; i++) {
    G4cout << time[i]/CLHEP::ns << " ";
    if ( i%10 == 0 && i != 0) 
      G4cout << G4endl << "\t";
  }
  G4cout << G4endl;

  G4cout << "\tTrackId: ";
  for (unsigned int i=0; i<trkId.size(); ++i) {
    G4cout << trkId[i] << " ";
  }
  G4cout << G4endl;
  

}//Print

//-----------------------------------------------------------------------------------------
void MEMPHYS::WCHit::AddPe(G4float hitTime) {
  // First increment the totalPe number
  totalPe++; 
  
  if (totalPe > maxPe) maxPe = totalPe;
  
  time.push_back(hitTime);
}//AddPe

//-----------------------------------------------------------------------------------------
void MEMPHYS::WCHit::AddTrk(G4int trackID) {
  trkId.push_back(trackID);
}//AddTrk

//-----------------------------------------------------------------------------------------

void MEMPHYS::WCHit::AddEdep(G4double energyDeposition) {
  edep.push_back(energyDeposition);
  total_edep += energyDeposition;
}//AddEdep


//-----------------------------------------------------------------------------------------

G4int MEMPHYS::WCHit::GetPeInGate(double pmtgate,double evgate) {
  // M Fechner; april 2005
  // assumes that time has already been sorted
  // pmtgate  and evgate are durations, ie not absolute times
  std::vector<G4float>::iterator tfirst = time.begin();
  std::vector<G4float>::iterator tlast = time.end();
  // select min time
  G4float mintime = (pmtgate < evgate) ? pmtgate : evgate;
  
  // return number of hits in the time window...
  
  G4int number = std::count_if(tfirst,
			       tlast, 
			       std::bind2nd(std::less_equal<G4float>(),mintime) );
  totalPeInGate = number;
  return number;
}//GetPeInGate
