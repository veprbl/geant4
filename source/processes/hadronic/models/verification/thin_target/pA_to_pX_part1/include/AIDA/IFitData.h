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
#ifndef AIDA_IFITDATA_H
#define AIDA_IFITDATA_H 1

//  This file is part of the AIDA library
//  Copyright (C) 2002 by the AIDA team.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU Library General Public License described in the LGPL.txt 

#include <string>
#include <vector>

namespace AIDA {

class ICloud1D;
class ICloud2D;
class ICloud3D;
class IDataPointSet;
class IEvaluator;
class IHistogram1D;
class IHistogram2D;
class IHistogram3D;
class IProfile1D;
class IProfile2D;
class IRangeSet;
class ITuple;

/** @interface
 *
 * Abstract representation of the fit data. Serves for several purposes:
 *  - decouple fitter from the details of the particular data 
 *    objects (Histograms etc)
 *  - provide range restriction capability ("cut of the tails...")
 *  - creates the image of the virtual data source
 *
 * Design notes:
 *
 * 1)
 * we try to be explicit in the method names which should indicate the
 * dimensionality of the fit data.
 *
 * this way we have more compile-time/run-time safety and clearer code.
 * in C++ we can handle default arguments which otherwise would be ambigeous.
 *
 * 2) 
 * string names of the arguments are at the end of the methods'
 * signatures and have default (empty) values.
 * so if function argument name is missing,
 * assume the ordering is taken from the histogram axis it connects to
 * the name of the pipes then are generated automatically using 
 * the same default scheme as for functions x[1], x[2] etc.
 *
 * 3)
 * There are two ends to pipes: one connected to the
 * data source and one pouring data out. Both their orders has to be specified.
 *
 * For IHistogram, ICloud, IProfile the connections to the data source are
 * ordered as the axis (x=0, y=1, z=2); the user has to provide the order
 * with which the pipes are providing the data. So for example in the constructor
 *   create2DConnection(hist2D,xIndex,yIndex);
 * xIndex specifies the exit order of the pipe connected to the x Axis (and similarly
 * for yIndex). So the default method
 *   create2DConnection(hist2D);
 * corresponds to
 *   create2DConnection(hist2D,0,1);
 * In the methods for IHistograms, IClouds, and IProfiles one index could have
 * been dropped as the second could be inferred by the dimensionality. We
 * preferred being redundant just to avoid confusion in interpreting what
 *   create2DConnection(hist2D,1);
 * really means.
 *
 * For ITuple and IDataPointSet the order in the connection to the data source
 * is specified by the user by providing, respectively, the array of column names (or evaluators)
 * and the array of coordinate indexes to which to connect the pipes.
 * In this case we assume that the pipes are parallel, i.e. the exit order is the same
 * as the one of the connection to the data source.
 * In the createXDConnection methods to IDataPointSets we preferred not to provide
 * defaults for the valIndex to avoid confusion in interpreting it.
 * We can always add it later.
 *
 * PENDING: clarify the relation between data range and normalization range
 *
 * @author The AIDA Team.
 *
 */

class IFitData {

public: 
    /// Destructor.
    virtual ~IFitData() { /* nop */; }

    /**
     * One dimensional connections.
     *
     */

    /**
     * Create a one dimensional connection to an IHistogram1D.
     * @param h       The 1D histogram to which to connect to.
     * @return false If something is wrong.
     *
     */
    virtual bool create1DConnection(const IHistogram1D & h) = 0;

    /**
     * Create a one dimensional connection to an ICloud1D.
     * @param c       The 1D cloud to which to connect to.
     * @return false If something is wrong.
     *
     */
    virtual bool create1DConnection(const ICloud1D & c) = 0;

    /**
     * Create a one dimensional connection to an IProfile1D.
     * @param p       The 1D profile to which to connect to.
     * @return false If something is wrong.
     *
     */
    virtual bool create1DConnection(const IProfile1D & p) = 0;

    /**
     * Create a one dimensional connection to an IDataPointSet.
     * @param dp       The dataPointSet to which to connect to.
     * @param xIndex   The index of the coordinate to which the variable is connected.
     * @param valIndex The index of the coordinate used as "height".
     * @return false If something is wrong.
     *
     */
    virtual bool create1DConnection(const IDataPointSet & dp, int xIndex, int valIndex) = 0;

    /**
     * Two dimensional connections.
     *
     */

    /**
     * Create a two dimensional connection to an IHistogram2D.
     * @param h        The 2D histogram to which to connect to.
     * @return false If something is wrong.
     *
     */
    virtual bool create2DConnection(const IHistogram2D & h) = 0;

    /**
     * Create a two dimensional connection to an IHistogram2D.
     * @param h        The 2D histogram to which to connect to.
     * @param xIndex   The index of the axis to which the x variable is connected.
     * @param yIndex   The index of the axis to which the y variable is connected.
     * @return false If something is wrong.
     *
     */
    virtual bool create2DConnection(const IHistogram2D & h, int xIndex, int yIndex) = 0;

    /**
     * Create a two dimensional connection to an ICloud2D.
     * @param c        The 2D cloud to which to connect to.
     * @return false If something is wrong.
     *
     */
    virtual bool create2DConnection(const ICloud2D & c) = 0;

