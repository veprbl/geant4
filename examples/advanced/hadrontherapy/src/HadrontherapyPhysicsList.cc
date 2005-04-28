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
// $Id: HadrontherapyPhysicsList.cc,v 1.13 2005-04-28 20:39:33 mpiergen Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------

#include "HadrontherapyPhysicsList.hh"
#include "HadrontherapyPhysicsListMessenger.hh"
#include "HadrontherapyParticles.hh"
#include "HadrontherapyPhotonStandard.hh"
#include "HadrontherapyPhotonEPDL.hh"
#include "HadrontherapyPhotonPenelope.hh"
#include "HadrontherapyPhotonPolarised.hh"
#include "HadrontherapyElectronStandard.hh"
#include "HadrontherapyElectronEEDL.hh"
#include "HadrontherapyElectronPenelope.hh"
#include "HadrontherapyPositronStandard.hh"
#include "HadrontherapyProtonLowE.hh"
#include "HadrontherapyProtonStandard.hh"
#include "HadrontherapyProtonHadro.hh"
#include "HadrontherapyMuon.hh"
#include "HadrontherapyDecay.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProductionCutsTable.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4VProcess.hh"
#include "G4LowEnergyPhotoElectric.hh"
#include "G4LowEnergyIonisation.hh"
#include "G4LowEnergyBremsstrahlung.hh"

 
HadrontherapyPhysicsList::HadrontherapyPhysicsList(): G4VModularPhysicsList(),
						      electronIsRegistered(false), 
						      positronIsRegistered(false),
						      photonIsRegistered(false),
						      protonIsRegistered(false),
						      protonHadroIsRegistered(false),
						      muonIsRegistered(false),
						      decayIsRegistered(false)
{
  defaultCutValue = 0.1 * mm;
  cutForGamma = defaultCutValue;
  cutForElectron = defaultCutValue;

  SetVerboseLevel(1);

  // UI messenger
  messenger = new HadrontherapyPhysicsListMessenger(this);
 
  // Particles
  RegisterPhysics( new HadrontherapyParticles("particles") );

}

HadrontherapyPhysicsList::~HadrontherapyPhysicsList()
{
  delete messenger;
}


void HadrontherapyPhysicsList::AddPhysicsList(const G4String& name)
{

  G4cout << "Adding PhysicsList chunk " << name << G4endl;

  // Register standard processes for photons
  if (name == "photon-standard") 
    {
      if (photonIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- photon List already existing" << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyPhotonStandard(name) );
	  photonIsRegistered = true;
	}
    }
  // Register LowE-EPDL processes for photons
  if (name == "photon-epdl") 
     {
      if (photonIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- photon List already existing" << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyPhotonEPDL(name) );
	  photonIsRegistered = true;
	}
   } 
  // Register processes a' la Penelope for photons
  if (name == "photon-penelope")
    {
     if (photonIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- photon List already existing" << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyPhotonPenelope(name) );
	  photonIsRegistered = true;
	}
    }
  // Register polarised processes for photons
  if (name == "photon-polarised")
    {
      if (photonIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- photon List already existing" << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyPhotonPolarised(name) );
	  photonIsRegistered = true;
	}
    }
  // Register standard processes for electrons
  if (name == "electron-standard") 
    {
      if (electronIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- electron List already existing" << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyElectronStandard(name) );	  
	  electronIsRegistered = true;
	}
    }
  // Register LowE-EEDL processes for electrons
  if (name == "electron-eedl") 
    {
      if (electronIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- electron List already existing" << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyElectronEEDL(name) );
	  electronIsRegistered = true;
	}
   } 
  // Register processes a' la Penelope for electrons
  if (name == "electron-penelope")
    {
     if (electronIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name 
		 << " cannot be registered ---- electron List already existing" << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyElectronPenelope(name) );
	  electronIsRegistered = true;
	}
    }
  // Register standard processes for positrons
  if (name == "positron-standard") 
    {
      if (positronIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- positron List already existing" << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyPositronStandard(name) );
	  positronIsRegistered = true;
	}
    }

  // Register standard processes for protons
  if (name == "proton-LowE") 
    {
      if (protonIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- proton List already existing" << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyProtonLowE(name) );
	  protonIsRegistered = true;
	}
    }

  if (name == "proton-standard") 
    {
      if (protonIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- proton List already existing" << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyProtonStandard(name) );
	  protonIsRegistered = true;
	}
    }

  if (name == "proton-hadronic") 
    {
      if (protonHadroIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- proton hadronic List already existing" << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyProtonHadro(name) );
	  protonIsRegistered = true;
	}
    }

  if (name == "muon") 
    {
      if (muonIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- muon List already existing" << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyMuon(name) );
	  muonIsRegistered = true;
	}
    }

  if (name == "decay") 
    {
      if (decayIsRegistered) 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name  
		 << " cannot be registered ---- decay List already existing" << G4endl;
	} 
      else 
	{
	  G4cout << "HadrontherapyPhysicsList::AddPhysicsList: " << name << " is registered" << G4endl;
	  RegisterPhysics( new HadrontherapyDecay(name) );
	  decayIsRegistered = true;
	}
    }


  if (electronIsRegistered && positronIsRegistered && photonIsRegistered && protonIsRegistered && muonIsRegistered && decayIsRegistered)
    {
      G4cout << "PhysicsList for electron, positron, photon, proton, muon, decay registered" << G4endl;
    }
}


