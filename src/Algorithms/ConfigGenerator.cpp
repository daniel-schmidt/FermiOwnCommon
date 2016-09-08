/*
 * ConfigGenerator.cpp
 *
 *  Created on: 02.09.2016
 *      Author: dschmidt
 */

#include "ConfigGenerator.h"

namespace FermiOwn {

ConfigGenerator::ConfigGenerator( size_t numThermal, size_t numConfs, size_t numUpdatesPerConfig, voidFun Step, std::function< bool(int) > onConfig ) :
	nTher( numThermal ),
	nConf( numConfs ),
	nUpPerConf( numUpdatesPerConfig ),
	step( Step ),
	onConf( onConfig )
	{}

ConfigGenerator::~ConfigGenerator() {}

void ConfigGenerator::run() {
	for( size_t currTher = 0; currTher < nTher; currTher++ ) {
		step();
	}
	for( size_t currConf = 0; currConf < nConf; currConf++ ) {
		for( size_t intermediate = 0; intermediate < nUpPerConf; intermediate++ ) {
			step();
		}
		bool more = onConf( currConf );
		if( !more ) break;
	}
}

} /* namespace FermiOwn */
