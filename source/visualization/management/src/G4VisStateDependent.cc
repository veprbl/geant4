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
// $Id: G4VisStateDependent.cc,v 1.3 2001-07-11 10:09:20 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#include "G4VisStateDependent.hh"

#include "G4VisManager.hh"
#include "G4StateManager.hh"

G4VisStateDependent::G4VisStateDependent (G4VisManager* pVisManager):
  fpVisManager (pVisManager) {}

G4bool G4VisStateDependent::Notify (G4ApplicationState requestedState) {
  G4StateManager* stateManager = G4StateManager::GetStateManager ();
  if(stateManager -> GetPreviousState () == EventProc &&
     requestedState == GeomClosed) {
    fpVisManager -> EndOfEvent ();
  }
  return true;
}
