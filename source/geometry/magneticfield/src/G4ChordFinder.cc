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
// $Id: G4ChordFinder.cc,v 1.40 2003-11-05 10:45:57 japost Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
// 25.02.97 John Apostolakis,  design and implimentation 
// 05.03.97 V. Grichine , style modification
// -------------------------------------------------------------------

#include "G4ChordFinder.hh"
#include "G4MagneticField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4ClassicalRK4.hh"

#include <iomanip>

// ..........................................................................

G4ChordFinder::G4ChordFinder(G4MagInt_Driver* pIntegrationDriver)
  : fDefaultDeltaChord( 3.00 * mm),     // Change to 0.25 * mm ? 
    fDeltaChord( fDefaultDeltaChord ),
    fAllocatedStepper(false),
    fEquation(0), 
    fDriversStepper(0), 
    fNoTrials(0), fNoCalls(0)
{
  // Simple constructor which does not create equation, ..
      // fDeltaChord= fDefaultDeltaChord; 
  fIntgrDriver= pIntegrationDriver;
  fAllocatedStepper= false;
  fLastStepEstimate_Unconstrained = DBL_MAX;          // Should move q, p to
}

// ..........................................................................

G4ChordFinder::G4ChordFinder( G4MagneticField*        theMagField,
                              G4double                stepMinimum, 
                              G4MagIntegratorStepper* pItsStepper )
  : fDefaultDeltaChord( 3.00 * mm),     // Change to 0.25 * mm ? 
    fDeltaChord( fDefaultDeltaChord ),
    fAllocatedStepper(false),
    fEquation(0), 
    fDriversStepper(0), 
    fNoTrials(0), fNoCalls(0)
{
  //  Construct the Chord Finder
  //  by creating in inverse order the  Driver, the Stepper and EqRhs ...
  G4Mag_EqRhs *pEquation = new G4Mag_UsualEqRhs(theMagField);
  fEquation = pEquation;                            
  fLastStepEstimate_Unconstrained = DBL_MAX;          // Should move q, p to
                                                      //    G4FieldTrack ??
  // --->>  Charge    Q = 0 
  // --->>  Momentum  P = 1       NOMINAL VALUES !!!!!!!!!!!!!!!!!!

  if( pItsStepper == 0 )
  { 
     pItsStepper = fDriversStepper = new G4ClassicalRK4(pEquation);
     fAllocatedStepper= true;
  }
  else
  {
     fAllocatedStepper= false; 
  }
  fIntgrDriver = new G4MagInt_Driver(stepMinimum, pItsStepper, 
                                     pItsStepper->GetNumberOfVariables() );
}

// ......................................................................

G4ChordFinder::~G4ChordFinder()
{
  delete   fEquation; // fIntgrDriver->pIntStepper->theEquation_Rhs;
  if( fAllocatedStepper)
  { 
     delete fDriversStepper; 
  }                                //  fIntgrDriver->pIntStepper;}
  delete   fIntgrDriver; 
}

// ......................................................................

G4double 
G4ChordFinder::AdvanceChordLimited( G4FieldTrack& yCurrent,
                                    G4double      stepMax,
                                    G4double      epsStep )
{
  G4double stepPossible;
  G4double dyErr;
  G4FieldTrack yEnd( yCurrent);
  G4double  startCurveLen= yCurrent.GetCurveLength();

  G4double nextStep;
  //            *************
  stepPossible= FindNextChord(yCurrent, stepMax, yEnd, dyErr, epsStep, &nextStep);
  //            *************
  G4bool good_advance;
  if ( dyErr < epsStep * stepPossible )
  {
     // Accept this accuracy.
     yCurrent = yEnd;
     good_advance = true; 
  }
  else
  {
     // Advance more accurately to "end of chord"
     //                           ***************
     good_advance = fIntgrDriver->AccurateAdvance(yCurrent, stepPossible, epsStep, nextStep);
     //                           ***************
     if ( ! good_advance ){ 
       // In this case the driver could not do the full distance
       stepPossible= yCurrent.GetCurveLength()-startCurveLen;
     }
  }

#ifdef G4DEBUG_FIELD
  G4cout << "Exiting FindNextChord Limited with:" << G4endl
         << "   yCurrent: " << yCurrent<< G4endl; 
#endif

  return stepPossible;
}

