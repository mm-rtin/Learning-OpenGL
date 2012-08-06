/*
     File:	GameEngine.cpp

  Authors:	Michael Martin
  Created:	Sep 12, 2010

  Purpose:	
	 Note:
*/

#include <GL/freeglut.h>

#include <game/GameEngine.hpp>

/** GLOBAL DEFINITION
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/// global class pointers
GameEngine* g_gameEngine;
EventManager* g_eventManager;
ScreenManager* g_screenManager;
ResourceManager* g_resourceManager;
Clock* g_systemClock;
Primitives* g_primitives;

/// global properties
bool DISPLAY_LIST = true;
bool VISUALIZATIONS_ON = false;
bool NORMALS_PER_VERTEX = true;
bool NORMALS_PER_VERTEX_WEIGHTED = false;
bool USE_TEXTURES = true;
int TEXTURE_MODE = 1;
bool GLOBAL_MATERIAL = false;
bool NORMAL_VIS_OFF = false;

/// crease angles
float CREASE_ANGLE1 = 15.0f;
float CREASE_ANGLE2 = 25.0f;
float CREASE_ANGLE3 = 35.0f;
float CREASE_ANGLE4 = 360.0f;
float CREASE_ANGLE = CREASE_ANGLE1;
bool SHARP_EDGES = false;

/// AABB Tree
bool VISUALIZE_AABB_TREE = false;
int AABB_TREE_DEPTH = 0;
int AABB_TREE_MAX_DEPTH = 10;
bool VISUALIZE_LEAF =  false;
int SHOW_NODE_ID = -1;

/// projectile type
int PROJECTILE_TYPE = 0;

/// spline
bool SHOW_SPLINE = true;
int SPLINE_MODE = 0;

/// control mode
int CONTROL_MODE = 0;

bool MOUSE_WARP = false;



 /**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                    GameEngine
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Constructor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
GameEngine::GameEngine() 
	: primitives(),
	  eventManager(),
	  screenManager(),
	  resourceManager(),
	  systemClock(),
	  realtimeClock() {
	
	/** INITIALIZATION
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/// set global pointers 
	g_eventManager = &eventManager;
	g_screenManager = &screenManager;
	g_resourceManager = &resourceManager;
	g_systemClock = &systemClock;
	g_primitives = &primitives;

	// set intialize properties
	frameRateLimit = 0.0f;
	lastFrameTime = 0.0;

	// set initial state
	resourceManager.setMinificationType(1);

	// add event listeners
	g_eventManager->addEventListener(*this, sf::Event::MouseMoved);
	g_eventManager->addEventListener(*this, sf::Event::MouseButtonPressed);
	g_eventManager->addEventListener(*this, sf::Event::MouseButtonReleased);
	g_eventManager->addEventListener(*this, sf::Event::MouseWheelMoved);
	g_eventManager->addEventListener(*this, sf::Event::KeyPressed);
	g_eventManager->addEventListener(*this, sf::Event::KeyReleased);

	/// set openGL properties
	/// set background color to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_NONE);
	glEnable(GL_DEPTH_TEST);
	
	/// load screens
	m_gameScreen = (GameScreen*)g_screenManager->addScreen(SCREENS::GAME_SCREEN, 0.0f, 0.0f, 1.0f, .95f);
	m_controlScreen = (ControlScreen*)g_screenManager->addScreen(SCREENS::CONTROL_SCREEN, 0.0f, .95f, 1.0f, .05f);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Destructor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
GameEngine::~GameEngine() {

}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** setupProjection
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameEngine::setupProjection(int width, int height) {

	g_screenManager->setupScreenProjections(width, height);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** update - updates game
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameEngine::update() {
	
	// UPDATE loaded screens
	g_screenManager->updateScreens();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** display 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameEngine::display() {

    if (frameRateLimit > 0) {
        float remainingTime = 1.0f / frameRateLimit - g_systemClock->getElapsedTime();
        if (remainingTime > 0) {
            Sleep(remainingTime * 1000);
		}
    }

	// DRAW loaded screens
	g_screenManager->displayScreens();

	// measure frame time
    lastFrameTime = g_systemClock->getElapsedTime();
    g_systemClock->reset();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** onInputEvent
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool GameEngine::onInputEvent(sf::Event& inputEvent) {
	
	// SWITCH based on sf::Event (EventType)
	switch (inputEvent.Type) {

	/// MOUSE MOVED
	case sf::Event::MouseMoved :
		g_screenManager->mouseMoved(inputEvent.MouseMove.X, inputEvent.MouseMove.Y);
		break;

	/// MOUSE PRESSED
	case sf::Event::MouseButtonPressed:

		switch (inputEvent.MouseButton.Button) {
		// LEFT
		case sf::Mouse::Left :
			g_screenManager->mousePressed(0, inputEvent.MouseMove.X, inputEvent.MouseMove.Y);
			break;
		// RIGHT
		case sf::Mouse::Right :
			g_screenManager->mousePressed(1, inputEvent.MouseMove.X, inputEvent.MouseMove.Y);
			break;
		}
		break;

	/// MOUSE RELEASED
	case sf::Event::MouseButtonReleased :

		switch (inputEvent.MouseButton.Button) {
		// LEFT
		case sf::Mouse::Left :
			g_screenManager->mouseReleased(0, inputEvent.MouseMove.X, inputEvent.MouseMove.Y);
			break;
		// RIGHT
		case sf::Mouse::Right :
			g_screenManager->mouseReleased(1, inputEvent.MouseMove.X, inputEvent.MouseMove.Y);
			break;
		}
		break;

	/// MOUSE WHEEL
	case sf::Event::MouseWheelMoved :
		g_screenManager->mouseWheelScrolled(inputEvent.MouseWheel.Delta);
		break;

	/// KEY PRESSED
	case sf::Event::KeyPressed :
		g_screenManager->keyPressed(inputEvent.Key.Code);
		break;

	/// KEY RELEASED
	case sf::Event::KeyReleased :
		g_screenManager->keyReleased(inputEvent.Key.Code);
		break;
	}

	return true;
}


/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** menuOption
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameEngine::menuOption(int optionID) {

	switch (optionID) {

		// quit
		case 0 :
			break;
		// view: top down
		case 1:
			m_gameScreen->setViewMode(0);
			break;
		// view: over view
		case 2: 
			m_gameScreen->setViewMode(1);
			break;
		// view: first person
		case 3:
			m_gameScreen->setViewMode(2);
			break;
		// view: third person
		case 60:
			m_gameScreen->setViewMode(3);
			break;

		// shading: smooth
		case 4:
			glShadeModel(GL_SMOOTH);
			break;
		// shading: flat
		case 5:
			glShadeModel(GL_FLAT);
			break;
		// display list: on
		case 6:
			DISPLAY_LIST = true;
			break;
		// display list: off
		case 7:
			DISPLAY_LIST = false;
			break;
		// framerate: 60
		case 8:
			frameRateLimit = 60.0f;
			break;
		// framerate: MAX
		case 9:
			frameRateLimit = 0.0f;
			break;
		// restart
		case 10:
			m_gameScreen->startNewGame();
			break;
		// slow
		case 11:
			m_gameScreen->setAnimationSpeed(.5);
			break;
		// medium
		case 12:
			m_gameScreen->setAnimationSpeed(1);
			break;
		// fast
		case 13:
			m_gameScreen->setAnimationSpeed(2.0);
			break;

		// visualization on
		case 14:
			VISUALIZATIONS_ON = true;
			break;
		// visualization off
		case 15:
			VISUALIZATIONS_ON = false;
			break;
		// visualization : per face
		case 16:
			NORMALS_PER_VERTEX = false;
			m_gameScreen->regenerateDisplayLists();
			break;
		// visualization : per vertex
		case 17:
			NORMALS_PER_VERTEX = true;
			NORMALS_PER_VERTEX_WEIGHTED = false;
			recalculateNormals();
			break;
		// textures : ON
		case 18:
			/// disable color tracking
			glDisable(GL_COLOR_MATERIAL);
			m_gameScreen->changeGlobalMaterial(0);
			USE_TEXTURES = true;
			m_gameScreen->regenerateDisplayLists();
			break;
		// textures : OFF
		case 19:
			/// enable color tracking
			glEnable(GL_COLOR_MATERIAL);
			/// set material properties to follow glColor values
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
			USE_TEXTURES = false;
			m_gameScreen->regenerateDisplayLists();
			break;
		// texture mode : replace
		case 20:
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			TEXTURE_MODE = 0;
			break;
		// texture mode : modulate
		case 21:
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			TEXTURE_MODE = 1;
			break;
		// minification - simple
		case 22:
			resourceManager.setMinificationType(0);
			break;
		// minification - smooth
		case 23:
			resourceManager.setMinificationType(1);
			break;
		// magnification - simple
		case 24:
			resourceManager.setMagnificationType(0);
			break;
		// magnification - smooth
		case 25:
			resourceManager.setMagnificationType(1);
			break;
		// materials - diffuse
		case 26:
			GLOBAL_MATERIAL = true;
			m_gameScreen->changeGlobalMaterial(0);
			break;
		// materials - specular
		case 27:
			GLOBAL_MATERIAL = true;
			m_gameScreen->changeGlobalMaterial(1);
			break;
		// materials - design1
		case 28:
			GLOBAL_MATERIAL = false;
			m_gameScreen->changeGlobalMaterial(2);
			break;
		// materials - design2
		case 29:
			GLOBAL_MATERIAL = true;
			m_gameScreen->changeGlobalMaterial(3);
			break;
		// navigation - light source
		case 30:
			m_gameScreen->setNavigationMode(0);
			break;
		// navigation - camera
		case 31:
			m_gameScreen->setNavigationMode(1);
			break;
		// crease1 - 15
		case 32:
			CREASE_ANGLE = CREASE_ANGLE1;
			recalculateNormals();
			break;
		// crease2 - 25
		case 33:
			CREASE_ANGLE = CREASE_ANGLE2;
			recalculateNormals();
			break;
		// crease3 - 35
		case 34:
			CREASE_ANGLE = CREASE_ANGLE3;
			recalculateNormals();
			break;
		// crease4 - OFF
		case 35:
			CREASE_ANGLE = CREASE_ANGLE4;
			recalculateNormals();
			break;
		// normals : per vertex weighted
		case 36:
			NORMALS_PER_VERTEX_WEIGHTED = true;
			recalculateNormals();
			break;
		// visualize sharp edges - ON
		case 37:
			SHARP_EDGES = true;
			m_gameScreen->regenerateDisplayLists();
			break;
		// visualize sharp edges - OFF
		case 38:
			SHARP_EDGES = false;
			m_gameScreen->regenerateDisplayLists();
			break;
		// only normals off
		case 39:
			if (NORMAL_VIS_OFF)
				NORMAL_VIS_OFF = false;
			else
				NORMAL_VIS_OFF = true;
			m_gameScreen->regenerateDisplayLists();
			break;

		// aabb visulations - on
		case 40 :
			VISUALIZE_AABB_TREE = true;
			break;

		// aabb visulations - off
		case 41 :
			VISUALIZE_AABB_TREE = false;
			break;
		// aabb depth - 0
		case 42 :
			AABB_TREE_DEPTH = 0;
			VISUALIZE_LEAF = false;
			m_gameScreen->regenerateDisplayLists();
			break;
		// aabb depth - 1
		case 43 :
			AABB_TREE_DEPTH = 1;
			VISUALIZE_LEAF = false;
			m_gameScreen->regenerateDisplayLists();
			break;
		// aabb depth - 2
		case 44 :
			AABB_TREE_DEPTH = 2;
			VISUALIZE_LEAF = false;
			m_gameScreen->regenerateDisplayLists();
			break;
		// aabb depth - 3
		case 45 :
			AABB_TREE_DEPTH = 3;
			VISUALIZE_LEAF = false;
			m_gameScreen->regenerateDisplayLists();
			break;
		// aabb depth - 4
		case 46 :
			AABB_TREE_DEPTH = 4;
			VISUALIZE_LEAF = false;
			m_gameScreen->regenerateDisplayLists();
			break;
		// aabb depth - 5
		case 47 :
			AABB_TREE_DEPTH = 5;
			VISUALIZE_LEAF = false;
			m_gameScreen->regenerateDisplayLists();
			break;
		// aabb depth - 6
		case 48 :
			AABB_TREE_DEPTH = 6;
			VISUALIZE_LEAF = false;
			m_gameScreen->regenerateDisplayLists();
			break;
		// aabb depth - 7
		case 49 :
			AABB_TREE_DEPTH = 7;
			VISUALIZE_LEAF = false;
			m_gameScreen->regenerateDisplayLists();
			break;
		// aabb depth - 8
		case 50 :
			AABB_TREE_DEPTH = 8;
			VISUALIZE_LEAF = false;
			m_gameScreen->regenerateDisplayLists();
			break;
		// aabb depth - 9
		case 51 :
			AABB_TREE_DEPTH = 9;
			VISUALIZE_LEAF = false;
			m_gameScreen->regenerateDisplayLists();
			break;
		// aabb depth - ALL
		case 52 :
			AABB_TREE_DEPTH = -1;
			VISUALIZE_LEAF = false;
			m_gameScreen->regenerateDisplayLists();
			break;
		// aabb depth - LEAF
		case 53 :
			AABB_TREE_DEPTH = -99;
			VISUALIZE_LEAF = true;
			m_gameScreen->regenerateDisplayLists();
			break;
		// projectile type - straight
		case 54 :
			PROJECTILE_TYPE = 0;
			break;
		// projectile type - strange 1
		case 55 :
			PROJECTILE_TYPE = 1;
			break;
		// projectile type - strange 2
		case 56 :
			PROJECTILE_TYPE = 2;
			break;

		// spline - move player car
		case 57 :
			m_gameScreen->setViewMode(3);
			SPLINE_MODE = 0;
			break;
		// spline - player moves car
		case 58 :
			m_gameScreen->setViewMode(2);
			SPLINE_MODE = 1;
			break;
		// spline - move object
		case 59 :
			SPLINE_MODE = 2;
			break;
		// fly
		case 61 :
			glutSetCursor(GLUT_CURSOR_NONE);
			CONTROL_MODE = 1;
			m_gameScreen->setViewMode(2);
			break;
		// drive
		case 62 :
			m_gameScreen->resetPlayer();
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			CONTROL_MODE = 0;
			break;
		// spline - show
		case 63 :
			SHOW_SPLINE = true;
			break;
		// spline - hide
		case 64 :
			SHOW_SPLINE = false;
			break;

		// projectile type - straight long
		case 65 :
			PROJECTILE_TYPE = 3;
			break;
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** recalculateNormals
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameEngine::recalculateNormals() {

	/// generate new normals based on new settings
	g_primitives->recalculateNormals(CREASE_ANGLE, NORMALS_PER_VERTEX_WEIGHTED);
	g_resourceManager->recalculateNormals(CREASE_ANGLE, NORMALS_PER_VERTEX_WEIGHTED);

	/// regenerate display lists
	m_gameScreen->regenerateDisplayLists();
}