/*
 * ConfigGenerator.h
 *
 *  Created on: 02.09.2016
 *      Author: dschmidt
 */

#ifndef FERMIOWNCOMMON_SRC_ACTIONS_CONFIGGENERATOR_H_
#define FERMIOWNCOMMON_SRC_ACTIONS_CONFIGGENERATOR_H_

#include "Constants.h"
#include "MetropolisStep.h"

namespace FermiOwn {

class ConfigGenerator {
public:
	inline ConfigGenerator( size_t numThermal, size_t numConfs, size_t numUpdatesPerConfig, MetropolisStep* step, voidFun measureOnConfig );
	virtual ~ConfigGenerator();

	inline void run();
protected:
	size_t currConf;
private:
	size_t nTher;
	size_t nConf;
	size_t nUpPerConf;

	MetropolisStep* met;
	voidFun measure;
};

ConfigGenerator::ConfigGenerator( size_t numThermal, size_t numConfs, size_t numUpdatesPerConfig, MetropolisStep* step, voidFun measureOnConfig ) :
	currConf(0),
	nTher( numThermal ),
	nConf( numConfs ),
	nUpPerConf( numUpdatesPerConfig ),
	met( step ),
	measure( measureOnConfig )
	{}

ConfigGenerator::~ConfigGenerator() {}

void ConfigGenerator::run() {
	for( size_t currTher = 0; currTher < nTher; currTher++ ) {
		met->step();
	}
	for( currConf = 0; currConf < nConf; currConf++ ) {
		for( size_t intermediate = 0; intermediate < nUpPerConf; intermediate++ ) {
			met->step();
		}
		bool more = met->onConfig( currConf );
		measure();
		if( !more ) break;
	}
}


} /* namespace FermiOwn */

#endif /* FERMIOWNCOMMON_SRC_ACTIONS_CONFIGGENERATOR_H_ */
