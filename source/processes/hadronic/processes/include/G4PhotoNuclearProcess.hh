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
// $Id: G4PhotoNuclearProcess.hh,v 1.6 2006-06-01 15:32:50 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Class Description
// Process for photon nuclear inelastic scattering; 
// to be used in your physics list in case you need this physics.
//

// Hadronic Process: Ion Inelastic Process
// J.P. Wellisch, CERN, Apr. 14 2000
// Last modified: 03-Apr-1997

#ifndef G4PhotoNuclearProcess_h
#define G4PhotoNuclearProcess_h 1
 
#include "G4HadronInelasticProcess.hh"
#include "G4PhotoNuclearCrossSection.hh"
 

 class G4PhotoNuclearProcess : public G4HadronInelasticProcess
 {
 public:
    
    G4PhotoNuclearProcess( const G4String& processName = "PhotonInelastic" );
    ~G4PhotoNuclearProcess();

 private:
 
   G4PhotoNuclearCrossSection theData;
 };

#endif

