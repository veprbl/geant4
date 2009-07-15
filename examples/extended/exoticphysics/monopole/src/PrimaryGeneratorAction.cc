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
// $Id: PrimaryGeneratorAction.cc,v 1.2 2009-07-15 10:19:47 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det)
  :detector(det)
{
  particleGun  = new G4ParticleGun(1);
  G4ParticleDefinition* particle = 
    G4ParticleTable::GetParticleTable()->FindParticle("monopole");
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleEnergy(100 * GeV);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(1., 0., 0.));
    
  rndmBeam   = 0.;
  EbeamCumul = 0.;
    
  //create a messenger for this class
  gunMessenger = new PrimaryGeneratorMessenger(this);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
  delete gunMessenger;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event
  G4double x0 = -0.5*(detector->GetWorldSizeX());
  G4double y0 = 0.*cm, z0 = 0.*cm;
    
  //randomize the beam, if requested.
  if (rndmBeam > 0.) 
    {
      if (rndmBeam > detector->GetAbsorSizeYZ())
        rndmBeam = detector->GetAbsorSizeYZ(); 
      G4double rbeam = 0.5*rndmBeam;
      y0 = (2*G4UniformRand()-1.)*rbeam;
      z0 = (2*G4UniformRand()-1.)*rbeam;
    }
  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));  
  particleGun->GeneratePrimaryVertex(anEvent);
  
  EbeamCumul += particleGun->GetParticleEnergy(); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

