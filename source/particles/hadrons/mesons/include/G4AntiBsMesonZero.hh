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
// $Id: G4AntiBsMesonZero.hh,v 1.4 2001-07-11 10:01:45 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
//      GEANT 4 class header file
//
//
//      Created,             Hisaya Kurashige, 15 June 1997
// **********************************************************************
//  Change both methods to get the pointer into non-inlined H.Kurashige 4 Aug. 1998
// ----------------------------------------------------------------

// Each class inheriting from G4VMeson
// corresponds to a particle type; one and only one
// instance for each class is guaranteed.

#ifndef G4AntiBsMesonZero_h
#define G4AntiBsMesonZero_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4VMeson.hh"

// ######################################################################
// ###                         AntiBsMesonZero                         ###
// ######################################################################

class G4AntiBsMesonZero : public G4VMeson
{
 private:
   static G4AntiBsMesonZero theAntiBsMesonZero;
   static G4double  theAntiBsMesonZeroLengthCut;
   static G4double* theAntiBsMesonZeroKineticEnergyCuts;

 private: // constructors are hide as private  
   G4AntiBsMesonZero(
       const G4String&     aName,        G4double            mass,
       G4double            width,        G4double            charge,   
       G4int               iSpin,        G4int               iParity,    
       G4int               iConjugation, G4int               iIsospin,   
       G4int               iIsospin3,    G4int               gParity,
       const G4String&     pType,        G4int               lepton,      
       G4int               baryon,       G4int               encoding,
       G4bool              stable,       G4double            lifetime,
       G4DecayTable        *decaytable
   );

 public:
   virtual ~G4AntiBsMesonZero(){} 

   static G4AntiBsMesonZero* AntiBsMesonZeroDefinition();
   static G4AntiBsMesonZero* AntiBsMesonZero();
   static G4double GetCuts() {return theAntiBsMesonZeroLengthCut;}   
   static G4double* GetCutsInEnergy() {return theAntiBsMesonZeroKineticEnergyCuts;};

   virtual void SetCuts(G4double aCut); 
};


#endif
