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
// $Id: G4VisCommandsViewer.hh,v 1.18 2005-10-13 18:06:44 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

// /vis/viewer commands - John Allison  25th October 1998

#ifndef G4VISCOMMANDSVIEWER_HH
#define G4VISCOMMANDSVIEWER_HH

#include "G4VVisCommand.hh"

class G4VViewer;
class G4ViewParameters;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3Vector;

class G4VVisCommandViewer: public G4VVisCommand {
public:
  G4VVisCommandViewer ();
  virtual ~G4VVisCommandViewer ();
protected:
  void SetViewParameters(G4VViewer*, const G4ViewParameters&);
private:
  G4VVisCommandViewer (const G4VVisCommandViewer&);
  G4VVisCommandViewer& operator = (const G4VVisCommandViewer&);
};

class G4VisCommandViewerClear: public G4VVisCommandViewer {
public:
  G4VisCommandViewerClear ();
  virtual ~G4VisCommandViewerClear ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandViewerClear (const G4VisCommandViewerClear&);
  G4VisCommandViewerClear& operator = (const G4VisCommandViewerClear&);
  G4UIcmdWithAString* fpCommand;
};

class G4VisCommandViewerCreate: public G4VVisCommandViewer {
public:
  G4VisCommandViewerCreate ();
  virtual ~G4VisCommandViewerCreate ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandViewerCreate (const G4VisCommandViewerCreate&);
  G4VisCommandViewerCreate& operator = (const G4VisCommandViewerCreate&);
  G4String NextName ();
  G4UIcommand* fpCommand;
  G4int fId;
};

class G4VisCommandViewerDolly: public G4VVisCommandViewer {
public:
  G4VisCommandViewerDolly ();
  virtual ~G4VisCommandViewerDolly ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandViewerDolly (const G4VisCommandViewerDolly&);
  G4VisCommandViewerDolly& operator = (const G4VisCommandViewerDolly&);
  G4UIcmdWithADoubleAndUnit* fpCommandDolly;
  G4UIcmdWithADoubleAndUnit* fpCommandDollyTo;
  G4double fDollyIncrement;
  G4double fDollyTo;
};

class G4VisCommandViewerFlush: public G4VVisCommandViewer {
public:
  G4VisCommandViewerFlush ();
  virtual ~G4VisCommandViewerFlush ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandViewerFlush (const G4VisCommandViewerFlush&);
  G4VisCommandViewerFlush& operator = (const G4VisCommandViewerFlush&);
  G4UIcmdWithAString* fpCommand;
};

class G4VisCommandViewerList: public G4VVisCommandViewer {
public:
  G4VisCommandViewerList ();
  virtual ~G4VisCommandViewerList ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandViewerList (const G4VisCommandViewerList&);
  G4VisCommandViewerList& operator = (const G4VisCommandViewerList&);
  G4UIcommand* fpCommand;
};

class G4VisCommandViewerPan: public G4VVisCommandViewer {
public:
  G4VisCommandViewerPan ();
  virtual ~G4VisCommandViewerPan ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandViewerPan (const G4VisCommandViewerPan&);
  G4VisCommandViewerPan& operator = (const G4VisCommandViewerPan&);
  G4UIcommand* fpCommandPan;
  G4UIcommand* fpCommandPanTo;
  G4double fPanIncrementRight, fPanIncrementUp;
  G4double fPanToRight, fPanToUp;
};

class G4VisCommandViewerRefresh: public G4VVisCommandViewer {
public:
  G4VisCommandViewerRefresh ();
  virtual ~G4VisCommandViewerRefresh ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandViewerRefresh (const G4VisCommandViewerRefresh&);
  G4VisCommandViewerRefresh& operator = (const G4VisCommandViewerRefresh&);
  G4UIcmdWithAString* fpCommand;
};

class G4VisCommandViewerReset: public G4VVisCommandViewer {
public:
  G4VisCommandViewerReset ();
  virtual ~G4VisCommandViewerReset ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandViewerReset (const G4VisCommandViewerReset&);
  G4VisCommandViewerReset& operator = (const G4VisCommandViewerReset&);
  G4UIcmdWithAString* fpCommand;
};

class G4VisCommandViewerScale: public G4VVisCommandViewer {
public:
  G4VisCommandViewerScale ();
  virtual ~G4VisCommandViewerScale ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandViewerScale (const G4VisCommandViewerScale&);
  G4VisCommandViewerScale& operator = (const G4VisCommandViewerScale&);
  G4UIcmdWith3Vector* fpCommandScale;
  G4UIcmdWith3Vector* fpCommandScaleTo;
  G4Vector3D fScaleMultiplier;
  G4Vector3D fScaleTo;
};

class G4VisCommandViewerSelect: public G4VVisCommandViewer {
public:
  G4VisCommandViewerSelect ();
  virtual ~G4VisCommandViewerSelect ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandViewerSelect (const G4VisCommandViewerSelect&);
  G4VisCommandViewerSelect& operator = (const G4VisCommandViewerSelect&);
  G4UIcmdWithAString* fpCommand;
};

class G4VisCommandViewerUpdate: public G4VVisCommandViewer {
public:
  G4VisCommandViewerUpdate ();
  virtual ~G4VisCommandViewerUpdate ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandViewerUpdate (const G4VisCommandViewerUpdate&);
  G4VisCommandViewerUpdate& operator = (const G4VisCommandViewerUpdate&);
  G4UIcmdWithAString* fpCommand;
};

class G4VisCommandViewerZoom: public G4VVisCommandViewer {
public:
  G4VisCommandViewerZoom ();
  virtual ~G4VisCommandViewerZoom ();
  G4String GetCurrentValue (G4UIcommand* command);
  void SetNewValue (G4UIcommand* command, G4String newValue);
private:
  G4VisCommandViewerZoom (const G4VisCommandViewerZoom&);
  G4VisCommandViewerZoom& operator = (const G4VisCommandViewerZoom&);
  G4UIcmdWithADouble* fpCommandZoom;
  G4UIcmdWithADouble* fpCommandZoomTo;
  G4double fZoomMultiplier;
  G4double fZoomTo;
};

#endif
