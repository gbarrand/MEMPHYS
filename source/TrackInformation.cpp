#include "../MEMPHYS/TrackInformation.hh"
#include "G4ios.hh"

//JEC 10/1/06 introduce MEMPHYS
namespace MEMPHYS {
  G4Allocator<TrackInformation> aTrackInfoAllocator;
}

MEMPHYS::TrackInformation::TrackInformation(const G4Track*)
{
  saveit = false;
  drawit = false;
  status = active;
}//Ctor


//------------------------------------------------------------------------
void MEMPHYS::TrackInformation::AddTrackStatusFlag(int s)
{
  if(s&active) //track is now active
    status&=~inactive; //remove any flags indicating it is inactive 
  else if(s&inactive) //track is now inactive
    status&=~active; //remove any flags indicating it is active
  status|=s; //add new flags
}//AddTrackStatusFlag

//------------------------------------------------------------------------
void MEMPHYS::TrackInformation::Print() const
{
  G4cout << "TrackInformation : [" << saveit 
	 << "]-[" << drawit 
	 << "]-[" << status << "]"
	 << G4endl;
}//Print


