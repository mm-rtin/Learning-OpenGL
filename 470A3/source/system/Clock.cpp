/*
     File:	Clock.cpp

  Authors:	Michael Martin
  Created:	Sep 17, 2010

  Purpose:	
	 Note:
*/

#include <iostream>
#include <system/Clock.hpp>

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Constructor - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Clock::Clock() {
	reset();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Destructor - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Clock::~Clock() {

}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** getSystemTime - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
double Clock::getSystemTime() {

    static LARGE_INTEGER frequency;
    static bool highPerformanceTimer = QueryPerformanceFrequency(&frequency);
	
	// high performance timer
    if (highPerformanceTimer) {
        
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);

        return static_cast<double>(currentTime.QuadPart) / frequency.QuadPart;
	
	// high performance timer unavailable
    } else {
        return GetTickCount() * 0.001;
    }
}


/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** getElapsedTime - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
float Clock::getElapsedTime() {
	return static_cast<float>(Clock::getSystemTime() - m_startTime);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** reset - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Clock::reset() {
	m_startTime = Clock::getSystemTime();
}