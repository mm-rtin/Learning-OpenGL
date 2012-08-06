/*
     File:  GameObject.cpp

   Author:  Michael Martin
  Created:  Aug 08, 2010
   Edited:  Aug 08, 2010

  Purpose:	Base class for all game objects - responds to input and game events
     Note:
*/

/// SFMLGame
#include <game/GameObject.hpp>
#include <system/Globals.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                      GameObject
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
GameObject::GameObject() {

}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Destructor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
GameObject::~GameObject() {

}
 
/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** initialize
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::initialize(float x, float y, float speed) {

	/// initialize properties
	moving_x = false;
	moving_y = false;
	GameObject::x = x;
	GameObject::y = y;
	vx = 0;
	vy = 0;

	GameObject::speed = speed;

	/// set object properties
	objectImage.SetSmooth(false);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** activate
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::activate() {
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** deactivate
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::deactivate() {
}

 
/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** update
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::update() {

	if (!moving_x && !moving_y) return;

	/// get real time property
	realTime = g_gameEngine->m_clock.GetElapsedTime();

	/// calculate relative animation time since startTime
	animationTime = realTime - startTime;

	/// update x/y properties for tween type
	if (realTime < endTime_x) {
		x = getAnimationUpdate(tweenType_x, begin_x, end_x - begin_x, duration_x);
	} else {
		x = end_x;
		moving_x = false;
	}
	if (realTime < endTime_y) {
		y = getAnimationUpdate(tweenType_y, begin_y, end_y - begin_y, duration_y);
	} else {
		y = end_y;
		moving_y = false;
	}

	/// update bounding box
	boundingBox.Left = x;
	boundingBox.Top = y;

	/// set new sprite position
	objectSprite.SetPosition(x, y);

	/// offScreen check
	if (x > g_window->GetWidth() || x < 0 || y >= g_window->GetHeight() || y < 0) {
		offScreen = true;
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** hitTest
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool GameObject::hitTest(int x, int y, int radius) {
	
	/// check bounding box first
	if (boundingBox.Contains(x, y)) {
		
		sf::Vector2f localPoint = hitSprite->TransformToLocal(sf::Vector2f(x, y));
		sf::Color colorAtPoint = hitSprite->GetPixel(localPoint.x, localPoint.y);

		/// if alpha value at mouse pointer is NOT 0 (invisible) register hit
		if (colorAtPoint.a)
			return true;
	}
	return false;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** moveTo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::moveTo(float x, float y) {
	GameObject::x = x;
	GameObject::y = y;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** moveBy
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::moveBy(float x, float y) {

	/// update new position - framerate independant
	GameObject::x += x * g_window->GetFrameTime();
	GameObject::y += y * g_window->GetFrameTime();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** animate
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::animate(int tweenType_x, int tweenType_y, float begin_x, float begin_y, float end_x, float end_y, float duration_x, float duration_y) {

	/// set moving properties
	if (begin_x != end_x) {
		moving_x = true;
	}

	if (begin_y != end_y) {
		moving_y = true;
	}

	/// set start properties
	x = begin_x;
	y = begin_y;

	/// save animation properties
	GameObject::tweenType_x = tweenType_x;
	GameObject::tweenType_y = tweenType_y;
	GameObject::begin_x = begin_x;
	GameObject::begin_y = begin_y;
	GameObject::end_x = end_x;
	GameObject::end_y = end_y;
	GameObject::startTime = g_gameEngine->m_clock.GetElapsedTime();
	GameObject::endTime_x = startTime + duration_x;
	GameObject::endTime_y = startTime + duration_y;
	GameObject::duration_x = duration_x;
	GameObject::duration_y = duration_y;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** setVelocity
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::setVelocity(float vx, float vy) {
	GameObject::vx = vx;
	GameObject::vy = vy;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** setVX
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::setVX(float vx) {
	GameObject::vx = vx;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** setVY
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::setVY(float vy) {
	GameObject::vy = vy;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** addVX
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::addVX(float vx) {
	GameObject::vx = GameObject::vx + vx;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** addVY
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::addVY(float vy) {
	GameObject::vy = GameObject::vy + vy;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** isOffScreen
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool GameObject::isOffScreen() {
	return GameObject::offScreen;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** onInputEvent
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool GameObject::onInputEvent(sf::Event& inputEvent) {
	return true;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** onGameEvent
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool GameObject::onGameEvent(GameEvent& gameEvent) {
	return true;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** getAnimationUpdate
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
float GameObject::getAnimationUpdate(int tweenType, float begin, float end, float duration) {

	/// select tween type and return updated animation property
	switch (tweenType) {
		case TWEEN::IN_BACK :
			return animation.inBack(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_CIRC :
			return animation.inCirc(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_CUBIC :
			return animation.inCubic(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_EXPO :
			return animation.inExpo(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_LINEAR :
			return animation.inLinear(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_OUT_BACK :
			return animation.inOutBack(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_OUT_CIRC :
			return animation.inOutCirc(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_OUT_CUBIC :
			return animation.inOutCubic(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_OUT_EXPO :
			return animation.inOutExpo(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_OUT_LINEAR :
			return animation.inOutLinear(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_OUT_QUAD :
			return animation.inOutQuad(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_OUT_QUART :
			return animation.inOutQuart(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_OUT_QUINT :
			return animation.inOutQuint(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_QUAD :
			return animation.inQuad(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_QUART :
			return animation.inQuart(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_QUINT :
			return animation.inQuint(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_SINE :
			return animation.inSine(animationTime, begin, end, duration);
			break;
		case TWEEN::LINEAR :
			return animation.inLinear(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_BACK :
			return animation.outBack(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_BOUNCE :
			return animation.outBounce(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_CIRC :
			return animation.outCirc(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_CUBIC :
			return animation.outCubic(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_EXPO :
			return animation.outExpo(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_LINEAR :
			return animation.outLinear(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_QUAD :
			return animation.outQuad(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_QUART :
			return animation.outQuart(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_QUINT :
			return animation.outQuint(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_SINE :
			return animation.outSine(animationTime, begin, end, duration);
			break;
	}
}