// #define  TEST_CHORD_PRINT  1

// ............................................................................

G4double
G4ChordFinder::FindNextChord( const  G4FieldTrack  yStart,
                                     G4double     stepMax,
                                     G4FieldTrack&   yEnd, // Endpoint
                                     G4double&   dyErrPos, // Error of endpoint
                                     G4double    epsStep,
			             G4double*  pStepForAccuracy)
  // Returns Length of Step taken
{
  // G4int       stepRKnumber=0;
  G4FieldTrack yCurrent=  yStart;  
  G4double    stepTrial, stepForAccuracy;
  G4double    dydx[G4FieldTrack::ncompSVEC]; 

  //  1.)  Try to "leap" to end of interval
  //  2.)  Evaluate if resulting chord gives d_chord that is good enough.
  //     2a.)  If d_chord is not good enough, find one that is.
  
  G4bool    validEndPoint= false;
  G4double  dChordStep, lastStepLength; //  stepOfLastGoodChord;

  fIntgrDriver-> GetDerivatives( yCurrent, dydx )  ;

  G4int     noTrials=0;
  const  G4double safetyFactor= (1-perThousand);  // TO-DO:  = 0.975 or 0.95;

  stepTrial = std::min( stepMax, 
                          safetyFactor * fLastStepEstimate_Unconstrained );

  G4double newStepEst_Uncons= 0.0; 
  do
  { 
     G4double stepForChord;  
     yCurrent = yStart;    // Always start from initial point

     //            ************
     fIntgrDriver->QuickAdvance( yCurrent, dydx, stepTrial, 
				 dChordStep, dyErrPos);
     //            ************

     // We check whether the criterion is met here.
     validEndPoint = AcceptableMissDist(dChordStep); 
                      //  && (dyErrPos < eps) ;

     lastStepLength = stepTrial; 

     // This method estimates to step size for a good chord.
     stepForChord = NewStep(stepTrial, dChordStep, newStepEst_Uncons );

     if( ! validEndPoint ) {
        if( (stepForChord <= lastStepLength) || (stepTrial<=0.0) )
          stepTrial = stepForChord;
        else
          stepTrial *= 0.1;

        // if(dbg) G4cerr<<"Dchord too big. Try new hstep="<<stepTrial<<G4endl;
     }
     // #ifdef  TEST_CHORD_PRINT
     // TestChordPrint( noTrials, lastStepLength, dChordStep, stepTrial );
     // #endif

     noTrials++; 
  }
  while( ! validEndPoint );   // End of do-while  RKD 

  if( newStepEst_Uncons > 0.0  ){ 
     fLastStepEstimate_Unconstrained= newStepEst_Uncons;
  }

  // stepOfLastGoodChord = stepTrial;

  if( pStepForAccuracy ){ 
     // Calculate the step size required for accuracy, if it is needed
     G4double dyErr_relative = dyErrPos/(epsStep*lastStepLength);
     if( dyErr_relative > 1.0 ) {
        stepForAccuracy =
	   fIntgrDriver->ComputeNewStepSize( dyErr_relative,
					     lastStepLength );
     }else{
        stepForAccuracy = 0.0;   // Convention to show step was ok 
     }
     *pStepForAccuracy = stepForAccuracy;
  }

#ifdef  TEST_CHORD_PRINT
  static int dbg=0;
  if( dbg ) 
    G4cout << "ChordF/FindNextChord:  NoTrials= " << noTrials 
           << " StepForGoodChord=" << std::setw(10) << stepTrial << G4endl;
#endif

  yEnd=  yCurrent;  
  return stepTrial; 
}

// ----------------------------------------------------------------------------
#if 0          
//   #ifdef G4VERBOSE
if( dbg ) {
   G4cerr << "Returned from QuickAdvance with: yCur=" << yCurrent <<G4endl;
   G4cerr << " dChordStep= "<< dChordStep <<" dyErr=" << dyErr << G4endl; 
}
#endif
// ----------------------------------------------------------------------------

// ...........................................................................

