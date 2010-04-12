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
// $Id: G4PreCompoundInuclCollider.hh,v 1.5 2010-04-12 23:39:41 mkelsey Exp $
// GEANT4 tag: $Name: not supported by cvs2svn $
//
// 20100315  M. Kelsey -- Remove "using" directive and unneeded #includes.
// 20100413  M. Kelsey -- Pass G4CollisionOutput by ref to ::collide()

#ifndef G4PRECOMPOUNDINUCL_COLLIDER_HH
#define G4PRECOMPOUNDINUCL_COLLIDER_HH
 
#include "G4IntraNucleiCascader.hh"
#include "G4NonEquilibriumEvaporator.hh"
#include "G4BigBanger.hh"
#include "G4ElementaryParticleCollider.hh"
#include "G4InteractionCase.hh"

class G4InuclParticle;
class G4InuclNuclei;

class G4PreCompoundInuclCollider {

public:

  G4PreCompoundInuclCollider();

  G4PreCompoundInuclCollider(G4ElementaryParticleCollider* ecollider,
		  G4IntraNucleiCascader* incascader, 
		  G4NonEquilibriumEvaporator* noeqevaporator,
		  G4BigBanger* bigbanger) {

    setElementaryParticleCollider(ecollider);
    setIntraNucleiCascader(incascader,ecollider);
    setNonEquilibriumEvaporator(noeqevaporator);
    setBigBanger(bigbanger);

  };

  void setElementaryParticleCollider(G4ElementaryParticleCollider* ecollider) {

    theElementaryParticleCollider = ecollider;   
  };

  void setIntraNucleiCascader(G4IntraNucleiCascader* incascader,
			      G4ElementaryParticleCollider* ecollider) {

    theIntraNucleiCascader = incascader;
    theIntraNucleiCascader->setElementaryParticleCollider(ecollider);
  };

  void setNonEquilibriumEvaporator(G4NonEquilibriumEvaporator* noeqevaporator) {

    theNonEquilibriumEvaporator = noeqevaporator;   
  };

  void setBigBanger(G4BigBanger* bigbanger) {

    theBigBanger = bigbanger;   
  };
  
  void collide(G4InuclParticle* bullet,
	       G4InuclParticle* target,
	       G4CollisionOutput& globalOutput);

private: 
  G4int verboseLevel;

  G4bool inelasticInteractionPossible(G4InuclParticle* bullet,
				      G4InuclParticle* target, 
				      G4double ekin) const;

  G4InteractionCase bulletTargetSetter(G4InuclParticle* bullet,
				       G4InuclParticle* target) const; 

  G4bool explosion(G4InuclNuclei* target) const; 
       
  G4ElementaryParticleCollider* theElementaryParticleCollider;
  G4IntraNucleiCascader* theIntraNucleiCascader;
  G4NonEquilibriumEvaporator* theNonEquilibriumEvaporator;
  G4BigBanger* theBigBanger;

};        

#endif // G4PRECOMPOUNDINUCL_COLLIDER_HH 


