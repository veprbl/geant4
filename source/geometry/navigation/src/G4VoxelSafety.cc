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
// $Id: G4VoxelSafety.cc,v 1.2 2010-11-03 12:07:10 japost Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//  Author:  John Apostolakis
//  First version:  31 May 2010
// 
#include "G4VoxelSafety.hh"

#include "G4GeometryTolerance.hh"

#include "G4SmartVoxelProxy.hh"
#include "G4SmartVoxelNode.hh"
#include "G4SmartVoxelHeader.hh"

// State 
//    - values used during computation of Safety 
// 
// Constructor
//     - copied from G4VoxelNavigation  (1st version)
G4VoxelSafety::G4VoxelSafety()
  : fBlockList(),
    fpMotherLogical(0),
    fVoxelDepth(-1),
    fVoxelAxisStack(kNavigatorVoxelStackMax,kXAxis),
    fVoxelNoSlicesStack(kNavigatorVoxelStackMax,0),
    fVoxelSliceWidthStack(kNavigatorVoxelStackMax,0.),
    fVoxelNodeNoStack(kNavigatorVoxelStackMax,0),
    fVoxelHeaderStack(kNavigatorVoxelStackMax,(G4SmartVoxelHeader*)0),
    fVoxelNode(0),
    fCheck(false),
    fVerbose(0)
{
  kCarTolerance = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();
}

G4VoxelSafety::~G4VoxelSafety()
{
}

// ********************************************************************
// ComputeSafety
//
// Calculates the isotropic distance to the nearest boundary from the
// specified point in the local coordinate system. 
// The localpoint utilised must be within the current volume.
// ********************************************************************
//
G4double
G4VoxelSafety::ComputeSafety(const G4ThreeVector&     localPoint,
			     const G4VPhysicalVolume& currentPhysical,
			           G4double           maxLength)
{
  // G4VPhysicalVolume *samplePhysical;
  G4LogicalVolume *motherLogical;
  G4VSolid *motherSolid;
  G4SmartVoxelHeader *motherVoxelHeader;
  G4double motherSafety, ourSafety;
  G4int localNoDaughters;  // , sampleNo;
  // G4SmartVoxelNode *curVoxelNode;
  // G4int    curNoVolumes, contentNo;
  G4double daughterSafety;

  G4cout << "*** G4VoxelSafety::ComputeSafety(): ***" << G4endl; 

  // motherPhysical = history.GetTopVolume();
  motherLogical = currentPhysical.GetLogicalVolume();
  fpMotherLogical= motherLogical;   // For use by the other methods
  motherSolid = motherLogical->GetSolid();
  motherVoxelHeader= motherLogical->GetVoxelHeader();

  // Check that point is inside mother volume
  EInside  insideMother= motherSolid->Inside(localPoint); 
  if( insideMother != kInside  ) { 
    if( insideMother == kOutside ) {
      G4cerr << " G4VoxelSafety>  Location for safety is Outside current volume. " << G4endl;
      G4cerr << "     The approximate distance to the solid (safety from outside ) is " 
	     << motherSolid->DistanceToIn( localPoint ) << G4endl; 
    }
    return 0.0;
  }   

  //  First limit:  mother safety - distance to outer boundaries
  motherSafety = motherSolid->DistanceToOut(localPoint);
  ourSafety = motherSafety;                 // Working isotropic safety

#ifdef G4VERBOSE
  if(( fCheck ) && ( fVerbose == 1 ))
  {
    G4cout << "*** G4VoxelNavigation::ComputeSafety(): ***" << G4endl
           << "    Invoked DistanceToOut(p) for mother solid: "
           << motherSolid->GetName()
           << ". Solid replied: " << motherSafety << G4endl
           << "    For local point p: " << localPoint
           << ", to be considered as 'mother safety'." << G4endl;
  }
#endif
  localNoDaughters = motherLogical->GetNoDaughters();

  fBlockList.Enlarge(localNoDaughters);
  fBlockList.Reset();

  fVoxelDepth = 0;  // Resets the depth -- must be done for next method
  daughterSafety= SafetyForVoxelHeader( motherVoxelHeader, localPoint ); 

  ourSafety= std::min( motherSafety, daughterSafety ); 

  return ourSafety;
}

// Calculate the safety for volumes included in current Voxel Node
// 
G4double
G4VoxelSafety::
SafetyForVoxelNode( G4SmartVoxelNode    *curVoxelNode, 
		    const G4ThreeVector& localPoint )
{
   G4double ourSafety= DBL_MAX;

   G4int    curNoVolumes, contentNo, sampleNo;
   G4VPhysicalVolume *samplePhysical;

   G4double      sampleSafety=0.0; 
   G4ThreeVector samplePoint;
   G4VSolid*     ptrSolid=0;

   curNoVolumes = curVoxelNode->GetNoContained();

   for ( contentNo=curNoVolumes-1; contentNo>=0; contentNo-- )
   {
      sampleNo = curVoxelNode->GetVolume(contentNo);
      if ( !fBlockList.IsBlocked(sampleNo) ) 
      { 
	fBlockList.BlockVolume(sampleNo);

	samplePhysical = fpMotherLogical->GetDaughter(sampleNo);
	G4AffineTransform sampleTf(samplePhysical->GetRotation(),
				   samplePhysical->GetTranslation());
	sampleTf.Invert();
	samplePoint =  sampleTf.TransformPoint(localPoint);
	ptrSolid =  samplePhysical->GetLogicalVolume()->GetSolid();

	sampleSafety = ptrSolid->DistanceToIn(samplePoint);
	ourSafety   = std::min( sampleSafety, ourSafety ); 
#ifdef G4VERBOSE
	if(( fCheck ) && ( fVerbose == 1 ))
	{
	  // ReportSolidSafetyToIn( MethodName, solid, value, point ); 
	  G4cout << "*** G4VoxelSafety::SafetyForVoxelNode : ***" << G4endl
		 << "    Invoked DistanceToIn(p) for daughter solid: "
		 << ptrSolid->GetName()
		 << ". Solid replied: " << sampleSafety << G4endl
		 << "    For local point p: " << samplePoint
		 << ", to be considered as 'daughter safety'." << G4endl;
        }
#endif
      }
   }  // end for contentNo

   return ourSafety; 
}


