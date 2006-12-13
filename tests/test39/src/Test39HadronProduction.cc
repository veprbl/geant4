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

#include "Test39HadronProduction.hh"
#include "Test39VSecondaryGenerator.hh"

#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4DynamicParticle.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4Tubs.hh"
#include "Randomize.hh"
#include "G4LorentzVector.hh"
#include "G4ParticleChange.hh"

// Initializes the Hadronic Process by name (Only G4VDiscreteProcess, not G4VRestProcess)
Test39HadronProduction::Test39HadronProduction(const G4String& aName)
 :G4VDiscreteProcess(aName), theGenerator(0) { InitializeMe(); }

// Model Navigation tool for initialization of the particular model (?)
void Test39HadronProduction::InitializeMe() {} // M.K.? What for is this member function?

// the G4VParticleChange must be cleaned up by caller! - Bad G4 style (?)
Test39HadronProduction::~Test39HadronProduction() {if(theGenerator) delete theGenerator;}

// Trivial definition of the PostStep InteractionLength definition
G4double Test39HadronProduction::PostStepGetPhysicalInteractionLength
                                 (const G4Track&, G4double, G4ForceCondition* condition)
{
  // condition is set to "Not Forced"
  *condition = NotForced;
  return DBL_MAX;
}

// Set the secondary generator for the Hadronic Process (not used for ElectroWeak Process)
void Test39HadronProduction::SetSecondaryGenerator(Test39VSecondaryGenerator* gen)
{
  if(theGenerator) delete theGenerator;
  theGenerator = gen;
}

// Converts Result of Hadronic Processes to G4VParticleChange form (not for ElectroWeak)
G4VParticleChange* Test39HadronProduction::PostStepDoIt(const G4Track& track,const G4Step&)
{
  G4HadFinalState* result = theGenerator->Secondaries(track);
  ClearNumberOfInteractionLengthLeft();

  theChange.Initialize(track);

  G4int ns = result->GetNumberOfSecondaries();
  G4int nb = ns;
  if(result->GetStatusChange() == isAlive) nb++;
  
  theChange.ProposeTrackStatus(fStopAndKill);
  theChange.SetNumberOfSecondaries(nb);

  for(G4int i=0; i<ns; i++)
  {
    G4Track* tr = new G4Track(result->GetSecondary(i)->GetParticle(),track.GetGlobalTime(),
	                          track.GetPosition());
    theChange.AddSecondary(tr);
  }

  if(result->GetStatusChange() == isAlive)
  {
    G4DynamicParticle* dp = new G4DynamicParticle(*(track.GetDynamicParticle()));
    G4Track* tr = new G4Track(dp,track.GetGlobalTime(),track.GetPosition());
    tr->SetKineticEnergy(result->GetEnergyChange());
    tr->SetMomentumDirection(result->GetMomentumChange());
    theChange.AddSecondary(tr);
  }
  result->Clear();
  return &theChange;
}
