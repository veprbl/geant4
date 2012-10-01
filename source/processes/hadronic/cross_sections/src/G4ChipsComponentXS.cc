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
//  Calculation of the total, elastic and inelastic cross-sections
//  of hadron (proton, neutron, pi+, pi-, K+, K-, anti_proton, anti_neutron
//  interactions with nuclei based on CHIPS model
//
//   Created by V. Uzhinsky, 31.05.2011  
//  Copied to hadronic/cross_sections by W. Pokorski

#include "G4ChipsComponentXS.hh"

#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"

#include "G4CrossSectionDataSetRegistry.hh"

///////////////////////////////////////////////////////////////////////////////


G4ChipsComponentXS::G4ChipsComponentXS():G4VComponentCrossSection("G4ChipsComponentXS"), fUpperLimit( 10000 * GeV ),
					 fLowerLimit( 10 * MeV )
{
  PxsManagerEl      = (G4ChipsProtonElasticXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsProtonElasticXS::Default_Name());
  PxsManagerInEl    = (G4ChipsProtonInelasticXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsProtonInelasticXS::Default_Name());

  NxsManagerEl      = (G4ChipsNeutronElasticXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsNeutronElasticXS::Default_Name());
  NxsManagerInEl    = (G4ChipsNeutronInelasticXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsNeutronInelasticXS::Default_Name());

  PBARxsManagerEl   = (G4ChipsAntiBaryonElasticXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsAntiBaryonElasticXS::Default_Name());
  PBARxsManagerInEl = (G4ChipsAntiBaryonInelasticXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsAntiBaryonInelasticXS::Default_Name());

  PIPxsManagerEl    = (G4ChipsPionPlusElasticXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsPionPlusElasticXS::Default_Name());
  PIPxsManagerInEl  = (G4ChipsPionPlusInelasticXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsPionPlusInelasticXS::Default_Name()); 

  PIMxsManagerEl    = (G4ChipsPionMinusElasticXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsPionMinusElasticXS::Default_Name());
  PIMxsManagerInEl  = (G4ChipsPionMinusInelasticXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsPionMinusInelasticXS::Default_Name());

  KPxsManagerEl     = (G4ChipsKaonPlusElasticXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsKaonPlusElasticXS::Default_Name());
  KPxsManagerInEl   = (G4ChipsKaonPlusInelasticXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsKaonPlusInelasticXS::Default_Name());

  KMxsManagerEl     = (G4ChipsKaonMinusElasticXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsKaonMinusElasticXS::Default_Name());
  KMxsManagerInEl   = (G4ChipsKaonMinusInelasticXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsKaonMinusInelasticXS::Default_Name());

  KZxsManagerEl     = (G4ChipsKaonZeroElasticXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsKaonZeroElasticXS::Default_Name());
  KZxsManagerInEl   = (G4ChipsKaonZeroInelasticXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsKaonZeroInelasticXS::Default_Name());

  HxsManagerEl     = (G4ChipsHyperonElasticXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsHyperonElasticXS::Default_Name());
  HxsManagerInEl   = (G4ChipsHyperonInelasticXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsHyperonInelasticXS::Default_Name());
}

///////////////////////////////////////////////////////////////////////////////////////
G4ChipsComponentXS::~G4ChipsComponentXS()
{
}

