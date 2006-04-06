// -------------------------------------------------------------------
// $Id: MicrobeamTrackingAction.hh,v 1.1 2006-04-06 15:32:44 sincerti Exp $
// -------------------------------------------------------------------

#ifndef MicrobeamTrackingAction_h
#define MicrobeamTrackingAction_h 1

#include "G4UserTrackingAction.hh"

class MicrobeamRunAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class MicrobeamTrackingAction : public G4UserTrackingAction 
{

  public:  
    MicrobeamTrackingAction(MicrobeamRunAction*);
   ~MicrobeamTrackingAction() {};
   
    void PostUserTrackingAction(const G4Track*);
    
  private:
   MicrobeamRunAction * Run;  
};

#endif
