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
//
// -------------------------------------------------------------------
//      GEANT 4 class file --- Copyright CERN 1998
//      CERN Geneva Switzerland
//
//
//      File name:     Test30
//
//      Author:        V.Ivanchenko 
// 
//      Creation date: 12 March 2002
//
//      Modifications: 
//
// -------------------------------------------------------------------

#include "globals.hh"
#include "G4ios.hh"
#include "g4std/fstream"
#include "g4std/iomanip"

#include "G4Material.hh"
#include "Test30Material.hh"
#include "Test30Physics.hh"
#include "G4VContinuousDiscreteProcess.hh"
#include "G4ProcessManager.hh"
#include "G4VParticleChange.hh"
#include "G4ParticleChange.hh"

#include "G4LowEnergyIonisation.hh"
#include "G4LowEnergyBremsstrahlung.hh"
#include "G4LowEnergyCompton.hh"
#include "G4LowEnergyGammaConversion.hh"
#include "G4LowEnergyPhotoElectric.hh"
#include "G4LowEnergyRayleigh.hh"
#include "G4hLowEnergyIonisation.hh"

#include "G4VeEnergyLoss.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"
#include "G4MultipleScattering.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleChange.hh"
#include "G4DynamicParticle.hh"
#include "G4AntiProton.hh"
#include "G4Neutron.hh"
#include "G4Proton.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "G4PionZero.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4ForceCondition.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4Step.hh"
#include "G4GRSVolume.hh"

#include "G4UnitsTable.hh"
#include "CLHEP/Hist/TupleManager.h"
#include "CLHEP/Hist/HBookFile.h"
#include "CLHEP/Hist/Histogram.h"
#include "CLHEP/Hist/Tuple.h"


#include "G4Timer.hh"

