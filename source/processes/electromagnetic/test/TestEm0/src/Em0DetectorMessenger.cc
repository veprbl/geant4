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
// $Id: Em0DetectorMessenger.cc,v 1.4 2001-07-11 10:03:37 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "Em0DetectorMessenger.hh"

#include "Em0DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

Em0DetectorMessenger::Em0DetectorMessenger(Em0DetectorConstruction * Em0Det)
:Em0Detector(Em0Det)
{ 
  Em0detDir = new G4UIdirectory("/calor/");
  Em0detDir->SetGuidance("Em0 detector control.");
      
  MaterCmd = new G4UIcmdWithAString("/calor/setMat",this);
  MaterCmd->SetGuidance("Select material of the box.");
  MaterCmd->SetParameterName("choice",true);
  MaterCmd->AvailableForStates(Idle);
  
  SizeCmd = new G4UIcmdWithADoubleAndUnit("/calor/setSize",this);
  SizeCmd->SetGuidance("Set size of the box");
  SizeCmd->SetParameterName("Size",false,false);
  SizeCmd->SetRange("Size>0.");
  SizeCmd->AvailableForStates(Idle);
      
  UpdateCmd = new G4UIcmdWithoutParameter("/calor/update",this);
  UpdateCmd->SetGuidance("Update calorimeter geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

Em0DetectorMessenger::~Em0DetectorMessenger()
{
  delete MaterCmd;
  delete SizeCmd; 
  delete UpdateCmd;
  delete Em0detDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void Em0DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == MaterCmd )
   { Em0Detector->SetMaterial(newValue);}
   
  if( command == SizeCmd )
   { Em0Detector->SetSize(SizeCmd->GetNewDoubleValue(newValue));}
   
  if( command == UpdateCmd )
   { Em0Detector->UpdateGeometry(); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