void HadrontherapyPhysicsList::SetGELowLimit(G4double cut)
{
  if (verboseLevel > 0)
    {
      G4cout << "HadrontherapyPhysicsList - Gamma and electron cut in energy = " 
	     << cut * MeV << " MeV" << G4endl;
    }  
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(cut,1e5);
}


void HadrontherapyPhysicsList::SetGammaLowLimit(G4double cut)
{
  if (verboseLevel > 0)
    {
      G4cout << "HadrontherapyPhysicsList - Gamma cut in energy = " 
	     << cut * MeV << " MeV" << G4endl;
    }  
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(cut,1e5);
}

void HadrontherapyPhysicsList::SetElectronLowLimit(G4double cut)
{
  if (verboseLevel > 0)
    {
      G4cout << "HadrontherapyPhysicsList - Electron cut in energy = " 
	     << cut * MeV << " MeV" << G4endl;
    }  
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(cut,1e5);
}

void HadrontherapyPhysicsList::SetGammaCut(G4double value)
{
  ResetCuts();
  cutForGamma = value;
}


void HadrontherapyPhysicsList::SetElectronCut(G4double value)
{
  ResetCuts();
  cutForElectron = value;
}


void HadrontherapyPhysicsList::SetCuts()
{
  SetCutValue(cutForGamma,"gamma");
  SetCutValue(cutForElectron,"e-");
  SetCutValue(cutForElectron,"e+");
}

