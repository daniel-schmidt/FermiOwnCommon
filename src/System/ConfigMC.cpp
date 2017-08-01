/*
 * ConfigMC.cpp
 *
 *  Created on: 20.10.2016
 *      Author: dschmidt
 */

#include "ConfigMC.h"

namespace FermiOwn {

ConfigMC::ConfigMC() :
		conf(),
		root(conf.getRoot())
{}

ConfigMC::~ConfigMC() {
	// TODO Auto-generated destructor stub
}

bool ConfigMC::writeToFile() {
	using namespace libconfig;

	if( !addToFile() ) return false;

	Setting& MC = root.add( "mc", Setting::TypeGroup );
	MC.add( "num_thermal", Setting::TypeInt ) = 100;
	MC.add( "num_measures", Setting::TypeInt ) = 1000;
	MC.add( "num_updates_per_measure", Setting::TypeInt ) = 10;

	const char* output_file = "new.conf";
	try {
		conf.writeFile( output_file );
	} catch( const FileIOException &fioex ) {
		std::cerr << "I/O error while writing file: " << output_file << std::endl;
		return false;
	}
	return true;
}

bool ConfigMC::readFromFile() {
	return true;
}

bool ConfigMC::addToFile() {
	return true;
}

} /* namespace FermiOwn */
