// $Id: ExN01PhysicsList.cc,v 1.2 2006-04-25 10:31:40 kmura Exp $
// $Name: not supported by cvs2svn $
// ====================================================================
//   ExN01PhysicsList.cc
//
//                                         2005 Q
// ====================================================================
#include "ExN01PhysicsList.hh"
#include "G4ParticleTypes.hh"

// ====================================================================
//
// class description
//
// ====================================================================

////////////////////////////////////
ExN01PhysicsList::ExN01PhysicsList()
////////////////////////////////////
{
}

/////////////////////////////////////
ExN01PhysicsList::~ExN01PhysicsList()
/////////////////////////////////////
{
}

//////////////////////////////////////////
void ExN01PhysicsList::ConstructParticle()
//////////////////////////////////////////
{
  G4Geantino::GeantinoDefinition();
}

/////////////////////////////////////////
void ExN01PhysicsList::ConstructProcess()
/////////////////////////////////////////
{
  AddTransportation();
}

////////////////////////////////
void ExN01PhysicsList::SetCuts()
////////////////////////////////
{
  G4int temp = GetVerboseLevel();
  SetVerboseLevel(0);

  SetCutsWithDefault();   

  SetVerboseLevel(temp);  
}

