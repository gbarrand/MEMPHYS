#include "../MEMPHYS/StackingAction.hh"

//Geant4
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "Randomize.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

//MEMPHYS
#include "../MEMPHYS/DetectorConstruction.hh"



MEMPHYS::StackingAction::StackingAction() {;}
MEMPHYS::StackingAction::~StackingAction(){;}


G4ClassificationOfNewTrack MEMPHYS::StackingAction::ClassifyNewTrack (const G4Track* aTrack) {
  G4ClassificationOfNewTrack classification    = fWaiting;
  G4ParticleDefinition*      particleType      = aTrack->GetDefinition();

  if( particleType == G4OpticalPhoton::OpticalPhotonDefinition() ) {
    const G4float wavelength[20] = 
      { 280., 300., 320., 340., 360., 380., 400., 420., 440., 460.,  
	480., 500., 520., 540., 560., 580., 600., 620., 640., 660.};

    const G4float qEfficiency[20] =
      { 0.00, .0139, .0854, .169,  .203,  .206,  .211,   .202,   .188,   .167, 
	.140,  .116, .0806, .0432, .0265, .0146, .00756, .00508, .00158, 0.00};

    G4float photonWavelength = (2.0*M_PI*197.3)/(aTrack->GetTotalEnergy()/eV);
    G4float wavelengthQE  = 0.0;
    
    if ( photonWavelength <= 280.0 || photonWavelength >= 660.0) return fKill;

    //interpolation of the Quantum efficiency
    for (int i=0; i<=18; i++) {
      if ( photonWavelength <= wavelength[i+1]) {
	wavelengthQE = qEfficiency[i] + 
	  (qEfficiency[i+1]-qEfficiency[i])/(wavelength[i+1]-wavelength[i])*
	  (photonWavelength - wavelength[i]);
	break;
      }
    }

//    if( G4UniformRand() > wavelengthQE )
// MF : better to increase the number of photons
// than to throw in a global factor  at Digitization time !
    if( G4UniformRand() > wavelengthQE/(1.0-0.25) ) //JEC FIXME this increase artificially the QE?
      classification = fKill;
  }
  
  return classification;
}//ClassifyNewTrack

void MEMPHYS::StackingAction::NewStage() {;}
void MEMPHYS::StackingAction::PrepareNewEvent() {;}




