#ifndef OPENSIM_ACTIVATION_FIBER_LENGTH_MUSCLE_H_
#define OPENSIM_ACTIVATION_FIBER_LENGTH_MUSCLE_H_

// ActivationFiberLengthMuscle.h
// Author: Ajay Seth
/*
 * Copyright (c)  2012, Stanford University. All rights reserved. 
* Use of the OpenSim software in source form is permitted provided that the following
* conditions are met:
* 	1. The software is used only for non-commercial research and education. It may not
*     be used in relation to any commercial activity.
* 	2. The software is not distributed or redistributed.  Software distribution is allowed 
*     only through https://simtk.org/home/opensim.
* 	3. Use of the OpenSim software or derivatives must be acknowledged in all publications,
*      presentations, or documents describing work in which OpenSim or derivatives are used.
* 	4. Credits to developers may not be removed from executables
*     created from modifications of the source.
* 	5. Modifications of source code must retain the above copyright notice, this list of
*     conditions and the following disclaimer. 
* 
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
*  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
*  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
*  SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
*  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*  OR BUSINESS INTERRUPTION) OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
*  WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


// INCLUDE
#include "Muscle.h"

#ifdef SWIG
	#ifdef OSIMSIMULATION_API
		#undef OSIMSIMULATION_API
		#define OSIMSIMULATION_API
	#endif
#endif

namespace OpenSim {

//==============================================================================
//                    ACTIVATION FIBER LENGTH MUSCLE
//==============================================================================
/**
 * A base class representing a two-state muscle-tendon actuator. 
 * It adds activation and fiber-length states and dynamics to the 
 * Muscle class, but does not implement all of the necessary methods,
 * so it is abstract as well. The path information for a muscle is contained
 * in the Muscle class, and the force-generating behavior should be defined in
 * the derived classes.
 *
 * @author Ajay Seth
 *
 * (Based on earlier work by Peter Loan and Frank C. Anderson.)
 */
class OSIMSIMULATION_API ActivationFiberLengthMuscle : public Muscle {
OpenSim_DECLARE_ABSTRACT_OBJECT(ActivationFiberLengthMuscle, Muscle);

public:
//===============================================================================
// PROPERTIES
//===============================================================================
    /** @name Property declarations
    These are the serializable properties associated with this class. **/
    /**@{**/
	OpenSim_DECLARE_PROPERTY(default_activation, double,
		"Assumed activation level if none is assigned.");
	OpenSim_DECLARE_PROPERTY(default_fiber_length, double,
		"Assumed fiber length, unless otherwise assigned.");
    /**@}**/


//==============================================================================
// PUBLIC METHODS
//==============================================================================
	//--------------------------------------------------------------------------
	// CONSTRUCTION
	//--------------------------------------------------------------------------
	ActivationFiberLengthMuscle();

    // Uses default (compiler-generated) destructor, copy constructor, and copy
    // assignment operator.

    //--------------------------------------------------------------------------
    // ActivationFiberLengthMuscle Parameters
    //--------------------------------------------------------------------------
    // Defaults
    double getDefaultActivation() const;
    void setDefaultActivation(double activation);
    double getDefaultFiberLength() const;
    void setDefaultFiberLength(double length);

	//--------------------------------------------------------------------------
    // State Variables
    //--------------------------------------------------------------------------
	void setActivation(SimTK::State& s, double activation) const;
	void setFiberLength(SimTK::State& s, double fiberLength) const;

	virtual Array<std::string> getStateVariableNames() const OVERRIDE_11;
	virtual SimTK::SystemYIndex getStateVariableSystemIndex(const std::string &stateVariableName) const OVERRIDE_11;

	//--------------------------------------------------------------------------
    // State Variable Derivative
    //--------------------------------------------------------------------------
	double getActivationRate(const SimTK::State& s) const;

	//--------------------------------------------------------------------------
	// DEPRECATED 
	//--------------------------------------------------------------------------
	virtual double computeIsokineticForceAssumingInfinitelyStiffTendon(SimTK::State& s, double aActivation) const;
   
	//--------------------------------------------------------------------------
	// SCALING
	//--------------------------------------------------------------------------
protected:
	virtual void postScale(const SimTK::State& s, const ScaleSet& aScaleSet);

	//--------------------------------------------------------------------------
	// FORCE APPLICATION
	//--------------------------------------------------------------------------
	virtual void computeForce(const SimTK::State& state, 
							  SimTK::Vector_<SimTK::SpatialVec>& bodyForces, 
							  SimTK::Vector& generalizedForce) const;

	/** Calculate activation rate */
	virtual double calcActivationRate(const SimTK::State& s) const = 0;
	/** compute initial fiber length (velocity) such that muscle fiber and tendon are 
	    in static equilibrium and update the state */
	virtual void computeInitialFiberEquilibrium(SimTK::State& s) const;

	/** compute the fiber-length state and resulting muscle force that yields
	    zero fiber velocity (isometric) for the given activation */
	virtual double computeIsometricForce(SimTK::State& s, double activation) const =0;


	/** Model Component Interface */
	virtual void addToSystem(SimTK::MultibodySystem& system) const;
	virtual void initStateFromProperties(SimTK::State& s) const;
    virtual void setPropertiesFromState(const SimTK::State& state);
	virtual SimTK::Vector computeStateVariableDerivatives(const SimTK::State& s) const;

	/** State derivative access helper methods */
	void setStateVariableDeriv(const SimTK::State& s, const std::string &aStateName, double aValue) const;
	double getStateVariableDeriv(const SimTK::State& s, const std::string &aStateName) const;

	static const std::string STATE_ACTIVATION_NAME;
	static const std::string STATE_FIBER_LENGTH_NAME;   

private:
	void constructProperties();

//==============================================================================
};	// END of class ActivationFiberLengthMuscle
//==============================================================================
//==============================================================================

} // end of namespace OpenSim

#endif // OPENSIM_ACTIVATION_FIBER_LENGTH_MUSCLE_H_


