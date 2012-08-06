#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <SFML/Window.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <system/Animation.hpp>

/// SFMLGame
#include <system/EventManager.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                     GameObject
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/
class GameObject : public InputEventListener, public GameEventListener {

private: 

	/// methods
	float getAnimationUpdate(int, float, float, float);

public:

	/// images
	sf::Image objectImage;
	sf::Image* objectImagePtr;

	/// sprites
	sf::Sprite objectSprite;
	sf::Sprite* hitSprite;
	sf::Sprite* renderSprite;

	/// bounding box
	sf::IntRect boundingBox;

	/// object properties
	float x;
	float y;
	float vx;
	float vy;
	float speed;
	bool offScreen;

	/// time properties
	double realTime;
	double animationTime;

	/// animation properties
	bool moving_x;
	bool moving_y;
	int tweenType_x;
	int tweenType_y;
	float begin_x;
	float begin_y;
	float end_x;
	float end_y;
	float startTime;
	float endTime_x;
	float endTime_y;
	float duration_x;
	float duration_y;

	/// objects
	Animation animation;

	/// methods
	GameObject();
	virtual ~GameObject();

	/** pure virtual
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/// initialization
	virtual void initialize(float, float, float) = 0;
	virtual void activate() = 0;
	virtual void deactivate() = 0;

	/// updates / testing
	virtual void update() = 0;

	/** virtual
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/// movement
	virtual void moveTo(float, float);
	virtual void moveBy(float, float);
	virtual void animate(int tweenType_x, int tweenType_y, float begin_x, float begin_y, float end_x, float end_y, float duration_x, float duration_y);

	/// set properties
	virtual void setVelocity(float, float);
	virtual void setVX(float);
	virtual void setVY(float);
	virtual void addVX(float);
	virtual void addVY(float);

	/// check properties
	virtual bool isOffScreen();
	virtual bool hitTest(int, int, int);

	/// events
	virtual bool onInputEvent(sf::Event&);
	virtual bool onGameEvent(GameEvent&);

};
#endif