////////////////////////////////////////////////////////////////////////////////
G4double G4ChipsComponentXS::GetTotalElementCrossSection
(const G4ParticleDefinition* aParticle, G4double kinEnergy, G4int Z, G4double N)
{
  G4double momentum = std::sqrt(kinEnergy*(kinEnergy+2.*aParticle->GetPDGMass()));
  G4int PDGcode=aParticle->GetPDGEncoding();

  G4ChipsBaseXS* CHIPSmanagerEl=0; 
  G4ChipsBaseXS* CHIPSmanagerInEl=0;

  if     (PDGcode == 2212)   // Projectile is Proton
  {
   CHIPSmanagerEl=PxsManagerEl;  CHIPSmanagerInEl=PxsManagerInEl;
  } else if(PDGcode == 2112)  // Projectile is Neutron 
  {
   CHIPSmanagerEl=NxsManagerEl;  CHIPSmanagerInEl=NxsManagerInEl;   
  } else if(PDGcode == -2212) // Projectile is Anti-Proton
  {
   CHIPSmanagerEl=PBARxsManagerEl; CHIPSmanagerInEl=PBARxsManagerInEl;
  } else if(PDGcode == -2112) // Projectile is Anti-Neutron
  {
   CHIPSmanagerEl=PBARxsManagerEl; CHIPSmanagerInEl=PBARxsManagerInEl;
  }else if(PDGcode == -3122 || PDGcode == -3222  || PDGcode == -3212  || PDGcode == -3112  || PDGcode == -3322  
	   || PDGcode == -3312  || PDGcode == -3334) // Projectile is other Anti-baryon
  {
   CHIPSmanagerEl=PBARxsManagerEl; CHIPSmanagerInEl=PBARxsManagerInEl;
  } else if(PDGcode ==   211) // Projectile is Pi+ 
  {
   CHIPSmanagerEl=PIPxsManagerEl; CHIPSmanagerInEl=PIPxsManagerInEl;
  } else if(PDGcode ==  -211) // Projectile is Pi-
  {
   CHIPSmanagerEl=PIMxsManagerEl; CHIPSmanagerInEl=PIMxsManagerInEl;
  } else if(PDGcode ==  321)  // Projectile is K+ 
  {
   CHIPSmanagerEl=KPxsManagerEl; CHIPSmanagerInEl=KPxsManagerInEl; 
  } else if(PDGcode ==  -321) // Projectile is K- 
  {
   CHIPSmanagerEl=KMxsManagerEl; CHIPSmanagerInEl=KMxsManagerInEl;  
  } else if(PDGcode ==  130 || PDGcode ==  310 || PDGcode ==  311 || PDGcode ==  -311) // Projectile is K0
  {
   CHIPSmanagerEl=KZxsManagerEl; CHIPSmanagerInEl=KZxsManagerInEl; 
  }else if(PDGcode ==   3122 || PDGcode ==  3222 || PDGcode ==  3112 || PDGcode ==  3212
	   || PDGcode ==   3312 || PDGcode ==  3322 || PDGcode ==   3334) // Projectile is hyperon
  {
   CHIPSmanagerEl=HxsManagerEl; CHIPSmanagerInEl=HxsManagerInEl; 
  } 

  G4double Xelastic(0.), Xinelastic(0.);

  if((CHIPSmanagerEl != 0) && (CHIPSmanagerInEl != 0))
  { 
   Xelastic   = CHIPSmanagerEl->GetChipsCrossSection(momentum,Z,(G4int)N,PDGcode);
   Xinelastic = CHIPSmanagerInEl->GetChipsCrossSection(momentum,Z,(G4int)N,PDGcode);
  }

  return Xelastic+Xinelastic; 
}

////////////////////////////////////////////////////////////////////////////////
G4double G4ChipsComponentXS::GetTotalIsotopeCrossSection
(const G4ParticleDefinition* aParticle, G4double kinEnergy, G4int Z, G4int A )
{ return GetTotalElementCrossSection(aParticle, kinEnergy, Z, (G4double) A);  }

