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

	size_t constexpr Nt = 16;
	size_t constexpr Ns = 16;
	size_t constexpr dim = 2;

	Lattice lat( Nt, Ns, dim );

	size_t constexpr dofPerPoint = 1;
	double constexpr J = 1.;
	double constexpr dT = 0.1;
	double constexpr T_min = 0.5;
	size_t constexpr num_T = 36;

	size_t constexpr numThermal = 500;
	size_t constexpr numConfs = 1000;
	size_t const numUpPerConf = lat.getVol() * 20;

	Timer timer;

	std::ofstream result_file("results/averages.dat");
	result_file << "beta\tavg spin\tavg abs spin\tacceptance" << std::endl;
	
#pragma omp parallel for
	for( size_t n_T = 0; n_T < num_T; n_T++ ) {

		std::ranlux48 rndGen;

		std::uniform_int_distribution<int> spin_dist(0,1);
		std::uniform_int_distribution<int> x_dist(0, lat.getVol()-1 );

		// initialization of physical parameters
		Field<int> spin( lat.getVol(), dofPerPoint, &rndGen, oneInit);

		for( size_t x = 0; x < lat.getVol(); x++ ) {
			if( spin_dist( rndGen) ) {
				spin( x ) *= -1;
			}
		}

		IsingHamiltonian H( lat, spin, J );

		double const T = (T_min + n_T*dT);
		double const beta = 1.0/T;
		MetropolisIsingStep met( beta, spin, H, &rndGen );

		// setting up measurements
		double averageSpin = 0.;
		double avSpinAbs = 0.;

		std::vector<std::pair<size_t, double>> mag_on_config{};
		mag_on_config.reserve(numConfs);
		size_t config_counter = 0;
		auto measure = [&](){
			// average spin on lattice
			int averageSpinOnConf = 0.;
			for(size_t x = 0; x < lat.getVol(); x++ )
				averageSpinOnConf += spin(x);

			double const m = averageSpinOnConf/static_cast<double>(lat.getVol());
			averageSpin += m;
			avSpinAbs += abs(m);
			mag_on_config.emplace_back(config_counter, m);
			config_counter++;
		};

		// running the simulation
		ConfigGenerator confGen( numThermal, numConfs, numUpPerConf, &met, measure );

		timer.start();
		confGen.run();
		timer.stop();

		std::ofstream avSpinOnConfig("results/avSpinOnConfig_" + std::to_string( T ) + ".dat");
		for(auto const & [config_number, mag] : mag_on_config) {
			avSpinOnConfig << config_number << "\t" << mag << "\n";
		}
		avSpinOnConfig.close();

		averageSpin /= numConfs;
		avSpinAbs /= numConfs;

		result_file << T << "\t" << averageSpin << "\t" << avSpinAbs << "\t" << met.getAcceptance() << std::endl;
		timer.printDuration( "Calculation" );
	}
}
