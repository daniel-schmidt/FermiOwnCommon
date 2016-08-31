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

namespace FermiOwn {

typedef std::function< void() > voidFun;
typedef std::function< double() > doubleFun;


/**
 * @brief Generic class to perform a single step for a Metropolis algorithm
 *
 * The step is split up into:
 * -# proposing a new configuration
 * -# getting the quotient of new and old probabilities
 * -# accept/reject based on random decision with 2)
 *
 * Functions for each of these steps must be passed to this class at construction.
 */
class MetropolisStep {
public:

	/**
	 * @brief Initialize a new MetropolisStep by defining the functions to execute at each substep.
	 *
	 * @param proposeNew		function, that alters the current configuration and generates a new proposition, must not have arguments or return value
	 * @param calculateChange	function, that returns the change in probability p_new / p_old resulting from the change by proposeNew
	 * @param onAccept			function to execute, if the proposed configuration is accepted, i.e. keeping it
	 * @param onReject			function to execute, if the proposed configuration is not accepted, i.e. resetting the memory to the old configuration
	 * @param rndGen			pointer to a random number generator to be used.
	 */
	MetropolisStep( voidFun proposeNew, doubleFun calculateChange, voidFun onAccept, voidFun onReject, std::ranlux48* rndGen );

	virtual ~MetropolisStep();

	/**
	 * @brief Execute a single Metropolis step
	 *
	 * Subsequently calls propose, change and either accept or reject depending on a drawn random number.
	 */
	void step();

	/**
	 * @brief Get the percentage of accepted configurations.
	 * @return the number of accepted steps divided by the total number of steps made
	 */
	inline double getAcceptance() const;

private:
	 voidFun propose;
	 doubleFun change;
	 voidFun accept;
	 voidFun reject;

	 std::ranlux48* rnd;
	 std::uniform_real_distribution<double> uniformDistribution01;

	 size_t stepCounter;
	 size_t acceptanceCounter;
};

inline double MetropolisStep::getAcceptance() const {
	return double(acceptanceCounter)/double(stepCounter);
}

} /* namespace FermiOwn */

#endif /* FERMIOWNCOMMON_SRC_METROPOLIS_H_ */
