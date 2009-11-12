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
// $Id: G4NeutronInelasticXS.cc,v 1.1 2009-11-12 00:36:01 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:    G4NeutronInelasticXS
//
// Author  Ivantchenko, Geant4, 3-Aug-09
//
// Modifications:
//

#include "G4NeutronInelasticXS.hh"
#include "G4Neutron.hh"
#include "G4DynamicParticle.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4PhysicsLogVector.hh"
#include "G4PhysicsVector.hh"
#include "G4GlauberGribovCrossSection.hh"
#include "G4NistManager.hh"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

G4NeutronInelasticXS::G4NeutronInelasticXS() 
{
  verboseLevel = 0;
  G4cout  << "G4NeutronInelasticXS::G4NeutronInelasticXS: Initialise " << G4endl;
  for(G4int i=0; i<93; ++i) {
    data[i] = 0;
    coeff[i]= 1.0;
  }
  ggXsection = new G4GlauberGribovCrossSection();
  isInitialized = false;
}

G4NeutronInelasticXS::~G4NeutronInelasticXS()
{
  for(G4int i=0; i<92; ++i) {
    delete data[i];
  }
}

G4bool 
G4NeutronInelasticXS::IsApplicable(const G4DynamicParticle*, 
				   const G4Element*)
{
  return true;
}

G4bool 
G4NeutronInelasticXS::IsZAApplicable(const G4DynamicParticle*,
				     G4double /*ZZ*/, G4double /*AA*/)
{
  return false;
}


G4double 
G4NeutronInelasticXS::GetCrossSection(const G4DynamicParticle* aParticle,
				      const G4Element* elm,
				      G4double)
{
  G4double xs = 0.0;
  G4double ekin = aParticle->GetKineticEnergy();

  G4int Z = G4int(elm->GetZ());
  G4PhysicsVector* pv = data[Z];
  //  G4cout  << "G4NeutronInelasticXS::GetCrossSection e= " << ekin << " Z= " << Z << G4endl;

  // element was not initialised
  if(!pv) {
    Initialise(Z);
    pv = data[Z];
    if(!pv) return xs;
  }

  G4double e1 = pv->Energy(0);
  if(ekin <= e1) return xs;

  G4int n = pv->GetVectorLength() - 1;
  G4double e2 = pv->Energy(n);
  if(ekin <= e2) { 
    xs = pv->Value(ekin); 
  } else {          
    ggXsection->GetCrossSection(aParticle, elm);
    xs = coeff[Z]*ggXsection->GetInelasticGlauberGribovXsc();
  }

  if(verboseLevel > 0){
    G4cout  << "ekin= " << ekin << ",  XSinel= " << xs << G4endl;
  }
  return xs;
}


void 
G4NeutronInelasticXS::BuildPhysicsTable(const G4ParticleDefinition& p)
{
  G4cout << "G4NeutronInelasticXS::BuildPhysicsTable: " << G4endl;
  G4cout << p.GetParticleName() << G4endl;
  if(p.GetParticleName() != "neutron") {
    return;
  }
  if(isInitialized) return;
  isInitialized = true;


  // check environment variable 
  // Build the complete string identifying the file with the data set
  char* path = getenv("G4NEUTRONXS");
  if (!path){
    G4cout << "G4NEUTRONXS environment variable not set" << G4endl;
  }

  G4DynamicParticle* dynParticle = 
    new G4DynamicParticle(G4Neutron::Neutron(),G4ThreeVector(1,0,0),1);

  // Access to elements
  const G4ElementTable* theElmTable = G4Element::GetElementTable();
  size_t numOfElm = G4Element::GetNumberOfElements();
  if(numOfElm > 0) {
    for(size_t i=0; i<numOfElm; ++i) {
      G4int Z = G4int(((*theElmTable)[i])->GetZ());
      if(Z < 1) Z = 1;
      else if(Z > 92) Z = 92;
      //G4cout << "Z= " << Z << G4endl;
      // Initialisation 
      if(!data[Z]) { Initialise(Z, dynParticle, path); }
    }
  }
  delete dynParticle;
}

void 
G4NeutronInelasticXS::DumpPhysicsTable(const G4ParticleDefinition&)
{
}

void 
G4NeutronInelasticXS::Initialise(G4int Z, G4DynamicParticle* dp, 
				 const char* p)
{
  if(data[Z]) return;
  const char* path = p;
  if(!p) {
  // check environment variable 
  // Build the complete string identifying the file with the data set
    path = getenv("G4NEUTRONXS");
    if (!path) {
      if(verboseLevel > 1) {
	G4cout << "G4NEUTRONXS environment variable not set" << G4endl;
      }
      return;
    }
  }
  G4DynamicParticle* dynParticle = dp;
  if(!dp) {
    dynParticle = 
      new G4DynamicParticle(G4Neutron::Neutron(),G4ThreeVector(1,0,0),1);
  }

  const G4Element* Elem = 
    G4NistManager::Instance()->FindOrBuildElement(Z, true);

  // upload data from file
  data[Z] = new G4PhysicsLogVector();

  std::ostringstream ost;
  ost << path << "/inelast" << Z ;
  std::ifstream filein(ost.str().c_str());
  if (!(filein)) {
    G4cout << " file " << ost << "  is not opened" << G4endl;
  }else{
    if(verboseLevel > 1) {
      G4cout << ost << " is opened" << G4endl;
    }
    // retrieve data from DB
    data[Z]->Retrieve(filein, true);

    // smooth transition 
    size_t n      = data[Z]->GetVectorLength() - 1;
    G4double emax = data[Z]->Energy(n);
    G4double sig1 = (*data[Z])[n];
    dynParticle->SetKineticEnergy(emax);
    ggXsection->GetCrossSection(dynParticle, Elem);
    G4double sig2 = ggXsection->GetInelasticGlauberGribovXsc();
    if(sig2 > 0.) coeff[Z] = sig1/sig2; 
  } 
  if(!dp) delete dynParticle;
}
