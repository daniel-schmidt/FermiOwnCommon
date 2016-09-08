/*
 * MetropolisIsing.cpp
 *
 *  Created on: 30.08.2016
 *      Author: dschmidt
 */

#include <iostream>
#include "MetropolisStep.h"
#include "Field.h"
#include "Lattice.h"
#include "IsingHamiltonian.h"
#include "ConfigGenerator.h"

namespace FermiOwn {

} /* namespace FermiOwn */

int main( int argc, char** argv ) {
	using namespace FermiOwn;
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


#pragma omp parallel for
	for( size_t nbeta = 0; nbeta < 20; nbeta++ ) {

		std::ranlux48 rndGen;

		std::uniform_int_distribution<int> spin_dist(0,1);
		std::uniform_int_distribution<int> x_dist(0, lat.getVol()-1 );

		int new_x = 0;

		// initialization of physical parameters
		double beta = betaMin + nbeta*dbeta;

		Field<int> spin( lat.getVol(), dofPerPoint, &rndGen, oneInit);

		for( size_t x = 0; x < lat.getVol(); x++ ) {
			if( spin_dist( rndGen) ) {
				spin( x ) *= -1;
			}
		}

		IsingHamiltonian H( lat, spin, J );

		// metropolis initialization
		auto propose = [&] () {
			new_x = x_dist( rndGen );
		};
		auto change = [&]() {
			return std::exp( -beta*H.changeAt( new_x ) );
		};
		auto accept = [&]() {
			spin( new_x ) *= -1;	// flip spin to accept config.
		};
		auto reject = [&]() {

		};
		MetropolisStep met( propose, change, accept, reject, &rndGen );

		// thermalization
//		for( size_t nthermal = 0; nthermal < numThermal; nthermal++ ) {
//			met.step();
//		}

		// generating configurations
		std::ofstream avSpinOnConfig("avSpinOnConfig_" + std::to_string( beta ) + ".dat");
		double averageSpin = 0.;
		double avSpinAbs = 0.;
//		for( size_t conf = 0; conf < numConfs; conf++ ) {
//			for( size_t intermediate = 0; intermediate < 20*lat.getVol(); intermediate++ )
//				met.step();
//
//			// average spin on lattice
//			int averageSpinOnConf = 0.;
//			for(size_t x = 0; x < lat.getVol(); x++ )
//				averageSpinOnConf += spin(x);
//
//			averageSpin += averageSpinOnConf/double(lat.getVol());
//			avSpinAbs += abs( averageSpinOnConf )/double(lat.getVol());
//			avSpinOnConfig << averageSpinOnConf/double(lat.getVol()) << std::endl;
//		}

		auto step = [&](){ met.step(); };
		auto onConfig = [&](int confNum){
			// average spin on lattice
			int averageSpinOnConf = 0.;
			for(size_t x = 0; x < lat.getVol(); x++ )
				averageSpinOnConf += spin(x);

			averageSpin += averageSpinOnConf/double(lat.getVol());
			avSpinAbs += abs( averageSpinOnConf )/double(lat.getVol());
			avSpinOnConfig << averageSpinOnConf/double(lat.getVol()) << std::endl;
			return true;
		};
		ConfigGenerator confGen( numThermal, numConfs, numUpPerConf, step, onConfig );
		confGen.run();

		avSpinOnConfig.close();

		averageSpin /= numConfs;
		avSpinAbs /= numConfs;

		std::cout << beta << "\t" << averageSpin << "\t" << avSpinAbs << "\t" << met.getAcceptance() << std::endl;
	}
}
