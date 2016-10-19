/*
 * Timer.cpp
 *
 *  Created on: 19.10.2016
 *      Author: dschmidt
 */

#include "Timer.h"

namespace FermiOwn {

Timer::Timer():
	running(false)
{}

Timer::~Timer() {}

void Timer::start() {
	t1 = hr_clock::now();
	running = true;
}

void Timer::stop() {
	if( !running ) std::cout << "Warning: you tried to stop a timer, that is not running!" << std::endl;
	t2 = hr_clock::now();
}

void Timer::printDuration( std::string text ) {

	if( running ) {
		auto duration = std::chrono::duration_cast<millisec>( t2 - t1 ).count();
		std::cout << text << " took " << duration/1000. << " seconds." << std::endl;
	} else {
		std::cout << "Timer is not running!" << std::endl;
	}

//	auto sec = std::chrono::duration_cast<seconds>( t2 - t1 ).count();
//	auto msec = std::chrono::duration_cast<millisec>( t2 - t1 ).count();

//	std::cout << "It took " << sec << "s and " << msec - sec*1000 << "ms" << std::endl;

}

} /* namespace FermiOwn */
