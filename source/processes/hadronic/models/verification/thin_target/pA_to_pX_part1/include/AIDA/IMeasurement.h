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
#ifndef AIDA_IMEASUREMENT_H
#define AIDA_IMEASUREMENT_H 1

//  This file is part of the AIDA library
//  Copyright (C) 2002 by the AIDA team.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU Library General Public License described in the LGPL.txt 

namespace AIDA {

/**
 * Basic user-level interface class for holding a single "measurement"
 * with positive and negative errors (to allow for asymmetric errors).
 * "IMeasurement" = "value" + "errorPlus" - "errorMinus"
 *
 * @author The AIDA team (http://aida.freehep.org/)
 *
 */
 
class IMeasurement {

public: 
    /// Destructor.
    virtual ~IMeasurement() { /* nop */; }

    /**
     * Get the value of the IMeasurement.
     * @return The value of the IMeasurement.
     *
     */
    virtual double value() const = 0;

    /**
     * Get the plus error of the IMeasurement.
     * @return The plus error.
     *
     */
    virtual double errorPlus() const = 0;

    /**
     * Get the minus error of the IMeasurement.
     * @return The minus error.
     *
     */
    virtual double errorMinus() const = 0;

    /**
     * Set the value of the IMeasurement.
     * @param value The new value of the IMeasurement.
     * @return false If the value cannot be set.
     *
     */
    virtual bool setValue(double value) = 0;

    /**
     * Set the plus error of the IMeasurement.
     * @param value The new plus error of the IMeasurement.
     * @return false If the error cannot be set or it is negative.
     *
     */
    virtual bool setErrorPlus(double errorPlus) = 0;

    /**
     * Set the minus error of the IMeasurement.
     * @param value The new minus error of the IMeasurement.
     * @return false If the error cannot be set or it is negative.
     *
     */
    virtual bool setErrorMinus(double errorMinus) = 0;
}; // class
}; // namespace AIDA
#endif /* ifndef AIDA_IMEASUREMENT_H */
