/*
     File:	Screen.cpp

  Authors:	Michael Martin
  Created:	July 25, 2010

  Purpose:	
	 Note:
*/

#include <iostream>
#include <GL/freeglut.h>
#include <system/Screen.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                      Screen
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Constructor 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Screen::Screen() 
	: isPreloaded(false), 
	  isStarted(false) {

	/// initialize properties
	m_windowWidth = 0;
	m_windowHeight = 0;
	m_viewWidth = 0;
	m_viewHeight = 0;
	m_centerWidth = 0;
	m_centerHeight = 0;

	/// viewport properties
	m_viewPort_xPosition = 0;
	m_viewPort_yPosition = 0;
	m_viewPortx = 0.0f;
	m_viewPorty = 0.0f;
	m_viewPortWidth = 0.0f;
	m_viewPortHeight = 0.0f;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Destructor 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Screen::~Screen() {

}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** preload
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Screen::preload() {
	isPreloaded = true;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** start
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Screen::start() {
	isStarted = true;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** stop
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Screen::stop() {
	isStarted = false;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** setupProjection
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Screen::setupProjection(int width, int height) {

	// prevent divide by 0
    if (height == 0) {
        height = 1;                 
    }

	// calculate viewport properties
	m_viewPort_xPosition = -GLint(width * m_viewPortx) + (width - GLint(width * m_viewPortWidth));
	m_viewPort_yPosition = -GLint(height * m_viewPorty) + (height - GLint(height * m_viewPortHeight));
	
	m_viewWidth = GLint(width * m_viewPortWidth);
	m_viewHeight = GLint(height * m_viewPortHeight);

	// reset the viewport to new dimensions
    glViewport(m_viewPort_xPosition, m_viewPort_yPosition, m_viewWidth, m_viewHeight);

	// save window width/height
	m_windowWidth = width;
	m_windowHeight = height;

	// calculate center
	m_centerWidth = width / 2;
	m_centerHeight = height / 2;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** setViewportParameters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Screen::setViewportParameters(float x, float y, float width, float height) {

	m_viewPortx = x;
	m_viewPorty = y;
	m_viewPortWidth = width;
	m_viewPortHeight = height;
}

void Screen::update() {}
void Screen::draw() {}

/// input methods
void Screen::mouseMoved(int, int) {}
void Screen::mousePressed(int, int, int) {}
void Screen::mouseReleased(int, int, int) {}
void Screen::mouseWheelScrolled(int) {}
void Screen::keyPressed(int) {}
void Screen::keyReleased(int) {}
