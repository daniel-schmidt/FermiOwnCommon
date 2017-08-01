/*
 * ConfigMC.h
 *
 *  Created on: 20.10.2016
 *      Author: dschmidt
 */

#ifndef SRC_SYSTEM_CONFIGMC_H_
#define SRC_SYSTEM_CONFIGMC_H_

#include <iostream>
#include <vector>
#include <libconfig.h++>

namespace FermiOwn {

class ConfigMC {
public:
	ConfigMC();
	virtual ~ConfigMC();

	bool writeToFile();
	bool readFromFile();

private:
	libconfig::Config conf;

protected:
	virtual bool addToFile();

	libconfig::Setting& root;

};

} /* namespace FermiOwn */

#endif /* SRC_SYSTEM_CONFIGMC_H_ */
