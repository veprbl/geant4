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
// $Id: Tst10PrimaryGeneratorAction.cc,v 1.5 2001-07-11 10:09:49 gunter Exp $
// ------------------------------------------------------------
//	GEANT 4 class header file 
//
//      This is a version for maximum particle set
//	History
//        first version              09  Sept. 1998 by S.Magni
// ------------------------------------------------------------

#include "Tst10PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "globals.hh"
#include "g4std/iostream"
#include "g4std/fstream"

Tst10PrimaryGeneratorAction::Tst10PrimaryGeneratorAction()
{
  particleGun = new G4ParticleGun();
}

Tst10PrimaryGeneratorAction::~Tst10PrimaryGeneratorAction()
{
  delete particleGun;
}

void Tst10PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* aParticleDefinition 
    = particleTable->FindParticle(particleName="opticalphoton");

  // create a new vertex in a random position 

  G4ThreeVector VertexPosition (GetRandomPosition());
//  G4ThreeVector VertexPosition (G4ThreeVector(0,0,0));
  G4PrimaryVertex* aVertex = 
    new G4PrimaryVertex( VertexPosition, 0);

  G4int NumberOfParticlesToBeGenerated = 10000;
	G4cout << "  A " << NumberOfParticlesToBeGenerated << 
	    " optical photons vertex has been generated at " << VertexPosition << G4endl;
  // create new primaries and set them to the vertex
  for( int i=0; i<NumberOfParticlesToBeGenerated; i++ )
  {
		G4ThreeVector m = GetRandomDirection();
//		G4ThreeVector m(1,0,0);
    G4PrimaryParticle* aPrimaryParticle =
      new G4PrimaryParticle(aParticleDefinition,m.x(),m.y(),m.z());
    aPrimaryParticle->SetMass (0);
		G4ThreeVector p = GetRandomPolarization ( m );
    aPrimaryParticle->SetPolarization(p.x(),p.y(),p.z());
    aVertex->SetPrimary( aPrimaryParticle );
  }
  anEvent->AddPrimaryVertex( aVertex );
}

G4ThreeVector Tst10PrimaryGeneratorAction::GetRandomDirection() {

  G4ThreeVector retval;

  G4double CosTheta;
  G4double SinTheta;

  G4double Phi;
  G4double SinPhi;
  G4double CosPhi;

  G4double rand;

  rand = G4UniformRand();

  CosTheta = 2.0*rand -1.0;
  SinTheta = sqrt (1.-CosTheta*CosTheta);
  rand = G4UniformRand();
  Phi = twopi*rand;
  SinPhi = sin (Phi);
  CosPhi = cos (Phi);
  retval.setX(SinTheta*CosPhi);
  retval.setY(SinTheta*SinPhi);
  retval.setZ(CosTheta);

  return retval;
}

G4ThreeVector Tst10PrimaryGeneratorAction::GetRandomPosition() {

  G4double x = (G4UniformRand()*2-1)*m;
  G4double y = (G4UniformRand()*2-1)*m;
  G4double z = (G4UniformRand()*2-1)*m;

  G4ThreeVector retval (x, y, z);

  return retval;
}

G4ThreeVector Tst10PrimaryGeneratorAction::GetRandomPolarization(G4ThreeVector Direction) {
  G4ThreeVector Polarization = Direction.orthogonal();
  G4ThreeVector retval = Polarization.unit();
  return retval;
}

