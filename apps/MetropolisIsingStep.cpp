/*
 * MetropolisIsingStep.cpp
 *
 *  Created on: 09.09.2016
 *      Author: dschmidt
 */

#include "MetropolisIsingStep.h"

namespace FermiOwn {

MetropolisIsingStep::MetropolisIsingStep( double newBeta, Field<int>& spinField, const IsingHamiltonian& hamilton, std::ranlux48* rndGen ) :
	MetropolisStep( rndGen ),
	spin( spinField ),
	H( hamilton ),
	x_dist( 0, spin.getSize()-1 ),
	new_x( 0 ),
	beta( newBeta )
	{}

MetropolisIsingStep::~MetropolisIsingStep() {
}

void MetropolisIsingStep::propose() {
	new_x = x_dist( *rnd );
}

double MetropolisIsingStep::change() {
	return std::exp( -beta*H.changeAt( new_x ) );
}

void MetropolisIsingStep::accept() {
	spin( new_x ) *= -1;
}

void MetropolisIsingStep::reject() {
}

} /* namespace FermiOwn */
