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
// $Id: G4QPDGCodeVector.hh,v 1.2 2001-07-11 10:04:00 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#ifndef G4QPDGCodeVector_h
#define G4QPDGCodeVector_h 1

// ------------------------------------------------------------
//      GEANT 4 class header file
//
//      ---------------- G4QCandidateVector ----------------
//             by Mikhail Kossov, Sept 1999.
// Type defenition for Hadron definition in CHIPS model
// ---------------------------------------------------------------

#include "G4QPDGCode.hh"
#include "g4rw/tpordvec.h"

typedef G4RWTPtrOrderedVector<G4QPDGCode> G4QPDGCodeVector;

#endif


