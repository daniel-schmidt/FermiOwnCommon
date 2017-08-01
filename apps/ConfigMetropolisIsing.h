/*
 * ConfigMetropolisIsing.h
 *
 *  Created on: 20.10.2016
 *      Author: dschmidt
 */

#ifndef APPS_CONFIGMETROPOLISISING_H_
#define APPS_CONFIGMETROPOLISISING_H_

#include "ConfigMC.h"

namespace FermiOwn {

class ConfigMetropolisIsing: public ConfigMC {
public:
	ConfigMetropolisIsing();
	virtual ~ConfigMetropolisIsing();
protected:
	inline virtual bool addToFile();
};

ConfigMetropolisIsing::ConfigMetropolisIsing(){};

ConfigMetropolisIsing::~ConfigMetropolisIsing(){};

bool ConfigMetropolisIsing::addToFile() {
	using namespace libconfig;
	Setting& Model = root.add("model", Setting::TypeGroup );
	Model.add( "beta", Setting::TypeFloat ) = 0.1;
	Setting& lattice_size = Model.add( "lattice_size", Setting::TypeArray );
	lattice_size.add( Setting::TypeInt ) = 16;
	lattice_size.add( Setting::TypeInt ) = 16;
	return true;
}

} /* namespace FermiOwn */

#endif /* APPS_CONFIGMETROPOLISISING_H_ */
