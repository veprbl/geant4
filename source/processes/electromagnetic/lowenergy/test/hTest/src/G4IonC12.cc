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
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
//
// ClassName:   G4IonC12
//  
// Description: Static C12+ ion
//
// Authors:   08.04.01  V.Ivanchenko 
//
// Modified:
//
//----------------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

//#include "g4std/fstream"
//#include "g4std/iomanip"

#include "G4IonC12.hh"

// ######################################################################
// ###                           IonC12                                ###
// ######################################################################

G4IonC12::G4IonC12(
       const G4String&     aName,        G4double            mass,
       G4double            width,        G4double            charge,   
       G4int               iSpin,        G4int               iParity,    
       G4int               iConjugation, G4int               iIsospin,   
       G4int               iIsospin3,    G4int               gParity,
       const G4String&     pType,        G4int               lepton,      
       G4int               baryon,       G4int               encoding,
       G4bool              stable,       G4double            lifetime,
       G4DecayTable        *decaytable )
 : G4VIon( aName,mass,width,charge,iSpin,iParity,
           iConjugation,iIsospin,iIsospin3,gParity,pType,
           lepton,baryon,encoding,stable,lifetime,decaytable )
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4IonC12::~G4IonC12()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

// ......................................................................
// ...                 static member definitions                      ...
// ......................................................................
//     
//    Arguments for constructor are as follows
//               name             mass          width         charge
//             2*spin           parity  C-conjugation
//          2*Isospin       2*Isospin3       G-parity
//               type    lepton number  baryon number   PDG encoding
//             stable         lifetime    decay table 

G4IonC12 G4IonC12::theIonC12(
              "IonC12",    11.14945*GeV,       0.0*MeV,  +6.0*eplus, 
		    0,              +1,             0,          
		    0,               0,             0,             
	    "static_nucleus",        0,            +12,           0,
		 true,            -1.0,          NULL
);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4IonC12* G4IonC12::IonC12Definition(){return &theIonC12;}
// initialization for static cut values
G4double   G4IonC12::theIonC12LengthCut;
G4double*  G4IonC12::theIonC12KineticEnergyCuts;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

