// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: TestEm5.cc,v 1.3 1999-03-10 11:44:31 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// --------------------------------------------------------------
//      GEANT 4 - TestEm5 
//
//      For information related to this code contact:
//      CERN, IT Division, ASD Group
// --------------------------------------------------------------
// Comments
//     
//   
// --------------------------------------------------------------

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "Randomize.hh"

#include "Em5DetectorConstruction.hh"
#include "Em5PhysicsList.hh"
#include "Em5PrimaryGeneratorAction.hh"
#include "Em5RunAction.hh"
#include "Em5EventAction.hh"
#include "Em5SteppingAction.hh"

#ifdef GNU_GCC
#include <rw/tpordvec.h>
#include "Em5CalorHit.hh"
template class RWTPtrOrderedVector <Em5CalorHit>;
template class RWTPtrVector <Em5CalorHit>;
template class G4Allocator <Em5CalorHit>;
#endif

int main(int argc,char** argv) {

  //choose the Random engine
  HepRandom::setTheEngine(new RanecuEngine);
  
  // Construct the default run manager
  G4RunManager * runManager = new G4RunManager;

  // set mandatory initialization classes
  Em5DetectorConstruction* detector;
  detector = new Em5DetectorConstruction;
  runManager->SetUserInitialization(detector);
  runManager->SetUserInitialization(new Em5PhysicsList(detector));
  
  // set user action classes
  runManager->SetUserAction(new Em5PrimaryGeneratorAction(detector));
  Em5RunAction* runaction = new Em5RunAction;
  runManager->SetUserAction(runaction);

  Em5EventAction* eventaction = new Em5EventAction(runaction);
  runManager->SetUserAction(eventaction);

  Em5SteppingAction* steppingaction = new Em5SteppingAction(detector,
                                               eventaction, runaction);
  runManager->SetUserAction(steppingaction);
    
  // get the pointer to the User Interface manager 
    G4UImanager* UI = G4UImanager::GetUIpointer();  
 
  if (argc==1)   // Define UI terminal for interactive mode  
    { 
     G4UIsession * session = new G4UIterminal;
     UI->ApplyCommand("/control/execute init.mac");    
     session->SessionStart();
     delete session;
    }
  else           // Batch mode
    { 
     G4String command = "/control/execute ";
     G4String fileName = argv[1];
     UI->ApplyCommand(command+fileName);
    }
  // job termination
  delete runManager;

  return 0;
}

