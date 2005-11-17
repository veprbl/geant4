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
// $Id: G4VSurface.hh,v 1.13 2005-11-17 16:59:31 link Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------------
// GEANT 4 class header file
//
//
// G4VSurface
//
// Class description:
//
//  Abstract base class for boundary surface of G4VSolid.

// Author: 
//   01-Aug-2002 - Kotoyo Hoshina (hoshina@hepburn.s.chiba-u.ac.jp)
//
// History:
//   13-Nov-2003 - O.Link (Oliver.Link@cern.ch), Integration in Geant4
//                 from original version in Jupiter-2.5.02 application.
// --------------------------------------------------------------------
#ifndef __G4VSURFACE__
#define __G4VSURFACE__

#include "G4VSolid.hh"
#include "geomdefs.hh"

#include "G4RotationMatrix.hh"

#define G4VSURFACENXX 10

class G4VSurface
{
 public:  // without description

   enum EValidate      {kDontValidate = 0, kValidateWithTol = 1, 
                        kValidateWithoutTol = 2, kUninitialized = 3};

 public:  // with description

   G4VSurface (const G4String &name);
   G4VSurface (const G4String &name,
               const G4RotationMatrix &rot,
               const G4ThreeVector    &tlate,
                     G4int             handedness,
               const EAxis             axis1,
               const EAxis             axis2,
                     G4double          axis0min = -kInfinity,
                     G4double          axis1min = -kInfinity,
                     G4double          axis0max = kInfinity,
                     G4double          axis1max = kInfinity);

   virtual ~G4VSurface();

   virtual G4int     AmIOnLeftSide(const G4ThreeVector &me, 
                                   const G4ThreeVector &vec, 
                                         G4bool withTol = true);

   virtual G4double  DistanceToBoundary(      G4int areacode,
                                              G4ThreeVector &xx,
                                        const G4ThreeVector &p) ;


   virtual G4double  DistanceToIn(const G4ThreeVector &gp,
                                  const G4ThreeVector &gv,
                                        G4ThreeVector &gxxbest);
   virtual G4double  DistanceToOut(const G4ThreeVector &gp,
                                   const G4ThreeVector &gv,
                                         G4ThreeVector &gxxbest);
   virtual G4double  DistanceTo(const G4ThreeVector &gp,
                                      G4ThreeVector &gxx);
      
   virtual G4int     DistanceToSurface(const G4ThreeVector &gp,
                                       const G4ThreeVector &gv,
                                             G4ThreeVector gxx[],
                                             G4double      distance[],
                                             G4int         areacode[],
                                             G4bool        isvalid[],
                                       EValidate validate=kValidateWithTol) = 0;

   virtual G4int     DistanceToSurface(const G4ThreeVector &gp,
                                             G4ThreeVector gxx[],
                                             G4double      distance[],
                                             G4int         areacode[]) = 0;
                                             
   void              DebugPrint() const;

   // get methods

   virtual G4ThreeVector GetNormal(const G4ThreeVector &xx,G4bool isGlobal) = 0;
   
   virtual G4String      GetName() const { return fName; }
   virtual void          GetBoundaryParameters(const G4int   &areacode,
                                               G4ThreeVector &d,
                                               G4ThreeVector &x0,
                                               G4int &boundarytype) const;
   virtual G4ThreeVector GetBoundaryAtPZ(G4int areacode,
                                         const G4ThreeVector &p) const;

   inline  G4double      DistanceToPlaneWithV(const G4ThreeVector &p,
                                              const G4ThreeVector &v,
                                              const G4ThreeVector &x0,
                                              const G4ThreeVector &n0,
                                                    G4ThreeVector &xx);

   inline  G4double      DistanceToPlane(const G4ThreeVector &p,
                                         const G4ThreeVector &x0,
                                         const G4ThreeVector &n0,
                                               G4ThreeVector &xx);
   
   inline  G4double      DistanceToPlane(const G4ThreeVector &p,
                                         const G4ThreeVector &x0,
                                         const G4ThreeVector &t1,
                                         const G4ThreeVector &t2,
                                               G4ThreeVector &xx,
                                               G4ThreeVector &n);

