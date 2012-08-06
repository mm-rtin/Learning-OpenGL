#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <list>

#include <system/Animation.hpp>
#include <graphics/ModelObject.hpp>
#include <graphics/PrimitiveModel.hpp>
#include <system/EventManager.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                     GameObject
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/
class GameObject : public InputEventListener, public GameEventListener {

private: 


public:

	/// models
	PrimitiveModel m_model;

	/// animation structure
	struct AnimationObject {
		int queueID;
		bool moving_x;
		bool moving_y;
		bool moving_z;
		int tweenType_x;
		int tweenType_y;
		int tweenType_z;
		float begin_x;
		float begin_y;
		float begin_z;
		float end_x;
		float end_y;
		float end_z;
		float startTime;
		float endTime_x;
		float endTime_y;
		float endTime_z;
		float duration_x;
		float duration_y;
		float duration_z;
		float totalDuration;
    };

	/// translate properties
	Vec3f position;
	float x;
	float y;
	float z;
	float vx;
	float vy;
	float vz;
	float speed;
	bool offScreen;

	/// scale properties
	float xScale;
	float yScale;
	float zScale;

	/// time properties
	float realTime;
	float animationTimeTranslate;
	float animationTimeScale;

	/// objects
	std::list<AnimationObject*> TranslationObjects;
	std::list<AnimationObject*> ScaleObjects;

	/// methods
	GameObject();
	virtual ~GameObject();

	/** virtual
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/// initialization
	virtual void initialize(float x, float y, float z, Mesh* modelMesh, int, int);
	virtual void activate();
	virtual void deactivate();

	/// updates / testing
	virtual void update();
	float getAnimationUpdate(int, float, float, float, float);

	/// movement
	virtual void moveTo(float x, float y, float z);
	virtual void moveBy(float x, float y, float z);
	virtual void stopAnimations();
	virtual void animate(int animationType, int tweenType_x, int tweenType_y, int tweenType_z, float begin_x, float begin_y, float begin_z, float end_x, float end_y, float end_z, float duration_x, float duration_y, float duration_z, bool queue, int queueID, bool clear);

	/// set properties
	virtual void setVelocity(float, float, float);
	virtual void setVX(float);
	virtual void setVY(float);
	virtual void setVZ(float);
	virtual void addVX(float);
	virtual void addVY(float);
	virtual void addVZ(float);

	/// check properties
	virtual bool isOffScreen();
	virtual bool hitTest(int, int, int, int);

	/// events
	virtual bool onInputEvent(sf::Event&);
	virtual bool onGameEvent(GameEvent&);

	virtual void draw();

	void regenerateDisplayList();
};
#endif