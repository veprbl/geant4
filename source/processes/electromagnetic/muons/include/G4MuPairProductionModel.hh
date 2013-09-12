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
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4MuPairProductionModel
//
// Author:        Vladimir Ivanchenko on base of Laszlo Urban code
//
// Creation date: 18.05.2002
//
// Modifications:
//
// 23-12-02 Change interface in order to move to cut per region (V.Ivanchenko)
// 27-01-03 Make models region aware (V.Ivanchenko)
// 13-02-03 Add name (V.Ivanchenko)
// 10-02-04 Update parameterisation using R.Kokoulin model (V.Ivanchenko)
// 10-02-04 Add lowestKinEnergy (V.Ivanchenko)
// 13-02-06 Add ComputeCrossSectionPerAtom (mma)
// 12-05-06 Add parameter to SelectRandomAtom (A.Bogdanov) 
// 11-10-07 Add ignoreCut flag (V.Ivanchenko) 
// 28-02-08 Reorganized protected methods and members (V.Ivanchenko) 

//
// Class Description:
//
// Implementation of e+e- pair production by muons
//

// -------------------------------------------------------------------
//

#ifndef G4MuPairProductionModel_h
#define G4MuPairProductionModel_h 1

#include "G4VEmModel.hh"
#include "G4NistManager.hh"
#include "G4ElementData.hh"
#include "G4Physics2DVector.hh"
#include <vector>

class G4Element;
class G4ParticleChangeForLoss;
class G4ParticleChangeForGamma;

class G4MuPairProductionModel : public G4VEmModel
{
public:

  G4MuPairProductionModel(const G4ParticleDefinition* p = 0,
                          const G4String& nam = "muPairProd");

  virtual ~G4MuPairProductionModel();

  virtual void Initialise(const G4ParticleDefinition*, const G4DataVector&);

  virtual void InitialiseLocal(const G4ParticleDefinition*,
			       G4VEmModel* masterModel);
			
  virtual G4double ComputeCrossSectionPerAtom(
				 const G4ParticleDefinition*,
				 G4double kineticEnergy,
				 G4double Z, G4double A,
				 G4double cutEnergy,
				 G4double maxEnergy);
				 
  virtual G4double ComputeDEDXPerVolume(const G4Material*,
                                const G4ParticleDefinition*,
                                G4double kineticEnergy,
                                G4double cutEnergy);

  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*, 
				 const G4MaterialCutsCouple*,
				 const G4DynamicParticle*,
				 G4double tmin,
				 G4double maxEnergy);

  virtual G4double MinPrimaryEnergy(const G4Material*,
                                    const G4ParticleDefinition*,
                                    G4double);

  inline void SetLowestKineticEnergy(G4double e);

  inline void SetParticle(const G4ParticleDefinition*);

protected:

  G4double ComputMuPairLoss(G4double Z, G4double tkin, G4double cut,
                            G4double tmax);

  G4double ComputeMicroscopicCrossSection(G4double tkin,
                                          G4double Z,
                                          G4double cut);

  virtual G4double ComputeDMicroscopicCrossSection(G4double tkin,
						   G4double Z,
						   G4double pairEnergy);

  virtual G4double MaxSecondaryEnergy(const G4ParticleDefinition*,
				      G4double kineticEnergy);

  inline void SetElement(G4int Z);

private:

  void MakeSamplingTables();

  void DataCorrupted(G4int Z, G4double logTkin);

  inline G4double FindScaledEnergy(G4int Z, G4double rand, G4double logTkin,
				   G4double yymin, G4double yymax); 

  // hide assignment operator
  G4MuPairProductionModel & operator=(const  G4MuPairProductionModel &right);
  G4MuPairProductionModel(const  G4MuPairProductionModel&);

protected:

  const G4ParticleDefinition* particle;
  G4NistManager*              nist;

  G4double factorForCross;
  G4double sqrte;
  G4double particleMass;
  G4double z13;
  G4double z23;
  G4double lnZ;
  G4int    currentZ;

  static const G4double xgi[8],wgi[8];

private:

  G4ParticleDefinition*       theElectron;
  G4ParticleDefinition*       thePositron;
  G4ParticleChangeForLoss*    fParticleChange;

  G4double minPairEnergy;
  G4double lowestKinEnergy;

  G4int nzdat;

  // gamma energy bins
  G4int    nYBinPerDecade;
  size_t   nbiny;
  size_t   nbine;
  G4double ymin;
  G4double dy;
  G4double emin;
  G4double emax;

  static const G4int zdat[5];
  static const G4double adat[5];
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void G4MuPairProductionModel::SetLowestKineticEnergy(G4double e) 
{
  lowestKinEnergy = e;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline
void G4MuPairProductionModel::SetParticle(const G4ParticleDefinition* p)
{
  if(!particle) {
    particle = p;
    particleMass = particle->GetPDGMass();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void G4MuPairProductionModel::SetElement(G4int Z)
{
  if(Z != currentZ) {
    currentZ = Z;
    z13 = nist->GetZ13(Z);
    z23 = z13*z13;
    lnZ = nist->GetLOGZ(Z);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double 
G4MuPairProductionModel::FindScaledEnergy(G4int Z, G4double rand,
					  G4double logTkin,
					  G4double yymin, G4double yymax)
{
  G4double x = rand;
  G4Physics2DVector* pv = fElementData->GetElement2DData(Z);
  if(!pv) { 
    DataCorrupted(Z, logTkin); 
  } else if(yymin > ymin || yymax < 0.0) {
    G4double pmin = pv->Value(yymin, logTkin);
    G4double pmax = pv->Value(yymax, logTkin);
    G4double p0   = pv->Value(0.0, logTkin);
    if(p0 <= 0.0) { DataCorrupted(Z, logTkin); }
    else { x = (pmin + rand*(pmax - pmin))/p0; }
  }
  return pv->FindLinearX(x, logTkin);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
