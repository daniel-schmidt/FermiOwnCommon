/*
 * MetropolisIsing.cpp
 *
 *  Created on: 30.08.2016
 *      Author: dschmidt
 */

#include "MetropolisStep.h"
#include "Field.h"
#include "Lattice.h"
#include "IsingHamiltonian.h"

namespace FermiOwn {

} /* namespace FermiOwn */

int main( int argc, char** argv ) {
	using namespace FermiOwn;

	std::ranlux48 rndGen;
	std::uniform_int_distribution<int> spin_dist(0,1);

	// testing to pass functions to MetropolisStep and calling 100 steps
	auto propose = [] () { std::cout << "Hello world!" << std::endl; };

	// we are randomly and uniformly generating 0 or 1, thus acceptance rate should be close to 0.5
	auto change = [&]() {
		int spinval = spin_dist( rndGen );
		std::cout << "Spin: " << spinval << std::endl;
		return double(spinval);
	};
	auto accept = []() { std::cout << "Accepted!" << std::endl << std::endl; };
	auto reject = []() { std::cout << "Rejected!" << std::endl << std::endl; };
	MetropolisStep met( propose, change, accept, reject, &rndGen );

	for( size_t i = 0; i < 100; i++ )
		met.step();
	std::cout << "Accepted: " << met.getAcceptance() << " should be close to 0.5" <<  std::endl;
}