void HadrontherapyPhysicsList::SetLowEnSecPhotCut(G4double cut)
{
  // This m.f. is pertinent to LowEnergy EPDL/EEDL processes only

  // Get the ProcessManager for photons and the list of photon processes
  G4ProcessManager* photonManager = G4Gamma::GammaDefinition()->GetProcessManager();
  G4ProcessVector* photonProcesses = photonManager->GetProcessList();
  G4int nPhotonProcesses = photonProcesses->size();

  // Loop over photon processes until one retrieves LowEnergyPhotoElectric
  for (G4int iPhoton=0; iPhoton<nPhotonProcesses; iPhoton++)
    {
      G4VProcess* process = (*photonProcesses)[iPhoton];
      const G4String& name = process->GetProcessName();
      G4String nameLowE("LowEnPhotoElec");
      if (name == nameLowE)
	{
	  // The only way to get access to the cut stting is through a dynamic_cast
	  // (it is ugly!)
	  G4LowEnergyPhotoElectric* lowEProcess = dynamic_cast<G4LowEnergyPhotoElectric*>(process);
	  if (lowEProcess != 0) 
	    {
	     lowEProcess->SetCutForLowEnSecPhotons(cut);
	     G4cout << "Low energy secondary photons cut is now set to: "
		    << cut * MeV
		    << " (MeV) for LowEnergyPhotoElectric"
		    << G4endl;
	    }
	}
    }
  
  // Get the ProcessManager for electrons and the list of electron processes
  G4ProcessManager* electronManager = G4Electron::ElectronDefinition()->GetProcessManager();
  G4ProcessVector* electronProcesses = electronManager->GetProcessList();
  G4int nElectronProcesses = electronProcesses->size();

  // Loop over electron processes until one retrieves LowEnergyIonisation or LowEnergyBremsstrahlung
  for (G4int iElectron=0; iElectron<nElectronProcesses; iElectron++)
    {
      G4VProcess* process = (*electronProcesses)[iElectron];
      const G4String& name = process->GetProcessName();

      G4String nameIoni("LowEnergyIoni");
      if (name == nameIoni)
	{
	  // The only way to get access to the cut setting is through a dynamic_cast
	  // (it is ugly!)
	  G4LowEnergyIonisation* lowEProcess = dynamic_cast<G4LowEnergyIonisation*>(process);
	  if (lowEProcess != 0) 
	    {
	      lowEProcess->SetCutForLowEnSecPhotons(cut);
	      G4cout << "Low energy secondary photons cut is now set to: "
		     << cut * MeV
		     << " (MeV) for LowEnergyIonisation"
		     << G4endl;
	    }
	}

      G4String nameBrems("LowEnBrem");
      if (name == nameBrems)
	{
	  // The only way to get access to the cut setting is through a dynamic_cast
	  // (it is ugly!)
	  G4LowEnergyBremsstrahlung* lowEProcess = dynamic_cast<G4LowEnergyBremsstrahlung*>(process);
	  if (lowEProcess != 0) 
	    {
	      lowEProcess->SetCutForLowEnSecPhotons(cut);
	      G4cout << "Low energy secondary photons cut is now set to: "
		     << cut * MeV
		     << " (MeV) for LowEnergyBremsstrahlung"
		     << G4endl;
	    }
	}
    }
}


void HadrontherapyPhysicsList::SetLowEnSecElecCut(G4double cut)
{  
  // This m.f. is pertinent to LowEnergy EPDL/EEDL processes only

  // Get the ProcessManager for photons and the list of photon processes
  G4ProcessManager* photonManager = G4Gamma::GammaDefinition()->GetProcessManager();
  G4ProcessVector* photonProcesses = photonManager->GetProcessList();
  G4int nPhotonProcesses = photonProcesses->size();

  // Loop over photon processes until one retrieves LowEnergyPhotoElectric
  for (G4int iPhoton=0; iPhoton<nPhotonProcesses; iPhoton++)
    {
      G4VProcess* process = (*photonProcesses)[iPhoton];
      const G4String& name = process->GetProcessName();
      G4String nameLowE("LowEnPhotoElec");
      if (name == nameLowE)
	{
	  // The only way to get access to the cut stting is through a dynamic_cast
	  // (it is ugly!)
	  G4LowEnergyPhotoElectric* lowEProcess = dynamic_cast<G4LowEnergyPhotoElectric*>(process);
	  if (lowEProcess != 0) 
	    {
	     lowEProcess->SetCutForLowEnSecElectrons(cut);
	     G4cout << "Low energy secondary electrons cut is now set to: "
		    << cut * MeV
		    << " (MeV) for LowEnergyPhotoElectric"
		    << G4endl;
	    }
	}
    }
  
  // Get the ProcessManager for electrons and the list of electron processes
  G4ProcessManager* electronManager = G4Electron::ElectronDefinition()->GetProcessManager();
  G4ProcessVector* electronProcesses = electronManager->GetProcessList();
  G4int nElectronProcesses = electronProcesses->size();

  // Loop over electron processes until one retrieves LowEnergyIonisation
  for (G4int iElectron=0; iElectron<nElectronProcesses; iElectron++)
    {
      G4VProcess* process = (*electronProcesses)[iElectron];
      const G4String& name = process->GetProcessName();
      G4String nameLowE("LowEnergyIoni");
      if (name == nameLowE)
	{
	  // The only way to get access to the cut setting is through a dynamic_cast
	  // (it is ugly!)
	  G4LowEnergyIonisation* lowEProcess = dynamic_cast<G4LowEnergyIonisation*>(process);
	  if (lowEProcess != 0) 
	    {
	      lowEProcess->SetCutForLowEnSecElectrons(cut);
	      G4cout << "Low energy secondary electrons cut is now set to: "
		     << cut * MeV
		     << " (MeV) for LowEnergyIonisation"
		     << G4endl;
	    }
	}
    }
}

