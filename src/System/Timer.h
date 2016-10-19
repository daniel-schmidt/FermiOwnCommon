/*
 * Timer.h
 *
 *  Created on: 19.10.2016
 *      Author: dschmidt
 */

#ifndef SRC_SYSTEM_TIMER_H_
#define SRC_SYSTEM_TIMER_H_

#include <iostream>
#include <string>
#include <chrono>

namespace FermiOwn {


typedef std::chrono::high_resolution_clock hr_clock;
typedef std::chrono::microseconds microsec;
typedef std::chrono::milliseconds millisec;
typedef std::chrono::seconds seconds;
typedef std::chrono::minutes minutes;
typedef std::chrono::hours hours;

/**
 * @brief Basic stop-watch timer to measure wall time
 *
 * This class wraps the functionality of the c++11 header chronos
 * to be easier to use and get formatted output.
 *
 * The timer can save two time points and returns the difference
 * between these.
 */
class Timer {
public:
	/**
	 * @brief Create a new timer.
	 */
	Timer();
	virtual ~Timer();

	/**
	 * @brief Start the timer
	 *
	 * Saves the current time as beginning of measurement.
	 * If called again, this simply overwrites the starting point.
	 */
	void start();

	/**
	 * @brief Stop the timer
	 *
	 * This just sets the second time point to the current time.
	 * Any further call overwrites previous stops.
	 */
	void stop();

	/**
	 * @brief Print a formatted text of measured time interval to standard output
	 *
	 * @param text is a string to be output together with the timing result.
	 * 		  The default output is "Something took xx s", where "Something" can
	 * 		  be overwritten by this argument.
	 */
	void printDuration( std::string text = "Something");

private:
	bool running;
	hr_clock timer;
	hr_clock::time_point t1;
	hr_clock::time_point t2;
};

} /* namespace FermiOwn */

#endif /* SRC_SYSTEM_TIMER_H_ */
