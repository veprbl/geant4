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
// $Id: G4PhotoElectricEffect.cc,v 1.24 2002-04-09 17:34:43 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// 12-06-96, Added SelectRandomAtom() method, by M.Maire
// 21-06-96, SetCuts implementation, M.Maire
// 17-09-96, PartialSumSigma(i)
//           split of ComputeBindingEnergy, M.Maire
// 08-01-97, crossection table + meanfreepath table, M.Maire
// 13-03-97, adapted for the new physics scheme, M.Maire
// 28-03-97, protection in BuildPhysicsTable, M.Maire
// 04-06-98, in DoIt, secondary production condition:
//                        range > G4std::min(threshold,safety)
// 13-08-98, new methods SetBining() PrintInfo()
// 17-11-98, use table of Atomic shells in PostStepDoIt
// 06-01-99, use Sandia crossSection below 50 keV, V.Grichine mma
// 20-05-99, protection against very low energy photons ,L.Urban
// 08-06-99, removed this above protection from the DoIt. mma
// 21-06-00, in DoIt, killing photon: aParticleChange.SetEnergyChange(0.); mma
// 22-06-00, in DoIt, absorbe very low energy photon (back to 20-05-99); mma
// 22-02-01, back to 08-06-99 after correc in SandiaTable (materials-V03-00-05)
// 28-05-01, V.Ivanchenko minor changes to provide ANSI -wall compilation
// 13-07-01, DoIt: suppression of production cut of the electron (mma)
// 06-08-01, new methods Store/Retrieve PhysicsTable (mma)
// 06-08-01, BuildThePhysicsTable() called from constructor (mma)
// 17-09-01, migration of Materials to pure STL (mma)
// 20-09-01, DoIt: fminimalEnergy = 1*eV (mma)
// 01-10-01, come back to BuildPhysicsTable(const G4ParticleDefinition&)       
// 10-01-02, moved few function from icc to cc
//    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4PhotoElectricEffect.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
// constructor
 
G4PhotoElectricEffect::G4PhotoElectricEffect(const G4String& processName)
  : G4VDiscreteProcess (processName),             // initialization
    theCrossSectionTable(NULL),
    theMeanFreePathTable(NULL),
    LowestEnergyLimit (50*keV),
    HighestEnergyLimit(50*MeV),
    NumbBinTable(100),
    fminimalEnergy(1*eV)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
// destructor
 
