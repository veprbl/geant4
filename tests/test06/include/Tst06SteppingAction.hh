
#ifndef Tst06SteppingAction_H
#define Tst06SteppingAction_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class Tst06SteppingAction : public G4UserSteppingAction
{
  public:
    Tst06SteppingAction();
    virtual ~Tst06SteppingAction();

    virtual void UserSteppingAction(const G4Step*);
};

#endif

