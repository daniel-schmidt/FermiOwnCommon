/*
 * ConfigGenerator.h
 *
 *  Created on: 02.09.2016
 *      Author: dschmidt
 */

#ifndef FERMIOWNCOMMON_SRC_ACTIONS_CONFIGGENERATOR_H_
#define FERMIOWNCOMMON_SRC_ACTIONS_CONFIGGENERATOR_H_

#include "Constants.h"

namespace FermiOwn {

class ConfigGenerator {
public:
	ConfigGenerator( size_t numThermal, size_t numConfs, size_t numUpdatesPerConfig, voidFun Step, std::function< bool(int) > onConfig );
	virtual ~ConfigGenerator();

	void run();
private:
	size_t nTher;
	size_t nConf;
	size_t nUpPerConf;

	voidFun step;
	std::function< bool(int) > onConf;
};

} /* namespace FermiOwn */

#endif /* FERMIOWNCOMMON_SRC_ACTIONS_CONFIGGENERATOR_H_ */
