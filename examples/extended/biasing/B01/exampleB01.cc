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
// $Id: exampleB01.cc,v 1.10 2002-05-31 11:46:23 dressel Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------
//      GEANT 4 - exampleB01
//
// --------------------------------------------------------------
// Comments
//
// 
// --------------------------------------------------------------

#include "g4std/set"
#include "g4std/iomanip"

#include "G4VPhysicalVolume.hh"
#include "G4RunManager.hh"

#include "B01DetectorConstruction.hh"
#include "B01PhysicsList.hh"
#include "B01PrimaryGeneratorAction.hh"

// Files specific for scoring 
#include "B01Scorer.hh"
#include "G4Sigma.hh"
#include "G4MassScoreSampler.hh"

// helper function for print out
G4std::string FillString(const G4std::string &name, char c, G4int n, G4bool back = true);

int main(int argc, char **argv)
{  

  G4std::ostream *myout = &G4cout;
  G4int numberOfEvent = 1000;

  G4String random_status_out_file, random_status_in_file;
  G4long myseed = 345354;

  HepRandom::setTheSeed(myseed);

  G4RunManager *runManager = new G4RunManager;
  
  // create the detector      ---------------------------
  runManager->SetUserInitialization(new B01DetectorConstruction);
  //  ---------------------------------------------------
  runManager->SetUserInitialization(new B01PhysicsList);
  runManager->SetUserAction(new B01PrimaryGeneratorAction);
  runManager->Initialize();

  // create scorer and sampler to score neutrons in the detector
  B01Scorer mScorer;
  G4MassScoreSampler msm(mScorer, "neutron"); // to be don after 
  msm.Initialize();                           // runManager->Initialize()

  runManager->BeamOn(numberOfEvent);

  // ======= after running ============================

  // print all the numbers calculated from the scorer
  *myout << "output mScorer, mass geometry, neutron" << G4endl;
  *myout << mScorer << G4endl;
  *myout << "----------------------------------------------"  << G4endl;

  // print some exclusive numbers

  // head line
  G4int FieldName = 25;
  G4int FieldValue = 12;
  G4std::string vname = FillString("Volume name", ' ', FieldName+1);
  *myout << vname << '|';
  vname = FillString(" AV E/Track ", ' ', FieldValue+1, false);
  *myout << vname << '|';
  vname = FillString(" sigma", ' ', FieldValue+1);
  *myout << vname << '|';
  vname = FillString("Coll_Ent.Tr", ' ', FieldValue+1, false);
  *myout << vname << '|';
  *myout << G4endl;



  const G4PMapPtkTallys &m = mScorer.GetMapPtkTallys();
  for (G4PMapPtkTallys::const_iterator mit = m.begin();
       mit != m.end(); mit++) {
    G4PTouchableKey ptk = (*mit).first; // get a key identifying a volume
    G4PMapNameTally mtallies = (*mit).second; // get tallies of the volume
    G4String name(ptk.fVPhysiclaVolume->GetName()); // print volume name
    G4int nEnteringTracks = 0;
    G4double colli_EnteringTrack = 0;
    G4double meanTrackEnergy = 0, sigmaTrackEnergy = 0;
    for (G4PMapNameTally::iterator mt = mtallies.begin();
	 mt != mtallies.end(); mt++) {
      G4String tmp((*mt).first);
      if (tmp == "HistorysEntering") {
	nEnteringTracks = G4int((*mt).second.GetXsum());
      }
      if (tmp == "EnergyEnteringHistory") {
	meanTrackEnergy =  (*mt).second.GetMean();
	sigmaTrackEnergy = (*mt).second.GetSigma();
      }
      if (tmp == "Collisions") {
	if (!nEnteringTracks) {
	  G4cout << "exampleB01: Error nEnteringTracks=0" <<G4endl;
	}
	else {
	  colli_EnteringTrack =  (*mt).second.GetXsum() / nEnteringTracks;
	}
      }
    }


    // print values

    G4std::string fname = FillString(name, '.', FieldName);
    *myout << fname << " |";
    *myout << G4std::setw(FieldValue) << meanTrackEnergy << " |"; 
    *myout << G4std::setw(FieldValue) << sigmaTrackEnergy << " |";
    *myout << G4std::setw(FieldValue) << colli_EnteringTrack << " |";
    *myout << G4endl;
  }

  return 0;
}

G4std::string FillString(const G4std::string &name, char c, G4int n, bool back)
{
  G4std::string fname;
  G4int k = n - name.size();
  if (k > 0) {
    if (back) {
      fname = name;
      fname += G4std::string(k,c);
    }
    else {
      fname = G4std::string(k,c);
      fname += name;
    }
  }
  else {
    fname = name;
  }
  return fname;
}
