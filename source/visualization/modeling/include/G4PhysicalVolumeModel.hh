//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: G4PhysicalVolumeModel.hh,v 1.36 2010-05-11 11:16:51 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// John Allison  31st December 1997.
//
// Class Description:
//
// Model for physical volumes.  It describes a physical volume and its
// daughters to any desired depth.  Note: the "requested depth" is
// specified in the modeling parameters; enum {UNLIMITED = -1}.
//
// For access to base class information, e.g., modeling parameters,
// use GetModelingParameters() inherited from G4VModel.  See Class
// Description of the base class G4VModel.
//
// G4PhysicalVolumeModel assumes the modeling parameters have been set
// up with meaningful information - default vis attributes and culling
// policy in particular.

#ifndef G4PHYSICALVOLUMEMODEL_HH
#define G4PHYSICALVOLUMEMODEL_HH

#include "G4VModel.hh"
#include "G4VTouchable.hh"

#include "G4Transform3D.hh"
#include "G4Plane3D.hh"
#include <iostream>
#include <vector>
#include <map>

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4VSolid;
class G4Material;
class G4VisAttributes;
class G4AttDef;
class G4AttValue;

class G4PhysicalVolumeModel: public G4VModel {

public: // With description

  enum {UNLIMITED = -1};

  enum ClippingMode {subtraction, intersection};

  class G4PhysicalVolumeNodeID {
  public:
    G4PhysicalVolumeNodeID
    (G4VPhysicalVolume* pPV = 0,
     G4int iCopyNo = 0,
     G4int depth = 0,
     const G4Transform3D& transform = G4Transform3D(),
     G4bool drawn = true):
      fpPV(pPV),
      fCopyNo(iCopyNo),
      fNonCulledDepth(depth),
      fTransform(transform),
      fDrawn(drawn) {}
    G4VPhysicalVolume* GetPhysicalVolume() const {return fpPV;}
    G4int GetCopyNo() const {return fCopyNo;}
    G4int GetNonCulledDepth() const {return fNonCulledDepth;}
    const G4Transform3D& GetTransform() const {return fTransform;}
    G4bool GetDrawn() const {return fDrawn;}
    G4bool operator< (const G4PhysicalVolumeNodeID& right) const;
  private:
    G4VPhysicalVolume* fpPV;
    G4int fCopyNo;
    G4int fNonCulledDepth;
    G4Transform3D fTransform;
    G4bool fDrawn;
  };
  // Nested class for identifying physical volume nodes.

  class G4PhysicalVolumeModelTouchable: public G4VTouchable {
  public:
    G4PhysicalVolumeModelTouchable
    (const std::vector<G4PhysicalVolumeNodeID>& fullPVPath);
    const G4ThreeVector& GetTranslation(G4int depth) const;
    const G4RotationMatrix* GetRotation(G4int depth) const;
    G4VPhysicalVolume* GetVolume(G4int depth) const;
    G4VSolid* GetSolid(G4int depth) const;
    G4int GetReplicaNumber(G4int depth) const;
    G4int GetHistoryDepth() const {return fFullPVPath.size();}
  private:
    const std::vector<G4PhysicalVolumeNodeID>& fFullPVPath;
  };
  // Nested class for handling nested parameterisations.

  G4PhysicalVolumeModel
  (G4VPhysicalVolume*,
   G4int requestedDepth = UNLIMITED,
   const G4Transform3D& modelTransformation = G4Transform3D(),
   const G4ModelingParameters* = 0,
   G4bool useFullExtent = false);

  virtual ~G4PhysicalVolumeModel ();

  void DescribeYourselfTo (G4VGraphicsScene&);
  // The main task of a model is to describe itself to the graphics scene
  // handler (a object which inherits G4VSceneHandler, which inherits
  // G4VGraphicsScene).

  G4String GetCurrentDescription () const;
  // A description which depends on the current state of the model.

  G4String GetCurrentTag () const;
  // A tag which depends on the current state of the model.

  G4VPhysicalVolume* GetTopPhysicalVolume () const {return fpTopPV;}

  G4int GetRequestedDepth () const {return fRequestedDepth;}

  const G4VSolid* GetClippingSolid () const
  {return fpClippingSolid;}

  G4int GetCurrentDepth() const {return fCurrentDepth;}
  // Current depth of geom. hierarchy.

