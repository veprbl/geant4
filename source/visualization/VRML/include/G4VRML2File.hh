// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4VRML2File.hh,v 1.5 1999-05-10 15:39:11 johna Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// G4VRML2File.hh
// Satoshi Tanaka & Yasuhide Sawada

#if defined (G4VIS_BUILD_VRMLFILE_DRIVER) || defined (G4VIS_USE_VRMLFILE)

#ifndef G4VRML2FILE_HH
#define G4VRML2FILE_HH

#include "G4VGraphicsSystem.hh"

class G4VSceneHandler;

class G4VRML2File: public G4VGraphicsSystem {
public:
	G4VRML2File(); 
	virtual ~G4VRML2File();
	G4VSceneHandler* CreateSceneHandler(const G4String& name = "");
	G4VViewer*  CreateViewer(G4VSceneHandler&, const G4String& name = "");

};

#endif //G4VRML2File_HH
#endif //G4VIS_BUILD_VRMLFILE_DRIVER

