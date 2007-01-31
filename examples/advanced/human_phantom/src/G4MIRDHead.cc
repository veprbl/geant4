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
// Authors: S. Guatelli and M. G. Pia, INFN Genova, Italy
// 
// Based on code developed by the undergraduate student G. Guerrieri 
// Note: this is a preliminary beta-version of the code; an improved 
// version will be distributed in the next Geant4 public release, compliant
// with the design in a forthcoming publication, and subject to a 
// design and code review.
//
#include "G4MIRDHead.hh"
//#include "G4Processor/GDMLProcessor.h"
#include "globals.hh"
#include "G4HumanPhantomMaterial.hh"
#include "G4SDManager.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4UnionSolid.hh"
#include "G4Ellipsoid.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4EllipticalTube.hh"
#include "G4HumanPhantomColour.hh"

G4MIRDHead::G4MIRDHead()
{
  material = new G4HumanPhantomMaterial();
}

G4MIRDHead::~G4MIRDHead()
{
  delete material;
}

G4VPhysicalVolume* G4MIRDHead::ConstructOrgan(const G4String& volumeName,G4VPhysicalVolume* mother,
					      const G4String& colourName, G4bool wireFrame, G4bool sensitivity)
{
  G4cout << "Construct " << volumeName <<G4endl;
  G4Material* soft = material -> GetMaterial("soft_tissue");
  
  // MIRD male model
  // Ellipsoid
  G4double ax = 7.0 * cm;
  G4double by = 10.0 * cm;
  G4double cz = 8.50 * cm;
  G4double zcut1 = 0.0 * cm;
  G4double zcut2 = 8.5 * cm;

  G4Ellipsoid* head1 = new G4Ellipsoid("Head1", ax, by, cz, zcut1, zcut2);

   G4double dx = 7.0 * cm;
   G4double dy = 10.0 * cm;
   G4double dz = 7.75 * cm;
 

  G4EllipticalTube* head2 = new G4EllipticalTube("Head2", dx, dy, dz);

  G4UnionSolid* head = new G4UnionSolid("Head",head2,head1,
  				0, // Rotation 
  				G4ThreeVector(0.* cm, 0.*cm, 7.7500 * cm) );

  G4LogicalVolume* logicHead = new G4LogicalVolume(head, soft,"logical" + volumeName,
						   0, 0,0);
  G4RotationMatrix* rm = new G4RotationMatrix();
  rm -> rotateX(90.* degree);
  
  // Define rotation and position here!
  G4VPhysicalVolume* physHead = new G4PVPlacement(rm,
						  G4ThreeVector(0.* cm,77.75 *cm, 0.*cm),
						  "physicalHead",
						  logicHead,
						  mother,
						  false,
						  0, true);

  // Sensitive Body Part
 
  if (sensitivity==true)
  { 
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    logicHead->SetSensitiveDetector( SDman->FindSensitiveDetector("BodyPartSD") );
  }

  // Visualization Attributes
  //  G4VisAttributes* HeadVisAtt = new G4VisAttributes(G4Colour(0.94,0.5,0.5));
  G4HumanPhantomColour* colourPointer = new G4HumanPhantomColour();
  G4Colour colour = colourPointer -> GetColour(colourName);
  G4VisAttributes* HeadVisAtt = new G4VisAttributes(colour);

  HeadVisAtt->SetForceSolid(wireFrame);
  logicHead->SetVisAttributes(HeadVisAtt);

  // Testing Head Volume
  G4double HeadVol = logicHead->GetSolid()->GetCubicVolume();
  G4cout << "Volume of Head = " << HeadVol/cm3 << " cm^3" << G4endl;
  
  // Testing Head Material
  G4String HeadMat = logicHead->GetMaterial()->GetName();
  G4cout << "Material of Head = " << HeadMat << G4endl;
  
  // Testing Density
  G4double HeadDensity = logicHead->GetMaterial()->GetDensity();
  G4cout << "Density of Material = " << HeadDensity*cm3/g << " g/cm^3" << G4endl;

  // Testing Mass
  G4double HeadMass = (HeadVol)*HeadDensity;
  G4cout << "Mass of Head = " << HeadMass/gram << " g" << G4endl;
  
  return physHead;
}
