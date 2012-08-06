/*
     File:	ScreenManager.cpp

  Authors:	Michael Martin
  Created:	July 25, 2010

  Purpose:	
	 Note:
*/

#include <GL/freeglut.h>
#include <system/ScreenManager.hpp>
#include <system/Globals.hpp>

/** GLOBAL DEFINITION
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
ControlScreen* g_controlScreen;

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                   ScreenManager
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Constructor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
ScreenManager::ScreenManager() {
	
	// set global pointer to m_controlScreen
	g_controlScreen = &m_controlScreen;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Destructor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
ScreenManager::~ScreenManager() {

}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** preloadScreen
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ScreenManager::preloadScreen(SCREENS::screenNames screenName) {

	switch(screenName) {

	case SCREENS::START_SCREEN :
		break;

	case SCREENS::GAME_SCREEN :
		m_gameScreen.preload();
		break;

	case SCREENS::CONTROL_SCREEN :
		m_controlScreen.preload();
		break;
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** addScreen
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Screen* ScreenManager::addScreen(SCREENS::screenNames screenName, float x, float y, float width, float height) {

	Screen* loadedScreen = NULL;

	switch(screenName) {
		case SCREENS::START_SCREEN :
			break;

		case SCREENS::GAME_SCREEN :
			loadedScreen = loadScreen(m_gameScreen);
			break;

		case SCREENS::CONTROL_SCREEN :
			loadedScreen = loadScreen(m_controlScreen);
			break;
	}

	if (loadedScreen) {
		loadedScreen->setViewportParameters(x, y, width, height);
		m_loadedScreens.push_back(loadedScreen);
	}

	return loadedScreen;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** loadScreen
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Screen* ScreenManager::loadScreen(Screen& screen) {
	
	// make sure screen is loaded
	if (!screen.isPreloaded)
		screen.preload();

	// start screen
	if (!screen.isStarted)
		screen.start();

	return &screen;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** setupScreenProjections
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ScreenManager::setupScreenProjections(int width, int height) {

	// for each loaded screen - update projection
	for (unsigned int i = 0; i < m_loadedScreens.size(); ++i) {
		m_loadedScreens[i]->setupProjection(width, height);
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** updateScreens
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ScreenManager::updateScreens() {

	// for each loaded screen - call update and draw
	for (unsigned int i = 0; i < m_loadedScreens.size(); ++i) {
		m_loadedScreens[i]->update();
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** displayScreens
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ScreenManager::displayScreens() {

    // clear screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     

	// for each loaded screen - call update and draw
	for (unsigned int i = 0; i < m_loadedScreens.size(); ++i) {
		m_loadedScreens[i]->draw();
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** mouseMoved
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ScreenManager::mouseMoved(int x, int y) {

	// for each loaded screen - call input method
	for (unsigned int i = 0; i < m_loadedScreens.size(); ++i) {
		m_loadedScreens[i]->mouseMoved(x, y);
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** mousePressed
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ScreenManager::mousePressed(int mouseButton, int x, int y) {

	// for each loaded screen - call input method
	for (unsigned int i = 0; i < m_loadedScreens.size(); ++i) {
		m_loadedScreens[i]->mousePressed(mouseButton, x, y);
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** mouseReleased
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ScreenManager::mouseReleased(int mouseButton, int x, int y) {

	// for each loaded screen - call input method
	for (unsigned int i = 0; i < m_loadedScreens.size(); ++i) {
		m_loadedScreens[i]->mouseReleased(mouseButton, x, y);
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** mouseWheelScrolled
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ScreenManager::mouseWheelScrolled(int delta) {

	// for each loaded screen - call input method
	for (unsigned int i = 0; i < m_loadedScreens.size(); ++i) {
		m_loadedScreens[i]->mouseWheelScrolled(delta);
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** keyPressed
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ScreenManager::keyPressed(int key) {

	// for each loaded screen - call input method
	for (unsigned int i = 0; i < m_loadedScreens.size(); ++i) {
		m_loadedScreens[i]->keyPressed(key);
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** keyReleased
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ScreenManager::keyReleased(int key) {

	// for each loaded screen - call input method
	for (unsigned int i = 0; i < m_loadedScreens.size(); ++i) {
		m_loadedScreens[i]->keyReleased(key);
	}
}