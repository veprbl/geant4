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
// $Id: G4CascadeInterface.cc,v 1.76 2010-05-21 17:56:34 mkelsey Exp $
// Geant4 tag: $Name: not supported by cvs2svn $
//
// 20100114  M. Kelsey -- Remove G4CascadeMomentum, use G4LorentzVector directly
// 20100413  M. Kelsey -- Pass G4CollisionOutput by ref to ::collide()
// 20100414  M. Kelsey -- Check for K0L/K0S before using G4InuclElemPart::type
// 20100418  M. Kelsey -- Reference output particle lists via const-ref, use
//		const_iterator for both.
// 20100428  M. Kelsey -- Use G4InuclParticleNames enum
// 20100429  M. Kelsey -- Change "case gamma:" to "case photon:"
// 20100517  M. Kelsey -- Follow new ctors for G4*Collider family.

#include "G4CascadeInterface.hh"
#include "globals.hh"
#include "G4DynamicParticleVector.hh"
#include "G4IonTable.hh"
#include "G4InuclCollider.hh"
// #include "G4IntraNucleiCascader.hh"
#include "G4InuclElementaryParticle.hh"
#include "G4InuclNuclei.hh"
#include "G4InuclParticle.hh"
#include "G4CollisionOutput.hh"
#include "G4V3DNucleus.hh"
#include "G4Track.hh"
#include "G4Nucleus.hh"
#include "G4NucleiModel.hh"
#include "G4LorentzRotation.hh"
#include "G4InuclParticleNames.hh"

using namespace G4InuclParticleNames;

//#define BERTDEV 1  // A flag to activate a development version of Bertini cascade

typedef std::vector<G4InuclElementaryParticle>::const_iterator particleIterator;
typedef std::vector<G4InuclNuclei>::const_iterator nucleiIterator;

G4CascadeInterface::G4CascadeInterface(const G4String& nam)
  :G4VIntraNuclearTransportModel(nam), verboseLevel(0)  
{
  if (verboseLevel > 3) {
    G4cout << " >>> G4CascadeInterface::G4CascadeInterface" << G4endl;
  }

}


G4CascadeInterface::~G4CascadeInterface()
{}
   
G4ReactionProductVector* G4CascadeInterface::Propagate(G4KineticTrackVector* , 
						       G4V3DNucleus* ) {
  return 0;
}

// #define debug_G4CascadeInterface

