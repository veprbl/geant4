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
// $Id: G4ggclos.cc,v 1.7 2001-07-11 09:59:00 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#include "G3toG4.hh"
#include "G3VolTable.hh"

void PG4ggclos(){
  G4ggclos();
}

void G4ggclos(){
  G4cout << "G4ggclos: setting top-level VolTableEntry" << G4endl;
  G3Vol.SetFirstVTE();
}
