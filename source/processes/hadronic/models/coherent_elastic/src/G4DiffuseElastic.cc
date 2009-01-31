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
// $Id: G4DiffuseElastic.cc,v 1.21 2009-01-31 15:30:32 grichine Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// Physics model class G4DiffuseElastic 
//
//
// G4 Model: optical diffuse elastic scattering with 4-momentum balance
//                         
// 24-May-07 V. Grichine
//

#include "G4DiffuseElastic.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4IonTable.hh"

#include "Randomize.hh"
#include "G4Integrator.hh"
#include "globals.hh"

#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4Deuteron.hh"
#include "G4Alpha.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"

#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4PhysicsTable.hh"
#include "G4PhysicsLogVector.hh"
#include "G4PhysicsFreeVector.hh"

/////////////////////////////////////////////////////////////////////////
//
// Test Constructor. Just to check xsc


G4DiffuseElastic::G4DiffuseElastic() 
  : G4HadronicInteraction(), fParticle(0)
{
  SetMinEnergy( 0.01*GeV );
  SetMaxEnergy( 100.*TeV );
  verboseLevel = 0;
  lowEnergyRecoilLimit = 100.*keV;  
  lowEnergyLimitQ  = 0.0*GeV;  
  lowEnergyLimitHE = 0.0*GeV;  
  lowestEnergyLimit= 0.0*keV;  
  plabLowLimit     = 20.0*MeV;

  theProton   = G4Proton::Proton();
  theNeutron  = G4Neutron::Neutron();
  theDeuteron = G4Deuteron::Deuteron();
  theAlpha    = G4Alpha::Alpha();
  thePionPlus = G4PionPlus::PionPlus();
  thePionMinus= G4PionMinus::PionMinus();

  fEnergyBin = 200;
  fAngleBin = 100;

  fEnergyVector = 0;
  fAngleTable = 0;

  fParticle = 0;
  fWaveVector = 0.;
  fAtomicWeight = 0.;
  fAtomicNumber = 0.;
  fNuclearRadius = 0.;
  fBeta = 0.;
  fZommerfeld = 0.;
  fAm = 0.;
  fAddCoulomb = false;
}

//////////////////////////////////////////////////////////////////////////
//
// Constructor with initialisation

G4DiffuseElastic::G4DiffuseElastic(const G4ParticleDefinition* aParticle) 
  : G4HadronicInteraction(), fParticle(aParticle)
{
  SetMinEnergy( 0.01*GeV );
  SetMaxEnergy( 100.*TeV );
  verboseLevel = 0;
  lowEnergyRecoilLimit = 100.*keV;  
  lowEnergyLimitQ  = 0.0*GeV;  
  lowEnergyLimitHE = 0.0*GeV;  
  lowestEnergyLimit= 0.0*keV;  
  plabLowLimit     = 20.0*MeV;

  theProton   = G4Proton::Proton();
  theNeutron  = G4Neutron::Neutron();
  theDeuteron = G4Deuteron::Deuteron();
  theAlpha    = G4Alpha::Alpha();
  thePionPlus = G4PionPlus::PionPlus();
  thePionMinus= G4PionMinus::PionMinus();

  fEnergyBin = 200; // 200; // 100;
  fAngleBin = 200; // 200; // 100;

  // fEnergyVector = 0;
  fEnergyVector = new G4PhysicsLogVector( theMinEnergy, theMaxEnergy, fEnergyBin );
  fAngleTable = 0;

  fParticle = aParticle;
  fWaveVector = 0.;
  fAtomicWeight = 0.;
  fAtomicNumber = 0.;
  fNuclearRadius = 0.;
  fBeta = 0.;
  fZommerfeld = 0.;
  fAm = 0.;
  fAddCoulomb = false;
  // Initialise();
}

//////////////////////////////////////////////////////////////////////////////
//
// Destructor

