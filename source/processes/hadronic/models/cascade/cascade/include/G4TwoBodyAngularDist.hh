//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
// Author:  Michael Kelsey (SLAC)
// Date:    21 February 2013
//
// Description: Singleton class to evaluate two-body angular distribution
//		functions based on intial/final state codes.
//
// 20130307  M. Kelsey -- Add verbosity interface for contained objects
// 20130422  M. Kelsey -- Add three-body distributions, for temporary use

#ifndef G4TwoBodyAngularDist_h
#define G4TwoBodyAngularDist_h 1

#include "globals.hh"

class G4VTwoBodyAngDst;
class G4GamP2NPipAngDst;
class G4GamP2PPi0AngDst;
class G4PP2PPAngDst;
class G4NP2NPAngDst;
class G4NuclNuclAngDst;
class G4PiNInelasticAngDst;
class G4HadNElastic1AngDst;
class G4HadNElastic2AngDst;
class G4GammaNuclAngDst;

class G4HadNucl3BodyAngDst;	// TEMPORARY, until migration to GENBOD
class G4NuclNucl3BodyAngDst;


class G4TwoBodyAngularDist {
public:
  ~G4TwoBodyAngularDist();

  static const G4TwoBodyAngularDist* GetInstance() { return &theInstance; }

  // Return appropriate generator for initial, final state, and kw flag
  static const G4VTwoBodyAngDst* GetDist(G4int is, G4int fs, G4int kw) {
    return theInstance.ChooseDist(is,fs,kw);
  }

  static const G4VTwoBodyAngDst* GetDist(G4int is) {
    return theInstance.ChooseDist(is,0,0);
  }

  // Pass verbosity through to owned objects
  static void setVerboseLevel(G4int vb=0);

private:
  // Constructor is private for singleton
  G4TwoBodyAngularDist();
  const G4VTwoBodyAngDst* ChooseDist(G4int is, G4int fs, G4int kw) const;

  void passVerbose(G4int verbose);

  static const G4TwoBodyAngularDist theInstance;

  // Generators for various initial/final state combinations
  G4GamP2NPipAngDst* gp_npip;		// gamma p -> n pi+
  G4GamP2PPi0AngDst* gp_ppi0;		// gamma p -> p pi0
  G4PP2PPAngDst* ppAngDst;              // pp, nn elastic
  G4NP2NPAngDst* npAngDst;              // np and pn elastic
  G4NuclNuclAngDst* nnAngDst;		// Y N elastic and inelastic
  G4PiNInelasticAngDst* qxAngDst;	// pi N charge/strangeness exchange
  G4HadNElastic1AngDst* hn1AngDst;	// pi+p and related elastic scattering
  G4HadNElastic2AngDst* hn2AngDst;	// pi-p and related elastic scattering
  G4GammaNuclAngDst* gnAngDst;		// gamma N inelastic

  // TEMPORARY generators for three-body final states
  G4HadNucl3BodyAngDst* hn3BodyDst;	// (pi,K,Y,g) N -> XYZ scattering
  G4NuclNucl3BodyAngDst* nn3BodyDst;	// N N -> XYZ scattering
};

#endif	/* G4TwoBodyAngularDist_h */
