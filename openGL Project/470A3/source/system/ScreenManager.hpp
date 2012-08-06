#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <vector>

#include <system/Screen.hpp>
#include <game/GameScreen.hpp>
#include <game/ControlScreen.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                   Enum Screens
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/
namespace SCREENS {
	enum screenNames { 
		START_SCREEN,
		GAME_SCREEN,
		CONTROL_SCREEN,
		NUM_SCREENS
	};
}

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                   ScreenManager
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

class ScreenManager {

private:

	/// screen objects
	std::vector<Screen*> m_loadedScreens;

	/// screens
	GameScreen m_gameScreen;
	ControlScreen m_controlScreen;

	/// methods
	Screen* loadScreen(Screen&);

public:
	ScreenManager();
	~ScreenManager();

	void preloadScreen(SCREENS::screenNames);
	Screen* addScreen(SCREENS::screenNames, float, float, float, float);
	//void removeScreen(SCREENS::screenNames);

	void setupScreenProjections(int, int);
	void updateScreens();
	void displayScreens();

	/// input methods
	void mouseMoved(int, int);
	void mousePressed(int, int, int);
	void mouseReleased(int, int, int);
	void mouseWheelScrolled(int);
	void keyPressed(int);
	void keyReleased(int);
};
#endif