G4DiffuseElastic::~G4DiffuseElastic()
{
  if(fEnergyVector) delete fEnergyVector;

  if( fAngleTable )
  {
      fAngleTable->clearAndDestroy();
      delete fAngleTable ;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
// Initialisation for given particle using element table of application

void G4DiffuseElastic::Initialise() 
{

  // fEnergyVector = new G4PhysicsLogVector( theMinEnergy, theMaxEnergy, fEnergyBin );

  const G4ElementTable* theElementTable = G4Element::GetElementTable();

  size_t jEl, numOfEl = G4Element::GetNumberOfElements();

  for(jEl = 0 ; jEl < numOfEl; ++jEl) // application element loop
  {
    fAtomicNumber = (*theElementTable)[jEl]->GetZ();     // atomic number
    fAtomicWeight = (*theElementTable)[jEl]->GetN();     // number of nucleons
    fNuclearRadius = CalculateNuclearRad(fAtomicWeight);

    if(verboseLevel > 0) 
    {   
      G4cout<<"G4DiffuseElastic::Initialise() the element: "
	    <<(*theElementTable)[jEl]->GetName()<<G4endl;
    }
    fElementNumberVector.push_back(fAtomicNumber);
    fElementNameVector.push_back((*theElementTable)[jEl]->GetName());

    BuildAngleTable();
    fAngleBank.push_back(fAngleTable);
  }  
  return;
}

//////////////////////////////////////////////////////////////////////////////
//
// Initialisation for given particle on fly using new element number

void G4DiffuseElastic::InitialiseOnFly(G4double Z, G4double A) 
{
  fAtomicNumber  = Z;     // atomic number
  fAtomicWeight  = A;     // number of nucleons
  fNuclearRadius = CalculateNuclearRad(fAtomicWeight);
  
  if(verboseLevel > 0)    
  {
    G4cout<<"G4DiffuseElastic::Initialise() the element with Z = "
	  <<Z<<"; and A = "<<A<<G4endl;
  }
  fElementNumberVector.push_back(fAtomicNumber);

  BuildAngleTable();
  fAngleBank.push_back(fAngleTable);
  return;
}

///////////////////////////////////////////////////////////////////////////////
//
// Build for given particle and element table of momentum, angle probability.
// For the moment in lab system. 

void G4DiffuseElastic::BuildAngleTable() 
{
  G4int i, j;
  G4double partMom, kinE, a = 0., z = fParticle->GetPDGCharge(), m1 = fParticle->GetPDGMass();
  G4double theta1, theta2, thetaMax, thetaCoulomb, sum = 0.;

  G4Integrator<G4DiffuseElastic,G4double(G4DiffuseElastic::*)(G4double)> integral;
  
  fAngleTable = new G4PhysicsTable(fEnergyBin);

  for( i = 0; i < fEnergyBin; i++)
  {
    kinE        = fEnergyVector->GetLowEdgeEnergy(i);
    partMom     = std::sqrt( kinE*(kinE + 2*m1) );
    fWaveVector = partMom/hbarc;

    G4double kRmax  = 18.; // 10.174; ~ 3 maxima of J1 or 15., 25.
    G4double kRcoul = 1.1; // 2.5; // on the first slope of J1

    thetaMax    = kRmax/(fWaveVector*fNuclearRadius);

    if (thetaMax > pi) thetaMax = pi;

    thetaCoulomb = (kRcoul/kRmax)*thetaMax;

    if(z)
    {
      a           = partMom/m1; // beta*gamma for m1
      fBeta       = a/std::sqrt(1+a*a);
      fZommerfeld = CalculateZommerfeld( fBeta, z, fAtomicNumber);
      fAm         = CalculateAm( partMom, fZommerfeld, fAtomicNumber);
    }
    G4PhysicsFreeVector* angleVector = new G4PhysicsFreeVector(fAngleBin);

    G4PhysicsLogVector*  angleBins = new G4PhysicsLogVector( 0.001*thetaMax, thetaMax, fAngleBin );
    
    sum = 0.;

    for(j = 1; j < fAngleBin; j++)
    {
      theta1 = angleBins->GetLowEdgeEnergy(j-1);
      theta2 = angleBins->GetLowEdgeEnergy(j);

      // theta1 = thetaMax*(j-1)/fAngleBin;
      // theta2 = thetaMax*(j)/fAngleBin;

      if( theta2 > thetaCoulomb && z) fAddCoulomb = true;

      sum   += integral.Legendre10(this,&G4DiffuseElastic::GetIntegrandFunction, theta1,theta2);
      // sum   += integral.Legendre96(this,&G4DiffuseElastic::GetIntegrandFunction, theta1,theta2);
      
      angleVector->PutValue( j-1 , theta2, sum );
      // G4cout<<"j-1 = "<<j-1<<"; theta2 = "<<theta2/degree<<"; sum = "<<sum<<G4endl;
    }
    fAddCoulomb = false;
    fAngleTable->insertAt(i,angleVector);

    // delete[] angleVector; 
    // delete[] angleBins; 
  }
  return;
}

////////////////////////////////////////////////////////////////////////////////
//
// Model analog of DoIt function

G4HadFinalState* 
G4DiffuseElastic::ApplyYourself( const G4HadProjectile& aTrack, 
                                       G4Nucleus& targetNucleus )
{
  theParticleChange.Clear();

  const G4HadProjectile* aParticle = &aTrack;

  G4double ekin = aParticle->GetKineticEnergy();

  if(ekin <= lowestEnergyLimit) 
  {
    theParticleChange.SetEnergyChange(ekin);
    theParticleChange.SetMomentumChange(aTrack.Get4Momentum().vect().unit());
    return &theParticleChange;
  }

  G4double aTarget = targetNucleus.GetN();
  G4double zTarget = targetNucleus.GetZ();

  G4double plab = aParticle->GetTotalMomentum();

  if (verboseLevel >1)
  { 
    G4cout << "G4DiffuseElastic::DoIt: Incident particle plab=" 
	   << plab/GeV << " GeV/c " 
	   << " ekin(MeV) = " << ekin/MeV << "  " 
	   << aParticle->GetDefinition()->GetParticleName() << G4endl;
  }
  // Scattered particle referred to axis of incident particle

  const G4ParticleDefinition* theParticle = aParticle->GetDefinition();
  G4double m1 = theParticle->GetPDGMass();

  G4int Z = static_cast<G4int>(zTarget+0.5);
  G4int A = static_cast<G4int>(aTarget+0.5);
  G4int N = A - Z;

  G4int projPDG = theParticle->GetPDGEncoding();

  if (verboseLevel>1) 
  {
    G4cout << "G4DiffuseElastic for " << theParticle->GetParticleName()
	   << " PDGcode= " << projPDG << " on nucleus Z= " << Z 
	   << " A= " << A << " N= " << N 
	   << G4endl;
  }
  G4ParticleDefinition * theDef = 0;

  if(Z == 1 && A == 1)       theDef = theProton;
  else if (Z == 1 && A == 2) theDef = theDeuteron;
  else if (Z == 1 && A == 3) theDef = G4Triton::Triton();
  else if (Z == 2 && A == 3) theDef = G4He3::He3();
  else if (Z == 2 && A == 4) theDef = theAlpha;
  else theDef = G4ParticleTable::GetParticleTable()->FindIon(Z,A,0,Z);
 
  G4double m2 = theDef->GetPDGMass();
  G4LorentzVector lv1 = aParticle->Get4Momentum();
  G4LorentzVector lv(0.0,0.0,0.0,m2);   
  lv += lv1;

  G4ThreeVector bst = lv.boostVector();
  lv1.boost(-bst);

  G4ThreeVector p1 = lv1.vect();
  G4double ptot = p1.mag();
  G4double tmax = 4.0*ptot*ptot;
  G4double t = 0.0;


  //
  // Sample t
  //
  
  // t = SampleT( theParticle, ptot, A);

  t = SampleTableT( theParticle, ptot, Z, A); // use initialised table

  // NaN finder
  if(!(t < 0.0 || t >= 0.0)) 
  {
    if (verboseLevel > 0) 
    {
      G4cout << "G4DiffuseElastic:WARNING: Z= " << Z << " N= " 
	     << N << " pdg= " <<  projPDG
	     << " mom(GeV)= " << plab/GeV 
              << " S-wave will be sampled" 
	     << G4endl; 
    }
    t = G4UniformRand()*tmax; 
  }
  if(verboseLevel>1)
  {
    G4cout <<" t= " << t << " tmax= " << tmax 
	   << " ptot= " << ptot << G4endl;
  }
  // Sampling of angles in CM system

  G4double phi  = G4UniformRand()*twopi;
  G4double cost = 1. - 2.0*t/tmax;
  G4double sint;

  if( cost >= 1.0 ) 
  {
    cost = 1.0;
    sint = 0.0;
  }
  else if( cost <= -1.0) 
  {
    cost = -1.0;
    sint =  0.0;
  }
  else  
  {
    sint = std::sqrt((1.0-cost)*(1.0+cost));
  }    
  if (verboseLevel>1) 
    G4cout << "cos(t)=" << cost << " std::sin(t)=" << sint << G4endl;

  G4ThreeVector v1(sint*std::cos(phi),sint*std::sin(phi),cost);
  v1 *= ptot;
  G4LorentzVector nlv1(v1.x(),v1.y(),v1.z(),std::sqrt(ptot*ptot + m1*m1));

  nlv1.boost(bst); 

  G4double eFinal = nlv1.e() - m1;

  if (verboseLevel > 1)
  { 
    G4cout << "Scattered: "
	   << nlv1<<" m= " << m1 << " ekin(MeV)= " << eFinal 
	   << " Proj: 4-mom " << lv1 
	   <<G4endl;
  }
  if(eFinal < 0.0) 
  {
    G4cout << "G4DiffuseElastic WARNING ekin= " << eFinal
	   << " after scattering of " 
	   << aParticle->GetDefinition()->GetParticleName()
	   << " p(GeV/c)= " << plab
	   << " on " << theDef->GetParticleName()
	   << G4endl;
    eFinal = 0.0;
    nlv1.setE(m1);
  }

  theParticleChange.SetMomentumChange(nlv1.vect().unit());
  theParticleChange.SetEnergyChange(eFinal);
  
  G4LorentzVector nlv0 = lv - nlv1;
  G4double erec =  nlv0.e() - m2;

  if (verboseLevel > 1) 
  {
    G4cout << "Recoil: "
	   << nlv0<<" m= " << m2 << " ekin(MeV)= " << erec 
	   <<G4endl;
  }
  if(erec > lowEnergyRecoilLimit) 
  {
    G4DynamicParticle * aSec = new G4DynamicParticle(theDef, nlv0);
    theParticleChange.AddSecondary(aSec);
  } else {
    if(erec < 0.0) erec = 0.0;
    theParticleChange.SetLocalEnergyDeposit(erec);
  }

  return &theParticleChange;
}


////////////////////////////////////////////////////////////////////////////
//
// return differential elastic cross section d(sigma)/d(omega) 

G4double 
G4DiffuseElastic::GetDiffuseElasticXsc( const G4ParticleDefinition* particle, 
                                        G4double theta, 
			                G4double momentum, 
                                        G4double A         )
{
  fParticle      = particle;
  fWaveVector    = momentum/hbarc;
  fAtomicWeight  = A;
  fAddCoulomb    = false;
  fNuclearRadius = CalculateNuclearRad(A);

  G4double sigma = fNuclearRadius*fNuclearRadius*GetDiffElasticProb(theta);

  return sigma;
}

////////////////////////////////////////////////////////////////////////////
//
// return invariant differential elastic cross section d(sigma)/d(tMand) 

G4double 
G4DiffuseElastic::GetInvElasticXsc( const G4ParticleDefinition* particle, 
                                        G4double tMand, 
			                G4double plab, 
                                        G4double A, G4double Z         )
{
  G4double m1 = particle->GetPDGMass();
  G4LorentzVector lv1(0.,0.,plab,std::sqrt(plab*plab+m1*m1));

  G4int iZ = static_cast<G4int>(Z+0.5);
  G4int iA = static_cast<G4int>(A+0.5);
  G4ParticleDefinition * theDef = 0;

  if      (iZ == 1 && iA == 1) theDef = theProton;
  else if (iZ == 1 && iA == 2) theDef = theDeuteron;
  else if (iZ == 1 && iA == 3) theDef = G4Triton::Triton();
  else if (iZ == 2 && iA == 3) theDef = G4He3::He3();
  else if (iZ == 2 && iA == 4) theDef = theAlpha;
  else theDef = G4ParticleTable::GetParticleTable()->FindIon(iZ,iA,0,iZ);
 
  G4double tmass = theDef->GetPDGMass();

  G4LorentzVector lv(0.0,0.0,0.0,tmass);   
  lv += lv1;

  G4ThreeVector bst = lv.boostVector();
  lv1.boost(-bst);

  G4ThreeVector p1 = lv1.vect();
  G4double ptot    = p1.mag();
  G4double ptot2 = ptot*ptot;
  G4double cost = 1 - 0.5*std::fabs(tMand)/ptot2;

  if( cost >= 1.0 )      cost = 1.0;  
  else if( cost <= -1.0) cost = -1.0;
  
  G4double thetaCMS = std::acos(cost);

  G4double sigma = GetDiffuseElasticXsc( particle, thetaCMS, ptot, A);

  sigma *= pi/ptot2;

  return sigma;
}

////////////////////////////////////////////////////////////////////////////
//
// return differential elastic cross section d(sigma)/d(omega) with Coulomb
// correction

G4double 
G4DiffuseElastic::GetDiffuseElasticSumXsc( const G4ParticleDefinition* particle, 
                                        G4double theta, 
			                G4double momentum, 
                                        G4double A, G4double Z         )
{
  fParticle      = particle;
  fWaveVector    = momentum/hbarc;
  fAtomicWeight  = A;
  fAtomicNumber  = Z;
  G4double z             = particle->GetPDGCharge();
  if(z)
  {
    fAddCoulomb = true;
    fBeta          = CalculateParticleBeta( particle, momentum);
    fZommerfeld    = CalculateZommerfeld( fBeta, z, fAtomicNumber);
    fAm            = CalculateAm( momentum, fZommerfeld, fAtomicNumber);
  }
  fNuclearRadius = CalculateNuclearRad(A);

  G4double sigma = fNuclearRadius*fNuclearRadius*GetDiffElasticSumProb(theta);

  return sigma;
}

////////////////////////////////////////////////////////////////////////////
//
// return invariant differential elastic cross section d(sigma)/d(tMand) with Coulomb
// correction

G4double 
G4DiffuseElastic::GetInvElasticSumXsc( const G4ParticleDefinition* particle, 
                                        G4double tMand, 
			                G4double plab, 
                                        G4double A, G4double Z         )
{
  G4double m1 = particle->GetPDGMass();
  G4LorentzVector lv1(0.,0.,plab,std::sqrt(plab*plab+m1*m1));

  G4int iZ = static_cast<G4int>(Z+0.5);
  G4int iA = static_cast<G4int>(A+0.5);
  G4ParticleDefinition * theDef = 0;

  if      (iZ == 1 && iA == 1) theDef = theProton;
  else if (iZ == 1 && iA == 2) theDef = theDeuteron;
  else if (iZ == 1 && iA == 3) theDef = G4Triton::Triton();
  else if (iZ == 2 && iA == 3) theDef = G4He3::He3();
  else if (iZ == 2 && iA == 4) theDef = theAlpha;
  else theDef = G4ParticleTable::GetParticleTable()->FindIon(iZ,iA,0,iZ);
 
  G4double tmass = theDef->GetPDGMass();

  G4LorentzVector lv(0.0,0.0,0.0,tmass);   
  lv += lv1;

  G4ThreeVector bst = lv.boostVector();
  lv1.boost(-bst);

  G4ThreeVector p1 = lv1.vect();
  G4double ptot    = p1.mag();
  G4double ptot2 = ptot*ptot;
  G4double cost = 1 - 0.5*std::fabs(tMand)/ptot2;

  if( cost >= 1.0 )      cost = 1.0;  
  else if( cost <= -1.0) cost = -1.0;
  
  G4double thetaCMS = std::acos(cost);

  G4double sigma = GetDiffuseElasticSumXsc( particle, thetaCMS, ptot, A, Z );

  sigma *= pi/ptot2;

  return sigma;
}

////////////////////////////////////////////////////////////////////////////
//
// return invariant differential elastic cross section d(sigma)/d(tMand) with Coulomb
// correction

G4double 
G4DiffuseElastic::GetInvCoulombElasticXsc( const G4ParticleDefinition* particle, 
                                        G4double tMand, 
			                G4double plab, 
                                        G4double A, G4double Z         )
{
  G4double m1 = particle->GetPDGMass();
  G4LorentzVector lv1(0.,0.,plab,std::sqrt(plab*plab+m1*m1));

  G4int iZ = static_cast<G4int>(Z+0.5);
  G4int iA = static_cast<G4int>(A+0.5);
  G4ParticleDefinition * theDef = 0;

  if      (iZ == 1 && iA == 1) theDef = theProton;
  else if (iZ == 1 && iA == 2) theDef = theDeuteron;
  else if (iZ == 1 && iA == 3) theDef = G4Triton::Triton();
  else if (iZ == 2 && iA == 3) theDef = G4He3::He3();
  else if (iZ == 2 && iA == 4) theDef = theAlpha;
  else theDef = G4ParticleTable::GetParticleTable()->FindIon(iZ,iA,0,iZ);
 
  G4double tmass = theDef->GetPDGMass();

  G4LorentzVector lv(0.0,0.0,0.0,tmass);   
  lv += lv1;

  G4ThreeVector bst = lv.boostVector();
  lv1.boost(-bst);

  G4ThreeVector p1 = lv1.vect();
  G4double ptot    = p1.mag();
  G4double ptot2 = ptot*ptot;
  G4double cost = 1 - 0.5*std::fabs(tMand)/ptot2;

  if( cost >= 1.0 )      cost = 1.0;  
  else if( cost <= -1.0) cost = -1.0;
  
  G4double thetaCMS = std::acos(cost);

  G4double sigma = GetCoulombElasticXsc( particle, thetaCMS, ptot, Z );

  sigma *= pi/ptot2;

  return sigma;
}

////////////////////////////////////////////////////////////////////////////
//
// return differential elastic probability d(probability)/d(omega) 

G4double 
G4DiffuseElastic::GetDiffElasticProb( // G4ParticleDefinition* particle, 
                                        G4double theta 
					//  G4double momentum, 
					// G4double A         
                                     )
{
  G4double sigma, bzero, bzero2, bonebyarg, bonebyarg2, damp, damp2;
  G4double delta, diffuse, gamma;
  G4double e1, e2, bone, bone2;

  // G4double wavek = momentum/hbarc;  // wave vector
  // G4double r0    = 1.08*fermi;
  // G4double rad   = r0*std::pow(A, 1./3.);

  G4double kr    = fWaveVector*fNuclearRadius; // wavek*rad;
  G4double kr2   = kr*kr;
  G4double krt   = kr*theta;

  bzero      = BesselJzero(krt);
  bzero2     = bzero*bzero;    
  bone       = BesselJone(krt);
  bone2      = bone*bone;
  bonebyarg  = BesselOneByArg(krt);
  bonebyarg2 = bonebyarg*bonebyarg;  

  if (fParticle == theProton)
  {
    diffuse = 0.63*fermi;
    gamma   = 0.3*fermi;
    delta   = 0.1*fermi*fermi;
    e1      = 0.3*fermi;
    e2      = 0.35*fermi;
  }
  else // as proton, if were not defined 
  {
    diffuse = 0.63*fermi;
    gamma   = 0.3*fermi;
    delta   = 0.1*fermi*fermi;
    e1      = 0.3*fermi;
    e2      = 0.35*fermi;
  }
  G4double lambda = 15.; // 15 ok

  //  G4double kg    = fWaveVector*gamma;   // wavek*delta;

  G4double kg    = lambda*(1.-std::exp(-fWaveVector*gamma/lambda));   // wavek*delta;
  G4double kg2   = kg*kg;

  // G4double dk2t  = delta*fWaveVector*fWaveVector*theta; // delta*wavek*wavek*theta;
  // G4double dk2t2 = dk2t*dk2t;
  // G4double pikdt = pi*fWaveVector*diffuse*theta;// pi*wavek*diffuse*theta;

  G4double pikdt    = lambda*(1.-std::exp(-pi*fWaveVector*diffuse*theta/lambda));   // wavek*delta;

  damp           = DampFactor(pikdt);
  damp2          = damp*damp;

  G4double mode2k2 = (e1*e1+e2*e2)*fWaveVector*fWaveVector;  
  G4double e2dk3t  = -2.*e2*delta*fWaveVector*fWaveVector*fWaveVector*theta;


  sigma  = kg2;
  // sigma  += dk2t2;
  sigma *= bzero2;
  sigma += mode2k2*bone2 + e2dk3t*bzero*bone;
  sigma += kr2*bonebyarg2;
  sigma *= damp2;          // *rad*rad;

  return sigma;
}

////////////////////////////////////////////////////////////////////////////
//
// return differential elastic probability d(probability)/d(omega) with 
// Coulomb correction

G4double 
G4DiffuseElastic::GetDiffElasticSumProb( // G4ParticleDefinition* particle, 
                                        G4double theta 
					//  G4double momentum, 
					// G4double A         
                                     )
{
  G4double sigma, bzero, bzero2, bonebyarg, bonebyarg2, damp, damp2;
  G4double delta, diffuse, gamma;
  G4double e1, e2, bone, bone2;

  // G4double wavek = momentum/hbarc;  // wave vector
  // G4double r0    = 1.08*fermi;
  // G4double rad   = r0*std::pow(A, 1./3.);

  G4double kr    = fWaveVector*fNuclearRadius; // wavek*rad;
  G4double kr2   = kr*kr;
  G4double krt   = kr*theta;

  bzero      = BesselJzero(krt);
  bzero2     = bzero*bzero;    
  bone       = BesselJone(krt);
  bone2      = bone*bone;
  bonebyarg  = BesselOneByArg(krt);
  bonebyarg2 = bonebyarg*bonebyarg;  

  if (fParticle == theProton)
  {
    diffuse = 0.63*fermi;
    // diffuse = 0.6*fermi;
    gamma   = 0.3*fermi;
    delta   = 0.1*fermi*fermi;
    e1      = 0.3*fermi;
    e2      = 0.35*fermi;
  }
  else // as proton, if were not defined 
  {
    diffuse = 0.63*fermi;
    gamma   = 0.3*fermi;
    delta   = 0.1*fermi*fermi;
    e1      = 0.3*fermi;
    e2      = 0.35*fermi;
  }
  G4double lambda = 15.; // 15 ok
  // G4double kg    = fWaveVector*gamma;   // wavek*delta;
  G4double kg    = lambda*(1.-std::exp(-fWaveVector*gamma/lambda));   // wavek*delta;

  // G4cout<<"kg = "<<kg<<G4endl;

  if(fAddCoulomb)  // add Coulomb correction
  {
    G4double sinHalfTheta  = std::sin(0.5*theta);
    G4double sinHalfTheta2 = sinHalfTheta*sinHalfTheta;

    kg += 0.5*fZommerfeld/kr/(sinHalfTheta2+fAm); // correction at J0()
  // kg += 0.65*fZommerfeld/kr/(sinHalfTheta2+fAm); // correction at J0()
  }

  G4double kg2   = kg*kg;

  // G4double dk2t  = delta*fWaveVector*fWaveVector*theta; // delta*wavek*wavek*theta;
  //   G4cout<<"dk2t = "<<dk2t<<G4endl;
  // G4double dk2t2 = dk2t*dk2t;
  // G4double pikdt = pi*fWaveVector*diffuse*theta;// pi*wavek*diffuse*theta;

  G4double pikdt    = lambda*(1.-std::exp(-pi*fWaveVector*diffuse*theta/lambda));   // wavek*delta;

  // G4cout<<"pikdt = "<<pikdt<<G4endl;

  damp           = DampFactor(pikdt);
  damp2          = damp*damp;

  G4double mode2k2 = (e1*e1+e2*e2)*fWaveVector*fWaveVector;  
  G4double e2dk3t  = -2.*e2*delta*fWaveVector*fWaveVector*fWaveVector*theta;

  sigma  = kg2;
  // sigma += dk2t2;
  sigma *= bzero2;
  sigma += mode2k2*bone2; 
  sigma += e2dk3t*bzero*bone;

  // sigma += kr2*(1 + 8.*fZommerfeld*fZommerfeld/kr2)*bonebyarg2;  // correction at J1()/()
  sigma += kr2*bonebyarg2;  // correction at J1()/()

  sigma *= damp2;          // *rad*rad;

  return sigma;
}


////////////////////////////////////////////////////////////////////////////
//
// return differential elastic probability 2*pi*sin(theta)*d(probability)/d(omega) 

G4double 
G4DiffuseElastic::GetIntegrandFunction( G4double theta )
{
  G4double result;

  result  = GetDiffElasticSumProb(theta);
  result *= 2*pi*std::sin(theta);

  return result;
}

////////////////////////////////////////////////////////////////////////////
//
// return integral elastic cross section d(sigma)/d(omega) integrated 0 - theta 

G4double 
G4DiffuseElastic::IntegralElasticProb(  const G4ParticleDefinition* particle, 
                                        G4double theta, 
			                G4double momentum, 
                                        G4double A         )
{
  G4double result;
  fParticle      = particle;
  fWaveVector    = momentum/hbarc;
  fAtomicWeight  = A;

  fNuclearRadius = CalculateNuclearRad(A);


  G4Integrator<G4DiffuseElastic,G4double(G4DiffuseElastic::*)(G4double)> integral;

  // result = integral.Legendre10(this,&G4DiffuseElastic::GetIntegrandFunction, 0., theta ); 
  result = integral.Legendre96(this,&G4DiffuseElastic::GetIntegrandFunction, 0., theta ); 

  return result;
}

////////////////////////////////////////////////////////////////////////////
//
// Return inv momentum transfer -t > 0

G4double G4DiffuseElastic::SampleT( const G4ParticleDefinition* aParticle, G4double p, G4double A)
{
  G4double theta = SampleThetaCMS( aParticle,  p, A); // sample theta in cms
  G4double t     = 2*p*p*( 1 - std::cos(theta) ); // -t !!!
  return t;
}

////////////////////////////////////////////////////////////////////////////
//
// Return inv momentum transfer -t > 0 from initialisation table

G4double G4DiffuseElastic::SampleTableT( const G4ParticleDefinition* aParticle, G4double p, 
                                               G4double Z, G4double A)
{
  G4double theta = SampleTableThetaCMS( aParticle,  p, Z, A); // sample theta in cms
  G4double t     = 2*p*p*( 1 - std::cos(theta) );             // -t !!!
  return t;
}

////////////////////////////////////////////////////////////////////////////
//
// Return scattering angle sampled in cms


G4double 
G4DiffuseElastic::SampleThetaCMS(const G4ParticleDefinition* particle, 
                                       G4double momentum, G4double A)
{
  G4int i, iMax = 100;  
  G4double norm, result, theta1, theta2, thetaMax, sum = 0.;

  fParticle      = particle;
  fWaveVector    = momentum/hbarc;
  fAtomicWeight  = A;

  fNuclearRadius = CalculateNuclearRad(A);
  
  thetaMax = 10.174/fWaveVector/fNuclearRadius;

  if (thetaMax > pi) thetaMax = pi;

  G4Integrator<G4DiffuseElastic,G4double(G4DiffuseElastic::*)(G4double)> integral;

  // result = integral.Legendre10(this,&G4DiffuseElastic::GetIntegrandFunction, 0., theta ); 
  norm = integral.Legendre96(this,&G4DiffuseElastic::GetIntegrandFunction, 0., thetaMax );

  norm *= G4UniformRand();

  for(i = 1; i <= iMax; i++)
  {
    theta1 = (i-1)*thetaMax/iMax; 
    theta2 = i*thetaMax/iMax;
    sum   += integral.Legendre10(this,&G4DiffuseElastic::GetIntegrandFunction, theta1, theta2);

    if ( sum >= norm ) 
    {
      result = 0.5*(theta1 + theta2);
      break;
    }
  }
  if (i > iMax ) result = 0.5*(theta1 + theta2);

  G4double sigma = pi*thetaMax/iMax;

  result += G4RandGauss::shoot(0.,sigma);

  if(result < 0.) result = 0.;
  if(result > thetaMax) result = thetaMax;

  return result;
}

////////////////////////////////////////////////////////////////////////////
//
// Return scattering angle sampled in cms according to precalculated table.


G4double 
G4DiffuseElastic::SampleTableThetaCMS(const G4ParticleDefinition* particle, 
                                       G4double momentum, G4double Z, G4double A)
{
  size_t iElement;
  G4int iMomentum, iAngle;  
  G4double randAngle, position, theta1, theta2, E1, E2, W1, W2, W;  
  G4double m1 = particle->GetPDGMass();

  for(iElement = 0; iElement < fElementNumberVector.size(); iElement++)
  {
    if( std::fabs(Z - fElementNumberVector[iElement]) < 0.5) break;
  }
  if ( iElement == fElementNumberVector.size() ) 
  {
    InitialiseOnFly(Z,A);
    // iElement--;

    // G4cout << "G4DiffuseElastic: Element with atomic number " << Z
    // << " is not found, return zero angle" << G4endl;
    // return 0.; // no table for this element
  }
  // G4cout<<"iElement = "<<iElement<<G4endl;

  fAngleTable = fAngleBank[iElement];

  G4double kinE = std::sqrt(momentum*momentum + m1*m1) - m1;

  for(iMomentum = 0; iMomentum < fEnergyBin; iMomentum++)
  {
    if( kinE < fEnergyVector->GetLowEdgeEnergy(iMomentum) ) break;
  }
  if ( iMomentum >= fEnergyBin ) iMomentum--;   // kinE is more then theMaxEnergy
  if ( iMomentum < 0 )           iMomentum = 0; // against negative index, kinE < theMinEnergy
  // G4cout<<"iMomentum = "<<iMomentum<<G4endl;

  if (iMomentum == fEnergyBin -1 || iMomentum == 0 ) // the table edges
  {
    position = (*(*fAngleTable)(iMomentum))(fAngleBin-2)*G4UniformRand();
    // G4cout<<"position = "<<position<<G4endl;

    for(iAngle = 0; iAngle < fAngleBin-1; iAngle++)
    {
      if( position < (*(*fAngleTable)(iMomentum))(iAngle) ) break;
    }
    if (iAngle >= fAngleBin-1) iAngle--;
    // G4cout<<"iAngle = "<<iAngle<<G4endl;

    randAngle = GetScatteringAngle(iMomentum, iAngle, position);
    // G4cout<<"randAngle = "<<randAngle<<G4endl;
  }
  else
  {
    position = (*(*fAngleTable)(iMomentum))(fAngleBin-2)*G4UniformRand();
    // G4cout<<"position = "<<position<<G4endl;

    for(iAngle = 0; iAngle < fAngleBin-1; iAngle++)
    {
      if( position < (*(*fAngleTable)(iMomentum))(iAngle) ) break;
    }
    if (iAngle >= fAngleBin-1) iAngle--;
    // G4cout<<"iAngle = "<<iAngle<<G4endl;

    theta2  = GetScatteringAngle(iMomentum, iAngle, position);
    // G4cout<<"theta2 = "<<theta2<<G4endl;
    E2 = fEnergyVector->GetLowEdgeEnergy(iMomentum);
    // G4cout<<"E2 = "<<E2<<G4endl;

    iMomentum--;

    position = (*(*fAngleTable)(iMomentum))(fAngleBin-2)*G4UniformRand();
    // G4cout<<"position = "<<position<<G4endl;

    for(iAngle = 0; iAngle < fAngleBin-1; iAngle++)
    {
      if( position < (*(*fAngleTable)(iMomentum))(iAngle) ) break;
    }
    if (iAngle >= fAngleBin-1) iAngle--;

    theta1  = GetScatteringAngle(iMomentum, iAngle, position);
    // G4cout<<"theta1 = "<<theta1<<G4endl;
    E1 = fEnergyVector->GetLowEdgeEnergy(iMomentum);
    // G4cout<<"E1 = "<<E1<<G4endl;

    W  = 1.0/(E2 - E1);
    W1 = (E2 - kinE)*W;
    W2 = (kinE - E1)*W;

    randAngle = W1*theta1 + W2*theta2;
    // G4cout<<"randAngle = "<<randAngle<<G4endl;
  }
  return randAngle;
}

/////////////////////////////////////////////////////////////////////////////////
//
//

G4double 
G4DiffuseElastic:: GetScatteringAngle(G4int iMomentum, G4int iAngle, G4double position)
{
 G4double x1, x2, y1, y2, randAngle;

  if( iAngle == 0 )
  {
    randAngle = (*fAngleTable)(iMomentum)->GetLowEdgeEnergy(iAngle);
  }
  else
  {
    if ( iAngle >= G4int((*fAngleTable)(iMomentum)->GetVectorLength()) )
    {
      iAngle = (*fAngleTable)(iMomentum)->GetVectorLength() - 1;
    }
    y1 = (*(*fAngleTable)(iMomentum))(iAngle-1);
    y2 = (*(*fAngleTable)(iMomentum))(iAngle);

    x1 = (*fAngleTable)(iMomentum)->GetLowEdgeEnergy(iAngle-1);
    x2 = (*fAngleTable)(iMomentum)->GetLowEdgeEnergy(iAngle);

    if ( x1 == x2 )    randAngle = x2;
    else
    {
      if ( y1 == y2  ) randAngle = x1 + (x2 - x1)*G4UniformRand();
      else
      {
        randAngle = x1 + (position - y1)*(x2 - x1)/(y2 - y1);
      }
    }
  }
  return randAngle;
}



////////////////////////////////////////////////////////////////////////////
//
// Return scattering angle sampled in lab system (target at rest)



G4double 
G4DiffuseElastic::SampleThetaLab( const G4HadProjectile* aParticle, 
                                        G4double tmass, G4double A)
{
  const G4ParticleDefinition* theParticle = aParticle->GetDefinition();
  G4double m1 = theParticle->GetPDGMass();
  G4double plab = aParticle->GetTotalMomentum();
  G4LorentzVector lv1 = aParticle->Get4Momentum();
  G4LorentzVector lv(0.0,0.0,0.0,tmass);   
  lv += lv1;

  G4ThreeVector bst = lv.boostVector();
  lv1.boost(-bst);

  G4ThreeVector p1 = lv1.vect();
  G4double ptot    = p1.mag();
  G4double tmax    = 4.0*ptot*ptot;
  G4double t       = 0.0;


  //
  // Sample t
  //
  
  t = SampleT( theParticle, ptot, A);

  // NaN finder
  if(!(t < 0.0 || t >= 0.0)) 
  {
    if (verboseLevel > 0) 
    {
      G4cout << "G4DiffuseElastic:WARNING: A = " << A 
	     << " mom(GeV)= " << plab/GeV 
             << " S-wave will be sampled" 
	     << G4endl; 
    }
    t = G4UniformRand()*tmax; 
  }
  if(verboseLevel>1)
  {
    G4cout <<" t= " << t << " tmax= " << tmax 
	   << " ptot= " << ptot << G4endl;
  }
  // Sampling of angles in CM system

  G4double phi  = G4UniformRand()*twopi;
  G4double cost = 1. - 2.0*t/tmax;
  G4double sint;

  if( cost >= 1.0 ) 
  {
    cost = 1.0;
    sint = 0.0;
  }
  else if( cost <= -1.0) 
  {
    cost = -1.0;
    sint =  0.0;
  }
  else  
  {
    sint = std::sqrt((1.0-cost)*(1.0+cost));
  }    
  if (verboseLevel>1) 
  {
    G4cout << "cos(t)=" << cost << " std::sin(t)=" << sint << G4endl;
  }
  G4ThreeVector v1(sint*std::cos(phi),sint*std::sin(phi),cost);
  v1 *= ptot;
  G4LorentzVector nlv1(v1.x(),v1.y(),v1.z(),std::sqrt(ptot*ptot + m1*m1));

  nlv1.boost(bst); 

  G4ThreeVector np1 = nlv1.vect();

    // G4double theta = std::acos( np1.z()/np1.mag() );  // degree;

  G4double theta = np1.theta();

  return theta;
}

////////////////////////////////////////////////////////////////////////////
//
// Return scattering angle in lab system (target at rest) knowing theta in CMS



G4double 
G4DiffuseElastic::ThetaCMStoThetaLab( const G4DynamicParticle* aParticle, 
                                        G4double tmass, G4double thetaCMS)
{
  const G4ParticleDefinition* theParticle = aParticle->GetDefinition();
  G4double m1 = theParticle->GetPDGMass();
  // G4double plab = aParticle->GetTotalMomentum();
  G4LorentzVector lv1 = aParticle->Get4Momentum();
  G4LorentzVector lv(0.0,0.0,0.0,tmass);   

  lv += lv1;

  G4ThreeVector bst = lv.boostVector();

  lv1.boost(-bst);

  G4ThreeVector p1 = lv1.vect();
  G4double ptot    = p1.mag();

  G4double phi  = G4UniformRand()*twopi;
  G4double cost = std::cos(thetaCMS);
  G4double sint;

  if( cost >= 1.0 ) 
  {
    cost = 1.0;
    sint = 0.0;
  }
  else if( cost <= -1.0) 
  {
    cost = -1.0;
    sint =  0.0;
  }
  else  
  {
    sint = std::sqrt((1.0-cost)*(1.0+cost));
  }    
  if (verboseLevel>1) 
  {
    G4cout << "cos(tcms)=" << cost << " std::sin(tcms)=" << sint << G4endl;
  }
  G4ThreeVector v1(sint*std::cos(phi),sint*std::sin(phi),cost);
  v1 *= ptot;
  G4LorentzVector nlv1(v1.x(),v1.y(),v1.z(),std::sqrt(ptot*ptot + m1*m1));

  nlv1.boost(bst); 

  G4ThreeVector np1 = nlv1.vect();


  G4double thetaLab = np1.theta();

  return thetaLab;
}
////////////////////////////////////////////////////////////////////////////
//
// Return scattering angle in CMS system (target at rest) knowing theta in Lab



G4double 
G4DiffuseElastic::ThetaLabToThetaCMS( const G4DynamicParticle* aParticle, 
                                        G4double tmass, G4double thetaLab)
{
  const G4ParticleDefinition* theParticle = aParticle->GetDefinition();
  G4double m1 = theParticle->GetPDGMass();
  G4double plab = aParticle->GetTotalMomentum();
  G4LorentzVector lv1 = aParticle->Get4Momentum();
  G4LorentzVector lv(0.0,0.0,0.0,tmass);   

  lv += lv1;

  G4ThreeVector bst = lv.boostVector();

  // lv1.boost(-bst);

  // G4ThreeVector p1 = lv1.vect();
  // G4double ptot    = p1.mag();

  G4double phi  = G4UniformRand()*twopi;
  G4double cost = std::cos(thetaLab);
  G4double sint;

  if( cost >= 1.0 ) 
  {
    cost = 1.0;
    sint = 0.0;
  }
  else if( cost <= -1.0) 
  {
    cost = -1.0;
    sint =  0.0;
  }
  else  
  {
    sint = std::sqrt((1.0-cost)*(1.0+cost));
  }    
  if (verboseLevel>1) 
  {
    G4cout << "cos(tlab)=" << cost << " std::sin(tlab)=" << sint << G4endl;
  }
  G4ThreeVector v1(sint*std::cos(phi),sint*std::sin(phi),cost);
  v1 *= plab;
  G4LorentzVector nlv1(v1.x(),v1.y(),v1.z(),std::sqrt(plab*plab + m1*m1));

  nlv1.boost(-bst); 

  G4ThreeVector np1 = nlv1.vect();


  G4double thetaCMS = np1.theta();

  return thetaCMS;
}

//
//
/////////////////////////////////////////////////////////////////////////////////
