//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: Em8EventActionMessenger.hh,v 1.3 2001-07-11 09:57:54 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef Em8EventActionMessenger_h
#define Em8EventActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class Em8EventAction;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class Em8EventActionMessenger: public G4UImessenger
{
  public:
    Em8EventActionMessenger(Em8EventAction*);
   ~Em8EventActionMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    Em8EventAction* eventAction;   
    G4UIcmdWithAnInteger* setVerboseCmd;
    G4UIcmdWithAString*   DrawCmd;
    G4UIcmdWithAnInteger* PrintCmd;
};

#endif
