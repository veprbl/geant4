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
// SprayParticleGun.hh
//
// Declaration of test particle gun
//
// G4ParticleGun unfortunately is not written with enough modularity to
// inherit. So, this stuff here is written from scratch, but while refering
// to G4ParticleGun's code.
//

#ifndef SprayParticleGun_HH
#define SprayParticleGun_HH

#include "globals.hh"
#include "G4VPrimaryGenerator.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4PrimaryVertex.hh"
#include "G4ParticleMomentum.hh"

class G4Event;
class SprayParticleGunMessenger;

class SprayParticleGun : public G4VPrimaryGenerator
{
	public:
	SprayParticleGun();
	SprayParticleGun( G4ThreeVector start );
	~SprayParticleGun();
	
	void GeneratePrimaryVertex( G4Event *evt );
	
	inline void SetPosition( G4ThreeVector position ) { start = position; }
	inline G4ThreeVector GetPosition() { return start; }
	
	inline void SetXSpray( G4int xSpray ) {spray[0] = xSpray; }
	inline G4int GetXSpray() { return spray[0]; }
	
	inline void SetYSpray( G4int ySpray ) {spray[1] = ySpray; }
	inline G4int GetYSpray() { return spray[1]; }
	
	inline void SetZSpray( G4int zSpray ) {spray[2] = zSpray; }
	inline G4int GetZSpray() { return spray[2]; }
	
	
	private:
	void SetDefaults();

	protected:
	G4int spray[3];
	G4ThreeVector start;
	G4ParticleDefinition  *particleType;

	private:
	SprayParticleGunMessenger	*messenger;
};

#endif
