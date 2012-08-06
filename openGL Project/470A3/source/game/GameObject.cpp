/*
     File:  GameObject.cpp

   Author:  Michael Martin
  Created:  Aug 08, 2010
   Edited:  Aug 08, 2010

  Purpose:	Base class for all game objects - responds to input and game events
     Note:
*/

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
void GameObject::initialize(float x, float y, float z, Mesh* modelMesh, int texture1, int texture2) {

	// load model
	m_model.loadModel(modelMesh, texture1, texture2);
	m_model.initialize();

	// set model properties
	m_model.m_xPos = x;
	m_model.m_yPos = y;
	m_model.m_zPos = z;

	/// initialize properties
	position = Vec3f(0.0f, 0.0f, 0.0f);
	GameObject::x = x;
	GameObject::y = y;
	GameObject::z = z;
	vx = 0.0f;
	vy = 0.0f;
	vz = 0.0f;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** activate
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::activate() {

	m_model.activate();

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

	/// get real time property
	realTime = g_gameEngine->realtimeClock.getElapsedTime();
	AnimationObject* ao;

	///** ANIMATE POSITION (TRANSLATION)
	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	if (!TranslationObjects.empty()) {

		/// get first ao from TranslationObjects queue
		ao = TranslationObjects.front();

		/// calculate relative animation time since startTime
		animationTimeTranslate = realTime - ao->startTime;

		/// update x/y/z properties for tween type
		if (realTime < ao->endTime_x) {
			x = getAnimationUpdate(ao->tweenType_x, ao->begin_x, ao->end_x - ao->begin_x, ao->duration_x, animationTimeTranslate);
		} else {
			x = ao->end_x;
		}
		if (realTime < ao->endTime_y) {
			y = getAnimationUpdate(ao->tweenType_y, ao->begin_y, ao->end_y - ao->begin_y, ao->duration_y, animationTimeTranslate);
		} else {
			y = ao->end_y;
		}
		if (realTime < ao->endTime_z) {
			z = getAnimationUpdate(ao->tweenType_z, ao->begin_z, ao->end_z - ao->begin_z, ao->duration_z, animationTimeTranslate);
		} else {
			z = ao->end_z;
		}

		/// update model position
		m_model.m_xPos = x;
		m_model.m_yPos = y;
		m_model.m_zPos = z;
		position.x = x;
		position.y = y;
		position.z = z;

		/// if ao is complete pop from queue
		if (realTime >= ao->endTime_x && realTime >= ao->endTime_y && realTime >= ao->endTime_z) {
			TranslationObjects.pop_front();
		}
	}

	///** ANIMATE SCALE
	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	if (!ScaleObjects.empty()) {

		/// get first ao from ScaleObjects queue
		ao = ScaleObjects.front();

		/// calculate relative animation time since startTime
		animationTimeScale = realTime - ao->startTime;

		/// update x/y/z properties for tween type
		if (realTime < ao->endTime_x) {
			xScale = getAnimationUpdate(ao->tweenType_x, ao->begin_x, ao->end_x - ao->begin_x, ao->duration_x, animationTimeScale);
		} else {
			xScale = ao->end_x;
		}
		if (realTime < ao->endTime_y) {
			yScale = getAnimationUpdate(ao->tweenType_y, ao->begin_y, ao->end_y - ao->begin_y, ao->duration_y, animationTimeScale);
		} else {
			yScale = ao->end_y;
		}
		if (realTime < ao->endTime_z) {
			zScale = getAnimationUpdate(ao->tweenType_z, ao->begin_z, ao->end_z - ao->begin_z, ao->duration_z, animationTimeScale);
		} else {
			zScale = ao->end_z;
		}

		/// update model position
		m_model.m_xScale = xScale;
		m_model.m_yScale = yScale;
		m_model.m_zScale = zScale;

		/// if ao is complete pop from queue
		if (realTime >= ao->endTime_x && realTime >= ao->endTime_y && realTime >= ao->endTime_z) {
			ScaleObjects.pop_front();
		}
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** hitTest
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool GameObject::hitTest(int x, int y, int z, int radius) {
	
	return false;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** moveTo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::moveTo(float x, float y, float z) {
	GameObject::x = x;
	GameObject::y = y;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** moveBy
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::moveBy(float x, float y, float z) {

	/// update new position - framerate independant
	GameObject::x += x * g_gameEngine->lastFrameTime;
	GameObject::y += y * g_gameEngine->lastFrameTime;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** stopAnimations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::stopAnimations() {

	/// stop translation
	TranslationObjects = list<AnimationObject*>();  
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** animate
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::animate(int animationType, int tweenType_x, int tweenType_y, int tweenType_z, float begin_x, float begin_y, float begin_z, float end_x, float end_y, float end_z, float duration_x, float duration_y, float duration_z, bool queue, int queueID, bool clear) {

	/// return if no animation possible
	if (begin_x == end_x && begin_y == end_y && begin_z == end_z) {
		return;
	}

	///** TRANSLATE
	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	if (animationType == ANIMATION::TRANSLATE) {

		/// Queued Animations will not be added if ID for a queued animation exists on the queue
		list<AnimationObject*>::iterator listIterator;
		for(listIterator = TranslationObjects.begin(); listIterator != TranslationObjects.end(); listIterator++) {
			
			if (queue && queueID == (*listIterator)->queueID) {
				return;
			}
		}

		/// if Clear, clear Queue
		if (clear) {
		
			// delete AnimationObjects on list
			for(listIterator = TranslationObjects.begin(); listIterator != TranslationObjects.end(); listIterator++) {
				delete *listIterator;
			}

			TranslationObjects.clear();
		}
		
		/// create AnimationObject
		AnimationObject* ao;

		if (!queue && !TranslationObjects.empty()) {
			ao = TranslationObjects.front();
		} else {
			ao = new AnimationObject();
			if (queue) {
				ao->queueID = queueID;
			} else {
				ao->queueID = -1;
			}
		}

		/// set start properties
		x = begin_x;
		y = begin_y;
		z = begin_z;

		float cx = end_x - begin_x;
		float cy = end_y - begin_y;
		float cz = end_z - begin_z;

		/// save animation properties
		ao->tweenType_x = tweenType_x;
		ao->tweenType_y = tweenType_y;
		ao->tweenType_z = tweenType_z;

		/// find total duration (longest duration)
		ao->totalDuration = duration_x;
		if (duration_y > ao->totalDuration)
			ao->totalDuration = duration_y;
		if (duration_z > ao->totalDuration)
			ao->totalDuration = duration_z;

		if (queue) {
			if (!TranslationObjects.empty()) {

				AnimationObject* lastAO = TranslationObjects.back();
				ao->begin_x = lastAO->end_x;
				ao->begin_y = lastAO->end_y;
				ao->begin_z = lastAO->end_z;
				ao->end_x = lastAO->end_x + cx;
				ao->end_y = lastAO->end_y + cy;
				ao->end_z = lastAO->end_z + cz;

				ao->startTime = lastAO->totalDuration + lastAO->startTime;
				ao->endTime_x = ao->startTime + duration_x;
				ao->endTime_y = ao->startTime + duration_y;
				ao->endTime_z = ao->startTime + duration_z;
	
			} else {

				ao->begin_x = begin_x;
				ao->begin_y = begin_y;
				ao->begin_z = begin_z;
				ao->end_x = end_x;
				ao->end_y = end_y;
				ao->end_z = end_z;

				ao->startTime = g_gameEngine->realtimeClock.getElapsedTime();
				ao->endTime_x = ao->startTime + duration_x;
				ao->endTime_y = ao->startTime + duration_y;
				ao->endTime_z = ao->startTime + duration_z;
			}

		} else {

				ao->begin_x = begin_x;
				ao->begin_y = begin_y;
				ao->begin_z = begin_z;
				ao->end_x = end_x;
				ao->end_y = end_y;
				ao->end_z = end_z;

				ao->startTime = g_gameEngine->realtimeClock.getElapsedTime();
				ao->endTime_x = ao->startTime + duration_x;
				ao->endTime_y = ao->startTime + duration_y;
				ao->endTime_z = ao->startTime + duration_z;

		}

		ao->duration_x = duration_x;
		ao->duration_y = duration_y;
		ao->duration_z = duration_z;

		/// push ao to vector
		if (queue || TranslationObjects.empty()) {
			TranslationObjects.push_back(ao);
		}


	///** SCALE
	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	} else if (animationType == ANIMATION::SCALE) {

		/// Queued Animations will not be added if ID for a queued animation exists on the queue
		list<AnimationObject*>::iterator listIterator;
		for(listIterator = ScaleObjects.begin(); listIterator != ScaleObjects.end(); listIterator++) {
			
			if (queue && queueID == (*listIterator)->queueID) {
				return;
			}
		}

		/// if Clear, clear Queue
		if (clear) {

			// delete AnimationObjects on list
			for(listIterator = ScaleObjects.begin(); listIterator != ScaleObjects.end(); listIterator++) {
				delete *listIterator;
			}

			ScaleObjects.clear();
		}


		/// create AnimationObject
		AnimationObject* ao;

		if (!queue && !ScaleObjects.empty()) {
			ao = ScaleObjects.front();
		} else {
			ao = new AnimationObject();
			if (queue) {
				ao->queueID = queueID;
			} else {
				ao->queueID = -1;
			}
		}

		/// set start properties
		x = begin_x;
		y = begin_y;
		z = begin_z;

		float cx = end_x - begin_x;
		float cy = end_y - begin_y;
		float cz = end_z - begin_z;

		/// save animation properties
		ao->tweenType_x = tweenType_x;
		ao->tweenType_y = tweenType_y;
		ao->tweenType_z = tweenType_z;

		/// find total duration (longest duration)
		ao->totalDuration = duration_x;
		if (duration_y > ao->totalDuration)
			ao->totalDuration = duration_y;
		if (duration_z > ao->totalDuration)
			ao->totalDuration = duration_z;

		/// QUEUED
		if (queue) {
			if (!ScaleObjects.empty()) {

				AnimationObject* lastAO = ScaleObjects.back();
				ao->begin_x = lastAO->end_x;
				ao->begin_y = lastAO->end_y;
				ao->begin_z = lastAO->end_z;
				ao->end_x = lastAO->end_x + cx;
				ao->end_y = lastAO->end_y + cy;
				ao->end_z = lastAO->end_z + cz;

				ao->startTime = lastAO->totalDuration + lastAO->startTime;
				ao->endTime_x = ao->startTime + duration_x;
				ao->endTime_y = ao->startTime + duration_y;
				ao->endTime_z = ao->startTime + duration_z;
	
			} else {

				ao->begin_x = begin_x;
				ao->begin_y = begin_y;
				ao->begin_z = begin_z;
				ao->end_x = end_x;
				ao->end_y = end_y;
				ao->end_z = end_z;

				ao->startTime = g_gameEngine->realtimeClock.getElapsedTime();
				ao->endTime_x = ao->startTime + duration_x;
				ao->endTime_y = ao->startTime + duration_y;
				ao->endTime_z = ao->startTime + duration_z;
			}

		/// NOT QUEUED
		} else {

			if (ScaleObjects.empty()) {

				ao->begin_x = begin_x;
				ao->begin_y = begin_y;
				ao->begin_z = begin_z;
				ao->end_x = end_x;
				ao->end_y = end_y;
				ao->end_z = end_z;

				ao->startTime = g_gameEngine->realtimeClock.getElapsedTime();
				ao->endTime_x = ao->startTime + duration_x;
				ao->endTime_y = ao->startTime + duration_y;
				ao->endTime_z = ao->startTime + duration_z;

			} else {

				ao->begin_x = begin_x;
				ao->begin_y = begin_y;
				ao->begin_z = begin_z;
				ao->end_x = end_x;
				ao->end_y = end_y;
				ao->end_z = end_z;

				ao->startTime = g_gameEngine->realtimeClock.getElapsedTime();
				ao->endTime_x = ao->startTime + duration_x;
				ao->endTime_y = ao->startTime + duration_y;
				ao->endTime_z = ao->startTime + duration_z;
			}
		}

		ao->duration_x = duration_x;
		ao->duration_y = duration_y;
		ao->duration_z = duration_z;

		/// push ao to vector
		if (queue || ScaleObjects.empty()) {
			ScaleObjects.push_back(ao);
		}

	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** setVelocity
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::setVelocity(float vx, float vy, float vz) {
	GameObject::vx = vx;
	GameObject::vy = vy;
	GameObject::vz = vz;
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
/** setVZ
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::setVZ(float vz) {
	GameObject::vz = vz;
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
/** addVZ
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::addVZ(float vz) {
	GameObject::vz = GameObject::vz + vz;
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
float GameObject::getAnimationUpdate(int tweenType, float begin, float end, float duration, float animationTime) {

	/// select tween type and return updated animation property
	switch (tweenType) {
		case TWEEN::IN_BACK :
			return Animation::inBack(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_CIRC :
			return Animation::inCirc(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_CUBIC :
			return Animation::inCubic(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_EXPO :
			return Animation::inExpo(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_LINEAR :
			return Animation::inLinear(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_OUT_BACK :
			return Animation::inOutBack(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_OUT_CIRC :
			return Animation::inOutCirc(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_OUT_CUBIC :
			return Animation::inOutCubic(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_OUT_EXPO :
			return Animation::inOutExpo(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_OUT_LINEAR :
			return Animation::inOutLinear(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_OUT_QUAD :
			return Animation::inOutQuad(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_OUT_QUART :
			return Animation::inOutQuart(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_OUT_QUINT :
			return Animation::inOutQuint(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_QUAD :
			return Animation::inQuad(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_QUART :
			return Animation::inQuart(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_QUINT :
			return Animation::inQuint(animationTime, begin, end, duration);
			break;
		case TWEEN::IN_SINE :
			return Animation::inSine(animationTime, begin, end, duration);
			break;
		case TWEEN::LINEAR :
			return Animation::inLinear(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_BACK :
			return Animation::outBack(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_BOUNCE :
			return Animation::outBounce(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_CIRC :
			return Animation::outCirc(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_CUBIC :
			return Animation::outCubic(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_EXPO :
			return Animation::outExpo(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_LINEAR :
			return Animation::outLinear(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_QUAD :
			return Animation::outQuad(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_QUART :
			return Animation::outQuart(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_QUINT :
			return Animation::outQuint(animationTime, begin, end, duration);
			break;
		case TWEEN::OUT_SINE :
			return Animation::outSine(animationTime, begin, end, duration);
			break;
		default :
			return Animation::outCubic(animationTime, begin, end, duration);
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** draw - draw m_model
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::draw() {

	// draw model
	m_model.draw();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** regenerateDisplayList - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameObject::regenerateDisplayList() {

	/// regenerate model
	m_model.regenerateDisplayList();
}