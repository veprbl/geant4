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
// $Id: G4UnstableFermiFragment.cc,v 1.2 2003-11-03 17:53:02 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Nov 1998)

#include "G4UnstableFermiFragment.hh"
#include "G4FermiPhaseSpaceDecay.hh"
#include <numeric>

G4UnstableFermiFragment::G4UnstableFermiFragment()
{
}

G4UnstableFermiFragment::G4UnstableFermiFragment(const G4UnstableFermiFragment &) : G4VFermiFragment()
{
  throw G4HadronicException(__FILE__, __LINE__, "G4UnstableFermiFragment::copy_constructor meant to not be accessable");
}


G4UnstableFermiFragment::~G4UnstableFermiFragment()
{
}

  
const G4UnstableFermiFragment & G4UnstableFermiFragment::operator=(const G4UnstableFermiFragment &)
{
  throw G4HadronicException(__FILE__, __LINE__, "G4UnstableFermiFragment::operator= meant to not be accessable");
  return *this;
}


G4bool G4UnstableFermiFragment::operator==(const G4UnstableFermiFragment &) const
{
  return false;
}

G4bool G4UnstableFermiFragment::operator!=(const G4UnstableFermiFragment &) const
{
  return true;
}



G4FragmentVector * G4UnstableFermiFragment::GetFragment(const G4LorentzVector& aMomentum) const
{
  G4FermiPhaseSpaceDecay thePhaseSpace;

  std::vector<G4LorentzVector*> * P_i = thePhaseSpace.Decay(aMomentum.m(), Masses);

  G4ThreeVector Beta = aMomentum.boostVector();

  G4FragmentVector * theResult = new G4FragmentVector;

  for (std::vector<G4LorentzVector*>::iterator i = P_i->begin(); i != P_i->end(); i++)
    {
      G4int n = std::distance(P_i->begin(),i);
      (*i)->boost(Beta);
      theResult->push_back(new G4Fragment(static_cast<G4int>(AtomNum[n]),
					  static_cast<G4int>(Charges[n]),
					  *(*i)));
      
      delete *i;
    }
  delete P_i;
  
  return theResult;
}
