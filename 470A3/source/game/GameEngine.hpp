#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <windows.h>

#include <system/EventManager.hpp>
#include <system/ScreenManager.hpp>
#include <system/ResourceManager.hpp>
#include <system/Clock.hpp>
#include <graphics/Primitives.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                      GameEngine
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

class GameEngine : public InputEventListener {

private:

	/// propeties
	float frameRateLimit;

	/// graphics classes
	Primitives primitives;

	/// manager classes
	EventManager eventManager;
	ScreenManager screenManager;
	ResourceManager resourceManager;

	/// utility classes
	Clock systemClock;

	/// screen pointers
	GameScreen* m_gameScreen;
	ControlScreen* m_controlScreen;

	// methods
	void recalculateNormals();

public:

	/// public clock
	Clock realtimeClock;

	/// properties
	float lastFrameTime;

	/// Constructor/Destructor
	GameEngine();
	~GameEngine();

	void setupProjection(int, int);
	void update();
	void display();

	/// InputEventListener interface
	virtual bool onInputEvent(sf::Event&);

	/// menu listener
	void menuOption(int);
};
#endif