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
//    **************************************
//    *                                    *
//    *  CellSurvivalCHOLQGueuletteP1.cc   *
//    *                                    *
//    **************************************
//
// Author: Barbara Mascialino (Barbara.Mascialino@ge.infn.it)
//
// History:
// -----------
// 12 October 2006 B. Mascialino      first implementation
// -------------------------------------------------------------------


#include "globals.hh"
#include "CellSurvivalCHOLQGueuletteP1.hh"
#include "CellPrimaryGeneratorAction.hh"
#include "G4RunManager.hh"

 
CellSurvivalCHOLQGueuletteP1::CellSurvivalCHOLQGueuletteP1()
{

}

CellSurvivalCHOLQGueuletteP1::~CellSurvivalCHOLQGueuletteP1()
{

}

void CellSurvivalCHOLQGueuletteP1::SurvivalFormula(G4double dose)
{
  G4double alpha = 10.00 * ( gray );
  G4double alpha_gray = alpha * gray; 
  G4double beta = 10.00 * gray;

  // DEBUGGING
  // G4cout << "I WILL SURVIVE !!!!!" << G4endl;

  //
  // ******************************************************************************
  // BIBLIOGRAPHY: Gueulette et al, Rad Res, vol. 145, pp. XXX-XXX, 1996.
  // Cell line: CHO
  // Monolayer experiment
  // Particle: proton
  // Energy: 85 MeV/u
  // Survival model: Linear-quadratic
  // Parameters:
  //            alpha = (0.10 +/- 0.14) Gy^-1
  //            beta  = (0.10 +/- 0.02) Gy^-2
  //********************************************************************************
  //

  probability = exp ( - ( ( dose/gray ) / ( alpha_gray / ( gray * gray ) ) + ( dose/gray ) / ( beta / gray ) ) );


  // DEBUGGING
  // G4cout << "alpha_gray= " <<  1 / ( alpha_gray / ( gray * gray ) )  << G4endl;
  // G4cout << "beta = " <<   1 / ( beta / gray )  << G4endl;
  // G4cout << "alpha_gray * dose/gray * dose/gray= " <<  ( ( dose * dose ) / ( gray * gray ) ) / ( alpha_gray / ( gray * gray ) ) << G4endl;
  // G4cout << "beta * dose/gray= " <<  ( dose/gray ) / ( beta / gray ) << G4endl;
  // G4cout << "alpha_gray * dose/gray * dose/gray + beta * dose/gray= " << ( dose/gray ) / ( alpha_gray / ( gray * gray ) ) + ( dose/gray ) / ( beta / gray )   << G4endl;
  // G4cout << "probability ( debugging ) = " <<  exp ( - ( ( dose/gray ) / ( alpha_gray / ( gray * gray ) ) + ( dose/gray ) / ( beta / gray ) ) )   << G4endl;



 CellPrimaryGeneratorAction* primary = 
   (CellPrimaryGeneratorAction*) G4RunManager::GetRunManager()-> GetUserPrimaryGeneratorAction();

  // Type of the primary particle
 G4String primaryParticleName = primary -> GetParticle();
   
  // Energy of the primary particle
 G4double primaryParticleEnergy = primary -> GetInitialEnergy();

  // TESTING
  // Expected survival probability
  // G4cout << "Expected survival probability= " << 0.99999 << G4endl;
 

 G4cout << "Primary particle: " << primaryParticleName << " with energy " <<
   primaryParticleEnergy/MeV << " MeV" << G4endl;
 

}
G4double CellSurvivalCHOLQGueuletteP1::GetSurvival()
{
  return probability; 
}