G4HadFinalState* G4CascadeInterface::ApplyYourself(const G4HadProjectile& aTrack, 
						     G4Nucleus& theNucleus) {
#ifdef debug_G4CascadeInterface
  static G4int counter(0);
  counter++;
  G4cerr << "Reaction number "<< counter << " "<<aTrack.GetDynamicParticle()->GetDefinition()->GetParticleName()<<" "<< aTrack.GetDynamicParticle()->GetKineticEnergy()<<G4endl;
#endif

  theResult.Clear();

  if (verboseLevel > 3) {
    G4cout << " >>> G4CascadeInterface::ApplyYourself" << G4endl;
  };

  G4double eInit     = 0.0;
  G4double eTot      = 0.0;
  G4double sumBaryon = 0.0;
  G4double sumEnergy = 0.0;

  // Make conversion between native Geant4 and Bertini cascade classes.
  // NOTE: Geant4 units are MeV = 1 and GeV = 1000. Cascade code by default use GeV = 1.

  G4int bulletType;
  if (aTrack.GetDefinition() == G4KaonZeroLong::KaonZeroLong() ||
      aTrack.GetDefinition() == G4KaonZeroShort::KaonZeroShort() )
    bulletType = (G4UniformRand() > 0.5) ? kaonZero : kaonZeroBar;
  else 
    bulletType = G4InuclElementaryParticle::type(aTrack.GetDefinition());

  // Code momentum and energy.
  G4LorentzVector projectileMomentum = aTrack.Get4Momentum();
  G4LorentzRotation toZ;
  toZ.rotateZ(-projectileMomentum.phi());
  toZ.rotateY(-projectileMomentum.theta());
  G4LorentzRotation toLabFrame = toZ.inverse();

  G4LorentzVector momentumBullet(0., 0., aTrack.GetTotalMomentum()/GeV,
				 aTrack.GetTotalEnergy()/GeV);

  G4InuclElementaryParticle* bullet =
    new G4InuclElementaryParticle(momentumBullet, bulletType); 

  sumEnergy = bullet->getKineticEnergy(); // In GeV 

  sumBaryon += bullet->baryon();	// Returns baryon number (0, 1, or 2)

  // Set target
  G4InuclNuclei*   target  = 0;
  G4InuclParticle* targetH = 0;
  // and outcoming particles
  G4DynamicParticle* cascadeParticle = 0;

  G4double theNucleusA = theNucleus.GetN();

  if ( !(G4int(theNucleusA) == 1) ) {
    target  = new G4InuclNuclei(theNucleusA, 
				theNucleus.GetZ());
    eInit = bullet->getEnergy() + target->getEnergy();

    sumBaryon += theNucleusA;

    if (verboseLevel > 2) {
      G4cout << "Bullet:  " << G4endl;  
      bullet->printParticle();
    }
    if (verboseLevel > 2) {
      G4cout << "Target:  " << G4endl;  
      target->printParticle();
    }
  }

  G4CollisionOutput output;

  // Colliders initialisation
  inc.setInteractionCase(1); 	// Interaction type is particle with nuclei.
  G4InuclCollider* collider = new G4InuclCollider;

      G4int  maxTries = 100; // maximum tries for inelastic collision to avoid infinite loop
      G4int  nTries   = 0;  // try counter

#ifdef BERTDEV
      G4int coulombOK =0;  // flag for correct Coulomb barrier
#endif
      if (G4int(theNucleusA) == 1) { // special treatment for target H(1,1) (proton)

	targetH = new G4InuclElementaryParticle(proton);

	G4float cutElastic[32];

	cutElastic[proton   ] = 1.0; // GeV
	cutElastic[neutron  ] = 1.0;
	cutElastic[pionPlus ] = 0.6;
	cutElastic[pionMinus] = 0.2;
	cutElastic[pionZero ] = 0.2;
        cutElastic[kaonPlus ] = 0.5;
        cutElastic[kaonMinus] = 0.5;
        cutElastic[kaonZero] = 0.5;
        cutElastic[kaonZeroBar] = 0.5;
        cutElastic[lambda] = 1.0;
        cutElastic[sigmaPlus] = 1.0;
        cutElastic[sigmaZero] = 1.0;
        cutElastic[sigmaMinus] = 1.0;
        cutElastic[xiZero] = 1.0;
        cutElastic[xiMinus] = 1.0;

	if (momentumBullet.z() > cutElastic[bulletType]) { // inelastic collision possible

	  do {   // we try to create inelastic interaction
	    output.reset();
	    collider->collide(bullet, targetH, output);
	    nTries++;
	  } while(
		  (nTries < maxTries)                                           &&
		  (output.getOutgoingParticles().size() == 2                    && // elastic: bullet + p = H(1,1) coming out
		   (output.getOutgoingParticles().begin()->type() == bulletType ||
		    output.getOutgoingParticles().begin()->type() == proton)
		   )
		  );
	} else { // only elastic collision is energetically possible
	  collider->collide(bullet, targetH, output);
	}

	sumBaryon += 1;

	eInit = bullet->getEnergy() + targetH->getEnergy();

	if (verboseLevel > 2) {
	  G4cout << "Target:  " << G4endl;
	  targetH->printParticle();
	}

      } else {  // treat all other targets excepet H(1,1)

	do  // we try to create inelastic interaction
	  {
#ifdef BERTDEV
            coulombOK=0;  // by default coulomb analysis is OK
#endif
	    output.reset();
	    collider->collide(bullet, target, output);
	    nTries++;

#ifdef BERTDEV
	    G4double coulumbBarrier = 8.7 * MeV; 
	    const std::vector<G4InuclElementaryParticle>& p= output.getOutgoingParticles();
	    if(!p.empty()) { 
	      for(    particleIterator ipart = p.begin(); ipart != p.end(); ipart++) {
		if (ipart->type() == proton) {
		  G4double e = ipart->getKineticEnergy()*GeV;
		  if (e < coulumbBarrier) coulombOK= 1; // If event with coulomb barrier violation detected -> retry
		  //		  G4cout << "///AH "<< e << "" << coulumbBarrier <<G4endl;
		}
	      }
	    }
	  } while( 
		  (nTries < maxTries) &&  // conditions for next try
		  (coulombOK==1) &&
		  ((output.getOutgoingParticles().size() + output.getNucleiFragments().size()) > 2.5) &&  
		  (output.getOutgoingParticles().size()!=0)                                      
		  );
#else

	  } while(
		   (nTries < maxTries)                                                               &&
		   (output.getOutgoingParticles().size() + output.getNucleiFragments().size() < 2.5) &&
		   (output.getOutgoingParticles().size()!=0) &&
                   (output.getOutgoingParticles().begin()->type()==bullet->type())
		   );
#endif
     }

  if (verboseLevel > 1) 
    {
      G4cout << " Cascade output: " << G4endl;
      output.printCollisionOutput();
    }
  
  // Convert cascade data to use hadronics interface
  const std::vector<G4InuclNuclei>& nucleiFragments = output.getNucleiFragments();
  const std::vector<G4InuclElementaryParticle>& particles = output.getOutgoingParticles();

  theResult.SetStatusChange(stopAndKill);

  if (!particles.empty()) { 
    particleIterator ipart;
    G4int outgoingParticle;

    for (ipart = particles.begin(); ipart != particles.end(); ipart++) {
      outgoingParticle = ipart->type();

      eTot += ipart->getEnergy();

      G4double ekin = ipart->getKineticEnergy() * GeV;
      G4ThreeVector aMom = ipart->getMomentum().vect().unit();

      if (ipart->baryon() ) {
	sumBaryon -= 1;
      }

      sumEnergy -= ekin / GeV;

      switch(outgoingParticle) {

      case proton: 
#ifdef debug_G4CascadeInterface
	G4cerr << "proton " << counter << " " << aMom << " " << ekin << G4endl;
#endif
	cascadeParticle = 
	  new G4DynamicParticle(G4Proton::ProtonDefinition(), aMom, ekin);
	break; 

      case neutron: 

#ifdef debug_G4CascadeInterface
	G4cerr << "neutron "<< counter<<" "<<aMom<<" "<<  ekin<<G4endl;
#endif
	cascadeParticle = 
	  new G4DynamicParticle(G4Neutron::NeutronDefinition(), aMom, ekin);
	break;

      case pionPlus: 
	cascadeParticle = 
	  new G4DynamicParticle(G4PionPlus::PionPlusDefinition(), aMom, ekin);

#ifdef debug_G4CascadeInterface
	G4cerr << "pionPlus "<< counter<<" "<<aMom<<" "<<  ekin<<G4endl;
#endif
	break;

      case pionMinus:
	cascadeParticle = 
	  new G4DynamicParticle(G4PionMinus::PionMinusDefinition(), aMom, ekin);

#ifdef debug_G4CascadeInterface
	G4cerr << "pionMinus "<< counter<<" "<<aMom<<" "<<  ekin<<G4endl;
#endif
	break;

      case pionZero: 
	cascadeParticle = 
	  new G4DynamicParticle(G4PionZero::PionZeroDefinition(), aMom, ekin);

#ifdef debug_G4CascadeInterface
	G4cerr << "pionZero "<< counter<<" "<<aMom<<" "<<  ekin<<G4endl;
#endif
	break;

      case photon: 
	cascadeParticle = 
	  new G4DynamicParticle(G4Gamma::Gamma(), aMom, ekin);

#ifdef debug_G4CascadeInterface
	G4cerr << "photon "<< counter<<" "<<aMom<<" "<<  ekin<<G4endl;
#endif
	break;


      case kaonPlus:
        cascadeParticle =
          new G4DynamicParticle(G4KaonPlus::KaonPlusDefinition(), aMom, ekin);
        break;

      case kaonMinus:
        cascadeParticle =
         new G4DynamicParticle(G4KaonMinus::KaonMinusDefinition(), aMom, ekin);
        break;

      case kaonZero:
      case kaonZeroBar:
        if (G4UniformRand() > 0.5) {
          cascadeParticle = new G4DynamicParticle(
                              G4KaonZeroLong::KaonZeroLongDefinition(),
                              aMom, ekin);
        } else {
          cascadeParticle = new G4DynamicParticle(
                              G4KaonZeroShort::KaonZeroShortDefinition(),
                              aMom, ekin);
        }
        break;

      case lambda:
        cascadeParticle =
         new G4DynamicParticle(G4Lambda::LambdaDefinition(), aMom, ekin);
        break;

      case sigmaPlus:
        cascadeParticle =
         new G4DynamicParticle(G4SigmaPlus::SigmaPlusDefinition(), aMom, ekin);
        break;

      case sigmaZero:
        cascadeParticle =
         new G4DynamicParticle(G4SigmaZero::SigmaZeroDefinition(), aMom, ekin);
        break;

      case sigmaMinus:
        cascadeParticle =
         new G4DynamicParticle(G4SigmaMinus::SigmaMinusDefinition(), aMom, ekin);
        break;

      case xiZero:
        cascadeParticle =
         new G4DynamicParticle(G4XiZero::XiZeroDefinition(), aMom, ekin);
        break;

      case xiMinus:
        cascadeParticle =
         new G4DynamicParticle(G4XiMinus::XiMinusDefinition(), aMom, ekin);
        break;

      default:
        G4cout << " ERROR: G4CascadeInterface::Propagate undefined particle type"
	       << G4endl;
      }

      cascadeParticle->Set4Momentum(cascadeParticle->Get4Momentum()*=toLabFrame);
      theResult.AddSecondary(cascadeParticle); 
    }
  }

  // get nuclei fragments
  G4DynamicParticle * aFragment = 0;
  G4ParticleDefinition * aIonDef = 0;
  G4ParticleTable *theTableOfParticles = G4ParticleTable::GetParticleTable();

  if (!nucleiFragments.empty()) { 
    nucleiIterator ifrag;

    for (ifrag = nucleiFragments.begin(); ifrag != nucleiFragments.end(); ifrag++) 
      {
	G4double eKin = ifrag->getKineticEnergy() * GeV;
        eTot += ifrag->getEnergy();
	G4ThreeVector aMom = ifrag->getMomentum().vect().unit();

	// hpw @@@ ==> Should be zero: G4double fragmentExitation = ifrag->getExitationEnergyInGeV();

	if (verboseLevel > 2) {
	  G4cout << " Nuclei fragment: " << G4endl;
	  ifrag->printParticle();
	}

	G4int A = G4int(ifrag->getA());
	G4int Z = G4int(ifrag->getZ());
	aIonDef = theTableOfParticles->FindIon(Z, A, 0, Z);
      
	aFragment =  new G4DynamicParticle(aIonDef, aMom, eKin);

	sumBaryon -= A;
	sumEnergy -= eKin / GeV;

        aFragment->Set4Momentum(aFragment->Get4Momentum()*=toLabFrame);
	theResult.AddSecondary(aFragment); 
      }
  }

  if (verboseLevel > 2) {
    if (sumBaryon != 0) {
      G4cout << "ERROR: no baryon number conservation, sum of baryons = "
             << sumBaryon << G4endl;
    }

    if (sumEnergy > 0.01 ) {
      G4cout << "Kinetic energy conservation violated by "
	     << sumEnergy << " GeV" << G4endl;
    }
     
    G4cout << "Total energy conservation at level ~"
	   << (eInit - eTot) * GeV << " MeV" << G4endl;
    
    if (sumEnergy < -5.0e-5 ) { // 0.05 MeV
      G4cout << "FATAL ERROR: energy created  "
             << sumEnergy * GeV << " MeV" << G4endl;
    }
  }

  delete bullet;
//  delete inc;
  delete collider;

  if(target != 0) delete target;
  if(targetH != 0) delete targetH;
 // if(cascadeParticle != 0) delete cascadeParticle;
 // if(aFragment != 0) delete aFragment;

  return &theResult;
}
