// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: Em5PrimaryGeneratorMessenger.hh,v 1.1 1999-10-12 12:23:31 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef Em5PrimaryGeneratorMessenger_h
#define Em5PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class Em5PrimaryGeneratorAction;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class Em5PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    Em5PrimaryGeneratorMessenger(Em5PrimaryGeneratorAction*);
   ~Em5PrimaryGeneratorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    Em5PrimaryGeneratorAction* Em5Action; 
    G4UIcmdWithoutParameter*   DefaultCmd;
};

#endif

