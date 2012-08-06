/*
     File:	GameScreen.cpp

  Authors:	Michael Martin
  Created:	July 25, 2010

  Purpose:	
	 Note:
*/

#include <math.h>
#include <Windows.h>
#include <GL/freeglut.h>

#include <system/Globals.hpp>
#include <game/ControlScreen.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                      ControlScreen
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Constructor - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
ControlScreen::ControlScreen() 
	: Screen(),
	gameClock() {
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Destructor - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
ControlScreen::~ControlScreen() {


}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** preload - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ControlScreen::preload() {

	// Base class method
	Screen::preload();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** start - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ControlScreen::start() {

	/// Base class method
	Screen::start();

	/// initialize properties

	// clock properties
	m_freezeClock = false;
	m_currentTime = 0.0f;

	// control properties
	m_displayVictory = false;

	// mouse properties
	m_mouseLeftPressed = false;
	m_mouseRightPressed = false;
	m_mouseX = 0;
	m_mouseY = 0;

	// intialize text
	strcpy_s(m_playerName, "Player 1");
	strcpy_s(m_victoryText, "VICTORY");
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** stop
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ControlScreen::stop() {

	// Base class method
	Screen::stop();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** update
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ControlScreen::update() {

}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** setupProjection
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ControlScreen::setupProjection(int width, int height) {

	// base class projection
	Screen::setupProjection(width, height);

	// projection matrx
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
    // orthographic projection - for 2d rendering
	glOrtho(-1.5f, (GLfloat)m_viewWidth * 3, -(GLfloat)m_viewHeight * 3, -1.5f, -1.0, 1.0);

	// modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	// clear depth buffer
    glClear(GL_DEPTH_BUFFER_BIT);     
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** draw
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ControlScreen::draw() {

	glDisable(GL_LIGHTING);

	setupProjection(m_windowWidth, m_windowHeight);

	/// green color
	glColor3f(0.7f, 1.0f, 0.3f);

	/// line
	glLineWidth(1.0f);

	/// calculate scale amount when viewHeight changes
	int baseHeight = 200;
	float ratio = (float)m_viewHeight / (float)baseHeight;
	
	glTranslatef(10.0, (GLfloat)-m_viewHeight, 0.0);
	glScalef(ratio, ratio, 1.0);

	
	/// draw player name text
	glPushMatrix();
		glTranslatef(0.0, 60.0, 0.0);
		for (char* c = m_playerName; *c != '\0'; c++) {
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
		}
	glPopMatrix();

	/// draw timer text
	if (!m_freezeClock)
		m_currentTime = gameClock.getElapsedTime();
	// conver to string
	std::stringstream ss;
	ss << m_currentTime;

	std::string displayTime1 = ss.str();
	char* displayTime = (char*)displayTime1.c_str();

	glPushMatrix();
		glTranslatef(1000.0, 60.0, 0.0);
		for (char* c = displayTime; *c != '\0'; c++) {
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
		}
	glPopMatrix();

	/// draw victory text
	if (m_displayVictory) {
		if (fmod(g_gameEngine->realtimeClock.getElapsedTime(), 1) < .5) {
			glPushMatrix();
			glTranslatef(1800.0, 60.0, 0.0);
			for (char* c = m_victoryText; *c != '\0'; c++) {
				glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
			}
			glPopMatrix();
		}
	}

	glEnable(GL_LIGHTING);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** resetTimer - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ControlScreen::resetTimer() {
	m_freezeClock = false;
	m_displayVictory = false;
	gameClock.reset();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** stopTimer - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ControlScreen::stopTimer() {
	m_freezeClock = true;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** displayVictoryMessage - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ControlScreen::displayVictoryMessage() {
	m_displayVictory = true;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** mouseMoved
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ControlScreen::mouseMoved(int x, int y) {

	if (m_mouseLeftPressed) {
		if ((x - m_mouseX) < 0) {
		}
		if ((x - m_mouseX) > 0) {
		}
	}

	if (m_mouseLeftPressed) {
		if ((y - m_mouseY) < 0) {

		}
		if ((y - m_mouseY) > 0) {
		}
	}

	/// save mouse coordinates
	m_mouseX = x;
	m_mouseY = y;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** mousePressed
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ControlScreen::mousePressed(int mouseButton, int x, int y) {

	switch (mouseButton) {
	case 0:
		m_mouseLeftPressed = true;
		break;
	case 1:
		m_mouseRightPressed = true;
		break;
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** mouseReleased
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ControlScreen::mouseReleased(int mouseButton, int x, int y) {
	switch (mouseButton) {
	case 0:
		m_mouseLeftPressed = false;
		break;
	case 1:
		m_mouseRightPressed = false;
		break;
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** mouseWheelScrolled
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ControlScreen::mouseWheelScrolled(int delta) {

}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** keyPressed
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ControlScreen::keyPressed(int key) {

	switch (key) {
		case 0 :
			break;
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** keyReleased
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ControlScreen::keyReleased(int key) {

}