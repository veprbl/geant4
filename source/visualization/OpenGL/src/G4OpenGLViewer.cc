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
// $Id: G4OpenGLViewer.cc,v 1.12 2001-07-11 10:08:56 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// Andrew Walkden  27th March 1996
// OpenGL view - opens window, hard copy, etc.

#ifdef G4VIS_BUILD_OPENGL_DRIVER

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include "G4OpenGLViewer.hh"
#include "G4OpenGLSceneHandler.hh"
#include "G4OpenGLTransform3D.hh"
#include "G4ios.hh"
#include <assert.h>
#include <unistd.h>

#include "G4Scene.hh"
#include "G4VisExtent.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4Point3D.hh"
#include "G4Normal3D.hh"
#include "G4Plane3D.hh"

int G4OpenGLViewer::snglBuf_RGBA[10] =
{ GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, 
  GLX_BLUE_SIZE, 1, GLX_DEPTH_SIZE, 1, None };

int G4OpenGLViewer::dblBuf_RGBA[11] =
{ GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1,
  GLX_BLUE_SIZE, 1, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 1, None };

G4OpenGLViewer::G4OpenGLViewer (G4OpenGLSceneHandler& scene):
G4VViewer (scene, -1),
white_background (False),
transparency_enabled (False),
antialiasing_enabled (False),
haloing_enabled (False),
fSceneHandler (scene)
{
  // Make changes to view parameters for OpenGL...
  fVP.SetAutoRefresh(true);
  fDefaultVP.SetAutoRefresh(true);

  //  glClearColor (0.0, 0.0, 0.0, 0.0);
  //  glClearDepth (1.0);
  //  glDisable (GL_BLEND);
  //  glDisable (GL_LINE_SMOOTH);
  //  glDisable (GL_POLYGON_SMOOTH);

}

G4OpenGLViewer::~G4OpenGLViewer () {}

void G4OpenGLViewer::InitializeGLView () 
{
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glClearDepth (1.0);
  glDisable (GL_BLEND);
  glDisable (GL_LINE_SMOOTH);
  glDisable (GL_POLYGON_SMOOTH);
}  

void G4OpenGLViewer::ClearView () {
  //Below line does not compile with Mesa includes. 
  //glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  glClear (GL_COLOR_BUFFER_BIT);
  glClear (GL_DEPTH_BUFFER_BIT);
  glClear (GL_STENCIL_BUFFER_BIT);
  glFlush ();
}

void G4OpenGLViewer::SetView () {
  
  // Calculates view representation based on extent of object being
  // viewed and (initial) viewpoint.  (Note: it can change later due
  // to user interaction via visualization system's GUI.)
  
  // Lighting.
  GLfloat lightPosition [4];
  lightPosition [0] = fVP.GetActualLightpointDirection().x();
  lightPosition [1] = fVP.GetActualLightpointDirection().y();
  lightPosition [2] = fVP.GetActualLightpointDirection().z();
  lightPosition [3] = 0.;
  // Light position is "true" light direction, so must come after gluLookAt.
  GLfloat ambient [] = { 0.2, 0.2, 0.2, 1.};
  GLfloat diffuse [] = { 0.8, 0.8, 0.8, 1.};
  glEnable (GL_LIGHT0);
  glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
  
  // Get radius of scene, etc.
  // Note that this procedure properly takes into account zoom, dolly and pan.
  const G4Point3D targetPoint
    = fSceneHandler.GetScene()->GetStandardTargetPoint()
    + fVP.GetCurrentTargetPoint ();
  G4double radius = fSceneHandler.GetScene()->GetExtent().GetExtentRadius();
  if(radius<=0.) radius = 1.;
  const G4double cameraDistance = fVP.GetCameraDistance (radius);
  const G4Point3D cameraPosition =
    targetPoint + cameraDistance * fVP.GetViewpointDirection().unit();
  const GLdouble near   = fVP.GetNearDistance (cameraDistance, radius);
  const GLdouble far    = fVP.GetFarDistance  (cameraDistance, near, radius);
  const GLdouble right  = fVP.GetFrontHalfHeight (near, radius);
  const GLdouble left   = -right;
  const GLdouble bottom = left;
  const GLdouble top    = right;
  
  glMatrixMode (GL_PROJECTION); // set up Frustum.
  glLoadIdentity();
  
  if (fVP.GetFieldHalfAngle() == 0.) {
    glOrtho (left, right, bottom, top, near, far);
  }
  else {
    glFrustum (left, right, bottom, top, near, far);
  }
  
  glMatrixMode (GL_MODELVIEW); // apply further transformations to scene.
  glLoadIdentity();
  
  const G4Normal3D& upVector = fVP.GetUpVector ();  
  G4Point3D gltarget;
  if (cameraDistance > 1.e-6 * radius) {
    gltarget = targetPoint;
  }
  else {
    gltarget = targetPoint - radius * fVP.GetViewpointDirection().unit();
  }

  const G4Point3D& pCamera = cameraPosition;  // An alias for brevity.
  gluLookAt (pCamera.x(),  pCamera.y(),  pCamera.z(),       // Viewpoint.
	     gltarget.x(), gltarget.y(), gltarget.z(),      // Target point.
	     upVector.x(), upVector.y(), upVector.z());     // Up vector.
  
  // Light position is "true" light direction, so must come after gluLookAt.
  glLightfv (GL_LIGHT0, GL_POSITION, lightPosition);

  // Clip planes.
  if (fVP.IsSection () ) {  // pair of back to back clip planes.
    const G4Plane3D& s = fVP.GetSectionPlane ();
    double sArray[4];
    sArray[0] = s.a();
    sArray[1] = s.b();
    sArray[2] = s.c();
    sArray[3] = s.d() + radius * 1.e-05;
    glClipPlane (GL_CLIP_PLANE0, sArray);
    glEnable (GL_CLIP_PLANE0);
    sArray[0] = -s.a();
    sArray[1] = -s.b();
    sArray[2] = -s.c();
    sArray[3] = -s.d() + radius * 1.e-05;
    glClipPlane (GL_CLIP_PLANE1, sArray);
    glEnable (GL_CLIP_PLANE1);
  }
  else {
    glDisable (GL_CLIP_PLANE0);
    glDisable (GL_CLIP_PLANE1);
  }
  
}

