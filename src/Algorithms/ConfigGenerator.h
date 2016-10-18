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

/**
 * @brief Basic class to run a typical Monte Carlo simulation of Metropolis steps
 *
 * Contains the usual loop of thermalization steps and config generation steps separated by
 * additional intermediate steps to decrease autocorrelation.
 */
class ConfigGenerator {
public:
	/**
	 * @brief Sets up the main parameter of the simulation.
	 *
	 * The total number of updates is given by the number of thermalization steps
	 * plus the number of configurations times the updates per configuration.
	 *
	 * @param numThermal			is the number of steps for themalization
	 * @param numConfs				is the number of configurations on which measurements are performed
	 * @param numUpdatesPerConfig	is the number of intermediate updates
	 * @param step					is the concrete implementation of a Metropolis update
	 * @param measureOnConfig		is a function object that is called to perform measurements on each configuration
	 */
	inline ConfigGenerator( size_t numThermal, size_t numConfs, size_t numUpdatesPerConfig, MetropolisStep* step, voidFun measureOnConfig );

	/**
	 * @brief Empty default destructor, does nothing.
	 */
	virtual ~ConfigGenerator();

	/**
	 * @brief start the simulation with the given number of updates.
	 */
	inline void run();
protected:
	size_t currConf;
private:
	size_t nTher;
	size_t nConf;
	size_t nUpPerConf;
	size_t nTotal;

	size_t onePercent; //TODO: think about data type...

	MetropolisStep* met;
	voidFun measure;
};

ConfigGenerator::ConfigGenerator( size_t numThermal, size_t numConfs, size_t numUpdatesPerConfig, MetropolisStep* step, voidFun measureOnConfig ) :
	currConf(0),
	nTher( numThermal ),
	nConf( numConfs ),
	nUpPerConf( numUpdatesPerConfig ),
	nTotal( nTher + nConf*nUpPerConf ),
	onePercent(nTotal/100),
	met( step ),
	measure( measureOnConfig )
	{}

ConfigGenerator::~ConfigGenerator() {}

void ConfigGenerator::run() {
	for( size_t currTher = 0; currTher < nTher; currTher++ ) {
		met->step();
		if( met->getStepCount()%onePercent == 0 ) {
			std::cout << "\r" << met->getStepCount()/onePercent << "% done " << std::flush;
		}
	}
	for( currConf = 0; currConf < nConf; currConf++ ) {
		for( size_t intermediate = 0; intermediate < nUpPerConf; intermediate++ ) {
			met->step();
			if( met->getStepCount()%onePercent == 0 ) {
				std::cout << "\r" << met->getStepCount()/onePercent << "% done " << std::flush;
			}
		}
		bool more = met->onConfig( currConf );
		measure();
		if( !more ) break;

	}
	std::cout << std::endl;
}


} /* namespace FermiOwn */

#endif /* FERMIOWNCOMMON_SRC_ACTIONS_CONFIGGENERATOR_H_ */
