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
// $Id: RE02PSPassageCellFlux.cc,v 1.2 2006-12-13 15:43:17 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// RE02PSPassageCellFlux
#include "RE02PSPassageCellFlux.hh"

///////////////////////////////////////////////////////////////////////////////
//
//  The developement and research on this code is supported by Core Research
// for Evolutional Science and Technology (CREST) Program of Japan Science
// and Technology Corporation (JST). You may acknowlege to JST and authers
// whenever you publish your results using this code, as well as the Geant4
// collaboration. Both JST and authers remain to have a right to refuse to use
// this code for any case.
//   Tsukasa Aso    ( aso@toyama-cmt.ac.jp   )
//   Akinori Kimura ( AKimura@ashitech.ac.jp )
//
///////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring cell charge.
//   The Cell Charge is defined by  a sum of charge inside the cell
//  which calculates the deposited charge in the cell.
//   
//    
//
//
//
// Created: 2006-06-20  Tsukasa ASO, Akinori Kimura.
// 
///////////////////////////////////////////////////////////////////////////////

RE02PSPassageCellFlux::RE02PSPassageCellFlux(G4String name,G4int nx, G4int ny, G4int nz)
  :G4PSPassageCellFlux(name),fNx(nx),fNy(ny),fNz(nz)
{;}

RE02PSPassageCellFlux::~RE02PSPassageCellFlux()
{;}

G4int RE02PSPassageCellFlux::GetIndex(G4Step* aStep)
{
  const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
  G4int ix = touchable->GetReplicaNumber(1);
  G4int iy = touchable->GetReplicaNumber(2);
  G4int iz = touchable->GetReplicaNumber(0);
  
  return iy*fNx*fNy+ix*fNz+iz;
}