  G4VPhysicalVolume* GetCurrentPV() const {return fpCurrentPV;}
  // Current physical volume.

  G4LogicalVolume* GetCurrentLV() const {return fpCurrentLV;}
  // Current logical volume.

  G4Material* GetCurrentMaterial() const {return fpCurrentMaterial;}
  // Current material.

  const std::vector<G4PhysicalVolumeNodeID>& GetDrawnPVPath() const
  {return fDrawnPVPath;}
  // Path of the current drawn (non-culled) volume in terms of drawn
  // (non-culled) ancesters.  It is a vector of physical volume node
  // identifiers corresponding to the geometry hierarchy actually
  // selected, i.e., not culled.  It is guaranteed that volumes are
  // presented to the scene handlers in top-down hierarchy order,
  // i.e., ancesters first, mothers before daughters, so the scene
  // handler can be assured that, if it is building its own scene
  // graph tree, a mother, if any, will have already been encountered
  // and there will already be a node in place on which to hang the
  // current volume.

  const std::map<G4String,G4AttDef>* GetAttDefs() const;
  // Attribute definitions for current solid.

  std::vector<G4AttValue>* CreateCurrentAttValues() const;
  // Attribute values for current solid.  Each must refer to an
  // attribute definition in the above map; its name is the key.  The
  // user must test the validity of this pointer (it must be non-zero
  // and conform to the G4AttDefs, which may be checked with
  // G4AttCheck) and delete the list after use.  See
  // G4XXXStoredSceneHandler::PreAddSolid for how to access and
  // G4VTrajectory::ShowTrajectory for an example of the use of
  // G4Atts.

  void SetRequestedDepth (G4int requestedDepth) {
    fRequestedDepth = requestedDepth;
  }

  void SetClippingSolid (G4VSolid* pClippingSolid) {
    fpClippingSolid = pClippingSolid;
  }

  void SetClippingMode (ClippingMode mode) {
    fClippingMode = mode;
  }

  G4bool Validate (G4bool warn);
  // Validate, but allow internal changes (hence non-const function).

  void CurtailDescent() {fCurtailDescent = true;}

protected:

  void VisitGeometryAndGetVisReps (G4VPhysicalVolume*,
				   G4int requestedDepth,
				   const G4Transform3D&,
				   G4VGraphicsScene&);

  void DescribeAndDescend (G4VPhysicalVolume*,
			   G4int requestedDepth,
			   G4LogicalVolume*,
			   G4VSolid*,
			   G4Material*,
			   const G4Transform3D&,
			   G4VGraphicsScene&);

  virtual void DescribeSolid (const G4Transform3D& theAT,
			      G4VSolid* pSol,
			      const G4VisAttributes* pVisAttribs,
			      G4VGraphicsScene& sceneHandler);

  void CalculateExtent ();

  /////////////////////////////////////////////////////////
  // Data members...

  G4VPhysicalVolume* fpTopPV;        // The physical volume.
  G4String           fTopPVName;     // ...of the physical volume.
  G4int              fTopPVCopyNo;   // ...of the physical volume.
  G4int              fRequestedDepth;
                     // Requested depth of geom. hierarchy search.
  G4bool             fUseFullExtent; // ...if requested.
  G4int              fCurrentDepth;  // Current depth of geom. hierarchy.
  G4VPhysicalVolume* fpCurrentPV;    // Current physical volume.
  G4LogicalVolume*   fpCurrentLV;    // Current logical volume.
  G4Material*    fpCurrentMaterial;  // Current material.
  G4Transform3D* fpCurrentTransform; // Current transform.
  std::vector<G4PhysicalVolumeNodeID> fFullPVPath;
  std::vector<G4PhysicalVolumeNodeID> fDrawnPVPath;
  G4bool             fCurtailDescent;// Can be set to curtail descent.
  G4VSolid*          fpClippingSolid;
  ClippingMode       fClippingMode;

private:

  // Private copy constructor and assigment operator - copying and
  // assignment not allowed.  Keeps CodeWizard happy.
  G4PhysicalVolumeModel (const G4PhysicalVolumeModel&);
  G4PhysicalVolumeModel& operator = (const G4PhysicalVolumeModel&);
};

std::ostream& operator<<
  (std::ostream& os, const G4PhysicalVolumeModel::G4PhysicalVolumeNodeID);

#endif
