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
// $Id: G4ASCIITreeViewer.hh,v 1.4 2001-07-11 10:09:06 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// John Allison  5th April 2001
// A dummy viewer for ASCIITreeSceneHandler.

#ifndef G4ASCIITREEVIEWER_HH
#define G4ASCIITREEVIEWER_HH

#include "G4VTreeViewer.hh"

class G4ASCIITreeViewer: public G4VTreeViewer {
public:
  G4ASCIITreeViewer(G4VSceneHandler&,const G4String& name);
  virtual ~G4ASCIITreeViewer();
};

#endif
