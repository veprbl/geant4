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
// $Id: _g4xvtgu.h,v 1.3 2001-07-11 09:56:52 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
/* 
	This is version 4.5 of XVT-Architect.
	This file was automatically generated by XVT-Architect,
	Do not modify its contents.
*/

#ifndef _g4xvtgu_factory_h
#define _g4xvtgu_factory_h

#include "facdec.h"


// Factory IDs:

#ifndef G4XvtGUI1002
#define G4XvtGUI1002 1002
#endif 

// Object Data Members:

	
class G4XvtGUI1002Data : public CDataMembers
{
		PWRClassInfo
		virtual void Initialize(PAFactory* theFactory);

	public:

		CTaskDoc* itsCTaskDoc1003;
		G4XvtDoc* itsG4Doc;
};

#define PAFACTORY_APP_ABOUT_BOX 0
#define PAFACTORY_APP_BASE_NAME "PwrApp"
#define PAFACTORY_APP_NAME "XVT - Geant 4"
#define PAFACTORY_APP_MENU MENU_BAR_RID
#define PAFACTORY_APP_TITLE "PwrApp"

#endif

