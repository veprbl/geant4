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
// FredDetectorConstruction.hh
//
// Definition of fred's detector
//

#ifndef FredDetectorConstruction_H
#define FredDetectorConstruction_H

#include "FredMessenger.hh"

class G4VPhysicalVolume;

#include "G4VUserDetectorConstruction.hh"

class FredDetectorConstruction : public G4VUserDetectorConstruction
{
	public:
	FredDetectorConstruction( FredMessenger *ourMessenger );
	~FredDetectorConstruction();
	
	G4VPhysicalVolume *Construct();
	
	inline G4VSolid	  *GetTestVolume() {return testVolume;}
	
	private:
	FredMessenger	*messenger;
	G4VSolid	*testVolume;
};

#endif
