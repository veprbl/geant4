#include "G4NeutronLENDBuilder.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"

G4NeutronLENDBuilder::
G4NeutronLENDBuilder() 
{
  theLENDElastic = 0;
  theLENDElasticCrossSection = 0;
  
  theLENDFission = 0;
  theLENDFissionCrossSection = 0;
  
  theLENDCapture = 0;
  theLENDCaptureCrossSection = 0;
  
  theLENDInelastic = 0;
  theLENDInelasticCrossSection = 0;

  theMin = 0;
  theIMin = theMin;
  theMax = 20*MeV;
  theIMax = theMax;

}

G4NeutronLENDBuilder::
~G4NeutronLENDBuilder() 
{
  delete theLENDElasticCrossSection;
  delete theLENDFissionCrossSection;
  delete theLENDCaptureCrossSection;
  delete theLENDInelasticCrossSection;
}

void G4NeutronLENDBuilder::
Build(G4HadronElasticProcess * aP)
{
  if(theLENDElastic==0) theLENDElastic = new G4LENDElastic( G4Neutron::Neutron() );
  theLENDElastic->SetMinEnergy(theMin);
  theLENDElastic->SetMaxEnergy(theMax);
  //theLENDElastic->AllowNaturalAbundanceTarget();
  theLENDElastic->AllowAnyCandidateTarget();
  if(theLENDElasticCrossSection == 0) theLENDElasticCrossSection = new G4LENDElasticCrossSection( G4Neutron::Neutron() );
  //theLENDElasticCrossSection->AllowNaturalAbundanceTarget();
  theLENDElasticCrossSection->AllowAnyCandidateTarget();
  aP->AddDataSet(theLENDElasticCrossSection);
  aP->RegisterMe(theLENDElastic);
}

void G4NeutronLENDBuilder::
Build(G4HadronFissionProcess * aP)
{
  if(theLENDFission == 0) theLENDFission = new G4LENDFission( G4Neutron::Neutron() );
  theLENDFission->SetMinEnergy(theMin);
  theLENDFission->SetMaxEnergy(theMax);
  //theLENDFission->AllowNaturalAbundanceTarget();
  theLENDFission->AllowAnyCandidateTarget();
  if(theLENDFissionCrossSection==0) theLENDFissionCrossSection=new G4LENDFissionCrossSection( G4Neutron::Neutron() );
  //theLENDFissionCrossSection->AllowNaturalAbundanceTarget();
  theLENDFissionCrossSection->AllowAnyCandidateTarget();
  aP->AddDataSet(theLENDFissionCrossSection);
  aP->RegisterMe(theLENDFission);
}

void G4NeutronLENDBuilder::
Build(G4HadronCaptureProcess * aP)
{
  if(theLENDCapture==0) theLENDCapture = new G4LENDCapture( G4Neutron::Neutron() );
  theLENDCapture->SetMinEnergy(theMin);
  theLENDCapture->SetMaxEnergy(theMax);
  //theLENDCapture->AllowNaturalAbundanceTarget();
  theLENDCapture->AllowAnyCandidateTarget();
  if(theLENDCaptureCrossSection==0) theLENDCaptureCrossSection = new G4LENDCaptureCrossSection( G4Neutron::Neutron() );
  //theLENDCaptureCrossSection->AllowNaturalAbundanceTarget();
  theLENDCaptureCrossSection->AllowAnyCandidateTarget();
  aP->AddDataSet(theLENDCaptureCrossSection);
  aP->RegisterMe(theLENDCapture);
}

void G4NeutronLENDBuilder::
Build(G4NeutronInelasticProcess * aP)
{
  if(theLENDInelastic==0) theLENDInelastic = new G4LENDInelastic( G4Neutron::Neutron() );
  theLENDInelastic->SetMinEnergy(theIMin);
  theLENDInelastic->SetMaxEnergy(theIMax);
  //theLENDInelastic->AllowNaturalAbundanceTarget();
  theLENDInelastic->AllowAnyCandidateTarget();
  if(theLENDInelasticCrossSection==0) theLENDInelasticCrossSection = new G4LENDInelasticCrossSection( G4Neutron::Neutron() );
  //theLENDInelasticCrossSection->AllowNaturalAbundanceTarget();
  theLENDInelasticCrossSection->AllowAnyCandidateTarget();
  aP->AddDataSet(theLENDInelasticCrossSection);
  aP->RegisterMe(theLENDInelastic);
}
