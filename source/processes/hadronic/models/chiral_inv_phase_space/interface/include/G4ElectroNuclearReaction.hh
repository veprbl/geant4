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
//
//
// $Id: G4ElectroNuclearReaction.hh,v 1.24 2008-09-01 17:30:42 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// GEANT4 physics class: G4ElectroNuclearReaction -- header file
// Created: J.P. Wellisch, 12/11/2001
// The last update: J.P. Wellisch, 06-June-02
//
#ifndef G4ElectroNuclearReaction_h
#define G4ElectroNuclearReaction_h 1

#include "globals.hh"
#include "G4HadronicInteraction.hh"
#include "G4ChiralInvariantPhaseSpace.hh"
#include "G4ElectroNuclearCrossSection.hh"
#include "G4PhotoNuclearCrossSection.hh"
#include "G4GammaParticipants.hh"
#include "G4QGSModel.hh"
#include "G4QGSMFragmentation.hh"

class G4TheoFSGenerator;
class G4GeneratorPrecompoundInterface;
class G4ExcitedStringDecay;

class G4ElectroNuclearReaction : public G4HadronicInteraction
{
public: 

  G4ElectroNuclearReaction();

  virtual ~G4ElectroNuclearReaction();
    
  virtual G4HadFinalState* ApplyYourself(const G4HadProjectile& aTrack, 
					 G4Nucleus& aTargetNucleus);

private:

  G4ChiralInvariantPhaseSpace theLEModel;
  G4TheoFSGenerator * theHEModel;
  G4GeneratorPrecompoundInterface * theCascade;
  G4QGSModel< G4GammaParticipants > theStringModel;
  G4QGSMFragmentation theFragmentation;
  G4ExcitedStringDecay * theStringDecay;
  G4ElectroNuclearCrossSection theElectronData;
  G4PhotoNuclearCrossSection thePhotonData;
  G4HadFinalState theResult;
};

#endif
