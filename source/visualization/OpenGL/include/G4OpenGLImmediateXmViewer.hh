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
// $Id: G4OpenGLImmediateXmViewer.hh,v 1.6 2001-07-11 10:08:49 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// Andrew Walkden  10th February 1997
// G4OpenGLXmViewer : Class derived from G4OpenGLXViewer, to provide
//                  (Motif) widget OpenGL functionality for GEANT4.

#ifdef G4VIS_BUILD_OPENGLXM_DRIVER

#ifndef G4OPENGLIMMEDIATEXMVIEWER_HH
#define G4OPENGLIMMEDIATEXMVIEWER_HH

#include "G4VViewer.hh"
#include "G4OpenGLImmediateViewer.hh"
#include "G4OpenGLXmViewer.hh"

#include "globals.hh"

class G4OpenGLImmediateSceneHandler;

class G4OpenGLImmediateXmViewer:
public G4OpenGLXmViewer, public G4OpenGLImmediateViewer{
  
public:
  G4OpenGLImmediateXmViewer (G4OpenGLImmediateSceneHandler& scene,
			   const G4String& name = "");
  virtual ~G4OpenGLImmediateXmViewer ();
  void DrawView ();

};

#endif

#endif
