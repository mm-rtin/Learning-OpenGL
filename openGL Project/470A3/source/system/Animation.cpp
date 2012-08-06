/*
     File:  Animation.cpp

   Author:  Michael Martin
  Created:  Aug 15, 2010
   Edited:  Aug 15, 2010

  Purpose:	Animation tweening algorithms
     Note:
*/

#include <math.h>
#include <system/Animation.hpp>


/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                     Animation
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

const float PI = 3.14;

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Constructor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Animation::Animation() {
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Deconstructor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Animation::~Animation() {

}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** LINEAR
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
float Animation::linear(float t, float b, float c, float d) {
	return c*t/d + b;
}
float Animation::inLinear(float t, float b, float c, float d) {
	return c*t/d + b;
}
float Animation::outLinear(float t, float b, float c, float d) {
	return c*t/d + b;
}
float Animation::inOutLinear(float t, float b, float c, float d) {
	return c*t/d + b;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** QUAD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
float Animation::inQuad(float t, float b, float c, float d) {
	return c*(t/=d)*t + b;
}
float Animation::outQuad(float t, float b, float c, float d) {
	return -c *(t/=d)*(t-2) + b;
}
float Animation::inOutQuad(float t, float b, float c, float d) {
	if ((t/=d/2) < 1) return c/2*t*t + b;
	return -c/2 * ((--t)*(t-2) - 1) + b;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** CUBIC
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
float Animation::inCubic(float t, float b, float c, float d) {
	return c*(t/=d)*t*t + b;
}
float Animation::outCubic(float t, float b, float c, float d) {
	return c*((t=t/d-1)*t*t + 1) + b;
}
float Animation::inOutCubic(float t, float b, float c, float d) {
	if ((t/=d/2) < 1) return c/2*t*t*t + b;
	return c/2*((t-=2)*t*t + 2) + b;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** QUART
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
float Animation::inQuart(float t, float b, float c, float d) {
	return c*(t/=d)*t*t*t + b;
}
float Animation::outQuart(float t, float b, float c, float d) {
	return -c * ((t=t/d-1)*t*t*t - 1) + b;
}
float Animation::inOutQuart(float t, float b, float c, float d) {
	if ((t/=d/2) < 1) return c/2*t*t*t*t + b;
	return -c/2 * ((t-=2)*t*t*t - 2) + b;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** QUINT
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
float Animation::inQuint(float t, float b, float c, float d) {
	return c*(t/=d)*t*t*t*t + b;
}
float Animation::outQuint(float t, float b, float c, float d) {
	return c*((t=t/d-1)*t*t*t*t + 1) + b;
}
float Animation::inOutQuint(float t, float b, float c, float d) {
	if ((t/=d/2) < 1) return c/2*t*t*t*t*t + b;
	return c/2*((t-=2)*t*t*t*t + 2) + b;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** EXPO
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
float Animation::inExpo(float t, float b, float c, float d) {
	return (t==0) ? b : c * pow(2, 10 * (t/d - 1)) + b;
}
float Animation::outExpo(float t, float b, float c, float d) {
	return (t==d) ? b+c : c * (-pow(2, -10 * t/d) + 1) + b;
	}
float Animation::inOutExpo(float t, float b, float c, float d) {
	if (t==0) return b;
	if (t==d) return b+c;
	if ((t/=d/2) < 1) return c/2 * pow(2, 10 * (t - 1)) + b;
	return c/2 * (-pow(2, -10 * --t) + 2) + b;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** CIRC
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
float Animation::inCirc(float t, float b, float c, float d) {
	return -c * (sqrt(1 - (t/=d)*t) - 1) + b;
}
float Animation::outCirc(float t, float b, float c, float d) {
	return c * sqrt(1 - (t=t/d-1)*t) + b;
}
float Animation::inOutCirc(float t, float b, float c, float d) {
	if ((t/=d/2) < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b;
	return c/2 * (sqrt(1 - (t-=2)*t) + 1) + b;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** SINE
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
float Animation::inSine(float t, float b, float c, float d) {
	return -c * cos(t/d * (PI/2)) + c + b;
}
float Animation::outSine(float t, float b, float c, float d) {
	return c * sin(t/d * (PI/2)) + b;
}
float Animation::inOutSine(float t, float b, float c, float d) {
	if ((t/=d/2) < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b;
	return -c/2 * (cos(PI*t/d) - 1) + b;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** BACK
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
float Animation::inBack(float t, float b, float c, float d) {
	float s = 1.70158;
	return c*(t/=d)*t*((s+1)*t - s) + b;
}
float Animation::outBack(float t, float b, float c, float d) {
	float s = 1.70158;
	return c*((t=t/d-1)*t*((s+1)*t + s) + 1) + b;
}
float Animation::inOutBack(float t, float b, float c, float d) {
	float s = 1.70158; 
	if ((t/=d/2) < 1) return c/2*(t*t*(((s*=(1.525))+1)*t - s)) + b;
	return c/2*((t-=2)*t*(((s*=(1.525))+1)*t + s) + 2) + b;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** BOUNCE
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
float Animation::outBounce(float t, float b, float c, float d) {
	if ((t/=d) < (1/2.75)) {
		return c*(7.5625*t*t) + b;
	} else if (t < (2/2.75)) {
		return c*(7.5625*(t-=(1.5/2.75))*t + .75) + b;
	} else if (t < (2.5/2.75)) {
		return c*(7.5625*(t-=(2.25/2.75))*t + .9375) + b;
	} else {
		return c*(7.5625*(t-=(2.625/2.75))*t + .984375) + b;
	}
}