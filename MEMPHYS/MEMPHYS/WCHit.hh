#ifndef MEMPHYSWCHit_h
#define MEMPHYSWCHit_h

//Geant4
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Transform3D.hh"
#include "G4LogicalVolume.hh"

#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"

#include <inlib/pointer>
#include <inlib/forit>

#include <iomanip>

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {

class WCHit : public G4VHit {
 public:
  
  WCHit():tubeID(0),pLogV(0),totalPe(0),total_edep(0),totalPeInGate(0){}

  virtual ~WCHit() {}
  G4int operator==(const WCHit& right) const {return (this==&right) ? 1 : 0;}
  
  void* operator new(size_t) {return (void *) allocator()->MallocSingle();}
  void operator delete(void *aHit) {allocator()->FreeSingle((WCHit*) aHit);}
  
  void Draw() {
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if(!pVVisManager) return; //JEC 16/11/05 no visualisation manager => return
    G4String volumeName  = pLogV->GetName();
    if ( volumeName != "WCPMT" ) return; //JEC 16/11/05 not a WC PMT => return
    //JEC 1/12/05 use transfomation defined by WCSD  G4Transform3D trans(rot, pos);
    G4VisAttributes attribs;
    //       G4cout << "PE: " << pe << " Max Pe " << maxPe << G4endl;
    if ( totalPe > maxPe()*.05 ) {      
      G4Colour _colour(1.,1.-(float(totalPe-.05*maxPe())/float(.95*maxPe())),0.0);
      attribs.SetColour(_colour);
      attribs.SetForceSolid(true);
      pVVisManager->Draw(*pLogV,attribs,transform);
    }
  }

  void Print() {
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
  }

  //JEC 1/12/05 implement same mechanism as Trajectory
  virtual const std::map<G4String,G4AttDef>* GetAttDefs() const {
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
  }

  virtual std::vector<G4AttValue>* CreateAttValues() const {
    std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

    std::string sv;
    inlib::p2s(pLogV,sv); //pointer <=> string must be consistent with what is done in exlib::geant4::utils.
    values->push_back(G4AttValue("LV",sv,""));

    inlib::p2s(&transform,sv);
    values->push_back(G4AttValue("TSF",sv,""));

    inlib::p2s(&time,sv);
    values->push_back(G4AttValue("Times",sv,""));

    //JEC 6/4/06 add vector of particle ids
    inlib::p2s(&trkId,sv);
    values->push_back(G4AttValue("TrkID",sv,""));
    
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
    s << totalPe;
    values->push_back(G4AttValue("TotalPe",s.str(),""));

    return values;
  }
  
 public:
  
  void SetTubeID       (G4int tube)                 { tubeID = tube; }
  //JEC 6/4/06  void SetTrackID      (G4int track)                { trackID = track; }
  //JEC 6/4/06  void SetEdep         (G4double de)                { edep = de; }
  void SetTransform (G4Transform3D aTrans) { transform = aTrans; }
  void SetLogicalVolume(G4LogicalVolume* logV)      { pLogV = logV;}
  void SetColour(G4Colour aColour) { colour = aColour; }
  void UpdateColour() {
    //modify color at the end of the event
    if ( totalPe > maxPe()*.05 ) {      
      G4Colour aColour(1.,1.-(float(totalPe-.05*maxPe())/float(.95*maxPe())),0.0);
      colour = aColour;
    }
  }

  // This is temporarily used for the drawing scale
  void SetMaxPe(G4int number = 0)  {maxPe() = number;}

  void AddPe(G4float hitTime) {
    // First increment the totalPe number
    totalPe++; 
    if (totalPe > maxPe()) maxPe() = totalPe;
    time.push_back(hitTime);
  }

  void AddTrk(G4int trackID) {trkId.push_back(trackID);}

  void AddEdep(G4double energyDeposition) {
    edep.push_back(energyDeposition);
    total_edep += energyDeposition;
  }//AddEdep


  G4int         GetTubeID()     { return tubeID; }
  //obsolete G4int         GetTrackID()    { return trackID; }
  G4int         GetTotalPe()    { return totalPe;}
  G4float       GetTime(G4int i)  { return time[i]; }
  G4int         GetTrack(G4int i) {return trkId[i];}
  
  G4LogicalVolume* GetLogicalVolume() { return pLogV; }

  const G4Transform3D& GetTransform()  const { return transform; }
  const G4Colour& GetColour() const { return colour; }
  
  
  void SortHitTimes() {   sort(time.begin(),time.end()); }

  G4int GetPeInGate(double pmtgate,double evgate) {
    // M Fechner; april 2005
    // assumes that time has already been sorted
    // pmtgate  and evgate are durations, ie not absolute times
    std::vector<G4float>::iterator tfirst = time.begin();
    std::vector<G4float>::iterator tlast = time.end();
    // select min time
    G4float mintime = (pmtgate < evgate) ? pmtgate : evgate;
  
    // return number of hits in the time window...
    G4int number = 0;
    inlib_vforcit(float,time,it) {if((*it)<=mintime) number++;}
  
    //G4int number = std::count_if(tfirst,
    //			       tlast, 
    //			       std::bind2nd(std::less_equal<G4float>(),mintime) ); //std::bind2nd is deprecated.
  
    totalPeInGate = number;
    return number;
  }

  G4double GetTotalEdep()    { return total_edep;}

 private:
  WCHit(const WCHit&) {}
  const WCHit& operator=(const WCHit&) {return *this;}
  
  G4int            tubeID;
  //JEC 6/4/06 G4int            trackID;
  //JEC 6/4/06 G4double         edep;

  //JEC to GB
  G4Colour         colour;
  G4Transform3D    transform;
  G4LogicalVolume* pLogV;
  G4int                 totalPe;
  std::vector<G4float>  time;
  std::vector<G4int>    trkId;   //JEC 6/4/06
  std::vector<G4double> edep; //JEC 6/4/06
  G4double              total_edep; //GB 1/Aug/2017
  
  //for digitization
  G4int                 totalPeInGate;

  // This is temporarily used for the drawing scale
  // Since its static *every* WChit sees the same value for this.
  static G4int& maxPe() {
    static G4int s_maxPe = 0; //JEC FIXME is it necessary (only use for Colour definition)
    return s_maxPe;
  }

  static G4Allocator<WCHit>* allocator() {
    //warning : it can't be on the stack, since it is managed by the G4RunManager.
    static G4Allocator<WCHit>* s_allocator = new G4Allocator<WCHit>;
    return s_allocator;
  }

};
}

//----------------------------------------------------------------

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {
  typedef G4THitsCollection<WCHit> WCHitsCollection;
}

#endif
