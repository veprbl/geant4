// -------------------------------------------------------------------
// $Id: MicrobeamEventAction.hh,v 1.2 2006-04-10 14:47:31 sincerti Exp $
// -------------------------------------------------------------------

#ifndef MicrobeamEventAction_h
#define MicrobeamEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class MicrobeamRunAction;
class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class MicrobeamEventAction : public G4UserEventAction
{
  public:
  
    MicrobeamEventAction(MicrobeamRunAction*);
   ~MicrobeamEventAction();

    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);
    
    void SetDrawFlag   (G4String val)  {drawFlag    = val;};
    void SetPrintModulo(G4int    val)  {printModulo = val;};
        
  private:
  
    MicrobeamRunAction*      Run;
    G4String                 drawFlag;
    G4int                    printModulo;         
};

#endif

    
