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
// * authors in the GEANT4 collaboration.                             *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// MODULE:		SSAEventActionMessenger.hh
//
// Version:		0.b.4
// Date:		16/08/99
// Author:		F Lei
// Organisation:	DERA UK
// Customer:		ESA/ESTEC, NOORDWIJK
// Contract:		12115/JG/NL Work Order No. 3
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// DESCRIPTION
// -----------
//
// The SSAEventActionMessenger is instatiated by the SSARunManager and introduces 
// into the UI event menu additional command to control the drawing of event
// trajectory. User can choose one from
//       1) none: no particle trajectory will be drawn.
//       2) charged: only for charged particles.
//       3) all: for all particles
// The default option is for all particles.  
//        
// The SSAEventActionMessenger modifies the state of the event Drawing flag 
// according to UI menu command issued by the user.
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// PUBLIC MEMBER FUNCTIONS
// -----------------------
//
// SSAEventActionMessenger (SSAEventAction*)
//    Constructor:  Defines the commands available to change the DrawFlag
//    status. 
//
// ~SSAEventActionMessenger ()
//    Destructor deletes G4UIdirectory and G4UIcommand objects.
//
// void SetNewValue (G4UIcommand *command, G4String newValues)
//    Identifies the command which has been invoked by the user, extracts the
//    parameters associated with that command (held in newValues, and uses
//    these values with the appropriate member function.
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
// CHANGE HISTORY
// --------------
//
// 16 August 1999, F Lei, DERA UK
// Adapted from a verson by Bill Lockman, SLAC, to whom all credits go:
//
// $Id: exrdm02EventActionMessenger.hh,v 1.1 2003-10-08 16:31:50 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// 

#ifndef exrdm02EventActionMessenger_h
#define exrdm02EventActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class exrdm02EventAction;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class exrdm02EventActionMessenger: public G4UImessenger
{
  public:
    exrdm02EventActionMessenger(exrdm02EventAction*);
   ~exrdm02EventActionMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    exrdm02EventAction*   eventAction;   
    G4UIcmdWithAString* DrawCmd;
};

#endif






