/*
 * MetropolisIsing.cpp
 *
 *  Created on: 30.08.2016
 *      Author: dschmidt
 */

#include <iostream>
#include "Field.h"
#include "Lattice.h"
#include "IsingHamiltonian.h"
#include "ConfigGenerator.h"
#include "MetropolisIsingStep.h"
#include "Timer.h"
#include "ConfigMetropolisIsing.h"

namespace FermiOwn {
} /* namespace FermiOwn */

int main( int argc, char** argv ) {
	using namespace FermiOwn;

	ConfigMetropolisIsing conf;
	conf.writeToFile();
	exit(0);

	size_t Nt = 16;
	size_t Ns = 16;
	size_t dim = 2;

	Lattice lat( Nt, Ns, dim );

	size_t dofPerPoint = 1;
	double J = 1.;
	double dbeta = 0.02;
	double betaMin = atof( argv[1] );

	size_t numThermal = 500;
	size_t numConfs = 1000;
	size_t numUpPerConf = lat.getVol() * 20;

	Timer timer;
#pragma omp parallel for
	for( size_t nbeta = 0; nbeta < 20; nbeta++ ) {

		std::ranlux48 rndGen;

		std::uniform_int_distribution<int> spin_dist(0,1);
		std::uniform_int_distribution<int> x_dist(0, lat.getVol()-1 );

		// initialization of physical parameters
		double beta = betaMin + nbeta*dbeta;

		Field<int> spin( lat.getVol(), dofPerPoint, &rndGen, oneInit);

		for( size_t x = 0; x < lat.getVol(); x++ ) {
			if( spin_dist( rndGen) ) {
				spin( x ) *= -1;
			}
		}

		IsingHamiltonian H( lat, spin, J );

		MetropolisIsingStep met( beta, spin, H, &rndGen );

		// setting up measurements
		std::ofstream avSpinOnConfig("avSpinOnConfig_" + std::to_string( beta ) + ".dat");
		double averageSpin = 0.;
		double avSpinAbs = 0.;

		auto measure = [&](){
			// average spin on lattice
			int averageSpinOnConf = 0.;
			for(size_t x = 0; x < lat.getVol(); x++ )
				averageSpinOnConf += spin(x);

			averageSpin += averageSpinOnConf/double(lat.getVol());
			avSpinAbs += abs( averageSpinOnConf )/double(lat.getVol());
			avSpinOnConfig << averageSpinOnConf/double(lat.getVol()) << std::endl;
		};

		// running the simulation
		ConfigGenerator confGen( numThermal, numConfs, numUpPerConf, &met, measure );

		timer.start();
		confGen.run();
		timer.stop();

		avSpinOnConfig.close();

		averageSpin /= numConfs;
		avSpinAbs /= numConfs;

		std::cout << beta << "\t" << averageSpin << "\t" << avSpinAbs << "\t" << met.getAcceptance() << std::endl;
		timer.printDuration( "Calculation" );
	}
}
