#ifndef G4NeutronIsotopeProduction_h
#define G4NeutronIsotopeProduction_h

#include "globals.hh"
#include "G4VIsotopeProduction.hh"
#include "G4NeutronElementIsoCrossSections.hh"

class G4NeutronIsotopeProduction : public G4VIsotopeProduction
{
  public:
  
  G4NeutronIsotopeProduction();
  ~G4NeutronIsotopeProduction();

  G4IsoResult * GetIsotope(const G4Track & aTrack, const G4Nucleus & aNucleus);

  private:
    
  G4NeutronElementIsoCrossSections ** theData;
  G4int numberOfElements;
};

#endif
