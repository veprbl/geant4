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
// $Id: G4Exception.cc,v 1.10 2001-07-11 10:00:56 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ----------------------------------------------------------------------
// G4Exception
//
// Global error function prints string to G4cerr, and aborts
// program
//
// History:
// 30.06.95 P.Kent

#include "G4ios.hh"
#include <stdlib.h>
#include "g4rw/cstring.h"
#include "G4StateManager.hh"

void G4Exception(const char* s)
{
   if(s)
	{
	    G4cerr << s << G4endl;
	}
   G4cerr << G4endl << "*** G4Exception: Aborting execution ***" << G4endl;
   G4StateManager::GetStateManager()->SetNewState(Abort);
   abort();
}

void G4Exception(G4std::string s)
{
  G4Exception(s.c_str());
}

void G4Exception(G4String s)
{
  G4Exception(s.c_str());
}
