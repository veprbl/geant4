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
// $Id: G4NuclearStopping.cc,v 1.2 2009-11-10 19:25:47 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -----------------------------------------------------------------------------
//
// GEANT4 Class file
//
// File name:     G4NuclearStopping
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 20 July 2009
// 
// Modified:
//
// -----------------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4NuclearStopping.hh"
#include "G4ICRU49NuclearStoppingModel.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using namespace std;

G4NuclearStopping::G4NuclearStopping(const G4String& processName)
  : G4VEmProcess(processName)
{
  isInitialized = false;  
  SetProcessSubType(fCoulombScattering);
  enableAlongStepDoIt = true; 
  modelICRU49 = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4NuclearStopping::~G4NuclearStopping()
{
  delete modelICRU49;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool G4NuclearStopping::IsApplicable (const G4ParticleDefinition& p)
{
  return (p.GetPDGCharge() != 0.0 && !p.IsShortLived());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4NuclearStopping::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialized) {
    isInitialized = true;
    SetBuildTableFlag(false);
    //    SetSecondaryParticle(G4Electron::Electron());
    if(!Model()) {
      modelICRU49 = new G4ICRU49NuclearStoppingModel();
      SetModel(modelICRU49);
    }
    AddEmModel(1, Model());
    Model()->SetParticleChange(&nParticleChange);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double 
G4NuclearStopping::AlongStepGetPhysicalInteractionLength(
     const G4Track&, G4double, G4double, G4double&, G4GPILSelection* selection)
{
  *selection = NotCandidateForSelection;
  return DBL_MAX;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VParticleChange* G4NuclearStopping::AlongStepDoIt(const G4Track& track,
						    const G4Step&  step)
{
  nParticleChange.InitializeForAlongStep(track);
  G4double length = step.GetStepLength();
  if(length > 0.0) {

    G4double T1 = step.GetPreStepPoint()->GetKineticEnergy();
    G4double T2 = step.GetPostStepPoint()->GetKineticEnergy();
    if(T2 > 0.0) {

      // primary
      G4double T = 0.5*(T1 + T2);
      const G4ParticleDefinition* part = track.GetDefinition();
      const G4MaterialCutsCouple* couple = track.GetMaterialCutsCouple(); 
      G4VEmModel* mod = SelectModel(T, couple->GetIndex());

      // sample stopping
      if(modelICRU49) modelICRU49->SetFluctuationFlag(true);
      G4double nloss = 
	length*mod->ComputeDEDXPerVolume(couple->GetMaterial(), part, T);
      if(nloss > T1) nloss = T1;
      nParticleChange.SetProposedKineticEnergy(T1 - nloss);
      nParticleChange.ProposeLocalEnergyDeposit(nloss);
      nParticleChange.ProposeNonIonizingEnergyDeposit(nloss);
      if(modelICRU49) modelICRU49->SetFluctuationFlag(false);
    }
  }
  return &nParticleChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4NuclearStopping::PrintInfo()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

