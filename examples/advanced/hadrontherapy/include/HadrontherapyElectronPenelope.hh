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
// $Id: HadrontherapyElectronPenelope.hh,v 1.2 2005-04-28 20:39:33 mpiergen Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
// Class description:
// System test for e/gamma, electron processes a' la Penelope for PhysicsList
// Further documentation available from http://www.ge.infn.it/geant4/lowE

// -------------------------------------------------------------------

#ifndef HADRONTHERAPYELECTRONPENELOPE_HH
#define HADRONTHERAPYELECTRONPENELOPE_HH 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class HadrontherapyElectronPenelope : public G4VPhysicsConstructor {

public: 

  HadrontherapyElectronPenelope(const G4String& name = "electron-penelope");
  
  virtual ~HadrontherapyElectronPenelope();
  
  // This method is dummy for physics
  virtual void ConstructParticle() {};
  
  virtual void ConstructProcess();
};

#endif








