// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4makevol.cc,v 1.12 1999-05-22 06:32:01 lockman Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "globals.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G3toG4.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4Para.hh"
#include "G4Hype.hh"
#include "G4Material.hh"
#include "G4makevol.hh"
#include "G3VolTable.hh"
#include "G3MedTable.hh"
        
void G4makevol(const G4String& vname, const G4String& shape, const G4int nmed,
	       const G4double* Rpar, const G4int npar){
    
  // EAxis axis = kXAxis;
  
  // Create the solid if no negative length parameters
  G4VSolid *solid = 0;
  G4bool NegVolPars = false;

  // if npar = 0 assume LV deferral
  G4bool Deferred = (npar == 0);
  
  if ( shape == "BOX" ) {
    G4double pX = Rpar[0]*cm;
    G4double pY = Rpar[1]*cm;
    G4double pZ = Rpar[2]*cm;
    
    NegVolPars = pX<0 || pY<0 || pZ<0;
    
    if (!(NegVolPars || Deferred)) { 
      solid = new G4Box(vname, pX, pY, pZ);
    }

  } else if ( shape == "TRD1" ) {
    G4double pdx1 = Rpar[0]*cm;
    G4double pdx2 = Rpar[1]*cm;
    G4double pdy1 = Rpar[2]*cm;
    G4double pdy2 = pdy1;
    G4double pdz  = Rpar[3]*cm;
    
    NegVolPars = pdx1<0 || pdx2<0 || pdy1<0 || pdz<0;

    if (!(NegVolPars || Deferred)) {
      solid = new G4Trd(vname, pdx1, pdx2, pdy1, pdy2, pdz);
    }

  } else if ( shape == "TRD2" ) {
    G4double pdx1 = Rpar[0]*cm;
    G4double pdx2 = Rpar[1]*cm;
    G4double pdy1 = Rpar[2]*cm;
    G4double pdy2 = Rpar[3]*cm;
    G4double pdz  = Rpar[4]*cm;

    NegVolPars = pdx1<0 || pdx2<0 || pdy1<0 || pdy2<0 || pdz<0;
 
    if (!(NegVolPars || Deferred)) {
      solid = new G4Trd(vname, pdx1, pdx2, pdy1, pdy2, pdz);
    }

  } else if ( shape == "TRAP" ) {
    G4double pDz    = Rpar[0]*cm;
    G4double pTheta = Rpar[1]*deg;
    G4double pPhi   = Rpar[2]*deg;
    G4double pDy1   = Rpar[3]*cm;
    G4double pDx1   = Rpar[4]*cm;
    G4double pDx2   = Rpar[5]*cm;
    G4double pAlp1  = Rpar[6]*deg;
    G4double pDy2   = Rpar[7]*cm;
    G4double pDx3   = Rpar[8]*cm;
    G4double pDx4   = Rpar[9]*cm;
    G4double pAlp2  = Rpar[10]*deg;

    NegVolPars= pDz<0 || pDy1<0 || pDx1<0 || pDx2<0 || pDy2<0 || pDx3<0 || pDx4<0;

    if (!(NegVolPars || Deferred)) {
      solid = new 
	G4Trap(vname, pDz, pTheta, pPhi, pDy1, pDx1, pDx2, pAlp1, pDy2, pDx3, 
	       pDx4, pAlp2);
    }

  } else if ( shape == "TUBE" ) {
    G4double pRMin = Rpar[0]*cm;
    G4double pRMax = Rpar[1]*cm;
    G4double pDz   = Rpar[2]*cm;
    G4double pSPhi = 0.*deg;
    G4double pDPhi = 360.*deg;
    
    NegVolPars = pRMin<0 || pRMax<0 || pDz<0;
    
    if (!(NegVolPars || Deferred)) {
      solid = new G4Tubs(vname, pRMin, pRMax, pDz, pSPhi, pDPhi);
    }

  } else if ( shape == "TUBS" ) {
    G4double pRMin = Rpar[0]*cm;
    G4double pRMax = Rpar[1]*cm;
    G4double pDz   = Rpar[2]*cm;
    G4double pSPhi = Rpar[3]*deg;
    G4double pDPhi = Rpar[4]*deg - pSPhi;
    if ( Rpar[4]*deg <= pSPhi ) pDPhi = pDPhi + 360.*deg;

    NegVolPars = pRMin<0 || pRMax<0 || pDz<0;

    if (!(NegVolPars || Deferred)){
      solid = new G4Tubs(vname, pRMin, pRMax, pDz, pSPhi, pDPhi);
    }

  } else if ( shape == "CONE" ) {
    G4double pDz    = Rpar[0]*cm;
    G4double pRmin1 = Rpar[1]*cm;
    G4double pRmax1 = Rpar[2]*cm;
    G4double pRmin2 = Rpar[3]*cm;
    G4double pRmax2 = Rpar[4]*cm;
    G4double pSPhi = 0.*deg;
    G4double pDPhi = 360.*deg;

    NegVolPars = pDz<0 || pRmin1<0 || pRmax1<0 || pRmin2<0 || pRmax2<0;

    if (!(NegVolPars || Deferred)){
      solid = new 
	G4Cons(vname, pRmin1, pRmax1, pRmin2, pRmax2, pDz, pSPhi, pDPhi);
    }

  } else if ( shape == "CONS" ) {
    G4double pDz    = Rpar[0]*cm;
    G4double pRmin1 = Rpar[1]*cm;
    G4double pRmax1 = Rpar[2]*cm;
    G4double pRmin2 = Rpar[3]*cm;
    G4double pRmax2 = Rpar[4]*cm;
    G4double pSPhi  = Rpar[5]*deg;
    G4double pDPhi  = Rpar[6]- pSPhi;
    if ( Rpar[6]*deg <= pSPhi ) pDPhi = pDPhi + 360.*deg;

    NegVolPars = pDz<0 || pRmin1<0 || pRmax1<0 || pRmin2<0 || pRmax2<0;

    if (!(NegVolPars || Deferred)){
      solid = new 
	G4Cons(vname, pRmin1, pRmax1, pRmin2, pRmax2, pDz, pSPhi, pDPhi);
    }

  } else if ( shape == "SPHE" ) {
    G4double pRmin  = Rpar[0]*cm;
    G4double pRmax  = Rpar[1]*cm;
    G4double pThe1  = Rpar[2]*deg;
    G4double pThe2  = Rpar[3]*deg;
    G4double pDThe  = pThe2 - pThe1;
    G4double pPhi1  = Rpar[4]*deg;
    G4double pPhi2  = Rpar[5]*deg;
    G4double pDPhi  = pPhi2 - pPhi1;

    NegVolPars = pRmin<0 || pRmax<0;

    if (!(NegVolPars || Deferred)) {
      solid = new G4Sphere(vname, pRmin, pRmax, pThe1, pDThe, pPhi1, pDPhi);
    }

  } else if ( shape == "PARA" ) {
    G4double pDx = Rpar[0]*cm;
    G4double pDy = Rpar[1]*cm;
    G4double pDz = Rpar[2]*cm;
    G4double pAlph = Rpar[3]*deg;
    G4double pThet = Rpar[4]*deg;
    G4double pPhi  = Rpar[5]*deg;

    NegVolPars = pDx<0 || pDy<0 || pDz<0;

    if (!(NegVolPars || Deferred)){
      solid = new G4Para(vname, pDx, pDy, pDz, pAlph, pThet, pPhi);
    }

  } else if ( shape == "PGON" ) {
    G4int i;
    G4int npdv = int(Rpar[2]);
    G4int nz = int(Rpar[3]);
    G4double pPhi1 = Rpar[0]*deg;
    G4double dPhi  = Rpar[1]*deg;
    G4double *DzArray = new G4double[nz];
    G4double *Rmax    = new G4double[nz];
    G4double *Rmin    = new G4double[nz];
    G4double RMIN=1.e-4*cm;

    NegVolPars = 0;

    for(i=0; i<nz; i++) {
      int i4=3*i+4;
      int i5=i4+1;
      int i6=i4+2;
      DzArray[i] = Rpar[i4]*cm;
      Rmin[i] = Rpar[i5]*cm;
      Rmax[i] = Rpar[i6]*cm;
    }
    solid = new G4Polyhedra(vname, pPhi1, dPhi, npdv, nz, DzArray, Rmin, Rmax);
    delete [] DzArray;
    delete [] Rmin;
    delete [] Rmax;

  } else if ( shape == "PCON" ) {
    G4int i;
    G4double pPhi1 =  Rpar[0]*deg;
    G4double dPhi  = Rpar[1]*deg;    
    G4int nz = int(Rpar[2]);
    G4double *DzArray = new G4double[nz];
    G4double *Rmax    = new G4double[nz];
    G4double *Rmin    = new G4double[nz];

    NegVolPars = 0;

    for(i=0; i<nz; i++){
      int i4=3*i+3;
      int i5=i4+1;
      int i6=i4+2;
      DzArray[i] = Rpar[i4]*cm;
      Rmin[i] = Rpar[i5]*cm;
      Rmax[i] = Rpar[i6]*cm;
    }
    solid = new G4Polycone(vname, pPhi1, dPhi, nz, DzArray, Rmin, Rmax);
    delete [] DzArray;
    delete [] Rmin;
    delete [] Rmax;

  } else if ( shape == "ELTU" ) {
    // $$$ not implemented.
    G4cerr << "ELTU not supported" << endl;

  } else if ( shape == "HYPE" ) {
    G4double pRmin = Rpar[0]*cm;
    G4double pRmax = Rpar[1]*cm;
    G4double pDz   = Rpar[2]*cm;
    G4double pThet = Rpar[3]*deg;

    NegVolPars = pRmin<0 || pRmax<0 || pDz<0;

    if (!(NegVolPars || Deferred)){
      solid = new G4Hype(vname, pRmin, pRmax, pThet, pThet, pDz);
    } else {
      G4cerr << "Negative length parameters not supported for shape " 
	     << shape << endl;
    }

  } else if ( shape == "GTRA" ) {
    // $$$ not implemented.
    G4cerr << "GTRA not supported" << endl;

  } else if ( shape == "CTUB" ) {
    // $$$ not implemented.
    G4cerr << "CTUB not supported" << endl;
  }
  
  // get the material corresponding to the tracking medium
  G4Material* material=0;
  if (nmed>0) {
    material = G3Med.get(nmed);
  } else {
    G4cerr << "No material found for tracking medium index " << nmed << endl;
    assert(material != 0);
  }

  // external store is needed to handle deferred LV creation 
  G3Vol.PutVTE(vname, shape, Rpar, npar, nmed, material, solid, Deferred, 
	      NegVolPars);
}












