void HadrontherapyPhysicsList::ActivateAuger(G4bool value)
{  
  // Get the ProcessManager for photons and the list of photon processes
  G4ProcessManager* photonManager = G4Gamma::GammaDefinition()->GetProcessManager();
  G4ProcessVector* photonProcesses = photonManager->GetProcessList();
  G4int nPhotonProcesses = photonProcesses->size();

  // Loop over photon processes until one retrieves LowEnergyPhotoElectric
  for (G4int iPhoton=0; iPhoton<nPhotonProcesses; iPhoton++)
    {
      G4VProcess* process = (*photonProcesses)[iPhoton];
      const G4String& name = process->GetProcessName();
      G4String nameLowE("LowEnPhotoElec");
      if (name == nameLowE)
	{
	  // The only way to get access to the Auger activation is through a dynamic_cast
	  // (it is ugly!)
	  G4LowEnergyPhotoElectric* lowEProcess = dynamic_cast<G4LowEnergyPhotoElectric*>(process);
	  if (lowEProcess != 0) 
	    {
	     lowEProcess->ActivateAuger(value);
	      G4cout << "Auger electron production flag is " << value
		     << " for LowEnergyPhotoElectric process" << G4endl;
	    }
	}
    }
  
  // Get the ProcessManager for electrons and the list of electron processes
  G4ProcessManager* electronManager = G4Electron::ElectronDefinition()->GetProcessManager();
  G4ProcessVector* electronProcesses = electronManager->GetProcessList();
  G4int nElectronProcesses = electronProcesses->size();

  // Loop over electron processes until one retrieves LowEnergyIonisation
  for (G4int iElectron=0; iElectron<nElectronProcesses; iElectron++)
    {
      G4VProcess* process = (*electronProcesses)[iElectron];
      const G4String& name = process->GetProcessName();
      G4String nameLowE("LowEnergyIoni");
      if (name == nameLowE)
	{
	  // The only way to get access to the Auger activation is through a dynamic_cast
	  // (it is ugly!)
	  G4LowEnergyIonisation* lowEProcess = dynamic_cast<G4LowEnergyIonisation*>(process);
	  if (lowEProcess != 0) 
	    {
	      lowEProcess->ActivateAuger(value);
	      G4cout << "Auger electron production flag is " << value
		     << " for LowEnergyIonisation process" << G4endl;
	    }
	}
    }
}

void HadrontherapyPhysicsList::SetAngularDistribution(const G4String& angularName)
{

  G4cout << "Setting angular distribution to " << angularName << G4endl;

  // Get the ProcessManager for electrons and the list of electron processes
  G4ProcessManager* electronManager = G4Electron::ElectronDefinition()->GetProcessManager();
  G4ProcessVector* electronProcesses = electronManager->GetProcessList();
  G4int nElectronProcesses = electronProcesses->size();

  // Loop over electron processes until one retrieves LowEnergyIonisation
  for (G4int iElectron=0; iElectron<nElectronProcesses; iElectron++)
    {
      G4VProcess* process = (*electronProcesses)[iElectron];
      const G4String& name = process->GetProcessName();
      G4String nameBrems("LowEnBrem");

      if (name == nameBrems)
	{
	  // The only way to get access to the cut setting is through a dynamic_cast
	  // (it is ugly!)
	  G4LowEnergyBremsstrahlung* lowEProcess = dynamic_cast<G4LowEnergyBremsstrahlung*>(process);
	  if (lowEProcess != 0) 
	    {
              lowEProcess->SetAngularGenerator(angularName);
	      G4cout << "Low energy Bremsstrahlung angular distribution is set to: "
		     << angularName
		     << G4endl;
	    }
	}
    }

}



