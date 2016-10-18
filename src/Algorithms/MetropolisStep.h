/*
 * Metropolis.h
 *
 *  Created on: 30.08.2016
 *      Author: dschmidt
 */

#ifndef FERMIOWNCOMMON_SRC_METROPOLIS_H_
#define FERMIOWNCOMMON_SRC_METROPOLIS_H_

#include <functional>
#include <random>
#include "Constants.h"

namespace FermiOwn {

/**
 * @brief Generic class to perform a single step for a Metropolis algorithm
 *
 * The step is split up into:
 * -# proposing a new configuration
 * -# getting the quotient of new and old probabilities
 * -# accept/reject based on random decision with 2)
 *
 * Functions for each of these steps must be defined in derived classes.
 */
class MetropolisStep {
public:

	/**
	 * @brief Initialize a new MetropolisStep.
	 *
	 * @param rndGen is a pointer to a random number generator to be used for the accept/reject step.
	 */
	inline MetropolisStep( std::ranlux48* rndGen );

	/**
	 * @brief Empty destructor, does nothing.
	 */
	virtual inline ~MetropolisStep();

	/**
	 * @brief Execute a single Metropolis step
	 *
	 * Subsequently calls propose, change and either accept or reject depending on a drawn random number.
	 */
	inline void step();

	/**
	 * @brief Actions to perform on each configuration.
	 *
	 * This can be overwritten to perform evaluations on things relevant to the metropolis step.
	 * The function can return false, if some evaluation leads to the conclusion that
	 * no further metropolis steps are needed. This is needed for Wang-Landa type (density of states)
	 * algorithms.
	 *
	 * If the function is not overwritten, it does nothing and always returns true.
	 *
	 * @param confNum is the consecutive number of the configuration, that was just accepted or rejected
	 * @return returns, if the simulation should continue, defaults to true
	 */
	inline virtual bool onConfig( size_t confNum );

	/**
	 * @brief Get the percentage of accepted configurations.
	 * @return the number of accepted steps divided by the total number of steps made
	 */
	inline double getAcceptance() const;

	/**
	 * @brief Get the total number of steps.
	 * @return The total number of times, step() was called.
	 */
	inline size_t getStepCount() const;
protected:

	 virtual void propose() =0;
	 virtual double change() =0;
	 virtual void accept() =0;
	 virtual void reject() =0;

	 std::ranlux48* rnd;
	 std::uniform_real_distribution<double> uniformDistribution01;

private:
	 size_t stepCounter;
	 size_t acceptanceCounter;
};

/*
 * Inline function definitions
 */

MetropolisStep::MetropolisStep( std::ranlux48* rndGen ) :
		rnd( rndGen ),
		stepCounter( 0 ),
		acceptanceCounter( 0 )
{}

MetropolisStep::~MetropolisStep() {}

void MetropolisStep::step() {
	propose();
	if( uniformDistribution01( *rnd ) < change() ) {
		accept();
		acceptanceCounter++;
	} else {
		reject();
	}
	stepCounter++;
}
bool MetropolisStep::onConfig( size_t confNum ) {
	return true;
}

double MetropolisStep::getAcceptance() const {
	return double(acceptanceCounter)/double(stepCounter);
}

size_t MetropolisStep::getStepCount() const {
	return stepCounter;
}

} /* namespace FermiOwn */

#endif /* FERMIOWNCOMMON_SRC_METROPOLIS_H_ */
