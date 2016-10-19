/*
 * libconfig_test.cpp
 *
 *  Created on: 19.10.2016
 *      Author: dschmidt
 */

#include <iostream>
#include <vector>
#include <libconfig.h++>

int main() {

	using namespace libconfig;

	Config conf;

	try {
		conf.readFile( "parameters.conf" );
	} catch( const FileIOException &fioex ) {
		std::cerr << "I/O error while reading file." << std::endl;
		return( EXIT_FAILURE );
	}
	catch( const ParseException &pex ) {
		std::cerr << "Parse error at " << pex.getFile() << ": line " << pex.getLine()
				  << " - " << pex.getError() << std::endl;
		return( EXIT_FAILURE );
	}

	try {
		std::string name = conf.lookup("name");
		std::cout << "name:" << name << std::endl;
	} catch( const SettingNotFoundException &nfex ) {
		std::cerr << "No 'name' in configuration file." << std::endl;
	}

	try {
			int Nf = conf.lookup("Nf");
			std::cout << "Flavour number:" << Nf << std::endl;
	} catch( const SettingNotFoundException &nfex ) {
			std::cerr << "No 'Nf' in configuration file." << std::endl;
	}

	const Setting & confSet = conf.getRoot();
	try {
		int dim = confSet["lattice_size"].getLength();
		std::vector<int> lattice_size(dim);
		int volume = 1;
		for( int mu = 0; mu < dim; mu++ ) {
			lattice_size[mu] = confSet["lattice_size"][mu];
			volume *= lattice_size[mu];
		}
		std::cout << "Time size: " << lattice_size[0] << " volume: " << volume << std::endl;
	}
	catch( const SettingNotFoundException &nfex ) {
		std::cerr << "No 'Nf' in configuration file." << std::endl;
	}

	return 0;
}
