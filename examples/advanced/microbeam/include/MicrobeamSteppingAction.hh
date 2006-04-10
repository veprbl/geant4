// -------------------------------------------------------------------
// $Id: MicrobeamSteppingAction.hh,v 1.2 2006-04-10 14:47:31 sincerti Exp $
// -------------------------------------------------------------------

#ifndef MicrobeamSteppingAction_h
#define MicrobeamSteppingAction_h 1

#include "G4UserSteppingAction.hh"

#include "MicrobeamRunAction.hh"
#include "MicrobeamDetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class MicrobeamSteppingAction : public G4UserSteppingAction
{
public:
  MicrobeamSteppingAction(MicrobeamRunAction* ,MicrobeamDetectorConstruction*);
  ~MicrobeamSteppingAction();
  
  void UserSteppingAction(const G4Step*);
  
private:
  MicrobeamRunAction*            Run;
  MicrobeamDetectorConstruction* Detector;
  G4float massPhantom;

};

#endif




