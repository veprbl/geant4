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

#include "F04Materials.hh"

F04Materials::F04Materials()
{
  nistMan = G4NistManager::Instance();

  nistMan->SetVerbose(2);

  CreateMaterials();
}

F04Materials::~F04Materials()
{
  delete    Vacuum;
  delete    Air;
  delete    Sci;
  delete    BeO;
}

F04Materials* F04Materials::instance = 0;

F04Materials* F04Materials::GetInstance()
{
  if (instance == 0)
    {
      instance = new F04Materials();
    }
  return instance;
}

G4Material* F04Materials::GetMaterial(const G4String material)  
{
  G4Material* mat =  nistMan->FindOrBuildMaterial(material);

  if (!mat) mat = G4Material::GetMaterial(material);
  if (!mat) {G4cout << material << "Not Found, Please Retry"<< G4endl;}

  return mat;
}

void F04Materials::CreateMaterials()
{
  G4double density;            
  std::vector<G4int>  natoms;
  std::vector<G4double> fractionMass;
  std::vector<G4String> elements;

  // Materials Definitions
  // =====================

  // Define Vacuum

  Vacuum = nistMan->FindOrBuildMaterial("G4_Galactic");

  // Define Air

  Air = nistMan->FindOrBuildMaterial("G4_AIR");

  // Define BeO

  BeO = nistMan->FindOrBuildMaterial("G4_BERYLLIUM_OXIDE");

  // Define Scintillator

  elements.push_back("C");     natoms.push_back(9);
  elements.push_back("H");     natoms.push_back(10);

  // density = 1.032*g/cm3; 
  // ==> when using ConstructNewMaterial give the density in g/cm3
  // ==> but do NOT multiply by the unit!!!

  density = 1.032;

  Sci = nistMan->
          ConstructNewMaterial("Scintillator", elements, natoms, density);

  elements.clear();
  natoms.clear();

  G4cout << G4endl << "The materials defined are: " << G4endl << G4endl;
  
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}
