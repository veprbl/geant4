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
#include "G4MIRDUterus.hh"
#include "globals.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Ellipsoid.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4HumanPhantomMaterial.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4HumanPhantomColour.hh"
G4MIRDUterus::G4MIRDUterus()
{
}

G4MIRDUterus::~G4MIRDUterus()
{
}

G4VPhysicalVolume* G4MIRDUterus::ConstructOrgan(const G4String& volumeName,G4VPhysicalVolume* mother,  
						const G4String& colourName, G4bool wireFrame, G4bool sensitivity)
{
 
  G4cout << "Construct " << volumeName << G4endl;
 
 G4HumanPhantomMaterial* material = new G4HumanPhantomMaterial();
 G4Material* soft = material -> GetMaterial("soft_tissue");
 delete material;

 G4double ax= 2.5*cm; //a
 G4double by= 1.5*cm; //c
 G4double cz= 5.*cm; //b
 G4double zcut1= -5.* cm; //-b
 G4double zcut2= 2.5*cm; //y1-y0

 G4Ellipsoid* uterus = new G4Ellipsoid("Uterus",
				       ax, by, cz,
				       zcut1, zcut2);

  G4LogicalVolume* logicUterus = new G4LogicalVolume(uterus,
						     soft,
						     "logical" + volumeName,
						     0, 0, 0);


  G4RotationMatrix* rm = new G4RotationMatrix();
  rm -> rotateX(90.* degree); 
  
  // Define rotation and position here!
  G4VPhysicalVolume* physUterus = new G4PVPlacement(rm,
				G4ThreeVector(0. *cm, 2*cm,-21 *cm),
						    "physicalUterus", //y0
  			       logicUterus,
			       mother,
			       false,
			       0, true);

  // Sensitive Body Part
  if (sensitivity==true)
  { 
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    logicUterus->SetSensitiveDetector( SDman->FindSensitiveDetector("BodyPartSD") );
  }

  // Visualization Attributes
  //  G4VisAttributes* UterusVisAtt = new G4VisAttributes(G4Colour(0.85,0.44,0.84));

  G4HumanPhantomColour* colourPointer = new G4HumanPhantomColour();
  G4Colour colour = colourPointer -> GetColour(colourName);
  G4VisAttributes* UterusVisAtt = new G4VisAttributes(colour);  
  UterusVisAtt->SetForceSolid(wireFrame);
  logicUterus->SetVisAttributes(UterusVisAtt);

  G4cout << "Uterus created !!!!!!" << G4endl;

  // Testing Uterus Volume
  G4double UterusVol = logicUterus->GetSolid()->GetCubicVolume();
  G4cout << "Volume of Uterus = " << UterusVol/cm3 << " cm^3" << G4endl;
  
  // Testing Uterus Material
  G4String UterusMat = logicUterus->GetMaterial()->GetName();
  G4cout << "Material of Uterus = " << UterusMat << G4endl;
  
  // Testing Density
  G4double UterusDensity = logicUterus->GetMaterial()->GetDensity();
  G4cout << "Density of Material = " << UterusDensity*cm3/g << " g/cm^3" << G4endl;

  // Testing Mass
  G4double UterusMass = (UterusVol)*UterusDensity;
  G4cout << "Mass of Uterus = " << UterusMass/gram << " g" << G4endl;
  
  return physUterus;
}
