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
// $Id: G4ParallelImportanceManager.hh,v 1.8 2002-05-30 15:57:26 dressel Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class G4ParallelImportanceManager
//
// Class description:
//
// A user should use this class to set up importance sampling
// in a "parallel" geometry.
// The user must create an object of this kind and initialise it.


// Author: Michael Dressel (Michael.Dressel@cern.ch)
// ----------------------------------------------------------------------
#ifndef G4ParallelImportanceManager_hh
#define G4ParallelImportanceManager_hh G4ParallelImportanceManager_hh 

#include "globals.hh"
#include "G4VSampler.hh"

class G4ParallelManager;
class G4VIStore;
class G4VImportanceAlgorithm;
class G4VImportanceSampler;
class G4ParallelImportanceProcess;

class G4ParallelImportanceManager : public G4VSampler
{

public:  // with description

  G4ParallelImportanceManager(G4VIStore &is, 
			      const G4String &particlename,
			      const G4VImportanceAlgorithm *ialg = 0);
    // if *ialg = 0: use the G4ImportanceAlgorithm 
    // use a customised  importance algorithm derived from
    // G4VImportanceAlgorithm  
    // create G4ParallelManager andG4ImportanceSampler

public: // used internally by importance sampling 

  G4ParallelImportanceManager(G4VIStore &is, 
			      G4ParallelManager &pmanager,
			      const G4VImportanceAlgorithm *ialg = 0);
    // create G4ParallelManager and G4ImportanceSampler
  
  ~G4ParallelImportanceManager();
    // delete G4ParallelManager and G4ImportanceSampler
    // and G4ImportanceAgorithm and G4ParallelImportanceProcess
    // if created

public:  // with description

  G4ParallelImportanceProcess *CreateParallelImportanceProcess();
    // create the parallel importance process 
    // don't use it if you use Initialize()

   void Initialize();
    // the G4ParallelImportanceManager has to be initialised after
    // the initialisation of the G4RunManager !
  
private:

  G4ParallelImportanceManager(const G4ParallelImportanceManager &);
  G4ParallelImportanceManager &operator=(const G4ParallelImportanceManager &);

private:

  G4ParallelManager &fParallelManager;
  G4bool fCreatedPM;

  G4bool fDeleteAlg;
  const G4VImportanceAlgorithm *fIalgorithm;

  G4VImportanceSampler *fSampler;
  G4ParallelImportanceProcess *fParallelImportanceProcess;
};

#endif
