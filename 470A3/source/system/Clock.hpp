#ifndef CLOCK_H
#define CLOCK_H

#include <windows.h>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                         Clock
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

class Clock {

private:

	/// properties
	double m_startTime;

public:

	Clock();
	~Clock();
	
	static double getSystemTime();
	float getElapsedTime();
	void reset();
};
#endif