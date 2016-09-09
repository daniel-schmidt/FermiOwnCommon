/*
 * Metropolis.cpp
 *
 *  Created on: 30.08.2016
 *      Author: dschmidt
 */

#include "MetropolisStep.h"

namespace FermiOwn {

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

} /* namespace FermiOwn */
