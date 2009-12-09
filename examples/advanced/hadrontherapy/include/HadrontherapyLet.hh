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
// $Id: HadrontherapyLet.hh,v 1.0, May 2007;

#ifndef HadrontherapyLet_h
#define HadrontherapyLet_h 1

#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include <fstream>
#include <vector>
#include <string>

struct ionLet 
 { 
     G4String fullName; //  AZ[excitation energy]: like He3[1277.4], He4[0.0], Li7[231.4], ...
     G4String name;     // simple name without excitation energy: He3, He4, Li7, ...
     //G4int Z;         // atomic number
     //G4int A;		// mass number
     G4double *stop;	// stopping power table
     G4int **spectrum; // energy spectrum 
     G4double *letT , *letD;  //Track averaged LET and dose averaged LET 
 };

class G4Material;
class HadrontherapyLetMessenger;
class HadrontherapyMatrix;
class HadrontherapyPrimaryGeneratorAction;
class HadrontherapyInteractionParameters;
class HadrontherapyDetectorConstruction;

class HadrontherapyLet
{
private:
  HadrontherapyLet(HadrontherapyDetectorConstruction*); // Ahh, yes another singleton like H.Matrix

public:
  ~HadrontherapyLet();
  static HadrontherapyLet* GetInstance(HadrontherapyDetectorConstruction*);
  static HadrontherapyLet* GetInstance();

  void Initialize();
  void Clear();
  void FillEnergySpectrum(G4ParticleDefinition* particleDef,
			  G4double KinEnergy,
			  G4int i, G4int j, G4int k); 
  void LetOutput(); 

  void StoreData(G4String filename);
  void SetValue(G4String);

  HadrontherapyLetMessenger* letMessenger;

private:
  static HadrontherapyLet *instance;
  HadrontherapyInteractionParameters* pParam;
  HadrontherapyPrimaryGeneratorAction* pPGA;
  // Detector
  G4Material* detectorMat;  
  G4double density;
  
  std::ofstream ofs;
  HadrontherapyMatrix *matrix;
  G4int nVoxels, numberOfVoxelAlongX, numberOfVoxelAlongY, numberOfVoxelAlongZ ;
  G4double primaryEnergy, energyLimit, binWidth; 
  G4int nBins;
  G4double  nT, dT, nD, dD;
  G4String nome_file;

  std::vector<ionLet> ionLetStore;
 
};
#endif
