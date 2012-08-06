#ifndef SCREEN_H
#define SCREEN_H

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                      Screen
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

#include <system/EventManager.hpp>

class Screen {

private:

public:

	/// screen properties
	int m_windowWidth;
	int m_windowHeight;
	int m_viewWidth;
	int m_viewHeight;
	int m_centerWidth;
	int m_centerHeight;

	/// viewport properties
	int m_viewPort_xPosition;
	int m_viewPort_yPosition;
	float m_viewPortx;
	float m_viewPorty;
	float m_viewPortWidth;
	float m_viewPortHeight;

	Screen();
	virtual ~Screen();

	bool isPreloaded;
	bool isStarted;

	virtual void preload() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void update() = 0;
	virtual void setupProjection(int, int) = 0;
	virtual void draw() = 0;

	/// input methods
	virtual void mouseMoved(int, int) = 0;
	virtual void mousePressed(int, int, int) = 0;
	virtual void mouseReleased(int, int, int) = 0;
	virtual void mouseWheelScrolled(int) = 0;
	virtual void keyPressed(int) = 0;
	virtual void keyReleased(int) = 0;

	virtual void setViewportParameters(float, float, float, float);
};

#endif