G4double G4ChordFinder::NewStep(G4double  stepTrialOld, 
                                G4double  dChordStep, // Curr. dchord achieved
                                G4double& stepEstimate_Unconstrained )  
//
// Is called to estimate the next step size, even for successful steps,
// in order to predict an accurate 'chord-sensitive' first step
// which is likely to assist in more performant 'stepping'.
//
       
{
  G4double stepTrial;
  static G4double lastStepTrial = 1.,  lastDchordStep= 1.;

#if 1 
  // const G4double  threshold = 1.21, multiplier = 0.9;
  //  0.9 < 1 / sqrt(1.21)

  if (dChordStep > 0.0)
  {
    stepEstimate_Unconstrained = stepTrialOld*sqrt( fDeltaChord / dChordStep );
    stepTrial =  0.98 * stepEstimate_Unconstrained;
  }
  else
  {
    // Should not update the Unconstrained Step estimate: incorrect!
    stepTrial =  stepTrialOld * 2.; 
  }

  // if ( dChordStep < threshold * fDeltaChord ){
  //    stepTrial= stepTrialOld *  multiplier;    
  // }
  if( stepTrial <= 0.001 * stepTrialOld)
  {
     if ( dChordStep > 1000.0 * fDeltaChord ){
        stepTrial= stepTrialOld * 0.03;   
     }else{
        if ( dChordStep > 100. * fDeltaChord ){
          stepTrial= stepTrialOld * 0.1;   
        }else{
    // Try halving the length until dChordStep OK
          stepTrial= stepTrialOld * 0.5;   
        }
     }
  }else if (stepTrial > 1000.0 * stepTrialOld)
  {
     stepTrial= 1000.0 * stepTrialOld;
  }

  if( stepTrial == 0.0 ){
     stepTrial= 0.000001;
  }

  lastStepTrial = stepTrialOld; 
  lastDchordStep= dChordStep;
#else
  if ( dChordStep > 1000. * fDeltaChord ){
        stepTrial= stepTrialOld * 0.03;   
  }else{
     if ( dChordStep > 100. * fDeltaChord ){
        stepTrial= stepTrialOld * 0.1;   
     }else{
        // Keep halving the length until dChordStep OK
        stepTrial= stepTrialOld * 0.5;   
     }
  }
#endif 

  // A more sophisticated chord-finder could figure out a better
  //   stepTrial, from dChordStep and the required d_geometry
  //   eg
  //      Calculate R, r_helix (eg at orig point)
  //      if( stepTrial < 2 pi  R )
  //          stepTrial = R arc_cos( 1 - fDeltaChord / r_helix )
  //      else    
  //          ??

  return stepTrial;
}

//
//  Given a starting curve point A (CurveA_PointVelocity),  a later 
//  curve point B (CurveB_PointVelocity) and a point E which is (generally)
//  not on the curve, find and return a point F which is on the curve and 
//  which is close to E. While advancing towards F utilise eps_step 
//  as a measure of the relative accuracy of each Step.
  