    /**
     * Create a two dimensional connection to an ICloud2D.
     * @param c        The 2D cloud to which to connect to.
     * @param xIndex   The index of the axis to which the x variable is connected.
     * @param yIndex   The index of the axis to which the y variable is connected.
     * @return false If something is wrong.
     *
     */
    virtual bool create2DConnection(const ICloud2D & c, int xIndex, int yIndex) = 0;

    /**
     * Create a two dimensional connection to an IProfile2D.
     * @param p       The 2D profile to which to connect to.
     * @return false If something is wrong.
     *
     */
    virtual bool create2DConnection(const IProfile2D & p) = 0;

    /**
     * Create a two dimensional connection to an IProfile2D.
     * @param p        The 2D profile to which to connect to.
     * @param xIndex   The index of the axis to which the x variable is connected.
     * @param yIndex   The index of the axis to which the y variable is connected.
     * @return false If something is wrong.
     *
     */
    virtual bool create2DConnection(const IProfile2D & p, int xIndex, int yIndex) = 0;

    /**
     * Create a two dimensional connection to an IDataPointSet.
     * @param dp       The dataPointSet to which to connect to.
     * @param xIndex   The index of the coordinate to which the x variable is connected.
     * @param yIndex   The index of the coordinate to which the y variable is connected.
     * @param valIndex The index of the coordinate used as "height".
     * @return false If something is wrong.
     *
     */
    virtual bool create2DConnection(const IDataPointSet & dp, int xIndex, int yIndex, int valIndex) = 0;

    /**
     * Three dimensional connections.
     *
     */

    /**
     * Create a three dimensional connection to an IHistogram3D.
     * @param h        The 3D histogram to which to connect to.
     * @return false If something is wrong.
     *
     */
    virtual bool create3DConnection(const IHistogram3D & h) = 0;

    /**
     * Create a three dimensional connection to an IHistogram3D.
     * @param h        The 3D histogram to which to connect to.
     * @param xIndex   The index of the axis to which the x variable is connected.
     * @param yIndex   The index of the axis to which the y variable is connected.
     * @param zIndex   The index of the axis to which the z variable is connected.
     * @return false If something is wrong.
     *
     */
    virtual bool create3DConnection(const IHistogram3D & h, int xIndex, int yIndex, int zIndex) = 0;

    /**
     * Create a three dimensional connection to an ICloud3D.
     * @param c        The 3D cloud to which to connect to.
     * @return false If something is wrong.
     *
     */
    virtual bool create3DConnection(const ICloud3D & c) = 0;

    /**
     * Create a three dimensional connection to an ICloud3D.
     * @param c        The 3D cloud to which to connect to.
     * @param xIndex   The index of the axis to which the x variable is connected.
     * @param yIndex   The index of the axis to which the y variable is connected.
     * @param zIndex   The index of the axis to which the z variable is connected.
     * @return false If something is wrong.
     *
     */
    virtual bool create3DConnection(const ICloud3D & c, int xIndex, int yIndex, int zIndex) = 0;

    /**
     * Create a three dimensional connection to an IDataPointSet.
     * @param dp       The dataPointSet to which to connect to.
     * @param xIndex   The index of the coordinate to which the x variable is connected.
     * @param yIndex   The index of the coordinate to which the y variable is connected.
     * @param zIndex   The index of the coordinate to which the z variable is connected.
     * @param valIndex The index of the coordinate used as "height".
     * @return false If something is wrong.
     *
     */
    virtual bool create3DConnection(const IDataPointSet & dp, int xIndex, int yIndex, int zIndex, int valIndex) = 0;

    /**
     * General connections.
     *
     */

    /**
     * Tuples are for UNBINNED fits, so no value column required.
     */

    /** 
     * Create a connection to an ITuple.
     * @param t        The ITuple.
     * @param colNames The names of the columns to which to connect.
     * @return false If something is wrong.
     *
     */
    virtual bool createConnection(const ITuple & t, std::vector<std::string>  colNames) = 0;

    /** 
     * Create a connection to an ITuple.
     * @param t     The ITuple.
     * @param evals The list of the IEvaluator used to calculate the variable's value.
     * @return false If something is wrong.
     *
     */
    virtual bool createConnection(const ITuple & t, std::vector<IEvaluator *>  & evals) = 0;

    /**
     * Create a connection to an IDataPointSet.
     * @param dp       The dataPointSet to which to connect to.
     * @param indeces  The indeces of the coordinates to which the variables are connected.
     * @param valIndex The index of the coordinate used as "height".
     * @return false If something is wrong.
     *
     */
    virtual bool createConnection(const IDataPointSet & dp, std::vector<int>  indeces, int valIndex) = 0;

    /**
     * Remove all Ranges and connections; after this is like newly create.
     *
     */
    virtual void reset() = 0;

    /**
     * The dimension of this IFitData, i.e. the number of connections.
     * @return The dimension of the IFitData.
     *
     */
    virtual int dimension() const = 0;

    /// Return a string describing the data. This should be considered as a
    /// hint to the user where to find the original data objects and how
    /// to connect the data pipes. PENDING: probably this can be part of
    /// XML streaming.
    virtual std::string dataDescription() const = 0;

    /**
     * Get Range of the corresponding connection by index. 
     * The index represent the order in which
     * connections are coming out of the IFitData.
     * @param  The index of the connection.
     * @return      The Range
     *
     */
    virtual IRangeSet & range(int index) const = 0;

    /** @link aggregation */
}; // class
}; // namespace AIDA
#endif /* ifndef AIDA_IFITDATA_H */
