// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: MyRunAction.cc,v 1.4 2000-05-26 13:11:40 barrand Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "MyRunAction.hh"

#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"

#ifdef G4VIS_USE_OPACS
OHistogram MyRunAction::myHisto1D = NULL;
OHistogram MyRunAction::myHisto2D = NULL;
#endif

MyRunAction::MyRunAction()
{
  timer = new G4Timer;
}

MyRunAction::~MyRunAction()
{
  delete timer;
}

void MyRunAction::BeginOfRunAction(const G4Run* aRun)
{
  //G4UImanager* UI = G4UImanager::GetUIpointer(); 
  //UI->ApplyCommand("/vis/scene/notifyHandlers");

  //aRun->transient(true);

#ifdef G4VIS_USE_OPACS
  // Histo are created in an osh script somewhere (test201.odb).
  myHisto1D = OHistogramGetIdentifier("HepJamesRandom1");
  myHisto2D = OHistogramGetIdentifier("HepJamesVsDRand48");
#endif

  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  timer->Start();
}

void MyRunAction::EndOfRunAction(const G4Run* aRun)
{
  //G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");

  timer->Stop();
  G4cout << "number of event = " << aRun->GetNumberOfEvent() 
       << " " << *timer << G4endl;
#ifdef G4VIS_USE_OPACS
  myHisto1D = NULL;
  myHisto2D = NULL;
#endif
}

