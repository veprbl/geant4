// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4ASCIITreeSceneHandler.cc,v 1.3 2001-05-22 12:16:54 johna Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// John Allison  5th April 2001
// A scene handler to dump geometry hierarchy.
// Based on a provisional G4ASCIITreeGraphicsScene (was in modeling).

#include "G4ASCIITreeSceneHandler.hh"

#include "G4ASCIITree.hh"
#include "G4VSolid.hh"
#include "G4PhysicalVolumeModel.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VPVParameterisation.hh"

G4ASCIITreeSceneHandler::G4ASCIITreeSceneHandler(G4VGraphicsSystem& system,
						 const G4String& name):
  G4VTreeSceneHandler(system, name) {}

G4ASCIITreeSceneHandler::~G4ASCIITreeSceneHandler () {}

void G4ASCIITreeSceneHandler::BeginModeling () {
  const G4ASCIITree* pSystem = (G4ASCIITree*)GetGraphicsSystem();
  const G4int verbosity = pSystem->GetVerbosity();
  G4cout << "\nG4ASCIITreeSceneHandler::BeginModeling:"
    "\n  set verbosity with \"/vis/ASCIITree/verbose <verbosity>\":"
    "\n  <= 0: mimimum verbosity:"
    "\n        - does not print daughters of repeated logical volumes."
    "\n        - does not repeat replicas."
    "\n   > 0: prints all physical volumes."
    "\n  Now printing with verbosity " << verbosity << G4endl;
}

void G4ASCIITreeSceneHandler::EndModeling () {
  fLVSet.clear();
  fReplicaSet.clear();
}

void G4ASCIITreeSceneHandler::Dump (const G4VSolid& solid) {

  const G4ASCIITree* pSystem = (G4ASCIITree*)GetGraphicsSystem();
  const G4int verbosity = pSystem->GetVerbosity();

  if (verbosity <= 0 && fReplicaSet.find(fpCurrentPV) != fReplicaSet.end()) {
    // Ignore if an already treated replica.  (Assumes that the model
    // which has invoked this function is a G4PhysicalVolumeModel - we
    // check this by testing fpCurrentPV.)
    if (fpCurrentPV) {
      ((G4PhysicalVolumeModel*)fpModel)->CurtailDescent();
      return;
    }
  }

  // Print indented text...
  for (G4int i = 0; i < fCurrentDepth; i++ ) G4cout << "  ";
  G4cout << "\"" << fpCurrentPV->GetName()
	 << "\", copy no. " << fpCurrentPV->GetCopyNo();

  if (fpCurrentPV->IsReplicated()) {
    fReplicaSet.insert(fpCurrentPV);  // Record new replica volume.
    if (verbosity <= 0) {
      // Add printing for replicas (when replicas are ignored)...
      EAxis axis;
      G4int nReplicas;
      G4double width;
      G4double offset;
      G4bool consuming;
      fpCurrentPV->GetReplicationData(axis,nReplicas,width,offset,consuming);
      G4VPVParameterisation* pP = fpCurrentPV->GetParameterisation();
      G4cout << " (" << nReplicas;
      if (pP) {
	G4cout << " parametrised volumes)";
      }
      else {
	G4cout << " replicas)";
      }
    }
  }
  else {
    if (fLVSet.find(fpCurrentLV) != fLVSet.end()) {
      if (verbosity <= 0) {
	// Add printing for repeated logical volume...
	G4cout << " (repeated logical volume)";
	// Ignore if an already treated logical volume.
	if (fpCurrentPV) {
	  ((G4PhysicalVolumeModel*)fpModel)->CurtailDescent();
	  G4cout << G4endl;
	  return;
	}
      }
    }
  }

  if (fLVSet.find(fpCurrentLV) == fLVSet.end()) {
    fLVSet.insert(fpCurrentLV);  // Record new logical volume.
  }

  G4cout << G4endl;
  return;
}
