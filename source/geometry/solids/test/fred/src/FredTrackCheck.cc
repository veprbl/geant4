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
// FredTrackCheck.cc
//
// Implemenation of track hit checker
//

#include "FredTrackCheck.hh"

//
// Constructor
//
FredTrackCheck::FredTrackCheck() {;}

//
// Destructor
//
FredTrackCheck::~FredTrackCheck()
{ 
  Clear();
}


//
// AddInHit
//
// Associated a new "in" hit with the track
//
void FredTrackCheck::AddInHit( const G4Track *track )
{
	//
	// Do we already know about this track?
	//
	FredTrackData	*trackData = 0;
	FredTrackData	*target = new FredTrackData( track->GetTrackID() );
	
        G4std::vector<FredTrackData*>::const_iterator i;
	for (i=hitList.begin(); i!=hitList.end(); ++i) {
	  if (**i==*target) {
	     trackData = *i;
	     break;
	  }
	}
	if (trackData) {
	
		//
		// Yup: increment appropriately
		//
		trackData->IncrementIn();
		delete target;
	}
	else {
		//
		// Nope: make one
		//
		target->IncrementIn();
		hitList.push_back( target );
	}
}

//
// AddOutHit
//
// Associated a new "out" hit with the track
//
void FredTrackCheck::AddOutHit( const G4Track *track )
{
	//
	// Do we already know about this track?
	//
	FredTrackData	*trackData = 0;
	FredTrackData	*target = new FredTrackData( track->GetTrackID() );
	
        G4std::vector<FredTrackData*>::const_iterator i;
	for (i=hitList.begin(); i!=hitList.end(); ++i) {
	  if (**i==*target) {
	     trackData = *i;
	     break;
	  }
	}
	if (trackData) {

		//
		// Yup: increment appropriately
		//
		trackData->IncrementOut();
		delete target;
	}
	else {
		//
		// Nope: make one
		//
		target->IncrementOut();
		hitList.push_back( target );
	}
}

//
// GetTrackStat
//
// Return statistics on specified track
//
void FredTrackCheck::GetTrackStat( const G4Track *track, G4int *nIn, G4int *nOut )
{
	GetTrackStatID( track->GetTrackID(), nIn, nOut );
}

//
// GetTrackStatID
//
// Return statistics on specified track
//
void FredTrackCheck::GetTrackStatID( G4int trackID, G4int *nIn, G4int *nOut )
{
	FredTrackData	*trackData = 0;
	FredTrackData	target( trackID );
	
        G4std::vector<FredTrackData*>::const_iterator i;
	for (i=hitList.begin(); i!=hitList.end(); ++i) {
	  if (**i==target) {
	     trackData = *i;
	     break;
	  }
	}
	if (trackData) {
		*nIn = trackData->GetNIn();
		*nOut = trackData->GetNOut();
	}
	else {
		*nIn = 0;
		*nOut = 0;
	}
}


//
// ClearAll
//
// Clear all track data
//
void FredTrackCheck::Clear()
{
  FredTrackData* a = 0;
  G4std::vector<FredTrackData*>::iterator i;
  while (hitList.size()>0)
  {
    a = hitList.back();
    hitList.pop_back();
    for (i=hitList.begin(); i!=hitList.end(); i++)
    {
      if (*i==a)
      {
	hitList.erase(i);
	i--;
      }
    } 
    if ( a )  delete a;    
  } 
}


