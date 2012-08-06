/*
     File:	main.cpp

  Authors:	Michael Martin
  Created:	Sep 12, 2010

  Purpose:	
	 Note:
*/

#include <windows.h>
#include <GL/freeglut.h>

#include <game/GameEngine.hpp>
#include <system/Globals.hpp>

extern GameEngine* g_gameEngine;

using namespace std;

// window parameters
GLint winId;
int position_x = 0;
int position_y = 0;
int width = 1024;
int height = 768;
bool doubleBuffer = true;

int menuOpen = false;

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** reshape
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void reshape(int x, int y) {

	g_gameEngine->setupProjection(x, y);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** idle
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void idle() {

	g_gameEngine->update();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** display
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void display() {

	g_gameEngine->display();

	if (doubleBuffer) {
		glutSwapBuffers();
	} else {
		glFlush();
	}

	glutPostRedisplay();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** keyboardEvent
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void keyboardEvent(unsigned char key, int x, int y) {
  
	sf::Event event;
	event.Type        = sf::Event::KeyPressed;
	event.Key.Alt     = NULL;
	event.Key.Control = NULL;
	event.Key.Shift   = NULL;
	event.Key.Code    = key;
	g_eventManager->dispatchEvent(event);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** keyboardUpEvent
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void keyboardUpEvent(unsigned char key, int x, int y) {
  
	sf::Event event;
	event.Type        = sf::Event::KeyReleased;
	event.Key.Alt     = NULL;
	event.Key.Control = NULL;
	event.Key.Shift   = NULL;
	event.Key.Code    = key;
	g_eventManager->dispatchEvent(event);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** specialKeyboardEvent
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void specialKeyboardEvent(int key, int x, int y ) {
	
	sf::Event event;
	
	switch (key) {

		case GLUT_KEY_F1 :
			event.Type        = sf::Event::KeyPressed;
			event.Key.Code    = 0;
			g_eventManager->dispatchEvent(event);
			break;

		case GLUT_KEY_F2 :
			event.Type        = sf::Event::KeyPressed;
			event.Key.Code    = 0;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_F12 :
			event.Type        = sf::Event::KeyPressed;
			event.Key.Code    = 0;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_PAGE_UP  :
			event.Type        = sf::Event::KeyPressed;
			event.Key.Code    = 0;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_PAGE_DOWN :
			event.Type        = sf::Event::KeyPressed;
			event.Key.Code    = 0;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_HOME :
			event.Type        = sf::Event::KeyPressed;
			event.Key.Code    = 0;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_END  :
			event.Type        = sf::Event::KeyPressed;
			event.Key.Code    = 0;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_LEFT :
			event.Type        = sf::Event::KeyPressed;
			event.Key.Code    = 291;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_RIGHT :
			event.Type        = sf::Event::KeyPressed;
			event.Key.Code    = 292;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_UP :
			event.Type        = sf::Event::KeyPressed;
			event.Key.Code    = 293;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_DOWN :
			event.Type        = sf::Event::KeyPressed;
			event.Key.Code    = 294;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_INSERT :
			event.Type        = sf::Event::KeyPressed;
			event.Key.Code    = 0;
			g_eventManager->dispatchEvent(event);
			break;
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** specialKeyboardUpEvent
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void specialKeyboardUpEvent(int key, int x, int y ) {

	sf::Event event;
	
	switch (key) {

		case GLUT_KEY_F1 :
			event.Type        = sf::Event::KeyReleased;
			event.Key.Code    = 0;
			g_eventManager->dispatchEvent(event);
			break;

		case GLUT_KEY_F2 :
			event.Type        = sf::Event::KeyReleased;
			event.Key.Code    = 0;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_F12 :
			event.Type        = sf::Event::KeyReleased;
			event.Key.Code    = 0;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_PAGE_UP  :
			event.Type        = sf::Event::KeyReleased;
			event.Key.Code    = 0;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_PAGE_DOWN :
			event.Type        = sf::Event::KeyReleased;
			event.Key.Code    = 0;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_HOME :
			event.Type        = sf::Event::KeyReleased;
			event.Key.Code    = 0;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_END  :
			event.Type        = sf::Event::KeyReleased;
			event.Key.Code    = 0;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_LEFT :
			event.Type        = sf::Event::KeyReleased;
			event.Key.Code    = 291;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_RIGHT :
			event.Type        = sf::Event::KeyReleased;
			event.Key.Code    = 292;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_UP :
			event.Type        = sf::Event::KeyReleased;
			event.Key.Code    = 293;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_DOWN :
			event.Type        = sf::Event::KeyReleased;
			event.Key.Code    = 294;
			g_eventManager->dispatchEvent(event);
			break;
		case GLUT_KEY_INSERT :
			event.Type        = sf::Event::KeyReleased;
			event.Key.Code    = 0;
			g_eventManager->dispatchEvent(event);
			break;
	}

}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** mouseButtonEvent
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void mouseButtonEvent(int button, int state, int x, int y) {
	
	sf::Event event;
	
	switch (button) {

		// LEFT/RIGHT BUTTONS
		case GLUT_LEFT_BUTTON :
		case GLUT_RIGHT_BUTTON :
			
			event.Type               = state == GLUT_DOWN ? sf::Event::MouseButtonPressed : sf::Event::MouseButtonReleased;
			event.MouseButton.Button = button == GLUT_LEFT_BUTTON ? sf::Mouse::Left : sf::Mouse::Right;
			event.MouseButton.X      = x;
			event.MouseButton.Y      = y;
			g_eventManager->dispatchEvent(event);
			break;

		// MIDDLE BUTTON
		case GLUT_MIDDLE_BUTTON :
			
            event.Type               = sf::Event::MouseButtonReleased;
            event.MouseButton.Button = sf::Mouse::Middle;
            event.MouseButton.X      = x;
            event.MouseButton.Y      = y;
			g_eventManager->dispatchEvent(event);
			break;
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** mouseMotionEvent
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void mouseMotionEvent(int x, int y) {

	sf::Event event;
	event.Type = sf::Event::MouseMoved;
	event.MouseMove.X = x;
	event.MouseMove.Y = y;
	g_eventManager->dispatchEvent(event);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** mouseWheelEvent
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void mouseWheelEvent(int button, int dir, int x, int y) {
    
	sf::Event event;
	event.Type = sf::Event::MouseWheelMoved;
	event.MouseWheel.Delta = dir;
	event.MouseButton.X    = x;
	event.MouseButton.Y    = y;
	g_eventManager->dispatchEvent(event);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** createMenu
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void optionSelected (int id) {

	if (id == 0) {
		glutDestroyWindow(winId);
		exit(0);
	} else {
		g_gameEngine->menuOption(id);
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** createMenu
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void createMenu() {	

	/// view sub
	GLint viewSubMenu = glutCreateMenu(optionSelected);
	glutAddMenuEntry("TOP DOWN", 1);
	glutAddMenuEntry("OVERVIEW", 2);
	glutAddMenuEntry("FIRST PERSON", 3);
	glutAddMenuEntry("THIRD PERSON", 60);

	/// speed sub
	GLint speedSubMenu = glutCreateMenu(optionSelected);
	glutAddMenuEntry("SLOW", 11);
	glutAddMenuEntry("MEDIUM", 12);
	glutAddMenuEntry("FAST", 13);

	/// shading sub
	GLint shadingSubMenu = glutCreateMenu(optionSelected);
	glutAddMenuEntry("SMOOTH", 4);
	glutAddMenuEntry("FLAT", 5);

	/// display list sub
	GLint displaySubMenu = glutCreateMenu(optionSelected);
	glutAddMenuEntry("ON", 6);
	glutAddMenuEntry("OFF", 7);

	GLint framerateSubMenu = glutCreateMenu(optionSelected);
	glutAddMenuEntry("60", 8);
	glutAddMenuEntry("MAX", 9);

	/// visualizations sub
	GLint normalVisualizationSubMenu = glutCreateMenu(optionSelected);
	glutAddMenuEntry("ON", 14);
	glutAddMenuEntry("OFF", 15);
	glutAddMenuEntry("--------------------", 99);
	glutAddMenuEntry("NORMAL VISUALIZATION OFF/ON", 39);

	/// normals sub
	GLint normalsSubMenu = glutCreateMenu(optionSelected);
	glutAddMenuEntry("PER FACE", 16);
	glutAddMenuEntry("PER VERTEX", 17);
	glutAddMenuEntry("PER VERTEX - WEIGHTED", 36);
	glutAddMenuEntry("VISUALIZE SHARP EDGES - ON", 37);
	glutAddMenuEntry("VISUALIZE SHARP EDGES - OFF", 38);

	/// textures sub
	GLint textureSubMenu = glutCreateMenu(optionSelected);
	glutAddMenuEntry("ON", 18);
	glutAddMenuEntry("OFF", 19);

	/// texture mode sub
	GLint textureModeSubMenu = glutCreateMenu(optionSelected);
	glutAddMenuEntry("REPLACE", 20);
	glutAddMenuEntry("MODULATE", 21);

	/// minification mode sub
	GLint minificationModeSubMenu = glutCreateMenu(optionSelected);
	glutAddMenuEntry("SIMPLE", 22);
	glutAddMenuEntry("SMOOTH", 23);

	/// magnification mode sub
	GLint magnificationModeSubMenu = glutCreateMenu(optionSelected);
	glutAddMenuEntry("SIMPLE", 24);
	glutAddMenuEntry("SMOOTH", 25);

	/// materials mode sub
	GLint materialsSubMenu = glutCreateMenu(optionSelected);
	glutAddMenuEntry("DIFFUSE", 26);
	glutAddMenuEntry("SPECULAR", 27);
	glutAddMenuEntry("DESIGN", 28);
	glutAddMenuEntry("--------------------", 99);
	glutAddMenuEntry("TEST MATERIAL", 29);

	/// navigation sub
	GLint navigationSubMenu = glutCreateMenu(optionSelected);
	glutAddMenuEntry("LIGHT SOURCE", 30);
	glutAddMenuEntry("CAMERA", 31);

	/// crease angles sub
	GLint creaseSubMenu = glutCreateMenu(optionSelected);
	glutAddMenuEntry("15", 32);
	glutAddMenuEntry("25", 33);
	glutAddMenuEntry("35", 34);
	glutAddMenuEntry("OFF", 35);

	/// aabbTree sub
	GLint aabbSubMenu = glutCreateMenu(optionSelected);
	glutAddMenuEntry("SHOW MODEL", 40);
	glutAddMenuEntry("NO MODEL", 41);
	glutAddMenuEntry("ALL LEVELS", 52);
	glutAddMenuEntry("LEVEL 0", 42);
	//glutAddMenuEntry("LEVEL 1", 43);
	glutAddMenuEntry("LEVEL 2", 44);
	//glutAddMenuEntry("LEVEL 3", 45);
	glutAddMenuEntry("LEVEL 4", 46);
	//glutAddMenuEntry("LEVEL 5", 47);
	//glutAddMenuEntry("LEVEL 6", 48);
	//glutAddMenuEntry("LEVEL 7", 49);
	glutAddMenuEntry("LEVEL 8", 50);
	//glutAddMenuEntry("LEVEL 9", 51);
	glutAddMenuEntry("LEVEL - LEAF NODES", 53);

	/// projectile sub
	GLint projectileSubMenu = glutCreateMenu(optionSelected);
	glutAddMenuEntry("PROJECTILE TYPE - STRAIGHT", 54);
	glutAddMenuEntry("PROJECTILE TYPE - STRAIGHT - LONG LASTING", 65);
	glutAddMenuEntry("PROJECTILE TYPE - STRANGE 1", 55);
	glutAddMenuEntry("PROJECTILE TYPE - STRANGE 2", 56);

	/// spline sub
	GLint splineSubMenu = glutCreateMenu(optionSelected);
	glutAddMenuEntry("MOVE CAR", 58);
	glutAddMenuEntry("STOP CAR", 57);
	//glutAddMenuEntry("MOVE CAR", 59);
	glutAddMenuEntry("SHOW SPLINE", 63);
	glutAddMenuEntry("HIDE SPLINE", 64);

	/// create menu
	glutCreateMenu(optionSelected);

	glutAddSubMenu("Viewing", viewSubMenu);
	glutAddSubMenu("Animation Speed", speedSubMenu);
	glutAddSubMenu("Shading Model", shadingSubMenu);
	glutAddSubMenu("Display Lists", displaySubMenu);
	glutAddSubMenu("Framerate", framerateSubMenu);

	glutAddMenuEntry("--------------------", 99);

	glutAddSubMenu("Normal Visualization", normalVisualizationSubMenu);
	glutAddSubMenu("Normals", normalsSubMenu);
	glutAddSubMenu("Texturing", textureSubMenu);
	glutAddSubMenu("Texture", textureModeSubMenu);
	glutAddSubMenu("Minification", minificationModeSubMenu);
	glutAddSubMenu("Magnification", magnificationModeSubMenu);
	glutAddSubMenu("Materials", materialsSubMenu);
	glutAddSubMenu("Navigation", navigationSubMenu);
	glutAddSubMenu("Crease Angle", creaseSubMenu);
	glutAddMenuEntry("--------------------", 99);
	glutAddMenuEntry("Fly", 61);
	glutAddMenuEntry("Drive", 62);
	glutAddSubMenu("Projectiles", projectileSubMenu);
	glutAddMenuEntry("--------------------", 99);
	glutAddSubMenu("BBox Tree", aabbSubMenu);
	glutAddSubMenu("Spline", splineSubMenu);
	glutAddMenuEntry("--------------------", 99);
	glutAddMenuEntry("Restart", 10);
	glutAddMenuEntry("Quit", 0);

	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	//glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** main
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void main(int argc, char* argv[]) {

	/// set double/single buffer display mode properties
	unsigned int displayProperties = 0;
	if (doubleBuffer)
		displayProperties = (GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	else
		displayProperties = (GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);

	/// initialize glut window
	glutInit(&argc,argv);
	glutInitDisplayMode(displayProperties);
	glutInitWindowPosition(position_x, position_y);
	glutInitWindowSize(width,height);
	winId = glutCreateWindow("Michael Martin - openGL");
	
	/// display events
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	/// input events
	glutMouseFunc(mouseButtonEvent);
	glutKeyboardFunc(keyboardEvent);
	glutKeyboardUpFunc(keyboardUpEvent);
	glutSpecialFunc(specialKeyboardEvent);
	glutSpecialUpFunc(specialKeyboardUpEvent);
	glutPassiveMotionFunc(mouseMotionEvent);
	glutMotionFunc(mouseMotionEvent);
	glutMouseWheelFunc(mouseWheelEvent);

	/** INITIALIZATION
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/// start gameEngine
	GameEngine gameEngine;
	/// set global pointer to gameEngine
	g_gameEngine = &gameEngine;

	createMenu();

	glutMainLoop();
}
