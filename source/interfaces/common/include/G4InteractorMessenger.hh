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

#ifndef G4InteractorMessenger_h
#define G4InteractorMessenger_h 1

#include "G4UImessenger.hh"

class G4VInteractiveSession;
class G4UIdirectory;
class G4UIcommand;

class G4InteractorMessenger : public G4UImessenger 
{
public:
  G4InteractorMessenger (G4VInteractiveSession* session);
  virtual ~G4InteractorMessenger ();
  void SetNewValue(G4UIcommand* command,G4String newValue);
private:
  G4VInteractiveSession* session;
  G4UIdirectory* interactorDirectory;
  G4UIcommand* addMenu;
  G4UIcommand* addButton;
  G4UIcommand* sys;
};

#endif