   inline  G4double      DistanceToLine (const G4ThreeVector &p,
                                         const G4ThreeVector &x0,
                                         const G4ThreeVector &d,
                                               G4ThreeVector &xx);
                                               
   inline  G4bool IsAxis0    (G4int areacode) const;
   inline  G4bool IsAxis1    (G4int areacode) const;
   inline  G4bool IsOutside  (G4int areacode) const;
   inline  G4bool IsInside   (G4int areacode, G4bool testbitmode = false) const;
   inline  G4bool IsBoundary (G4int areacode, G4bool testbitmode = false) const;
   inline  G4bool IsCorner   (G4int areacode, G4bool testbitmode = false) const;
   inline  G4bool IsValidNorm() const { return fIsValidNorm; }
           G4bool IsSameBoundary (G4VSurface *surface1, G4int areacode1,
                                  G4VSurface *surface2, G4int areacode2 ) const;
   inline  G4int  GetAxisType(G4int areacode, G4int whichaxis) const;

   inline  G4ThreeVector ComputeGlobalPoint     (const G4ThreeVector &lp) const;
   inline  G4ThreeVector ComputeLocalPoint      (const G4ThreeVector &gp) const;
   inline  G4ThreeVector ComputeGlobalDirection (const G4ThreeVector &lp) const;
   inline  G4ThreeVector ComputeLocalDirection  (const G4ThreeVector &gp) const;
  
   // set methods

   inline void SetAxis(G4int i, const EAxis axis)  { fAxis[i] = axis; }
   inline void SetNeighbours(G4VSurface* axis0min, G4VSurface* axis1min, 
                             G4VSurface* axis0max, G4VSurface* axis1max);

   virtual G4ThreeVector SurfacePoint(G4double , G4double, G4bool isGlobal = false ) = 0 ;
   virtual G4double GetBoundaryMin(G4double) = 0 ;
   virtual G4double GetBoundaryMax(G4double) = 0 ;
   virtual G4double GetSurfaceArea() = 0 ;

 public:  // without description

   G4VSurface(__void__&);
     // Fake default constructor for usage restricted to direct object
     // persistency for clients requiring preallocation of memory for
     // persistifiable objects.

 protected:  // with description
 
   // get methods

   inline  G4VSurface**  GetNeighbours() { return fNeighbours; } 
   inline  G4int         GetNeighbours(G4int areacode, G4VSurface* surfaces[]);
   inline  G4ThreeVector GetCorner(G4int areacode) const;
           void GetBoundaryAxis(G4int areacode, EAxis axis[]) const;
           void GetBoundaryLimit(G4int areacode, G4double limit[]) const;
   virtual G4int GetAreaCode(const G4ThreeVector &xx, G4bool withtol=true) = 0;
      
   // set methods

   virtual void SetBoundary(const G4int         &axiscode, 
                            const G4ThreeVector &direction,
                            const G4ThreeVector &x0, 
                            const G4int         &boundarytype);
     // areacode must be one of them:
     // sAxis0 & sAxisMin, sAxis0 & sAxisMax,
     // sAxis1 & sAxisMin, sAxis1 & sAxisMax.
     // boundarytype represents the shape of locus
     // from the start point to end point of boundary.
     // ex.
     // sAxisRho = linear line which start point is fixed at origin.
     // sAxisPhi = part of circle which center placed at the origin.
                            
   void SetCorner(G4int areacode, G4double x, G4double y, G4double z);

 private:

   virtual void SetBoundaries() = 0;
   virtual void SetCorners()    = 0;
   
 // data members ---------------------------------------------------------

 public:

   static const G4int sOutside ;
   static const G4int sInside  ;
   static const G4int sBoundary;
   static const G4int sCorner;
   static const G4int sC0Min1Min;
   static const G4int sC0Max1Min;
   static const G4int sC0Max1Max;
   static const G4int sC0Min1Max;
   static const G4int sAxisMin;
   static const G4int sAxisMax;
   static const G4int sAxisX;
   static const G4int sAxisY;
   static const G4int sAxisZ;
   static const G4int sAxisRho;
   static const G4int sAxisPhi;
   static const G4int sAxis0;
   static const G4int sAxis1;
   static const G4int sSizeMask;
   static const G4int sAxisMask;
   static const G4int sAreaMask;

