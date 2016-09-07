/*
 * Metropolis.cpp
 *
 *  Created on: 30.08.2016
 *      Author: dschmidt
 */

#include "MetropolisStep.h"

namespace FermiOwn {

MetropolisStep::MetropolisStep( voidFun proposeNew, doubleFun calculateChange, voidFun onAccept, voidFun onReject, std::ranlux48* rndGen ) :
		propose( proposeNew ),
		change( calculateChange ),
		accept( onAccept ),
		reject( onReject ),
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
