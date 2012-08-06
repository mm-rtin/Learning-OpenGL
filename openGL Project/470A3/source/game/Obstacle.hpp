#ifndef OBSTACLE_H
#define OBSTACLE_H


#include <game/GameObject.hpp>
#include <graphics/PrimitiveModel.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                     Obstacle
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/
class Obstacle : public GameObject {

private: 

	/// primitive meshes
	Mesh* m_cubeMesh;
	Mesh* m_cylinderMesh;
	Mesh* m_icosahedron;

public:

	/// models
	PrimitiveModel m_model;
	PrimitiveModel m_component1;
	PrimitiveModel m_component2;
	PrimitiveModel m_component3;
	PrimitiveModel m_component4;
	PrimitiveModel m_component5;

	/// array of primitive objects
	std::vector<PrimitiveModel*> componentModels;

	/// methods
	Obstacle();
	~Obstacle();

	/** virtual
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/// initialization
	virtual void initialize(float x, float y, float z, float width, float length, float height, Mesh* mesh, int texture1, int texture2);
	virtual void activate();
	virtual void deactivate();

	/// updates / testing
	virtual void update();

	/// check properties
	virtual bool isOffScreen();
	virtual bool hitTest(Vec3f&);

	/// events
	virtual bool onInputEvent(sf::Event&);
	virtual bool onGameEvent(GameEvent&);

	virtual void draw();

	/** obstacle methods
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	void createComponentModels();
	void regenerateDisplayList();

};
#endif