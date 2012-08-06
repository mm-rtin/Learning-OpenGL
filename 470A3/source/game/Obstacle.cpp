/*
     File:  Obstacle.cpp

   Author:  Michael Martin
  Created:  Sep 20, 2010
   Edited:  Sep 20, 2010

  Purpose:	
     Note:
*/

#include <random>

#include <game/Obstacle.hpp>
#include <system/Globals.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                      Obstacle
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Obstacle::Obstacle() {

}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Destructor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Obstacle::~Obstacle() {

}
 
/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** initialize
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Obstacle::initialize(float x, float y, float z, float width, float length, float height, Mesh* mesh, int texture1, int texture2) {

	/// load obstacle model
	m_model.loadModel(mesh, texture1, texture2);
	m_model.initialize();

	// set model properties
	m_model.m_xPos = x;
	m_model.m_yPos = y;
	m_model.m_zPos = z;
	m_model.m_xScale = width;
	m_model.m_yScale = height;
	m_model.m_zScale = length;

	/// initialize properties
	GameObject::x = x;
	GameObject::y = y;
	GameObject::z = z;
	vx = 0.0f;
	vy = 0.0f;
	vz = 0.0f;

	/// initialize objects
	m_cubeMesh = g_primitives->m_cubeMesh;
	m_cylinderMesh = g_primitives->m_cylinderMesh;
	m_icosahedron = g_primitives->m_icosahedron;

	/// create component models
	//createComponentModels();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** createComponentModels
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Obstacle::createComponentModels() {

	/// create component models
	m_component1.loadModel(m_cubeMesh, 0, 1);
	m_component2.loadModel(m_cubeMesh, 0, 1);
	m_component3.loadModel(m_cubeMesh, 0, 1);
	m_component4.loadModel(m_cubeMesh, 0, 1);
	m_component5.loadModel(m_cubeMesh, 0, 1);

	/// add components to component models vector
	//componentModels.push_back(&m_component1);
	//componentModels.push_back(&m_component2);
	//componentModels.push_back(&m_component3);
	//componentModels.push_back(&m_component4);
	//componentModels.push_back(&m_component5);
	
	// random number generator
	std::tr1::mt19937 e;
	// set seed
	e.seed(g_systemClock->getSystemTime() + GameObject::x + GameObject::y + GameObject::z);

	// random numbers for position
	std::tr1::uniform_real<float> rpx(-m_model.m_xScale, m_model.m_xScale);
	std::tr1::uniform_real<float> rpy(-m_model.m_yScale, m_model.m_yScale);
	std::tr1::uniform_real<float> rpz(-m_model.m_zScale, m_model.m_zScale);
	// random numbers for scale
	std::tr1::uniform_real<float> rsx(.2, m_model.m_xScale);
	std::tr1::uniform_real<float> rsy(.2, m_model.m_yScale);
	std::tr1::uniform_real<float> rsz(.2, m_model.m_zScale);
	// random numbers for rotation
	std::tr1::uniform_real<float> rra(0.0, 15.0);
	std::tr1::uniform_real<float> rrvx(-1.0, 1.0);
	std::tr1::uniform_real<float> rrvy(-1.0, 1.0);
	std::tr1::uniform_real<float> rrvz(-1.0, 1.0);

	// base color
	float color_r = m_model.m_color1.x;
	float color_g = m_model.m_color1.y;
	float color_b = m_model.m_color1.z;

	// color increment
	float ci = .2;

	// for each component create random: position/scale/rotation and color based on base model
	for (unsigned int i = 0; i < componentModels.size(); ++i) {

		// uniform scale
		float uniformScale = rsx(e);

		// position
		componentModels[i]->m_xPos = x + rpx(e);
		componentModels[i]->m_yPos = y + rpy(e);
		componentModels[i]->m_zPos = z + rpz(e);
		// scale
		componentModels[i]->m_xScale = uniformScale;
		componentModels[i]->m_yScale = uniformScale;
		componentModels[i]->m_zScale = uniformScale;
		// color
		componentModels[i]->m_color1 = Vec3f(color_r, color_g, color_b);
		componentModels[i]->m_color2 = Vec3f(color_r, color_g, color_b);
		// rotation
		componentModels[i]->m_xRotate = rrvx(e);
		componentModels[i]->m_yRotate = rrvy(e);
		componentModels[i]->m_zRotate = rrvz(e);
		componentModels[i]->m_rotateAngle = rra(e);

		// initialize component
		componentModels[i]->activate();

		// increment colors
		color_r += ci;
		color_g += ci;
		color_b += ci;
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** activate
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Obstacle::activate() {

	m_model.activate();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** deactivate
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Obstacle::deactivate() {
}

 
/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** update
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Obstacle::update() {

	// call base method
	GameObject::update();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** hitTest
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool Obstacle::hitTest(Vec3f& v) {
	
	return m_model.checkCollision(v);
}


/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** isOffScreen
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool Obstacle::isOffScreen() {
	return Obstacle::offScreen;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** onInputEvent
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool Obstacle::onInputEvent(sf::Event& inputEvent) {
	return true;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** onGameEvent
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool Obstacle::onGameEvent(GameEvent& gameEvent) {
	return true;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** draw - draw obstacles
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Obstacle::draw() {

	glPushMatrix();

		/// draw model
		m_model.draw();
		/// draw components
		for (unsigned int i = 0; i < componentModels.size(); ++i) {
			componentModels[i]->draw();
		}

	glPopMatrix();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** regenerateDisplayList - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Obstacle::regenerateDisplayList() {

	/// regenerate model
	m_model.regenerateDisplayList();
	/// regenerate components
	for (unsigned int i = 0; i < componentModels.size(); ++i) {
		componentModels[i]->regenerateDisplayList();
	}
}