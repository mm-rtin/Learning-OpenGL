#ifndef CONTROL_SCREEN_H
#define CONTROL_SCREEN_H

#include <system/Clock.hpp>
#include <system/Screen.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                    ControlScreen
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

class ControlScreen : public Screen {

private:

	/// clock properties
	bool m_freezeClock;
	float m_currentTime;

	/// control properties
	bool m_displayVictory;

	/// mouse properties
	int m_mouseX;
	int m_mouseY;
	bool m_mouseLeftPressed;
	bool m_mouseRightPressed;

	/// objects
	Clock gameClock;

	/// text
	char m_playerName[50];
	char m_victoryText[10];

public:

	ControlScreen();
	~ControlScreen();

	virtual void preload();
	virtual void start();
	virtual void stop();
	virtual void update();
	virtual void setupProjection(int, int);
	virtual void draw();
	
	/// input methods
	virtual void mouseMoved(int, int);
	virtual void mousePressed(int, int, int);
	virtual void mouseReleased(int, int, int);
	virtual void mouseWheelScrolled(int);
	virtual void keyPressed(int);
	virtual void keyReleased(int);

	/// control screen methods
	void resetTimer();
	void stopTimer();
	void displayVictoryMessage();
};

#endif