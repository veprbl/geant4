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
// FredSensMother.cc
//
// Implementation of our sensitive mother volume
//

#include "FredSensMother.hh"
#include "G4StepStatus.hh"

//
// Constructor
//
FredSensMother::FredSensMother( G4String name ) : G4VSensitiveDetector( name )
{
	//
	// Now we play the c++ guessing game: which inherited
	// parameters do we fill? Should be obvious, but,
	// the only clue I got was in the examples.
	//
	// IMHO: it would be
	// better to have an inherited class with more arguments
	// whose constructor is explicitly invoked here.
	// Then there would be no ambiguities.
	//
	collectionName.insert( "fredsMotherStuff" );
}

//
// Destructor
//
// Odd we don't bother deleting the hits here... shouldn't we?
// Perhaps it doesn't matter, since G4 is pretty much static when
// it comes to detector configuration. But, we're all c++ experts,
// so we need to go through the moves.
//
FredSensMother::~FredSensMother() {;}

//
// Initialize
//
// Despite the misnomer, this is done at the beginning of each event.
// HCE is a meta-list of hit collections where one is allowed to store
// stuff. I'm not sure why you are forced to do this.
//
void FredSensMother::Initialize( G4HCofThisEvent *HCE )
{
	hits = new FredHitCollection( SensitiveDetectorName, collectionName[0] );

	static int HCID = -1;
	if (HCID < 0) HCID = GetCollectionID(0);
	HCE->AddHitsCollection( HCID, hits );
}

//
// ProcessHits
//
// Another misnomer: this member function processes one step in the 
// sensitive volume.
//
// IMHO: the fact that a argument is delivered which I don't need
// is not good design.
//
G4bool FredSensMother::ProcessHits( G4Step *step, G4TouchableHistory *notUsed )
{
	G4bool imHit = false;
	
	//
	// Okay, for test purposes, I want to store hits that just
	// started.
	//
	if (step->GetPreStepPoint()->GetGlobalTime() == 0.0) {
		FredHit *hit = new FredHit( step->GetPreStepPoint()->GetPosition(), true, step->GetTrack() );
		hits->insert( hit );
		imHit = true;
	}
	
	return imHit;
}

