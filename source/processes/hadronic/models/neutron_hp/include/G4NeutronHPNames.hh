// This code implementation is the intellectual property of
// neutron_hp -- header file
// J.P. Wellisch, Nov-1996
// A prototype of the low energy neutron transport model.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4NeutronHPNames.hh,v 1.7 2001-05-29 14:13:09 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
#ifndef G4NeutronHPNames_h
#define G4NeutronHPNames_h 1

#include "G4ios.hh"
#include "g4std/fstream"
#include "g4std/strstream"
#include <stdlib.h>
#include "globals.hh"
#include "G4NeutronHPDataUsed.hh"

class G4NeutronHPNames
{
  public:
  
  G4NeutronHPNames(){theMaxOffSet = 5;}
  G4NeutronHPNames(G4int maxOffSet){theMaxOffSet = maxOffSet;}
  ~G4NeutronHPNames(){}
  
  G4NeutronHPDataUsed GetName(G4int A, G4int Z, G4String base, G4String rest, G4bool & active);
  G4String GetName(G4int i) { return theString[i]; }
  void SetMaxOffSet(G4int anOffset) { theMaxOffSet = anOffset; }
  
  public:
  
  static const G4String theString[99];
  G4int theMaxOffSet;
  G4String itoa(int current)
  {
    const char theDigits[11] = "0123456789";
    G4String result;
    int digit;
    do
    {
      digit = current-10*(current/10);
      result=theDigits[digit]+result;
      current/=10;
    }
    while(current!=0);
    return result;
  }
};

#endif
