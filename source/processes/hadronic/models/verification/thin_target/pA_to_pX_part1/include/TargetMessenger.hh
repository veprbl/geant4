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
// $Id: TargetMessenger.hh,v 1.1 2003-05-27 13:44:45 hpw Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
#ifndef TargetMessenger_h
#define TargetMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class TargetConstruction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;


class TargetMessenger: public G4UImessenger
{
  public:
    TargetMessenger(TargetConstruction* );
    ~TargetMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    TargetConstruction* Target;
    
    G4UIdirectory* TargDir;
    G4UIcmdWithAString* TgtMaterCmd;
    G4UIcmdWithADoubleAndUnit* TgtThickCmd;
    G4UIcmdWithADoubleAndUnit* TgtRadiusCmd;
    G4UIcmdWithAnInteger* NumEvtCmd;
    G4UIcmdWithADoubleAndUnit* MagFieldCmd;
    G4UIcmdWithoutParameter* UpdateCmd;
};

#endif





