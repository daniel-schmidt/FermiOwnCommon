/*
 * MetropolisIsingStep.h
 *
 *  Created on: 09.09.2016
 *      Author: dschmidt
 */

#ifndef FERMIOWNCOMMON_APPS_METROPOLISISINGSTEP_H_
#define FERMIOWNCOMMON_APPS_METROPOLISISINGSTEP_H_

#include <random>
#include "Field.h"
#include "MetropolisStep.h"
#include "IsingHamiltonian.h"

namespace FermiOwn {

/**
 * @brief Example implementation of a metropolis step.
 *
 * This performs a single update of the Ising model.
 */
class MetropolisIsingStep: public MetropolisStep {
public:
	MetropolisIsingStep( double newBeta, Field<int>& spinField, const IsingHamiltonian& hamilton, std::ranlux48* rndGen );
	virtual ~MetropolisIsingStep();
	inline void setBeta( double newBeta );

protected:
	virtual void propose();
	virtual double change();
	virtual void accept();
	virtual void reject();

	Field<int>& spin;
	const IsingHamiltonian& H;
	std::uniform_int_distribution<int> x_dist;
	size_t new_x;
	double beta;

private:
};

void MetropolisIsingStep::setBeta( double newBeta ) {
	beta = newBeta;
}

} /* namespace FermiOwn */

#endif /* FERMIOWNCOMMON_APPS_METROPOLISISINGSTEP_H_ */