void G4OpenGLViewer::HaloingFirstPass () {
  
  //To perform haloing, first Draw all information to the depth buffer
  //alone, using a chunky line width, and then Draw all info again, to
  //the colour buffer, setting a thinner line width an the depth testing 
  //function to less than or equal, so if two lines cross, the one 
  //passing behind the other will not pass the depth test, and so not
  //get rendered either side of the infront line for a short distance.

  //First, disable writing to the colo(u)r buffer...
  glColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

  //Now enable writing to the depth buffer...
  glDepthMask (GL_TRUE);
  glDepthFunc (GL_LESS);
  glClearDepth (1.0);

  //Finally, set the line width to something wide...
  glLineWidth (3.0);

}

void G4OpenGLViewer::HaloingSecondPass () {

  //And finally, turn the colour buffer back on with a sesible line width...
  glColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthFunc (GL_LEQUAL);
  glLineWidth (1.0);

}

void G4OpenGLViewer::HLRFirstPass () {

  G4cout << "First pass HLR" << G4endl;

  //Hidden line drawing requires three renderings to different buffers, so it
  //cannot be treated in G4OpenGLSceneHandler alone (as can wireframe or hidden 
  //surface (solid) mode)

  //So, after SIGGRAPH97

  //1) Disable the colour and depth buffers, and Draw polygons in wireframe 
  //   mode to the stencil buffer, setting stencil value to 1 where pixels go.

  //First, disable writing to the colo(u)r buffer...
  glColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

  //Enable the stencil buffer...
  glEnable (GL_STENCIL_TEST);

  //Now disable writing to the depth buffer...
  glDisable (GL_DEPTH_TEST);

  //Clear all stencil buffer values to 0...
  glClearStencil (0);
  glClear (GL_STENCIL_BUFFER_BIT);

  //Prepare stencil buffer...
  glStencilFunc (GL_ALWAYS, //When to pass a pixel in the stencil test
		 0x1,       //The ref val to compare with masked stencil val
		 0x1);      //The mask to AND with ref and val before test

  glStencilOp (GL_KEEP,     //If stencil test fails *irrelevant*
	       GL_KEEP,     //If depth test passes *irrelevant*
	       GL_REPLACE); //If no depth test *this is applied to every pixel
                            //drawn to the stencil buffer in this pass*

  //Hence, everywhere a pixel is drawn, stencil value=1, 0 everywhere else.

  //Set the drawing style to wireframe...
  fVP.SetDrawingStyle (G4ViewParameters::wireframe);

  NeedKernelVisit ();
  ProcessView ();
}

void G4OpenGLViewer::HLRSecondPass () {

  G4cout << "Second pass HLR" << G4endl;

  //2) Use the stencil buffer to mask out pixels where stencil value = 1, and
  //   render to the stencil buffer as depth tested filled polygons.

  glStencilFunc (GL_EQUAL, 0x1, 0x1);
  glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
  glDepthFunc (GL_LEQUAL);
  glEnable (GL_DEPTH_TEST);

  //Set the drawing style to hlhsr (solid)...
  fVP.SetDrawingStyle (G4ViewParameters::hsr);
  
  NeedKernelVisit ();
  ProcessView ();

}

void G4OpenGLViewer::HLRThirdPass () {
  
  //3) Turn off the stencil buffer, turn on the colour buffer
  //   and render polygons in wireframe mode.

  G4cout << "Third pass HLR" << G4endl;

  glDisable (GL_STENCIL_TEST);
  glColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

  //Set the drawing style to  hlr wireframe...
  fVP.SetDrawingStyle (G4ViewParameters::wireframe);

  NeedKernelVisit ();
  ProcessView ();

  fVP.SetDrawingStyle (G4ViewParameters::hlr);
}

#endif
