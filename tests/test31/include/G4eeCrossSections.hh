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
// $Id: G4eeCrossSections.hh,v 1.1 2004-08-19 16:30:06 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4eeCrossSections
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 15.08.2004
//
// Modifications:
//

//
// Class Description:
//

// -------------------------------------------------------------------
//

#ifndef G4eeCrossSections_h
#define G4eeCrossSections_h 1

#include "G4ThreeVector.hh"
#include "globals.hh"
#include <complex>

class G4PhysicsVector;

class G4eeCrossSections 
{

public:

  G4eeCrossSections();

  virtual ~G4eeCrossSections();

  G4double CrossSection2pi(G4double);

  G4double CrossSection3pi(G4double);

  G4double CrossSection2Kcharged(G4double);

  G4double CrossSection2Kneutral(G4double);

private:

  void Initialise();

  G4double Width2p(G4double s,  G4double mres, G4double g, 
                   G4double br, G4double mp);

  G4double Width3p(G4double s,  G4double mres, G4double g, 
                   G4double br);

  G4double PhaseSpace3p(G4double e); 

  G4double WidthPg(G4double s,  G4double mres, G4double g, 
                   G4double br, G4double mp);

  G4double WidthRho(G4double e);

  G4double WidthOm(G4double e);

  G4double WidthPhi(G4double e);

  std::complex<G4double> DpRho(G4double e); 

  std::complex<G4double> DpOm(G4double e);

  std::complex<G4double> DpPhi(G4double e);


  // hide assignment operator
  G4eeCrossSections & operator=(const  G4eeCrossSections &right);
  G4eeCrossSections(const  G4eeCrossSections&);

  G4double MsPi, MsPi0, MsEta, MsEtap, MsKs, MsKc, MsRho, MsOm;
  G4double MsF0, MsA0, MsPhi, MsK892, MsK0892;
  G4double GRho, GOm, GPhi, GK892, GK0892, PhRho, PhOm, PhPhi, PhRhoPi;
  G4double BrRhoPiG, BrRhoPi0G, BrRhoEtaG, BrRhoEe, BrOm3Pi;
  G4double BrOmPi0G, BrOmEtaG, BrOm2Pi, PhOm2Pi, BrOmEe;
  G4double BrPhi2Kc, BrPhiKsKl, BrPhi3Pi, BrPhiPi0G, BrPhiEtaG;
  G4double BrPhi2Pi, PhPhi2Pi, BrPhiEe, MeVnb, Alpha;
  G4double AOmRho, ARhoPRho, cterm, mssig, gsig, brsigpipi;
  G4double msrho1450, msrho1700, grho1450, grho1700;
  G4double arhoompi0, arho1450ompi0, arho1700ompi0, phrhoompi0;
  G4double phrho1450ompi0, phrho1700ompi0, aomrhopi0, phomrhopi0;
  G4double arhopi0pi0g, aompi0pi0g, phrhopi0pi0g, phompi0pi0g;
  G4double brrho1450ompi0, brrho1450pipi, brrho1700ompi0;
  G4double brrho1700pipi, aphirhopi0, phphirhopi0;
  G4double arhosigg, phrhosigg, aomsigg, phomsigg;

  G4double MsRho3, MsOm3, MsPhi3;

  G4PhysicsVector*  ph3p;

};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif
