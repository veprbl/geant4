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
// -*- C++ -*-
// AID-GENERATED
// =========================================================================
// This class was generated by AID - Abstract Interface Definition          
// DO NOT MODIFY, but use the org.freehep.aid.Aid utility to regenerate it. 
// =========================================================================
#ifndef AIDA_IFITRESULT_H
#define AIDA_IFITRESULT_H 1

//  This file is part of the AIDA library
//  Copyright (C) 2002 by the AIDA team.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU Library General Public License described in the LGPL.txt 

#include <string>
#include <vector>

namespace AIDA {

class IFitParameterSettings;
class IFunction;

/** @interface
 *
 * Represents the result of the fit.
 *
 * Holds internal copies of fit parameter settings (set in IFitter) and the
 * fitted function (clone of original function). The value of the parameters
 * of the fitted function are obtained from the fitting.
 *
 * @author The AIDA team (http://aida.freehep.org/)
 *
 */
 
class IFitResult {

public: 
    /// Destructor.
    virtual ~IFitResult() { /* nop */; }

    /// True if fit successful, otherwise false.
    virtual bool isValid() = 0;

    /// Status of the fit. PENDING: what are the suggested values?
    virtual int fitStatus() = 0;

    /// Fitted function. The parameters hold the results of the fit.
    virtual IFunction * fittedFunction() = 0;

    /// Quality of the fit. Depending on the fit method this may be the value
    /// of Chi2 or Maximu Likelihood.
    virtual double quality() = 0;

    /// Number of degrees of freedom.
    virtual int ndf() = 0;

    /// Covariance matrix. PENDING: interface for matrix to be defined.
    virtual double covMatrixElement(int i, int j) = 0;

    /// Fit method name as set in the fitter.
    virtual std::string fitMethodName() = 0;

    /// Engine used to do fitting.
    virtual std::string engineName() = 0;

    /// Textual description of data - this is a hint for the user how to find
    /// the data in the tree if he wanted to repeat the fit later and lost
    /// the pointer to his orignal data object.
    /// PENDING: the contents and format of this string to be defined.
    /// PENDING: XML? stor name and path? ranges of DataPipe?
    virtual std::string dataDescription() = 0;

    /// Vector of constraints.
    virtual const std::vector<std::string>  & constraints() = 0;

    /// Access to applied parameter settings.
    virtual IFitParameterSettings * fitParameterSettings(std::string name) = 0;

    /**
     * Get the array of the fitted parameter values (including fixed and constrained ones).
     * @return The array containing the fitted values.
     *
     */
    virtual const std::vector<double>  & fittedParameters() const = 0;

    /**
     * Get the names of the fitted parameters (including fixed and constrained ones).
     * @return The array containing the names of the fitted parameters.
     *
     */
    virtual const std::vector<std::string>  & fittedParameterNames() const = 0;

    /**
     * Get the value of a fitted parameter by name.
     * @return The value.
     *
     */
    virtual double fittedParameter(std::string & name) = 0;

    /**
     * Get the parabolic errors on the fitted parameters. If a parameter was fixed
     * or constrained the error is zero.
     * @return The parabolic errors on the fitted parameters.
     *
     */
    virtual const std::vector<double>  & errors() const = 0;

    /**
     * Get the positive part of the asymmetric errors on the fitted parameters. If a parameter was fixed
     * or constrained the error is zero.
     * @return The positive part of the asymmetric errors on the fitted parameters.
     *
     */
    virtual const std::vector<double>  & errorsPlus() const = 0;

    /**
     * Get the negative part of the asymmetric errors on the fitted parameters. If a parameter was fixed
     * or constrained the error is zero.
     * @return The negative part of the asymmetric errors on the fitted parameters.
     *
     */
    virtual const std::vector<double>  & errorsMinus() const = 0;

    /** @link aggregation 
     * @supplierRole <<internal copy>>*/
    /*#  IFitParameterSettings lnkIFitParameterProperty; */
}; // class
}; // namespace AIDA
#endif /* ifndef AIDA_IFITRESULT_H */