//
//  Pseudo-code for  Compute Safety
//
//  for each (potential) dimension of depth
//     iterate through VoxelProxies (Header, Node) 
//      until  distanceToVoxel  > estimatedSafety
// 
//  Better:
//    iterate through/down the three of VoxelProxies 
//      while    distanceToVoxel <= estimatedSafety
//      Examine one  Nodes for which this condition holds. 
//                   (version 0 can examine all nodes)


// How to step through voxels ??   Thu 29 April 2010 @ 17:00

// ********************************************************************
//  SafetyForVoxelHeader method
//   - which cycles through levels of headers to process each node level
//   - Obtained by modifying VoxelLocate (to cycle through Node Headers)
// *********************************************************************
//
G4double
G4VoxelSafety::SafetyForVoxelHeader( G4SmartVoxelHeader* pHeader,
		      const G4ThreeVector& localPoint )
{
  const G4SmartVoxelHeader *targetVoxelHeader=pHeader;
  G4SmartVoxelNode *targetVoxelNode=0;

  G4SmartVoxelProxy *sampleProxy;
  EAxis    targetHeaderAxis;
  G4double targetHeaderMin, targetHeaderNodeWidth;
  G4int    targetHeaderNoSlices, targetNodeNo;
  G4double minSafety= DBL_MAX; 
 
  // fVoxelDepth  set by ComputeSafety or previous level call

  targetHeaderAxis =      targetVoxelHeader->GetAxis();
  targetHeaderNoSlices =  targetVoxelHeader->GetNoSlices();
  targetHeaderMin =       targetVoxelHeader->GetMinExtent();
  targetHeaderNodeWidth = (targetVoxelHeader->GetMaxExtent()-targetHeaderMin)
                          / targetHeaderNoSlices;

  // Stack info for stepping
  //
  fVoxelAxisStack[fVoxelDepth] = targetHeaderAxis;
  fVoxelNoSlicesStack[fVoxelDepth] = targetHeaderNoSlices;
  fVoxelSliceWidthStack[fVoxelDepth] = targetHeaderNodeWidth;
  fVoxelNodeNoStack[fVoxelDepth] = targetNodeNo;
  fVoxelHeaderStack[fVoxelDepth] = pHeader;
  
  G4cout << G4endl;
  G4cout << "**** G4VoxelSafety::SafetyForVoxelHeader  " << G4endl;
  G4cout << "   Depth            = " << fVoxelDepth ; // << G4endl;
  G4cout << "   Number of Slices = " << targetHeaderNoSlices ; // << G4endl;
  G4cout << "   Header (address) = " << targetVoxelHeader << G4endl;
  G4int  numSlicesCheck= targetHeaderNoSlices; 

  // G4cout << "---> Current Voxel Header has " << *targetVoxelHeader << G4endl;

  for( targetNodeNo= 0; targetNodeNo<targetHeaderNoSlices; targetNodeNo++ )
  {
     G4double nodeSafety= DBL_MAX, levelSafety= DBL_MAX;

     sampleProxy = targetVoxelHeader->GetSlice(targetNodeNo);

     // G4cout << " -Checking node " << targetNodeNo; 
     //        << " is proxy with address " << sampleProxy; //  << G4endl;

     if ( sampleProxy->IsNode() ) 
     {
	targetVoxelNode = sampleProxy->GetNode();
	// G4cout << " -- It is a Node " << G4endl;

	// Deal with the node here [ i.e. the last level ] 
	nodeSafety= SafetyForVoxelNode( targetVoxelNode, localPoint); 
        if( targetHeaderNoSlices != numSlicesCheck ) 
	  G4cerr << "Number of slices changed - to " << targetHeaderNoSlices << G4endl;
        minSafety= std::min( minSafety, nodeSafety ); 
     }
     else  
     {
        G4SmartVoxelHeader *pNewVoxelHeader = sampleProxy->GetHeader();
	fVoxelDepth++;
	// Recurse to deal with lower levels
	// G4cout << " -- It is a Header " << G4endl;

        // G4cout << " Increased  fVoxelDepth to " << fVoxelDepth << G4endl;
	G4cout << "  Recurse to deal with next lower level, fVoxelDepth= " 
	       << fVoxelDepth << G4endl;
	
	levelSafety= SafetyForVoxelHeader( pNewVoxelHeader, localPoint); 
        fVoxelDepth--; 

        G4cout << "  Returned from SafetyForVoxelHeader. Depth=  "
	       << fVoxelDepth << G4endl;
        // G4cout << "  Decreased  fVoxelDepth to " << fVoxelDepth << G4endl;
	// G4cout << "  Header (address)= " << targetVoxelHeader << G4endl;

	minSafety= std::min( minSafety, levelSafety );
     }

  } 
  G4cout << " Ended for targetNodeNo -- checked "
	 << targetHeaderNoSlices << " slices" << G4endl;
  G4cout << " ===== Returning from SafetyForVoxelHeader " 
	 << "  Depth= " << fVoxelDepth << G4endl
	 << G4endl;  

  return minSafety;
}