G4PhotoElectricEffect::~G4PhotoElectricEffect()
{
   if (theCrossSectionTable) {
      theCrossSectionTable->clearAndDestroy();
      delete theCrossSectionTable;
   }

   if (theMeanFreePathTable) {
      theMeanFreePathTable->clearAndDestroy();
      delete theMeanFreePathTable;
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4PhotoElectricEffect::SetPhysicsTableBining(
                                     G4double lowE, G4double highE, G4int nBins)
{
  LowestEnergyLimit = lowE; HighestEnergyLimit = highE; NumbBinTable = nBins;
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void G4PhotoElectricEffect::BuildPhysicsTable(const G4ParticleDefinition&)

// Build cross section per atom and mean free path tables
{
   G4double LowEdgeEnergy, Value;
   G4PhysicsLogVector* ptrVector;

// Build cross section per atom tables for the Photo Electric Effect

   if (theCrossSectionTable) {
       theCrossSectionTable->clearAndDestroy(); delete theCrossSectionTable;}

   theCrossSectionTable = new G4PhysicsTable( G4Element::GetNumberOfElements());
   const G4ElementTable* theElementTable = G4Element::GetElementTable();
   G4double AtomicNumber;
   size_t J;

   for ( J=0 ; J < G4Element::GetNumberOfElements(); J++ )  
      { 
        //create physics vector then fill it ....
        ptrVector = new G4PhysicsLogVector(LowestEnergyLimit,HighestEnergyLimit,
                                           NumbBinTable ) ;
        AtomicNumber = (*theElementTable)[J]->GetZ();
 
        for ( G4int i = 0 ; i < NumbBinTable ; i++ )      
           {
             LowEdgeEnergy = ptrVector->GetLowEdgeEnergy( i ) ;
             Value = ComputeCrossSectionPerAtom( LowEdgeEnergy, AtomicNumber);  
             ptrVector->PutValue( i , Value ) ;
           }

        theCrossSectionTable->insertAt( J , ptrVector ) ;

      }

// Build mean free path table for the Photo Electric Effect

   if (theMeanFreePathTable) {
       theMeanFreePathTable->clearAndDestroy(); delete theMeanFreePathTable;}

   theMeanFreePathTable= new G4PhysicsTable(G4Material::GetNumberOfMaterials());
   const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable() ;
   G4Material* material;

   for ( J=0 ; J < G4Material::GetNumberOfMaterials(); J++ )  
     { 
        //create physics vector then fill it ....
        ptrVector = new G4PhysicsLogVector(LowestEnergyLimit,HighestEnergyLimit,
                                           NumbBinTable );
        material = (*theMaterialTable)[J];
 
        for ( G4int i = 0 ; i < NumbBinTable ; i++ )      
           {
             LowEdgeEnergy = ptrVector->GetLowEdgeEnergy( i ) ;
             Value = ComputeMeanFreePath( LowEdgeEnergy, material);  
             ptrVector->PutValue( i , Value ) ;
           }

        theMeanFreePathTable->insertAt( J , ptrVector ) ;

     }
                                    
    PrintInfoDefinition();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4double G4PhotoElectricEffect::ComputeCrossSectionPerAtom(
                                                        G4double PhotonEnergy,
                                                        G4double AtomicNumber)
 
// Calculates the cross section per atom in GEANT4 internal units.
// A parametrized formula from L. Urban is used to estimate the 
// total cross section.
// It gives a good description of the elements : 5 < Atomic Number < 100 and
//                                               from 10 keV to 50 MeV.
 
{
 G4double CrossSection = 0.0 ;
 if ( AtomicNumber < 1. )      return CrossSection;
 if ( PhotonEnergy > 50.*MeV ) return CrossSection;
      
 static const G4double
   p1K =-8.8893e+2*nanobarn, p2K = 2.4394   *nanobarn, p3K = 2.8835e+2*nanobarn,
   p4K = 1.2133e+1*nanobarn, p5K =-3.1104e+2*nanobarn, p6K =-1.7284e-1*nanobarn,
   p7K = 1.4400e+1*nanobarn, p8K = 6.8357e+1*nanobarn, p9K = 7.3945e-4*nanobarn,
   p10K=-4.8149e-2*nanobarn, p11K= 5.5823e-1*nanobarn, p12K=-1.0089e-1*nanobarn;
 static const G4double
   p1L1=-1.0927e+3*nanobarn, p2L1=-9.7897e-1*nanobarn, p3L1= 1.2854e+2*nanobarn;
 static const G4double
   p1L2=-4.5803e+3*nanobarn, p2L2= 1.6858e-3*nanobarn, p3L2= 1.2013e+2*nanobarn;
 static const G4double 
   p1M = 1.6924e+1*nanobarn;

 const G4double pwZ = 3.845 ,  pwE = 2.975 ; 
    
 G4double Z  = AtomicNumber,                  Z2  = Z*Z,   Z3  = Z*Z*Z;
 G4double Em = PhotonEnergy/electron_mass_c2, Em2 = Em*Em, Em3 = Em*Em*Em;

 CrossSection = pow(Z,pwZ)/pow(Em,pwE);

 if (PhotonEnergy > ComputeKBindingEnergy(Z) ) {
      CrossSection *= (p1K/Z  + p2K/Em + p3K + p4K*Z + p5K*Em
                    +  p6K*Z2 + p7K *Z *Em + p8K   *Em2
                    +  p9K*Z3 + p10K*Z2*Em + p11K*Z*Em2 + p12K*Em3);
      if (CrossSection < 0.) CrossSection = 0. ;
    }

 else if (PhotonEnergy > ComputeL1BindingEnergy(Z) ) {
      CrossSection *= (p1L1/Z + p2L1/Em + p3L1 );
      if (CrossSection < 0.) CrossSection = 0. ;
    }

 else if (PhotonEnergy > ComputeL2BindingEnergy(Z) ) {
      CrossSection *= (p1L2/Z + p2L2/Em + p3L2 );
      if (CrossSection < 0.) CrossSection = 0. ;
    }

 else CrossSection *= p1M;

 return CrossSection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4double G4PhotoElectricEffect::ComputeKBindingEnergy (G4double Z)
 
// Calculates the binding energy of the K electronic shell, as a function
// of the Atomic Number, from a parametrized formula of L. Urban.

{
  const G4double
  aK (6.6644*eV), bK (2.2077e-1*eV), cK (-3.2552e-3*eV), dK (1.8199e-5*eV);

  return Z*Z*(aK  + Z* (bK + Z* (cK + Z* dK))); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4PhotoElectricEffect::ComputeL1BindingEnergy (G4double Z)
 
// Calculates the binding energy of the L1 electronic shell, as a function
// of the Atomic Number, from a parametrized formula of L. Urban.

{
  const G4double
  aL1(-2.9179e-1*eV), bL1(8.7983e-2*eV), cL1(-1.2589e-3*eV), dL1(6.9602e-6*eV);
  
  return Z*Z*(aL1 + Z* (bL1 + Z* (cL1 + Z* dL1)));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4PhotoElectricEffect::ComputeL2BindingEnergy (G4double Z)
 
// Calculates the binding energy of the L2 electronic shell, as a function
// of the Atomic Number, from a parametrized formula of L. Urban.

{
  const G4double
  aL2(-6.8606e-1*eV), bL2(1.0078e-1*eV), cL2(-1.4496e-3*eV), dL2(7.8809e-6*eV);

  return Z*Z*(aL2 + Z* (bL2 + Z* (cL2 + Z* dL2)));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
                                                          
G4double G4PhotoElectricEffect::ComputeSandiaCrossSection(G4double PhotonEnergy,
                                                          G4double AtomicNumber)
{  
  G4double energy2 = PhotonEnergy*PhotonEnergy, energy3 = PhotonEnergy*energy2, 
           energy4 = energy2*energy2;

  G4double* SandiaCof 
           = G4SandiaTable::GetSandiaCofPerAtom((int)AtomicNumber,PhotonEnergy);

  return SandiaCof[0]/PhotonEnergy + SandiaCof[1]/energy2 +
	 SandiaCof[2]/energy3      + SandiaCof[3]/energy4; 
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4PhotoElectricEffect::ComputeMeanFreePath(G4double GammaEnergy,
                                                          G4Material* aMaterial)

// returns the gamma mean free path in GEANT4 internal units

{
  const G4ElementVector* theElementVector = aMaterial->GetElementVector();
  const G4double* NbOfAtomsPerVolume = aMaterial->GetVecNbOfAtomsPerVolume();   

  G4double SIGMA = 0;

  for ( size_t elm=0 ; elm < aMaterial->GetNumberOfElements() ; elm++ )
      {             
            SIGMA += NbOfAtomsPerVolume[elm] * 
                     ComputeCrossSectionPerAtom(GammaEnergy,
                                             (*theElementVector)[elm]->GetZ());
      }       

  return SIGMA > DBL_MIN ? 1./SIGMA : DBL_MAX ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
                                                         
G4double G4PhotoElectricEffect::ComputeSandiaMeanFreePath(G4double GammaEnergy,
                                                          G4Material* aMaterial)
{
  G4double energy2 = GammaEnergy*GammaEnergy, energy3 = GammaEnergy*energy2, 
           energy4 = energy2*energy2;

  G4double* SandiaCof = aMaterial->GetSandiaTable()
                                 ->GetSandiaCofForMaterial(GammaEnergy);

  G4double SIGMA = SandiaCof[0]/GammaEnergy + SandiaCof[1]/energy2 +
	           SandiaCof[2]/energy3     + SandiaCof[3]/energy4; 
          
  return SIGMA > DBL_MIN ? 1./SIGMA : DBL_MAX ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VParticleChange* G4PhotoElectricEffect::PostStepDoIt(const G4Track& aTrack,
                                                      const G4Step&  aStep)
//
// Generate an electron resulting of a photo electric effect.
// The incident photon disappear.
// GEANT4 internal units
//
 
{  aParticleChange.Initialize(aTrack);
   G4Material* aMaterial = aTrack.GetMaterial();

   const G4DynamicParticle* aDynamicPhoton = aTrack.GetDynamicParticle();

   G4double PhotonEnergy = aDynamicPhoton->GetKineticEnergy();
   G4ParticleMomentum PhotonDirection = aDynamicPhoton->GetMomentumDirection();
   
   // select randomly one element constituing the material.
   G4Element* anElement = SelectRandomAtom(aDynamicPhoton, aMaterial);

   //
   // Photo electron
   //

   G4int NbOfShells = anElement->GetNbOfAtomicShells();
   G4int i=0;
   while ((i<NbOfShells)&&(PhotonEnergy<anElement->GetAtomicShell(i))) i++;

   if (i==NbOfShells) return G4VDiscreteProcess::PostStepDoIt(aTrack,aStep);
      
   G4double ElecKineEnergy = PhotonEnergy - anElement->GetAtomicShell(i);

   if (ElecKineEnergy > fminimalEnergy)
     {
      // the electron is created in the direction of the incident photon ...  
      G4DynamicParticle* aElectron = new G4DynamicParticle (
                        G4Electron::Electron(),PhotonDirection, ElecKineEnergy);
      aParticleChange.SetNumberOfSecondaries(1);
      aParticleChange.AddSecondary( aElectron ); 
     }
   else
     {
      ElecKineEnergy = 0.;
      aParticleChange.SetNumberOfSecondaries(0);
     }

   //
   // Kill the incident photon 
   //
   aParticleChange.SetLocalEnergyDeposit(PhotonEnergy-ElecKineEnergy);
   aParticleChange.SetEnergyChange(0.);  
   aParticleChange.SetStatusChange(fStopAndKill); 

   //  Reset NbOfInteractionLengthLeft and return aParticleChange
   return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Element* G4PhotoElectricEffect::SelectRandomAtom(
                                     const G4DynamicParticle* aDynamicPhoton,
                                           G4Material* aMaterial)
{
  // select randomly 1 element within the material

  const G4int NumberOfElements            = aMaterial->GetNumberOfElements();
  const G4ElementVector* theElementVector = aMaterial->GetElementVector();
  if (NumberOfElements == 1) return (*theElementVector)[0];

  const G4double* NbOfAtomsPerVolume = aMaterial->GetVecNbOfAtomsPerVolume();

  G4double PartialSumSigma = 0. ;
  G4double rval = G4UniformRand();
 
  for ( G4int elm=0 ; elm < NumberOfElements ; elm++ )
     {PartialSumSigma += NbOfAtomsPerVolume[elm] *
                         GetCrossSectionPerAtom(aDynamicPhoton,
                                         (*theElementVector)[elm]);
      if (rval<=PartialSumSigma*MeanFreePath) return ((*theElementVector)[elm]);
     }
  return ((*theElementVector)[NumberOfElements-1]);    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool G4PhotoElectricEffect::StorePhysicsTable(G4ParticleDefinition* particle,
				              const G4String& directory, 
				              G4bool          ascii)
{
  G4String filename;

  // store cross section table
  filename = GetPhysicsTableFileName(particle,directory,"CrossSection",ascii);
  if ( !theCrossSectionTable->StorePhysicsTable(filename, ascii) ){
    G4cout << " FAIL theCrossSectionTable->StorePhysicsTable in " << filename
           << G4endl;
    return false;
  }

  // store mean free path table
  filename = GetPhysicsTableFileName(particle,directory,"MeanFreePath",ascii);
  if ( !theMeanFreePathTable->StorePhysicsTable(filename, ascii) ){
    G4cout << " FAIL theMeanFreePathTable->StorePhysicsTable in " << filename
           << G4endl;
    return false;
  }
  
  G4cout << GetProcessName() << " for " << particle->GetParticleName()
         << ": Success to store the PhysicsTables in "  
         << directory << G4endl;
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool G4PhotoElectricEffect::RetrievePhysicsTable(
                                                 G4ParticleDefinition* particle,
					         const G4String& directory, 
				                 G4bool          ascii)
{
  // delete theCrossSectionTable and theMeanFreePathTable
  if (theCrossSectionTable != 0) {
    theCrossSectionTable->clearAndDestroy();
    delete theCrossSectionTable;
  }
  if (theMeanFreePathTable != 0) {
    theMeanFreePathTable->clearAndDestroy();
    delete theMeanFreePathTable;
  }

  G4String filename;

  // retreive cross section table
  filename = GetPhysicsTableFileName(particle,directory,"CrossSection",ascii);
  theCrossSectionTable = new G4PhysicsTable(G4Element::GetNumberOfElements());
  if ( !theCrossSectionTable->RetrievePhysicsTable(filename, ascii) ){
    G4cout << " FAIL theCrossSectionTable->RetrievePhysicsTable in " << filename
           << G4endl;  
    return false;
  }

  // retreive mean free path table
  filename = GetPhysicsTableFileName(particle,directory,"MeanFreePath",ascii);
  theMeanFreePathTable = new G4PhysicsTable(G4Material::GetNumberOfMaterials());
  if ( !theMeanFreePathTable->RetrievePhysicsTable(filename, ascii) ){
    G4cout << " FAIL theMeanFreePathTable->RetrievePhysicsTable in " << filename
           << G4endl;  
    return false;
  }
  
  G4cout << GetProcessName() << " for " << particle->GetParticleName()
         << ": Success to retrieve the PhysicsTables from "
         << directory << G4endl;
  return true;
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4PhotoElectricEffect::PrintInfoDefinition()
{
  G4String comments = "Total cross sections from a parametrisation. ";
           comments += "Good description from 10 KeV to 50 MeV for all Z";
           comments += "\n        Sandia crossSection below 50 KeV";
	             
  G4cout << G4endl << GetProcessName() << ":  " << comments
         << "\n       PhysicsTables from "
	           << G4BestUnit(LowestEnergyLimit, "Energy")
         << " to " << G4BestUnit(HighestEnergyLimit,"Energy") 
         << " in " << NumbBinTable << " bins. \n";
}         

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