 protected:
 
   class CurrentStatus 
   {
    public:

      CurrentStatus();
      virtual ~CurrentStatus();
      
      inline G4ThreeVector GetXX(G4int i)       const { return fXX[i];       }
      inline G4double      GetDistance(G4int i) const { return fDistance[i]; }
      inline G4int         GetAreacode(G4int i) const { return fAreacode[i]; }
      inline G4int         GetNXX()             const { return fNXX;         }
      inline G4bool        IsDone()             const { return fDone;        }
      inline G4bool        IsValid(G4int i)     const { return fIsValid[i];  }

      void SetCurrentStatus(G4int                i, 
                            G4ThreeVector       &xx, 
                            G4double            &dist, 
                            G4int               &areacode, 
                            G4bool              &isvalid,
                            G4int                nxx,
                            EValidate            validate,
                      const G4ThreeVector *p, 
                      const G4ThreeVector *v = 0);

      void ResetfDone(EValidate            validate,
                const G4ThreeVector *p, 
                const G4ThreeVector *v = 0);


      void DebugPrint() const;

    private:

      G4double             fDistance[G4VSURFACENXX];
      G4ThreeVector        fXX[G4VSURFACENXX];
      G4int                fAreacode[G4VSURFACENXX];
      G4bool               fIsValid[G4VSURFACENXX];
      G4int                fNXX;
      G4ThreeVector        fLastp;
      G4ThreeVector        fLastv;
      EValidate            fLastValidate;
      G4bool               fDone;
   };
      
   class Boundary 
   {
    public:
      Boundary();
      virtual ~Boundary();
      
      void SetFields(const G4int         &areacode, 
                     const G4ThreeVector &d, 
                     const G4ThreeVector &x0, 
                     const G4int         &boundarytype);
      
      G4bool IsEmpty() const;
      
      G4bool GetBoundaryParameters(const G4int         &areacode, 
                                         G4ThreeVector &d,
                                         G4ThreeVector &x0, 
                                         G4int         &boundarytype) const;

    private:
      G4int          fBoundaryAcode;
      G4ThreeVector  fBoundaryDirection;
      G4ThreeVector  fBoundaryX0;
      G4int          fBoundaryType;
   };

   EAxis               fAxis[2];
   G4double            fAxisMin[2];
   G4double            fAxisMax[2];
   CurrentStatus       fCurStatWithV;
   CurrentStatus       fCurStat;
   G4RotationMatrix    fRot;
   G4ThreeVector       fTrans;
   G4int               fHandedness;
   class G4SurfCurNormal
   {
     public:
       G4ThreeVector p;
       G4ThreeVector normal;
   };
   G4SurfCurNormal     fCurrentNormal;
   G4bool              fIsValidNorm;
                        
 private:
                     
   G4VSurface    *fNeighbours[4]; // {0,1,2,3} = sAxis0min, sAxis1min, 
                                  //             sAxis0max, sAxis1max
   G4ThreeVector fCorners[4];     // corners of the surface in local coordinate
   Boundary      fBoundaries[4];  // boundaries of the surface.
   G4String      fName;
   
   class G4SurfSideQuery
   {
     public:
       G4ThreeVector me;
       G4ThreeVector vec;
       G4bool        withTol;
       G4int         amIOnLeftSide;
   };
   G4SurfSideQuery fAmIOnLeftSide;
};

//========================================================
// inline functions
//========================================================

struct Intersection {
    
  G4double phi ;  // parameter phi
  G4double u ;    // parameter u
  G4ThreeVector xx ;   // intersection point in cartesian
  G4double distance ;  // distance to intersection
  G4int areacode;      // the areacode of the intersection
  G4bool isvalid ;     // valid intersection ??

} ;
  

inline
G4bool DistanceSort( const Intersection &a, const Intersection &b) 
{
  return a.distance < b.distance ;
}

inline
G4bool EqualIntersection( const Intersection &a, const Intersection &b)
{
  return ( ( a.xx - b.xx ).mag() < kCarTolerance ) ;
}


#include "G4VSurface.icc"

#endif
