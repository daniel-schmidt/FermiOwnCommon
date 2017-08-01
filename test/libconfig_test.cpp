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

	try {
		const Setting & mcSettings = conf.lookup("mc");
		int numThermal = 0;
		if( mcSettings.lookupValue("num_thermal", numThermal) ) {
			std::cout << "num_thermal: " << numThermal << std::endl;
		} else {
			std::cerr << "num_thermal not found in mc-section of configuration file." << std::endl;
		}
	} catch( const SettingNotFoundException &nfex ) {
			std::cerr << "No 'mc' in configuration file." << std::endl;
	}

	Config newConf;

	Setting& root = newConf.getRoot();
	Setting& MC = root.add( "mc", Setting::TypeGroup );
	MC.add( "num_thermal", Setting::TypeInt ) = 100;
	MC.add( "num_measures", Setting::TypeInt ) = 1000;
	MC.add( "num_updates_per_measure", Setting::TypeInt ) = 10;

	Setting& Model = root.add("model", Setting::TypeGroup );
	Model.add( "Nf", Setting::TypeInt ) = 1;
	Setting& lattice_size = Model.add( "lattice_size", Setting::TypeArray );
	lattice_size.add( Setting::TypeInt ) = 4;
	lattice_size.add( Setting::TypeInt ) = 3;
	lattice_size.add( Setting::TypeInt ) = 3;

	const char* output_file = "new.conf";
	try {
		newConf.writeFile( output_file );
	} catch( const FileIOException &fioex ) {
		std::cerr << "I/O error while writing file: " << output_file << std::endl;
		return(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
