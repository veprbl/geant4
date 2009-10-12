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
// $Id: PhysListEmPenelope.cc,v 1.2 2009-10-12 21:47:57 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#include "PhysListEmPenelope.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

// *** Processes and models

// gamma

#include "G4PhotoElectricEffect.hh"
#include "G4PenelopePhotoElectricModel.hh"

#include "G4ComptonScattering.hh"
#include "G4PenelopeComptonModel.hh"

#include "G4GammaConversion.hh"
#include "G4PenelopeGammaConversionModel.hh"

#include "G4RayleighScattering.hh" 
#include "G4PenelopeRayleighModel.hh"

// e- and e+

#include "G4UniversalFluctuation.hh"

#include "G4eIonisation.hh"
#include "G4PenelopeIonisationModel.hh"

#include "G4eBremsstrahlung.hh"
#include "G4PenelopeBremsstrahlungModel.hh"

// e+ only

#include "G4eplusAnnihilation.hh"
#include "G4PenelopeAnnihilationModel.hh"

// mu

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

// hadrons

#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"

//

#include "G4LossTableManager.hh"
#include "G4EmProcessOptions.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysListEmPenelope::PhysListEmPenelope(const G4String& name)
  : G4VPhysicsConstructor(name)
{
  G4LossTableManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysListEmPenelope::~PhysListEmPenelope()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysListEmPenelope::ConstructProcess()
{
  // Add Penelope EM Processes

  theParticleIterator->reset();

  while( (*theParticleIterator)() ){
  
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    //Applicability range for Penelope models
    //for higher energies, the Standard models are used   
    G4double PenelopeHighEnergyLimit = 1.0*GeV;

    if (particleName == "gamma") {

      //Photo-electric effect
      G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
      G4PenelopePhotoElectricModel* thePEPenelopeModel = new 
	G4PenelopePhotoElectricModel();
      thePEPenelopeModel->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      thePhotoElectricEffect->AddEmModel(0,thePEPenelopeModel);
      pmanager->AddDiscreteProcess(thePhotoElectricEffect);

      //Compton scattering
      G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
      G4PenelopeComptonModel* theComptonPenelopeModel = 
	new G4PenelopeComptonModel();
      theComptonPenelopeModel->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      theComptonScattering->AddEmModel(0,theComptonPenelopeModel);
      pmanager->AddDiscreteProcess(theComptonScattering);

      //Gamma conversion
      G4GammaConversion* theGammaConversion = new G4GammaConversion();
      G4PenelopeGammaConversionModel* theGCPenelopeModel = 
	new G4PenelopeGammaConversionModel();
      theGammaConversion->AddEmModel(0,theGCPenelopeModel);
      pmanager->AddDiscreteProcess(theGammaConversion);

      //Rayleigh scattering
      G4RayleighScattering* theRayleigh = new G4RayleighScattering();
      G4PenelopeRayleighModel* theRayleighPenelopeModel = 
	new G4PenelopeRayleighModel();
      theRayleighPenelopeModel->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      theRayleigh->AddEmModel(0,theRayleighPenelopeModel);
      pmanager->AddDiscreteProcess(theRayleigh);

    } else if (particleName == "e-") {
      
      //Ionisation
      G4eIonisation* eIoni = new G4eIonisation();
      G4PenelopeIonisationModel* theIoniPenelope = 
	new G4PenelopeIonisationModel();
      theIoniPenelope->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      eIoni->AddEmModel(0,theIoniPenelope,new G4UniversalFluctuation());
      eIoni->SetStepFunction(0.2, 100*um); //     
      pmanager->AddProcess(eIoni,                 -1, 1, 1);
      
      //Bremsstrahlung
      G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
      G4PenelopeBremsstrahlungModel* theBremPenelope = new 
	G4PenelopeBremsstrahlungModel();
      theBremPenelope->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      eBrem->AddEmModel(0,theBremPenelope);
      pmanager->AddProcess(eBrem, -1,-1, 2);

    } else if (particleName == "e+") {

      //Ionisation
      G4eIonisation* eIoni = new G4eIonisation();
      G4PenelopeIonisationModel* theIoniPenelope = 
	new G4PenelopeIonisationModel();
      theIoniPenelope->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      eIoni->AddEmModel(0,theIoniPenelope,new G4UniversalFluctuation());
      eIoni->SetStepFunction(0.2, 100*um); //     
      pmanager->AddProcess(eIoni,                 -1, 1, 1);

       //Bremsstrahlung
      G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
      G4PenelopeBremsstrahlungModel* theBremPenelope = new 
	G4PenelopeBremsstrahlungModel();
      theBremPenelope->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      eBrem->AddEmModel(0,theBremPenelope);
      pmanager->AddProcess(eBrem, -1,-1, 2);
      
      //Annihilation
      G4eplusAnnihilation* eAnni = new G4eplusAnnihilation();
      G4PenelopeAnnihilationModel* theAnnPenelope = new 
	G4PenelopeAnnihilationModel();
      theAnnPenelope->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      eAnni->AddEmModel(0,theAnnPenelope);
      pmanager->AddProcess(eAnni,0,-1, 3);

    } else if (particleName == "mu+" ||
               particleName == "mu-"    ) {

      // Identical to G4EmStandardPhysics_option3

      G4MuIonisation* muIoni = new G4MuIonisation();
      muIoni->SetStepFunction(0.2, 50*um);          

      pmanager->AddProcess(muIoni,                    -1, 1, 1);      
      pmanager->AddProcess(new G4MuBremsstrahlung,    -1,-1, 2);      
      pmanager->AddProcess(new G4MuPairProduction,    -1,-1, 3);

    } else if (particleName == "GenericIon") {

      G4ionIonisation* ionIoni = new G4ionIonisation();
      ionIoni->SetEmModel(new G4IonParametrisedLossModel());
      ionIoni->SetStepFunction(0.1, 20*um);
      pmanager->AddProcess(ionIoni,                   -1, 1, 1);

    } else if (particleName == "alpha" ||
               particleName == "He3" ) {

      // Identical to G4EmStandardPhysics_option3

      G4ionIonisation* ionIoni = new G4ionIonisation();
      ionIoni->SetStepFunction(0.1, 20*um);
      pmanager->AddProcess(ionIoni,                   -1, 1, 1);

    } else if (particleName == "pi+" ||
               particleName == "pi-" ||
	       particleName == "kaon+" ||
               particleName == "kaon-" ||
               particleName == "proton" ) {

      // Identical to G4EmStandardPhysics_option3
      
      G4hIonisation* hIoni = new G4hIonisation();
      hIoni->SetStepFunction(0.2, 50*um);

      pmanager->AddProcess(hIoni,                     -1, 1, 1);
      pmanager->AddProcess(new G4hBremsstrahlung,     -1,-1, 2);
      pmanager->AddProcess(new G4hPairProduction,     -1,-1, 3);

    } else if (particleName == "B+" ||
	       particleName == "B-" ||
	       particleName == "D+" ||
	       particleName == "D-" ||
	       particleName == "Ds+" ||
	       particleName == "Ds-" ||
               particleName == "anti_lambda_c+" ||
               particleName == "anti_omega-" ||
               particleName == "anti_proton" ||
               particleName == "anti_sigma_c+" ||
               particleName == "anti_sigma_c++" ||
               particleName == "anti_sigma+" ||
               particleName == "anti_sigma-" ||
               particleName == "anti_xi_c+" ||
               particleName == "anti_xi-" ||
               particleName == "deuteron" ||
	       particleName == "lambda_c+" ||
               particleName == "omega-" ||
               particleName == "sigma_c+" ||
               particleName == "sigma_c++" ||
               particleName == "sigma+" ||
               particleName == "sigma-" ||
               particleName == "tau+" ||
               particleName == "tau-" ||
               particleName == "triton" ||
               particleName == "xi_c+" ||
               particleName == "xi-" ) {

      // Identical to G4EmStandardPhysics_option3
      
      pmanager->AddProcess(new G4hIonisation,         -1, 1, 1);

    }
  }
    
  // Em options
  //      
  G4EmProcessOptions opt;
    
  // Physics tables
  //
  opt.SetMinEnergy(100*eV);
  opt.SetMaxEnergy(10*TeV);
  opt.SetDEDXBinning(220);
  opt.SetLambdaBinning(220);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
