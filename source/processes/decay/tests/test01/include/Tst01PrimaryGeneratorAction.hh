//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: Tst01PrimaryGeneratorAction.hh,v 1.2 2001-07-11 10:02:28 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef Tst01PrimaryGeneratorAction_h
#define Tst01PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class Tst01ParticleGun;
class G4Event;

class Tst01PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    Tst01PrimaryGeneratorAction();
    ~Tst01PrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event* anEvent);

  private:
    Tst01ParticleGun* particleGun;
};

#endif