G4FieldTrack
G4ChordFinder::ApproxCurvePointV( const G4FieldTrack& CurveA_PointVelocity, 
                                  const G4FieldTrack& CurveB_PointVelocity, 
                                  const G4ThreeVector& CurrentE_Point,
                                        G4double eps_step)
{
  // 1st implementation:
  //    if r=|AE|/|AB|, and s=true path lenght (AB)
  //    return the point that is r*s along the curve!

  G4FieldTrack    Current_PointVelocity= CurveA_PointVelocity; 

  G4ThreeVector  CurveA_Point= CurveA_PointVelocity.GetPosition();
  G4ThreeVector  CurveB_Point= CurveB_PointVelocity.GetPosition();

  G4ThreeVector  ChordAB_Vector= CurveB_Point   - CurveA_Point;
  G4ThreeVector  ChordAE_Vector= CurrentE_Point - CurveA_Point;

  G4double       ABdist= ChordAB_Vector.mag();
  G4double  curve_length;  //  A curve length  of AB
  G4double  AE_fraction; 
  
  curve_length= CurveB_PointVelocity.GetCurveLength()
              - CurveA_PointVelocity.GetCurveLength();  

  // const 
  G4double  integrationInaccuracyLimit= std::max( perMillion, 0.5*eps_step ); 
  if( curve_length < ABdist * (1. - integrationInaccuracyLimit) ){ 
#ifdef G4DEBUG_FIELD
    G4cerr << " Warning in G4ChordFinder::ApproxCurvePoint: "
           << G4endl
           << " The two points are further apart than the curve length "
           << G4endl
           << " Dist = "         << ABdist
           << " curve length = " << curve_length 
           << " relativeDiff = " << (curve_length-ABdist)/ABdist 
           << G4endl;
    if( curve_length < ABdist * (1. - 10*eps_step) ) {
      G4cerr << " ERROR: the size of the above difference"
             << " exceeds allowed limits.  Aborting." << G4endl;
      G4Exception("G4ChordFinder::ApproxCurvePointV()", "PrecisionError",
                  FatalException, "Unphysical curve length.");
    }
#endif
    // Take default corrective action: 
    //    -->  adjust the maximum curve length. 
    //  NOTE: this case only happens for relatively straight paths.
    curve_length = ABdist; 
  }

  G4double  new_st_length; 

  if ( ABdist > 0.0 ){
     AE_fraction = ChordAE_Vector.mag() / ABdist;
  }else{
     AE_fraction = 0.5;                         // Guess .. ?; 
#ifdef G4DEBUG_FIELD
     G4cout << "Warning in G4ChordFinder::ApproxCurvePoint:"
            << " A and B are the same point!" << G4endl
            << " Chord AB length = " << ChordAE_Vector.mag() << G4endl
            << G4endl;
#endif
  }
  
  if( (AE_fraction> 1.0 + perMillion) || (AE_fraction< 0.) ){
#ifdef G4DEBUG_FIELD
    G4cerr << " G4ChordFinder::ApproxCurvePointV - Warning:"
           << " Anomalous condition:AE > AB or AE/AB <= 0 " << G4endl
           << "   AE_fraction = " <<  AE_fraction << G4endl
           << "   Chord AE length = " << ChordAE_Vector.mag() << G4endl
           << "   Chord AB length = " << ABdist << G4endl << G4endl;
    G4cerr << " OK if this condition occurs after a recalculation of 'B'"
           << G4endl << " Otherwise it is an error. " << G4endl ; 
#endif
     // This course can now result if B has been re-evaluated, 
     //   without E being recomputed   (1 July 99)
     //  In this case this is not a "real error" - but it undesired
     //   and we cope with it by a default corrective action ...
     AE_fraction = 0.5;                         // Default value
  }

  new_st_length= AE_fraction * curve_length; 

  G4bool good_advance;
  if ( AE_fraction > 0.0 ) { 
     good_advance = 
      fIntgrDriver->AccurateAdvance(Current_PointVelocity, 
                                    new_st_length,
                                    eps_step ); // Relative accuracy
     // In this case it does not matter if it cannot advance the full distance
  }

  // If there was a memory of the step_length actually require at the start 
  // of the integration Step, this could be re-used ...

  return Current_PointVelocity;
}

void
G4ChordFinder::TestChordPrint( G4int    noTrials, 
			       G4int    lastStepTrial, 
			       G4double dChordStep, 
			       G4double nextStepTrial )
{
     G4int oldprec= G4cout.precision(5);
     G4cout << " ChF/fnc: notrial " << std::setw( 3) << noTrials 
            << " this_step= "       << std::setw(10) << lastStepTrial;
     if( fabs( (dChordStep / fDeltaChord) - 1.0 ) < 0.001 ){
             G4cout.precision(8);
     }else{  G4cout.precision(6); }
     G4cout << " dChordStep=  "     << std::setw(12) << dChordStep;
     if( dChordStep > fDeltaChord ) { G4cout << " d+"; }
     else                           { G4cout << " d-"; }
     G4cout.precision(5);
     G4cout <<  " new_step= "       << std::setw(10)
            << fLastStepEstimate_Unconstrained
            << " new_step_constr= " << std::setw(10)
            << lastStepTrial << G4endl;
     G4cout << " nextStepTrial = " << std::setw(10) << nextStepTrial << G4endl;
     G4cout.precision(oldprec);
}