////////////////////////////////////////////////////////////////////////////////
G4double G4ChipsComponentXS::GetInelasticElementCrossSection
(const G4ParticleDefinition* aParticle, G4double kinEnergy, G4int Z, G4double N)
{
  G4double momentum = std::sqrt(kinEnergy*(kinEnergy+2.*aParticle->GetPDGMass()));
  G4int PDGcode=aParticle->GetPDGEncoding();

  G4ChipsBaseXS* CHIPSmanagerInEl=0;

  if     (PDGcode == 2212)   // Projectile is Proton
  {
   CHIPSmanagerInEl=PxsManagerInEl;
  } else if(PDGcode == 2112)  // Projectile is Neutron 
  {
   CHIPSmanagerInEl=NxsManagerInEl;   
  } else if(PDGcode == -2212) // Projectile is Anti-Proton
  {
   CHIPSmanagerInEl=PBARxsManagerInEl;
  } else if(PDGcode == -2112) // Projectile is Anti-Neutron
  {
   CHIPSmanagerInEl=PBARxsManagerInEl;
  } else if(PDGcode ==   211) // Projectile is Pi+ 
  {
   CHIPSmanagerInEl=PIPxsManagerInEl;
  } else if(PDGcode ==  -211) // Projectile is Pi-
  {
   CHIPSmanagerInEl=PIMxsManagerInEl;
  } else if(PDGcode ==  321)  // Projectile is K+ 
  {
   CHIPSmanagerInEl=KPxsManagerInEl; 
  } else if(PDGcode ==  -321) // Projectile is K- 
  {
   CHIPSmanagerInEl=KMxsManagerInEl; 
  } 

  G4double Xinelastic(0.);

  if(CHIPSmanagerInEl != 0)
  { 
   Xinelastic = CHIPSmanagerInEl->GetChipsCrossSection(momentum,Z,(G4int)N,PDGcode);
  }
  return Xinelastic; 
}

///////////////////////////////////////////////////////////////////////////////
G4double G4ChipsComponentXS::GetInelasticIsotopeCrossSection
(const G4ParticleDefinition* aParticle, G4double kinEnergy, G4int Z, G4int A)
{return GetInelasticElementCrossSection(aParticle, kinEnergy, Z, (G4double) A); }
 
///////////////////////////////////////////////////////////////////////////////
G4double G4ChipsComponentXS::GetElasticElementCrossSection
(const G4ParticleDefinition* aParticle, G4double kinEnergy, G4int Z, G4double N)
{
  G4double momentum = std::sqrt(kinEnergy*(kinEnergy+2.*aParticle->GetPDGMass()));
  G4int PDGcode=aParticle->GetPDGEncoding();

  G4ChipsBaseXS* CHIPSmanagerEl=0; 

  if     (PDGcode == 2212)   // Projectile is Proton
  {
   CHIPSmanagerEl=PxsManagerEl;
  } else if(PDGcode == 2112)  // Projectile is Neutron 
  {
   CHIPSmanagerEl=NxsManagerEl;   
  } else if(PDGcode == -2212) // Projectile is Anti-Proton
  {
   CHIPSmanagerEl=PBARxsManagerEl;
  } else if(PDGcode == -2112) // Projectile is Anti-Neutron
  {
   CHIPSmanagerEl=PBARxsManagerEl;
  } else if(PDGcode ==   211) // Projectile is Pi+ 
  {
   CHIPSmanagerEl=PIPxsManagerEl;
  } else if(PDGcode ==  -211) // Projectile is Pi-
  {
   CHIPSmanagerEl=PIMxsManagerEl;
  } else if(PDGcode ==  321)  // Projectile is K+ 
  {
   CHIPSmanagerEl=KPxsManagerEl;
  } else if(PDGcode ==  -321) // Projectile is K- 
  {
   CHIPSmanagerEl=KMxsManagerEl;
  } 

  G4double Xelastic(0.);

  if(CHIPSmanagerEl != 0)
  { 
   Xelastic   = CHIPSmanagerEl->GetChipsCrossSection(momentum,Z,(G4int)N,PDGcode);
  }
  return Xelastic; 
}
 
///////////////////////////////////////////////////////////////////////////////
G4double G4ChipsComponentXS::GetElasticIsotopeCrossSection
(const G4ParticleDefinition* aParticle, G4double kinEnergy, G4int Z, G4int A)
{ return GetElasticElementCrossSection(aParticle, kinEnergy, Z, (G4double) A); }
