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
// $Id: G4PhysicsVectorType.hh,v 1.3 2001-07-11 10:00:51 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// --------------------------------------------------------------
//
// G4PhysicsVectorType
//
// Class Description:
//
// Enumerator to define the physics vector type:
//   G4PhysicsVector - base
//   G4PhysicsLinearVector
//   G4PhysicsLogVector
//   G4PhysicsLnVector
//   G4PhysicsFreeVector
//   G4PhysicsOrderedFreeVector
//   G4LPhysicsFreeVector

// --------------------------------------------------------------

#ifndef G4PhysicsVectorType_h
#define G4PhysicsVectorType_h

enum G4PhysicsVectorType
{
  T_G4PhysicsVector =0,
  T_G4PhysicsLinearVector,
  T_G4PhysicsLogVector,
  T_G4PhysicsLnVector,
  T_G4PhysicsFreeVector,
  T_G4PhysicsOrderedFreeVector,
  T_G4LPhysicsFreeVector
};

#endif
