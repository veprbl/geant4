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
//    **********************************
//    *                                *
//    *      BrachyEventAction.hh      *
//    *                                *
//    **********************************

#ifndef BrachyEventAction_h
#define BrachyEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
class BrachyDetectorConstruction; 
class BrachyAnalysisManager;
class BrachyEventAction : public G4UserEventAction
{
  public:
    BrachyEventAction(G4String&);
    ~BrachyEventAction();

  public:
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);

 


  private:
  G4double  EnergyDep;
     G4int m_NumVoxelX;
     G4int m_NumVoxelZ;
    
  BrachyDetectorConstruction *pDetector;
  G4double VoxelWidth_Z;
  G4double VoxelWidth_X;
 
 private:
    G4int m_HitsCollectionID;
    G4String       drawFlag;
    G4int          j;
  G4int i;
  G4int k;
  G4double x;
  G4double z;
    G4String      SDname;
    G4int  printModulo;      
   
};

#endif

    

