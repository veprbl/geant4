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
/// \file hadronic/Hadr02/src/StackingMessenger.cc
/// \brief Implementation of the StackingMessenger class
//
// $Id$
//
/////////////////////////////////////////////////////////////////////////
//
// StackingMessenger
//
// Created: 31.05.2006 V.Ivanchenko
//
// Modified:
// 04.06.2006 Adoptation of hadr01 (V.Ivanchenko)
//
////////////////////////////////////////////////////////////////////////
//

#include "StackingMessenger.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "StackingAction.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingMessenger::StackingMessenger(StackingAction* stack)
:fStackAction(stack)
{
  fKillCmd = new G4UIcmdWithABool("/testhadr/killAll",this);
  fKillCmd->SetGuidance("  Choice : true false");
  fKillCmd->SetGuidance("Kill all secondaries");
  fKillCmd->SetParameterName("choice",true);
  fKillCmd->SetDefaultValue(false);

  fKCmd = new G4UIcmdWithABool("/testhadr/killEM", this);
  fKCmd->SetGuidance("  Choice : true false");
  fKCmd->SetGuidance("Kill secondary e+, e-, gamma");
  fKCmd->SetParameterName("ch", true);
  fKCmd->SetDefaultValue(true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingMessenger::~StackingMessenger()
{
  delete fKillCmd;
  delete fKCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void StackingMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{     
  if(command == fKillCmd) {
    fStackAction->SetKillAll(fKillCmd->GetNewBoolValue(newValue));
  } else if(command == fKCmd) {
    fStackAction->SetKillEM(fKCmd->GetNewBoolValue(newValue));               
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