int main(int argc, char** argv)
{
  HepTupleManager* hbookManager;

  // -------------------------------------------------------------------
  // Setup

  G4String  namePart = "proton";
  G4String  nameMat  = "Si";
  G4String  nameGen  = "stringCHIPS";
  G4bool    usepaw   = false;
  G4int    verbose   = 0;
  G4double energy    = 100.*MeV;
  G4int    nevt      = 1000;
  G4String hFile     = "";
  G4double theStep   = 0.01*micrometer;
  G4double range     = 1.0*micrometer;
  G4Material* material = 0; 
 

  G4cout.setf( ios::scientific, ios::floatfield );


  // -------------------------------------------------------------------
  // Control on input

  if(argc < 2) {
    G4cout << "Input file is not specified! Exit" << G4endl;
    exit(1);
  }

  ifstream* fin = new ifstream();
  string fname = argv[1];
  fin->open(fname.c_str());
  if( !fin->is_open()) {
    G4cout << "Input file <" << fname << "> does not exist! Exit" << G4endl;
    exit(1);
  }

  // -------------------------------------------------------------------
  //--------- Materials definition ---------
 
  Test30Material*  mate = new Test30Material();
  Test30Physics*   phys = new Test30Physics();
	
  // Geometry 

  G4double dimX = 100.0*cm;
  G4double dimY = 100.0*cm;
  G4double dimZ = 100.0*cm;  

  G4Box* sFrame = new G4Box ("Box",dimX, dimY, dimZ);
  G4LogicalVolume* lFrame = new G4LogicalVolume(sFrame,material,"Box",0,0,0);
  G4PVPlacement* pFrame = new G4PVPlacement(0,G4ThreeVector(),"Box",
                                            lFrame,0,false,0);

  // -------------------------------------------------------------------
  // ---- Read input file
  G4cout << "Available commands are: " << G4endl;
  G4cout << "#events" << G4endl;
  G4cout << "#particle" << G4endl;
  G4cout << "#energy(MeV)" << G4endl;
  G4cout << "#range(mm)" << G4endl;
  G4cout << "#step(mm)" << G4endl;
  G4cout << "#material" << G4endl;
  G4cout << "#generator" << G4endl;
  G4cout << "#paw" << G4endl;
  G4cout << "#verbose" << G4endl;
  G4cout << "#run" << G4endl;
  G4cout << "#exit" << G4endl;



  string line, line1;
  G4bool end = true;
  for(G4int run=0; run<100; run++) {
    do {
      (*fin) >> line;
      G4cout << "Next line " << line << G4endl;
      if(line == "#particle") {
        (*fin) >> namePart;
      } else if(line == "#energy(MeV)") {
        (*fin) >> energy;
        energy *= MeV;
      } else if(line == "#events") {
        (*fin) >> nevt;
      } else if(line == "#range(mm)") {
        (*fin) >> range;
        range *= mm;
      } else if(line == "#step(mm)") {
        (*fin) >> theStep;
        theStep *= mm;
      } else if(line == "#material") {
        (*fin) >> nameMat;
      } else if(line == "#particle") {
        (*fin) >> namePart;				
      } else if(line == "#generator") {
        (*fin) >> nameGen;
      } else if(line == "#paw") {
        usepaw = true;
        (*fin) >> hFile;
      } else if(line == "#run") {
        break;
      } else if(line == "#verbose") {
        (*fin) >> verbose;
      } else if(line == "#exit") {
        end = false;
        break;
      }
    } while(end);

    if(!end) break;

    G4cout << "###### Start new run # " << run << "     #####" << G4endl;
 
    material = mate->GetMaterial(nameMat);

		G4ParticleDefinition* part = (G4ParticleTable::GetParticleTable())->FindParticle(namePart);

		G4VProcess* proc = phys->GetProcess(nameGen, namePart, material);
		G4double amass = phys->GetNucleusMass();
				
    const G4ParticleDefinition* proton = G4Proton::Proton();
    const G4ParticleDefinition* neutron = G4Neutron::Neutron();
    const G4ParticleDefinition* pin = G4PionMinus::PionMinus();
    const G4ParticleDefinition* pip = G4PionPlus::PionPlus();
    const G4ParticleDefinition* pi0 = G4PionZero::PionZero();
		
		if(!proc) {
      G4cout << "For particle: " << part->GetParticleName() 
					   << " generator " << nameGen << " is unavailable"
					   << G4endl;			
			exit(1);
		}
		
    G4cout << "The particle: " << part->GetParticleName() << G4endl;
    G4cout << "The material: " << material->GetName() << G4endl;

    G4cout << "The step:     " << theStep/mm << " mm" << G4endl;
 
    // -------------------------------------------------------------------
    // ---- HBOOK initialization

    hbookManager = new HBookFile(hFile, 58);
    assert (hbookManager != 0);
  
    // ---- Book a histogram and ntuples
    G4cout << "Hbook file name: <" 
           << ((HBookFile*) hbookManager)->filename() << ">" << G4endl;
    G4double mass = part->GetPDGMass();
    G4double pmax = sqrt(energy*(energy + 2.0*mass));

    HepHistogram* h[22];

    h[0]=hbookManager->histogram("Number of Secondaries",50,-0.5,49.5);
    h[1]=hbookManager->histogram("Type of Secondaries",10,-0.5,9.5);
    h[2]=hbookManager->histogram("Phi(degrees) of Secondaries",90,-180.0,180.0);
    h[3]=hbookManager->histogram("P (MeV) for protons",100,0.,pmax);
    h[4]=hbookManager->histogram("P (MeV) for pi-",100,0.,pmax);
    h[5]=hbookManager->histogram("P (MeV) for pi+",100,0.,pmax);
    h[6]=hbookManager->histogram("P (MeV) for neutrons",100,0.,pmax);
    h[7]=hbookManager->histogram("Pt (MeV) for protons",100,0.,pmax);
    h[8]=hbookManager->histogram("Pt (MeV) for pi-",100,0.,pmax);
    h[9]=hbookManager->histogram("Pt (MeV) for pi+",100,0.,pmax);
    h[10]=hbookManager->histogram("Pt (MeV) for neutrons",100,0.,pmax);
    h[11]=hbookManager->histogram("E (MeV) for protons",100,0.,energy);
    h[12]=hbookManager->histogram("E (MeV) for pi-",100,0.,energy);
    h[13]=hbookManager->histogram("E (MeV) for pi+",100,0.,energy);
    h[14]=hbookManager->histogram("E (MeV) for neutrons",100,0.,energy);
    h[15]=hbookManager->histogram("delta E (MeV)",20,-1.,1.);
    h[16]=hbookManager->histogram("delta P (GeV)",20,-1.,1.);
    h[17]=hbookManager->histogram("delta Pt (GeV)",20,-1.,1.);
		/*
    h[18]=hbookManager->histogram("pi+pi- inv mass (GeV)",100,0.,2.);
    h[19]=hbookManager->histogram("pi+ p inv mass (GeV)",150,0.,3.);
    h[20]=hbookManager->histogram("pi0 p inv mass (GeV)",150,0.,3.);
    h[21]=hbookManager->histogram("pi- p inv mass (GeV)",150,0.,3.);
*/
		
		    //    assert (hDebug != 0);  
    G4cout<< "Histograms is initialised" << G4endl;

    // Create a DynamicParticle  
  
    G4ParticleMomentum gDir(0.0,0.0,1.0);
    G4DynamicParticle dParticle(part,gDir,energy);

    // Track 
    G4ThreeVector aPosition(0.,0.,0.);
    G4double aTime = 0. ;

    G4Track* gTrack;
    gTrack = new G4Track(&dParticle,aTime,aPosition);
 
    // Step 

    G4Step* step;
    step = new G4Step();  
    step->SetTrack(gTrack);

    G4StepPoint *aPoint, *bPoint;
    aPoint = new G4StepPoint();
    aPoint->SetPosition(aPosition);
    aPoint->SetMaterial(material);
    G4double safety = 10000.*cm;
    aPoint->SetSafety(safety);
    step->SetPreStepPoint(aPoint);

    bPoint = aPoint;
    G4ThreeVector bPosition(0.,0.,theStep);
    bPoint->SetPosition(bPosition);
    step->SetPostStepPoint(bPoint);
    step->SetStepLength(theStep);

    G4Timer* timer = new G4Timer();
    timer->Start();
		const G4DynamicParticle* sec;
		G4ParticleDefinition* pd;
		G4ThreeVector  mom;
		G4LorentzVector labv, fm;
		G4double e, p, m, px, py, pz, pt;
    G4VParticleChange* aChange = 0;
			
    for (G4int iter=0; iter<nevt; iter++) {

      gTrack->SetStep(step); 
      gTrack->SetKineticEnergy(energy); 
      G4double x = 0.0;

			labv = G4LorentzVector(0.0, 0.0, pmax, energy + mass + amass);
      aChange = proc->PostStepDoIt(*gTrack,*step);

      G4double de = aChange->GetLocalEnergyDeposit();
      G4int n = aChange->GetNumberOfSecondaries();
      h[0]->accumulate((float)n,1.0);
			
			if(verbose && de > 0.0) {
        G4cout << " de(MeV) = " << de/MeV << " n= " << n << G4endl;
      }
			 					
      for(G4int i=0; i<n+1; i++) {

				if(i<n) {
					sec = aChange->GetSecondary(i)->GetDynamicParticle();
					pd  = sec->GetDefinition();					
					mom = sec->GetMomentumDirection();
					e   = sec->GetKineticEnergy();

			  } else {
				  if(aChange->GetStatusChange() == fStopAndKill) break;
					pd = part;
					G4ParticleChange* bChange = dynamic_cast<G4ParticleChange*>(aChange);
					mom = *(bChange->GetMomentumDirectionChange());
					e   = bChange->GetEnergyChange();	
				}
				if (e < 0.0) {
					e = 0.0;
				}
				m = pd->GetPDGMass();
				p = sqrt(e*(e + 2.0*m));
			  mom *= p;
				h[2]->accumulate(mom.phi()/degree,1.0);
					
				de += e;
        if(verbose) {
            G4cout << i << "-th secondary  " 
								   << pd->GetParticleName() << "   Ekin(MeV)= "
                   << e/MeV
								   << "   p(MeV)= " << mom/MeV
								   << "   m(MeV)= " << m/MeV
						       << "   Etot(MeV)= " << (e+m)/MeV
                   << G4endl;
        }
				
        m  = pd->GetPDGMass();
        fm = G4LorentzVector(mom, e + m);
        labv -= fm;
        px = mom.x();
        py = mom.y();
        pz = mom.z();
        p  = sqrt(px*px +py*py + pz*pz);
        pt = sqrt(px*px +py*py);

				if(pd == proton) { 
						
          h[1]->accumulate(1.0, 1.0);						
          h[3]->accumulate(p/MeV, 1.0); 
          h[7]->accumulate(pt/MeV, 1.0);
          h[11]->accumulate(e/MeV, 1.0);
		
						/*				
            for( j = i+1; j != mcp->end(); j++ ) {
          mp1 = (*j);
          if(1 == mp1->ParentID()) {
            pd1 = tabl->FindParticle(mp1->ParticlePDGcode());
            if(pd1) {
              fm1 = mp1->FourMomentum();
              mass = fm1.invariantMass(fm)/GeV;
            }
            if(pd1 == pip) {
              h[15]->accumulate(mass);
            } else if(pd1 == pi0) {
              h[16]->accumulate(mass);
            } else if(pd1 == pin) {
              h[17]->accumulate(mass);
            }
          }
					*/


        } else if(pd == pin) {
    
			  	h[1]->accumulate(4.0, 1.0);						
          h[4]->accumulate(p/MeV, 1.0); 
          h[8]->accumulate(pt/MeV, 1.0);
          h[12]->accumulate(e/MeV, 1.0);
		
/*
        for( j = i+1; j != mcp->end(); j++ ) {
          mp1 = (*j);
          if(1 == mp1->ParentID()) {
            pd1 = tabl->FindParticle(mp1->ParticlePDGcode());
            if(pd1) {
              fm1 = mp1->FourMomentum();
              mass = fm1.invariantMass(fm)/GeV;
            }
            if(pd1 == pip) {
              h[14]->accumulate(mass);
            } else if(pd1 == proton) {
              h[17]->accumulate(mass);
            }
          }
        }
*/			
				
        } else if(pd == pip) {
    
					h[1]->accumulate(3.0, 1.0);						
          h[5]->accumulate(p/MeV, 1.0); 
          h[9]->accumulate(pt/MeV, 1.0);
          h[13]->accumulate(e/MeV, 1.0);
		
        } else if(pd == neutron) {
    
					h[1]->accumulate(2.0, 1.0);						
          h[6]->accumulate(p/MeV, 1.0); 
          h[10]->accumulate(pt/MeV, 1.0);
          h[14]->accumulate(e/MeV, 1.0);
				}
			}
				
		  if(verbose > 0) {
        G4cout << "Energy/Momentum balance= " << labv << G4endl;
      }	

		  h[15]->accumulate(labv.e()/MeV, 1.0);
			px = labv.px();	
			py = labv.py();
			pz = labv.pz();							
      p  = sqrt(px*px +py*py + pz*pz);
      pt = sqrt(px*px +py*py);
			h[16]->accumulate(p/GeV, 1.0);
			h[17]->accumulate(pt/GeV, 1.0);
			aChange->Clear();
	
    }

    timer->Stop();
    G4cout << "  "  << *timer << G4endl;
    delete timer;

    if(usepaw)hbookManager->write();
    G4cout << "# hbook is writed" << G4endl;
    delete hbookManager;    
    G4cout << "# hbook is deleted" << G4endl;
    G4cout << "###### End of run # " << run << "     ######" << G4endl;

  } while(end);

	delete mate;
	delete fin;

  G4cout << "###### End of test #####" << G4endl;
}
