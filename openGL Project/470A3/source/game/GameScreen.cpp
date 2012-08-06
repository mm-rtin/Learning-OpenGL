/*
     File:	GameScreen.cpp

  Authors:	Michael Martin
  Created:	Sep 10, 2010

  Purpose:	
	 Note:
*/

#include <Windows.h>
#include <math.h>
#include <random>

#include <system/Globals.hpp>
#include <game/GameScreen.hpp>
#include <system/Math3d.hpp>
#include <ImathMatrix.h>

#define PI 3.141592653589793f
#define DEG2RAD(x) (x*PI)/180.0f
#define RAD2DEG(x) (x/PI)*180.0f

/// MS openGL doesn't include these definitions
/// allow for specular light with textures
#ifndef SEPARATE_SPECULAR_COLOR
#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
#endif
#ifndef GL_LIGHT_MODEL_COLOR_CONTROL
#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81F8
#endif

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                      GameScreen
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Constructor - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
GameScreen::GameScreen() 
	: Screen() {

	/// initialize properties
	// screen properties
	m_viewMode = 2;
	m_navigationMode = 0;
	m_initialWidth = 0;
	m_initialHeight = 0;

	// light properties
    m_ambientLight[0] = 0.0f;
	m_ambientLight[1] = 0.0f;
	m_ambientLight[2] = 0.0f;
	m_ambientLight[3] = 1.0f;

	m_diffuseLight[0] = 1.0f;
	m_diffuseLight[1] = 1.0f;
	m_diffuseLight[2] = 1.0f;
	m_diffuseLight[3] = 1.0f;

	m_specular[0] = 1.0f;
	m_specular[1] = 1.0f;
	m_specular[2] = 1.0f;
	m_specular[3] = 1.0f;

	/// materials
	// no mat
	m_no_mat[0] = 0.0f;
	m_no_mat[1] = 0.0f;
	m_no_mat[2] = 0.0f;
	m_no_mat[3] = 1.0f;
	// high ambient
	m_mat_high_ambient[0] = 0.8f;
	m_mat_high_ambient[1] = 0.8f;
	m_mat_high_ambient[2] = 0.8f;
	m_mat_high_ambient[3] = 1.0f;
	// low ambient
	m_mat_low_ambient[0] = 0.5f;
	m_mat_low_ambient[1] = 0.5f;
	m_mat_low_ambient[2] = 0.5f;
	m_mat_low_ambient[3] = 1.0f;
	// ambient color
	m_mat_ambient_color[0] = 0.6f;
	m_mat_ambient_color[1] = 0.4f;
	m_mat_ambient_color[2] = 0.4f;
	m_mat_ambient_color[3] = 1.0f;
	// high diffuse
	m_mat_high_diffuse[0] = 1.0f;
	m_mat_high_diffuse[1] = 1.0f;
	m_mat_high_diffuse[2] = 1.0f;
	m_mat_high_diffuse[3] = 1.0f;
	// low diffuse
	m_mat_high_diffuse[0] = 0.5f;
	m_mat_high_diffuse[1] = 0.5f;
	m_mat_high_diffuse[2] = 0.5f;
	m_mat_high_diffuse[3] = 1.0f;
	// high specular
	m_mat_high_specular[0] = 1.0f;
	m_mat_high_specular[1] = 1.0f;
	m_mat_high_specular[2] = 1.0f;
	m_mat_high_specular[3] = 1.0f;
	// low specular
	m_mat_low_specular[0] = 0.3f;
	m_mat_low_specular[1] = 0.3f;
	m_mat_low_specular[2] = 0.3f;
	m_mat_low_specular[3] = 1.0f;
	// emission
	m_mat_emission[0] = 0.4f;
	m_mat_emission[1] = 0.4f;
	m_mat_emission[2] = 0.4f;
	m_mat_emission[3] = 1.0f;
	// low shininess
	m_mat_low_shininess[0] = 50.0f;
	// high shininess
	m_mat_high_shininess[0] = 25.0f;

	// light location/direction
	m_lightDistanceIncrement = 10.0f;
	m_lightAngleX = -150.0f;
	m_lightAngleY = 82.0f;
	m_lightAngleXSpeed = 65.0f;
	m_lightAngleYSpeed = 65.0f;;
	m_lightAngleXVelocity = 0.0f;
	m_lightAngleYVelocity = 0.0f;

	m_lightPos[0] = 0.0f;
	m_lightPos[1] = 0.0f;
	m_lightPos[2] = 280.0f;
	m_lightPos[3] = 1.0f;

	m_spotDir[0] = 0.0f;
	m_spotDir[1] = 0.0f;
	m_spotDir[2] = -1.0f;

	// world properties
	m_worldWidth = 250;
	m_worldHeight = 250;
	m_groundPlanePosition = 0.0f;
	m_worldPlaneSquareSize = 4.0;
	m_backgroundPlaneSquareSize = 20.0;
	m_requiredObjectsBoundaryPadding = 8.0;
	m_requiredObjectsScale = 4.0;

	// game properties
	m_animationSpeed = 1.0;
	m_victory = false;
	m_numberOfObstacles = 250;
	m_obstacleBoundaryPadding = 20;
	m_obstacleMaxWidth = 6.0f;
	m_obstacleMaxLength = 6.0f;
	m_obstacleMaxHeight = 9.0f;

	// player movement
	m_movementVector = Vec3f(0.0f, 0.0f, 0.0f);
	m_rotationAngle = 0.0f;
	m_movementSpeed = 0.0f;
	m_activeRotationVelocity = 0.0f;
	m_rotationVelocity = 0.0f;
	m_movementVelocity = 0.0f;
	m_verticalMovementSpeed = 15.0f;
	m_verticalMovementVelocity = 0.0f;
	m_movementDirection = 1;
	m_turningLeft = false;
	m_turningRight = false;

	m_movementSpeedLimit = 50.0f;
	m_rotationIncrement = 50.0f;
	m_accelerationSpeed = 50.0f;
	m_brakingSpeed = 40.0f;

	// player model properties
	m_playerHeading = 0.0f;
	m_playerPitch = 0.0f;
	m_playerRoll = 0.0f;

	// mouse properties
	m_mouseLeftPressed = false;
	m_mouseRightPressed = false;
	m_mouseX = 0;
	m_mouseY = 0;

	// view properties
	m_rotateAngle = 0.0f;
	m_viewX = 0.0f;
	m_viewY = 0.0f;
	m_viewZ = 0.0f;
	m_viewYOffset = 3.0f;

	// camera properties - first person
	m_cameraX = 0.0f;
	m_cameraY = 0.0;
	m_cameraZ = 0.0f;
	m_cameraDistance = 7.0f;
	m_distanceIncrement = .2f;
	m_cameraElevation = 2.6f;
	m_cameraElevationIncrement1 = .15f;
	m_cameraElevationIncrement2 = .15f;
	m_cameraRotationIncrement = .2f;

	// camera properties - overview
	m_viewRotateAngle = 0.0f;
	m_viewCameraX = 0.0f;
	m_viewCameraY = 40.0f;
	m_viewCameraZ = 0.0f;
	m_viewCameraLookY = 10.0f;
	m_viewCameraDistance = 250.0f;

	m_viewRotationSpeed = 65.0f;
	m_viewHeightSpeed = 65.0f;
	m_viewRotationVelocity = 0.0f;
	m_viewHeightVelocity = 0.0f;
	m_viewDistanceIncrement = 12.0f;
	m_viewDistanceSpeed = 60.0f;
	m_viewDistanceVelocity = 0.0f;

	// orthographic properties
	m_orthoSize = 52;
	m_orthoSizeIncrement = 3.0f;

	// rotating box properties
	m_boxAngle = 0.0f;
	m_boxRotationIncrement = 20.0f;
	m_box2_x = 0.0f;
	m_box2_z = 0.0f;

	// rotating box 2 properties
	m_box2Angle = 0.0f;
	m_boxRotation2Increment = 20.0f;

	// projectile properties
	m_projectilePoolCount = 250;
	m_projectileDistance = 300.0f;
	m_currentProjectileID = 0;
	/// the duration it takes to cover the distance in seconds
	m_projectileSpeed = 5.0f;
	m_projectileFallRate = .9f;
	m_shooting = false;
	m_lastFired = 0.0f;
	m_firingRate = .05;

	// spline properites
	m_splineSegments = 12;
	m_splineSteps = 500;
	m_currentSplineIndex = 0;

}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Destructor - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
GameScreen::~GameScreen() {


}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** preload - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::preload() {

	// Base class method
	Screen::preload();

	/// load models
	loadModels();

	/// load game objects
	loadGameObjects();

	/// spline display list
	m_splineDL = glGenLists(1);

	cout << "PRELOAD > DONE" << endl;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** loadModels
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::loadModels() {

	/// asu model
	m_asuModel.m_xPos = 30.0;
	m_asuModel.m_yPos = -60.0f;
	m_asuModel.m_zPos = m_worldHeight / 3;
	m_asuModel.m_xScale = m_requiredObjectsScale * 3.0f;
	m_asuModel.m_yScale = m_requiredObjectsScale * 3.0f;
	m_asuModel.m_zScale = m_requiredObjectsScale * 3.0f;
	// rotate asu
	m_asuModel.m_xRotate1 = 0.0f;
	m_asuModel.m_yRotate1 = 1.0f;
	m_asuModel.m_zRotate1 = 0.0f;
	m_asuModel.m_rotateAngle1 = 180.0f;
	m_asuModel.loadModel(g_resourceManager->m_modelMeshes[5]);

	/// load player model (required model #1)
	m_playerModel.m_yPos = 0.0;
	m_playerModel.m_xScale = 1.2f;
	m_playerModel.m_yScale = 1.2f;
	m_playerModel.m_zScale = 1.2f;
	// correct model orientation
	m_playerModel.m_xRotate1 = 1.0f;
	m_playerModel.m_yRotate1 = 0.0f;
	m_playerModel.m_zRotate1 = 0.0f;
	m_playerModel.m_rotateAngle1 = 270.0f;
	m_playerModel.loadModel(g_resourceManager->m_modelMeshes[0]);

	/// load remaining 5 required models
	/// car model1
	m_carModel1.m_xPos = m_worldWidth / 2 + m_requiredObjectsBoundaryPadding;
	m_carModel1.m_yPos = 0.0f;
	m_carModel1.m_zPos = 0.0f;
	m_carModel1.m_xScale = m_requiredObjectsScale * 2.0f;
	m_carModel1.m_yScale = m_requiredObjectsScale * 2.0f;
	m_carModel1.m_zScale = m_requiredObjectsScale * 2.0f;
	// correct model orientation
	m_carModel1.m_xRotate1 = 1.0f;
	m_carModel1.m_yRotate1 = 0.0f;
	m_carModel1.m_zRotate1 = 0.0f;
	m_carModel1.m_rotateAngle1 = 270.0f;
	m_carModel1.loadModel(g_resourceManager->m_modelMeshes[1]);
	
	/// car model2
	m_carModel2.m_xPos = -m_worldWidth / 2 - m_requiredObjectsBoundaryPadding;
	m_carModel2.m_yPos = 0.0f;
	m_carModel2.m_zPos = 0.0f;
	m_carModel2.m_xScale = m_requiredObjectsScale * 2.0f;
	m_carModel2.m_yScale = m_requiredObjectsScale * 2.0f;
	m_carModel2.m_zScale = m_requiredObjectsScale * 2.0f;
	// correct model orientation
	m_carModel2.m_xRotate1 = 1.0f;
	m_carModel2.m_yRotate1 = 0.0f;
	m_carModel2.m_zRotate1 = 0.0f;
	m_carModel2.m_rotateAngle1 = 270.0f;
	m_carModel2.loadModel(g_resourceManager->m_modelMeshes[2]);

	/// building model
	m_buildingModel.m_xPos = 0.0f;
	m_buildingModel.m_yPos = 4.0f;
	m_buildingModel.m_zPos = m_worldHeight / 2 + m_requiredObjectsBoundaryPadding * 2;
	m_buildingModel.m_xScale = .01f;
	m_buildingModel.m_yScale = .01f;
	m_buildingModel.m_zScale = .01f;
	// rotate building
	m_buildingModel.m_xRotate1 = 0.0f;
	m_buildingModel.m_yRotate1 = 1.0f;
	m_buildingModel.m_zRotate1 = 0.0f;
	m_buildingModel.m_rotateAngle1 = 90.0f;
	m_buildingModel.loadModel(g_resourceManager->m_modelMeshes[3]);

	/// al model
	m_alModel.m_xPos = 0.0f;
	m_alModel.m_yPos = 26.0f;
	m_alModel.m_zPos = 0.0f;
	m_alModel.m_xScale = m_requiredObjectsScale * 2.0;
	m_alModel.m_yScale = m_requiredObjectsScale * 2.0;
	m_alModel.m_zScale = m_requiredObjectsScale * 2.0;
	m_alModel.loadModel(g_resourceManager->m_modelMeshes[4]);

	/// add models to models vector
	modelObjects.push_back(&m_playerModel);
	modelObjects.push_back(&m_carModel1);
	modelObjects.push_back(&m_carModel2);
	modelObjects.push_back(&m_buildingModel);
	modelObjects.push_back(&m_alModel);
	modelObjects.push_back(&m_asuModel);
	
	/// activate required models
	for (unsigned int i = 0; i < modelObjects.size(); ++i) {
		modelObjects[i]->activate();
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** loadGameObjects
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::loadGameObjects() {

	/// load projectiles
	for (unsigned int i = 0; i < m_projectilePoolCount; i++) {

		// create projectile
		GameObject* projectile = new GameObject();
		projectile->initialize(0.0, -100.0, 0.0, g_primitives->m_icosahedron, 8, 8);
		playerProjectiles.push_back(projectile);
		projectile->activate();
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** createEnvironmentObjects
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::createEnvironmentObjects() {

	/// CREATE PLANES
	// world plane
	Mesh* worldPlaneMesh = g_primitives->createPlaneMesh(m_worldWidth, m_worldHeight, m_worldPlaneSquareSize);
	m_worldPlane.loadModel(worldPlaneMesh, 0, 0);
	m_worldPlane.m_yPos = m_groundPlanePosition;
	m_worldPlane.activate();

	// background plane
	Mesh* backgroundPlaneMesh = g_primitives->createPlaneMesh(m_worldWidth * 3, m_worldHeight * 3, m_backgroundPlaneSquareSize);
	m_backgroundPlane.loadModel(backgroundPlaneMesh, 10, 10);
	m_backgroundPlane.m_yPos = m_groundPlanePosition - 0.1;
	m_backgroundPlane.m_color1 = Vec3f(0.0f, 0.95f, 1.0f);
	m_backgroundPlane.m_color2 = Vec3f(0.0f, 0.85f, 0.9f);
	m_backgroundPlane.activate();

	// start plane
	Mesh* worldSquarePlaneMesh = g_primitives->createPlaneMesh(m_worldPlaneSquareSize, m_worldPlaneSquareSize, m_worldPlaneSquareSize);
	m_startPlane.loadModel(worldSquarePlaneMesh, 8, 8);
	m_startPlane.m_xPos = -m_worldWidth / 2 + m_worldPlaneSquareSize / 2;
	m_startPlane.m_yPos = m_groundPlanePosition + 0.1;
	m_startPlane.m_zPos = -m_worldHeight / 2 + m_worldPlaneSquareSize / 2;
	m_startPlane.m_color1 = Vec3f(0.0f, 1.0f, 0.0f);
	m_startPlane.m_color2 = Vec3f(0.0f, 0.5f, 0.0f);
	m_startPlane.activate();
	// end plane
	m_endPlane.loadModel(worldSquarePlaneMesh, 9, 9);
	m_endPlane.m_xPos = m_worldWidth / 2 - m_worldPlaneSquareSize / 2;
	m_endPlane.m_yPos = m_groundPlanePosition + 0.1;
	m_endPlane.m_zPos = m_worldHeight / 2 - m_worldPlaneSquareSize / 2;
	m_endPlane.m_color1 = Vec3f(1.0f, 0.0f, 0.0f);
	m_endPlane.m_color2 = Vec3f(0.5f, 0.0f, 0.0f);
	m_endPlane.activate();

	/// PUSH PLANES TO primitiveModels vector
	primitiveModels.push_back(&m_worldPlane);
	primitiveModels.push_back(&m_backgroundPlane);
	primitiveModels.push_back(&m_startPlane);
	primitiveModels.push_back(&m_endPlane);
	
	/// CREATE SPECIAL ROTATING OBJECTS	
	m_rotatingObject1.loadModel(g_primitives->m_icosahedron, 0, 1);
	m_rotatingObject2.loadModel(g_primitives->m_icosahedron, 0, 1);
	m_rotatingObject3.loadModel(g_primitives->m_icosahedron, 0, 1);
	m_rotatingObject4.loadModel(g_primitives->m_icosahedron, 0, 1);

	// rotating objects color properties
	m_rotatingObject1.m_color1 = Vec3f(.5f, .1f, .9f);
	m_rotatingObject1.m_color2 = Vec3f(.4f, 0.0f, .8f);

	m_rotatingObject2.m_color1 = Vec3f(.1f, .8f, .3f);
	m_rotatingObject2.m_color2 = Vec3f(.2f, .9f, .4f);

	m_rotatingObject3.m_color1 = Vec3f(.9f, .2f, .2f);
	m_rotatingObject3.m_color2 = Vec3f(.8f, .1f, .1f);

	m_rotatingObject4.m_color1 = Vec3f(.9f, 0.0f, .8f);
	m_rotatingObject4.m_color2 = Vec3f(.7f, 0.0f, .6f);

	// rotating objects scale properties
	m_rotatingObject1.m_xScale = 2.8f;
	m_rotatingObject1.m_yScale = 2.8f;
	m_rotatingObject1.m_zScale = 2.8f;

	m_rotatingObject2.m_xScale = 2.4f;
	m_rotatingObject2.m_yScale = 2.4f;
	m_rotatingObject2.m_zScale = 2.4f;

	m_rotatingObject3.m_xScale = 1.7f;
	m_rotatingObject3.m_yScale = 1.7f;
	m_rotatingObject3.m_zScale = 1.7f;

	m_rotatingObject4.m_xScale = 1.8f;
	m_rotatingObject4.m_yScale = 1.8f;
	m_rotatingObject4.m_zScale = 1.8f;

	// add to specialPrimitives
	specialPrimitives.push_back(&m_rotatingObject1);
	specialPrimitives.push_back(&m_rotatingObject2);
	specialPrimitives.push_back(&m_rotatingObject3);
	specialPrimitives.push_back(&m_rotatingObject4);

	// initialize rotating objects
	m_rotatingObject1.activate();
	m_rotatingObject2.activate();
	m_rotatingObject3.activate();
	m_rotatingObject4.activate();

	/// CREATE SPECIAL TEXTURE OBJECTS	
	m_textureObject1.loadModel(g_primitives->m_cubeMesh, 13, 13);
	m_textureObject2.loadModel(g_primitives->m_cubeMesh, 14, 14);
	m_textureObject3.loadModel(g_primitives->m_cubeMesh, 15, 15);

	// texture objects position
	m_textureObject1.m_xPos = -20.0f;
	m_textureObject1.m_yPos = 8.0f;
	m_textureObject1.m_zPos = -m_worldHeight / 2 - m_requiredObjectsBoundaryPadding;

	m_textureObject2.m_xPos = 0.0f;
	m_textureObject2.m_yPos = 8.0f;
	m_textureObject2.m_zPos = -m_worldHeight / 2 - m_requiredObjectsBoundaryPadding;

	m_textureObject3.m_xPos = 20.0f;
	m_textureObject3.m_yPos = 8.0f;
	m_textureObject3.m_zPos = -m_worldHeight / 2 - m_requiredObjectsBoundaryPadding;

	// texture objects scale properties
	m_textureObject1.m_xScale = 6.0f;
	m_textureObject1.m_yScale = 6.0f;
	m_textureObject1.m_zScale = 6.0f;

	m_textureObject2.m_xScale = 6.0f;
	m_textureObject2.m_yScale = 6.0f;
	m_textureObject2.m_zScale = 6.0f;
	
	m_textureObject3.m_xScale = 6.0f;
	m_textureObject3.m_yScale = 6.0f;
	m_textureObject3.m_zScale = 6.0f;
	
	// add to specialPrimitives
	specialPrimitives.push_back(&m_textureObject1);
	specialPrimitives.push_back(&m_textureObject2);
	specialPrimitives.push_back(&m_textureObject3);

	// initialize texture objects
	m_textureObject1.activate();
	m_textureObject2.activate();
	m_textureObject3.activate();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** createObstacles
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::createObstacles() {

	/// remove existing obstacles
	if (!obstacleObjects.empty()) {
		// clear obstacle object memory
		for (unsigned int i = 0; i < obstacleObjects.size(); ++i) {
			delete obstacleObjects[i];
		}
		// clear vector
		obstacleObjects.clear();
	}

	// random number generator
	std::tr1::mt19937 e;
	// set seed
	e.seed(g_systemClock->getSystemTime());

	// random numbers for position
	std::tr1::uniform_real<float> rpx(-(m_worldWidth - m_obstacleBoundaryPadding) / 2, (m_worldWidth - m_obstacleBoundaryPadding) / 2);
	std::tr1::uniform_real<float> rpz(-(m_worldHeight - m_obstacleBoundaryPadding) / 2, (m_worldHeight - m_obstacleBoundaryPadding) / 2);
	// random numbers for scale
	std::tr1::uniform_real<float> rsx(.5, m_obstacleMaxWidth);
	std::tr1::uniform_real<float> rsx2(1.0, 4.0f);
	std::tr1::uniform_real<float> rsy(.5, m_obstacleMaxHeight);
	std::tr1::uniform_real<float> rsz(.5, m_obstacleMaxLength);
	// random numbers for color
	std::tr1::uniform_real<float> rcr(0.8, 1.0);
	std::tr1::uniform_real<float> rcg(0.4, 0.8);
	std::tr1::uniform_real<float> rcb(0.0, 0.0);

	/// CREATE OBSTACLES
	for (int i = 0; i < m_numberOfObstacles; ++i) {

		// uniform scale
		float uniformScale = rsx(e);

		float color_r = rcr(e);
		float color_g = rcg(e);
		float color_b = rcb(e);

		/// create new obstacle
		Obstacle* obstacle = new Obstacle();

		/// set base obstacle color
		obstacle->m_model.m_color1 = Vec3f(color_r, color_g, color_b);
		obstacle->m_model.m_color2 = Vec3f(color_r, color_g, color_b);

		/// initialize obstacle
		Mesh* currentMesh = g_primitives->m_cubeMesh;
		float xPos, yPos, zPos;
		float xScale = uniformScale;
		float yScale = uniformScale;
		float zScale = uniformScale;
		int texture1 = 0;
		int texture2 = 1;

		/// cubes
		// stone
		if (i % 3 == 0) {
			// set texture
			texture1 = 7;
			texture2 = 7;
		}

		// icosahedron
		if (i % 7 == 0) {
			// set texture
			texture1 = 7;
			texture2 = 7;
			currentMesh = g_primitives->m_icosahedron;
		}
		

		// cylinder
		if (i % 11 == 0 || i % 11 == 1) {
			currentMesh = g_primitives->m_cylinderMesh;
			
			// set texture
			if (i % 11 == 0) {
				texture1 = 3;
				texture2 = 4;
			} else {
				texture1 = 5;
				texture2 = 6;
			}

			// scale
			xScale = rsx2(e);
			yScale = rsy(e);
			zScale = xScale;

			// set rotation
			obstacle->m_model.m_xRotate = 1.0;
			obstacle->m_model.m_yRotate = 0.0;
			obstacle->m_model.m_zRotate = 0.0;
			obstacle->m_model.m_rotateAngle = -90.0;
		}

		/// random position
		xPos = rpx(e);
		yPos = yScale;
		zPos = rpz(e);

		// if random position falls within center area > reposition
		if (abs(xPos) < 10)
			xPos = rpx(e);
		if (abs(zPos) < 10)
			zPos = rpz(e);

		obstacle->initialize(xPos, yPos, zPos, xScale, zScale, yScale, currentMesh, texture1, texture2);
		obstacle->activate();
		
		/// push obstacle to vector
		obstacleObjects.push_back(obstacle);
	}

	cout << "CREATE OBSTACLES > DONE" << endl;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** start - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::start() {

	/// Base class method
	Screen::start();

	/// setup lighting
	setupLighting();

	/// create and initialize environment objects (buildings/obstacles)
	createEnvironmentObjects();

	/// start new game
	startNewGame();

	//test();

	changeGlobalMaterial(2);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** setupLighting - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::setupLighting() {

    /// set lighting properties
    glEnable(GL_LIGHTING);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, m_ambientLight);

    /// setup light number 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, m_ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, m_diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, m_specular);
   
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 85.0f);
    glEnable(GL_LIGHT0);
	
    /// set default material
	glMaterialfv(GL_FRONT, GL_AMBIENT, m_mat_low_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m_mat_high_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m_no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, m_no_mat);
    glMaterialfv(GL_FRONT, GL_EMISSION, m_mat_emission);

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	/// allow specular light on textures
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	/// ensure normalized normal vectors
    glEnable(GL_NORMALIZE);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** changeGlobalMaterial - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::changeGlobalMaterial(int materialSelection) {

	switch (materialSelection) {

		/// high diffuse - low specular - low emmisive
		case 0:
			glMaterialfv(GL_FRONT, GL_AMBIENT, m_mat_high_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, m_mat_high_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, m_no_mat);
			glMaterialfv(GL_FRONT, GL_SHININESS, m_no_mat);
			glMaterialfv(GL_FRONT, GL_EMISSION, m_mat_emission);
			break;
		/// high diffuse - high specular - low emmisive
		case 1:
			glMaterialfv(GL_FRONT, GL_AMBIENT, m_mat_high_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, m_mat_high_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, m_mat_high_specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, m_mat_high_shininess);
			glMaterialfv(GL_FRONT, GL_EMISSION, m_mat_emission);
			break;
		/// design 1
		case 2:
			glMaterialfv(GL_FRONT, GL_AMBIENT, m_mat_high_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, m_mat_high_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, m_mat_high_specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, m_mat_high_shininess);
			glMaterialfv(GL_FRONT, GL_EMISSION, m_mat_emission);
			break;
		/// design 2
		case 3:
			glMaterialfv(GL_FRONT, GL_AMBIENT, m_mat_high_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, m_mat_high_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, m_mat_high_specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, m_mat_high_shininess);
			glMaterialfv(GL_FRONT, GL_EMISSION, m_no_mat);
			break;
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** updateLighting - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::updateLighting() {

	glPushMatrix();	

        // rotate lights
		glRotatef(m_lightAngleY, 0.0f, 1.0f, 0.0f);
		glRotatef(m_lightAngleX, 1.0f, 0.0f, 0.0f);


		//cout << m_lightAngleY << " " << m_lightAngleX << " " << m_lightPos[2] << endl;

        // apply new position and direction
        glLightfv(GL_LIGHT0, GL_POSITION, m_lightPos);
        //glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, m_spotDir);

		/// draw light source reference
        glColor3ub(255,0,0);	

        // move reference to light source location
        glTranslatef(m_lightPos[0], m_lightPos[1], m_lightPos[2]);
        glutSolidCone(4.0f, 6.0f, 15, 15);

		/// draw "light bulb" - save lighting state
        glPushAttrib(GL_LIGHTING_BIT);
            // turn of lighting
            glDisable(GL_LIGHTING);
            glColor3ub(255,255,0);
            glutSolidSphere(3.0f, 15, 15);

        // restore lighting state
        glPopAttrib();

	glPopMatrix();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** stop
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::stop() {

	// Base class method
	Screen::stop();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** update
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::update() {

	/// update player movement vectors
	updateMovementVector();

	/// PLAYER MOVES CAR
	if (SPLINE_MODE == 0) {

		/// DRIVE
		if (CONTROL_MODE == 0) {

			// update player model properties
			m_playerModel.m_xPos += m_movementVector.x * m_movementSpeed * m_animationSpeed * g_gameEngine->lastFrameTime;
			m_playerModel.m_yPos += m_verticalMovementVelocity * g_gameEngine->lastFrameTime * m_animationSpeed;
			m_playerModel.m_zPos += m_movementVector.z * m_movementSpeed * m_animationSpeed * g_gameEngine->lastFrameTime;
		
			// update player model facing direction
			m_playerHeading = atan2(m_movementVector.z, m_movementVector.x);
			m_playerModel.m_rotateAngle = float(-RAD2DEG(m_playerHeading) + 180.0);
		
		/// FLY
		} else if (CONTROL_MODE == 1) {

			m_flyingVector.x = cos(m_playerHeading) * sin(m_playerPitch);
			m_flyingVector.y = cos(m_playerPitch);
			m_flyingVector.z = sin(m_playerHeading) * sin(m_playerPitch);

			Math3d::normalizeVector(m_flyingVector);

			m_playerModel.m_xPos += m_flyingVector.x * m_movementSpeed * m_animationSpeed * g_gameEngine->lastFrameTime;
			m_playerModel.m_yPos += m_flyingVector.y * m_movementSpeed * m_animationSpeed * g_gameEngine->lastFrameTime;
			m_playerModel.m_zPos += m_flyingVector.z * m_movementSpeed * m_animationSpeed * g_gameEngine->lastFrameTime;

			/// y-axis angle (heading)
			m_playerModel.m_rotateAngle = -90.0;

			m_movementVector = Vec3f(m_flyingVector.x, m_flyingVector.y, m_flyingVector.z);

			Vec3f up = Vec3f(0.0f, -1.0f, 0.0f);
			Vec3f xAxis;

			Math3d::crossProduct(xAxis, m_flyingVector, up);
			Math3d::normalizeVector(xAxis);
			Math3d::crossProduct(up, xAxis, m_flyingVector);
			Math3d::normalizeVector(up);

			orientationMatrix[ 0] = xAxis.x;
			orientationMatrix[ 1] = xAxis.y;
			orientationMatrix[ 2] = xAxis.z;
			orientationMatrix[ 3] = 0.0f;

			orientationMatrix[ 4] = up.x;
			orientationMatrix[ 5] = up.y;
			orientationMatrix[ 6] = up.z;
			orientationMatrix[ 7] = 0.0f;

			orientationMatrix[ 8] = m_flyingVector.x;
			orientationMatrix[ 9] = m_flyingVector.y;
			orientationMatrix[10] = m_flyingVector.z;
			orientationMatrix[11] = 0.0f;

			orientationMatrix[12] = 0.0;
			orientationMatrix[13] = 0.0;
			orientationMatrix[14] = 0.0;
			orientationMatrix[15] = 1.0f;

			m_playerModel.orientationMatrix = orientationMatrix;
		}


	/// PLAYER CAR MOVES ALONG SPLINE
	} else if (SPLINE_MODE == 1) {

		if (m_currentSplineIndex >= splineVertices.size()) {
			m_currentSplineIndex = 0;
		}

		// step car to next spline segment
		Vec3f position = splineVertices[m_currentSplineIndex];
		
		// update player model properties
		m_playerModel.m_xPos = position.x;
		m_playerModel.m_yPos = position.y;
		m_playerModel.m_zPos = position.z;

		// update player model facing direction
		// get direction vector
		int splineSpeed = (g_gameEngine->lastFrameTime * 60);

		m_splineDirection = splineVertices[m_currentSplineIndex + 1] - splineVertices[m_currentSplineIndex];
		m_playerHeading = atan2(m_splineDirection.z, m_splineDirection.x);
		m_cameraY = position.y + m_cameraElevation;

		/// y-axis angle (heading)
		m_playerModel.m_rotateAngle = -90.0;

		Math3d::normalizeVector(m_splineDirection);

		m_movementVector = Vec3f(m_splineDirection.x, m_splineDirection.y, m_splineDirection.z);

		Vec3f up = Vec3f(0.0f, -1.0f, 0.0f);
		Vec3f xAxis;

		Math3d::crossProduct(xAxis, m_splineDirection, up);
		Math3d::normalizeVector(xAxis);
		Math3d::crossProduct(up, xAxis, m_splineDirection);
		Math3d::normalizeVector(up);

		orientationMatrix[ 0] = xAxis.x;
		orientationMatrix[ 1] = xAxis.y;
		orientationMatrix[ 2] = xAxis.z;
		orientationMatrix[ 3] = 0.0f;

		orientationMatrix[ 4] = up.x;
		orientationMatrix[ 5] = up.y;
		orientationMatrix[ 6] = up.z;
		orientationMatrix[ 7] = 0.0f;

		orientationMatrix[ 8] = m_splineDirection.x;
		orientationMatrix[ 9] = m_splineDirection.y;
		orientationMatrix[10] = m_splineDirection.z;
		orientationMatrix[11] = 0.0f;

		orientationMatrix[12] = 0.0;
		orientationMatrix[13] = 0.0;
		orientationMatrix[14] = 0.0;
		orientationMatrix[15] = 1.0f;

		/*
		m[0] = 1.0f; m[4] = 0.0f; m[8] = 0.0f; m[12] = 0.0f;
		m[1] = 0.0f; m[5] = 1.0f; m[9] = 0.0f; m[13] = 10.0f;
		m[2] = 0.5f; m[6] = 0.0f; m[10] = 1.0f; m[14] = 0.0f;
		m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;
		*/

		m_playerModel.orientationMatrix = orientationMatrix;

		m_currentSplineIndex += splineSpeed + 1;
	}

	/// snap camera to behind vehicle
	if (!m_mouseLeftPressed) {
		m_rotateAngle = -RAD2DEG(m_playerHeading) + -90;
	}

	/// update camera position and view
	m_viewX = m_playerModel.m_xPos;
	m_viewY = m_playerModel.m_yPos;
	m_viewZ = m_playerModel.m_zPos;
	updateView();

	/// update shoot vector
	m_shootVector.x = m_viewX - m_cameraX;
	m_shootVector.y = m_viewY - m_cameraY;
	m_shootVector.z = m_viewZ - m_cameraZ;
	m_shootVector.normalize();

	/// update lighting position
	m_lightAngleX += m_lightAngleXVelocity * g_gameEngine->lastFrameTime * m_animationSpeed;
	m_lightAngleY += m_lightAngleYVelocity * g_gameEngine->lastFrameTime * m_animationSpeed;

	/// update rotating boxes
	m_boxAngle += m_boxRotationIncrement * g_gameEngine->lastFrameTime * m_animationSpeed;

	/// update box 2 x/y position
	m_box2_x = cos(DEG2RAD(m_box2Angle)) * 10;
	m_box2_z = sin(DEG2RAD(m_box2Angle)) * 10;
	m_box2Angle += m_boxRotation2Increment * g_gameEngine->lastFrameTime * m_animationSpeed;

	/// UPDATE GAME OBSTACLES
	for (unsigned int i = 0; i < obstacleObjects.size(); ++i) {
		obstacleObjects[i]->update();
	}

	/// UPDATE REQUIRED MODELS
	for (unsigned int i = 0; i < modelObjects.size(); ++i) {
		modelObjects[i]->update();
	}

	int i, j;
	/// UPDATE PROJECTILES OBJECTS
	for (i = 0; i < playerProjectiles.size(); ++i) {
		playerProjectiles[i]->update();

		/// check collisions with model objects
		for (j = 0; j < modelObjects.size(); ++j) {
			/// check collisions
			if (modelObjects[j]->checkCollision(playerProjectiles[i]->position)) {
				playerProjectiles[i]->stopAnimations();
			}
		}

		/// check collisions with obstacle objects
		for (j = 0; j < obstacleObjects.size(); ++j) {
			/// check collisions
			if (obstacleObjects[j]->hitTest(playerProjectiles[i]->position)) {
				playerProjectiles[i]->stopAnimations();
			}
		}

		/// check for ground plane collision
		if (playerProjectiles[i]->position.y <= m_groundPlanePosition) {
			playerProjectiles[i]->stopAnimations();
		}
	}

	/// shoot projectiles
	if (m_shooting && m_lastFired + m_firingRate < g_gameEngine->realtimeClock.getElapsedTime()) {
		m_lastFired = g_gameEngine->realtimeClock.getElapsedTime();
		shootProjectile(PROJECTILE_TYPE);
	}

	/// CHECK VICTORY CONDITIONS
	checkVictoryConditions(m_playerModel.m_xPos, m_playerModel.m_zPos);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** updateView
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::updateView() {

	/// update camera view values
	// rotation
	if (m_viewMode == 2 || m_viewMode == 3) 

		if (SPLINE_MODE == 0)
			m_cameraY = m_playerModel.m_yPos + m_cameraElevation;
	else
		m_viewCameraY += m_viewHeightVelocity * g_gameEngine->lastFrameTime * m_animationSpeed;

	// distance and rotation updates
	m_viewCameraDistance += m_viewDistanceVelocity * g_gameEngine->lastFrameTime * m_animationSpeed;
	m_viewRotateAngle += m_viewRotationVelocity * g_gameEngine->lastFrameTime * m_animationSpeed;

	/// calculate new camera x/z position based on circlular rotation around target
	m_cameraX = float(sin(DEG2RAD(m_rotateAngle))*m_cameraDistance/2.0 + m_viewX);
	m_cameraZ = float(cos(DEG2RAD(m_rotateAngle))*m_cameraDistance/2.0 + m_viewZ);

	/// calculate 45 degree view camera positions based on cirular rotation
	m_viewCameraX = float(sin(DEG2RAD(m_viewRotateAngle))*m_viewCameraDistance/2.0);
	m_viewCameraZ = float(cos(DEG2RAD(m_viewRotateAngle))*m_viewCameraDistance/2.0);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** updateMovementVector
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::updateMovementVector() {
	
	// set speed limit
	if (abs(m_movementSpeed) <= m_movementSpeedLimit)
		m_movementSpeed += m_movementVelocity * g_gameEngine->lastFrameTime;
	else
		m_movementSpeed = m_movementSpeedLimit * m_movementDirection;
	
	// calculate rotation speed based on movement speed
	m_activeRotationVelocity = m_rotationVelocity / (abs(m_movementSpeed) + (m_movementSpeedLimit/2));

	// add friction on to movement speed - framerate independant taking dampenining float to the power of lastFrameTime
	if (m_victory) {
		m_movementSpeed *= pow(.01, double(g_gameEngine->lastFrameTime));
	} else {
		m_movementSpeed *= pow(.5, double(g_gameEngine->lastFrameTime));
	}

	// add velocity to amount
	m_rotationAngle += m_activeRotationVelocity * g_gameEngine->lastFrameTime;

	// set new position
	m_movementVector.x = cos(m_rotationAngle);
	m_movementVector.z = sin(m_rotationAngle);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** setupProjection
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::setupProjection(int width, int height) {

	// base class projection
	Screen::setupProjection(width, height);

	if (m_initialWidth == 0) {
		m_initialWidth = m_viewWidth;
		m_initialHeight = m_viewHeight;
	}

	// projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	// select view type based on m_viewMode selection
	switch (m_viewMode) {
		// orthographic projection
		case 0:
		    if (m_viewWidth <= m_viewHeight) {
				glOrtho (-m_orthoSize, m_orthoSize, -m_orthoSize*m_viewHeight/m_viewWidth, m_orthoSize*m_viewHeight/m_viewWidth, -100, 100);
			} else {
				glOrtho (-m_orthoSize*m_viewWidth/m_viewHeight, m_orthoSize*m_viewWidth/m_viewHeight, -m_orthoSize, m_orthoSize, -100, 100);
			}
			break;
		// perspective projection with aspect ratio calcuation based on the viewport dimensions
		// 45 degrees
		case 1:
		// camera follow
		case 2:
		case 3:
			gluPerspective(52.0f,(GLfloat)m_viewWidth/((GLfloat)m_viewHeight),1.0f,1000.0f);
			break;
	}
	
	// clear depth buffer
    glClear(GL_DEPTH_BUFFER_BIT); 
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** draw
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::draw() {

	/// update projection
	setupProjection(m_windowWidth, m_windowHeight);
	
	/// modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	/// set camera location and view
	switch (m_viewMode) {
		// orthographic
		case 0:
			gluLookAt(0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
			break;
		// over view
		case 1:
			gluLookAt(m_viewCameraX, m_viewCameraY, m_viewCameraZ, 0.0, m_viewCameraLookY, 0.0, 0.0, 1.0, 0.0);
			break;
		// first person
		case 2:
		case 3:

			/// Driving/Flying Mode
			if (SPLINE_MODE == 0 || m_viewMode == 3 || m_viewMode == 2) {

				// flying
				if (CONTROL_MODE == 1) {

					// viewing
					if (m_mouseLeftPressed) {
						gluLookAt(m_cameraX, m_cameraY, m_cameraZ, m_viewX, m_viewY + m_viewYOffset, m_viewZ, 0.0, 1.0, 0.0);

					// camera follow
					} else {
						gluLookAt(m_viewX - m_movementVector.x * m_cameraDistance, 
								  m_viewY - m_movementVector.y * m_cameraDistance + 8.0f, 
								  m_viewZ - m_movementVector.z * m_cameraDistance,
								   m_viewX + m_movementVector.x * 100, 
								   m_viewY + m_movementVector.y * 100, 
								   m_viewZ + m_movementVector.z * 100, 
								   0.0, 1.0, 0.0);
					}

				// driving
				} else {
					gluLookAt(m_cameraX, m_cameraY, m_cameraZ, m_viewX, m_viewY + m_viewYOffset, m_viewZ, 0.0, 1.0, 0.0);
				}

			break;
		}
	}

	/// update lighting
	updateLighting();

	/// DRAW REQUIRED MODELS
	for (unsigned int i = 0; i < modelObjects.size(); ++i) {
		modelObjects[i]->draw();
	}

	/// DRAW GAME OBSTACLES
	for (unsigned int i = 0; i < obstacleObjects.size(); ++i) {
		obstacleObjects[i]->draw();
	}

	/// DRAW PRIMITIVE MODELS
	for (unsigned int i = 0; i < primitiveModels.size(); ++i) {
		primitiveModels[i]->draw();
	}

	/// DRAW PROJECTILES
	for (unsigned int i = 0; i < playerProjectiles.size(); ++i) {
		playerProjectiles[i]->draw();
	}

	/// object moving in orbit
	glPushMatrix();
		glTranslatef(m_box2_x, m_obstacleMaxHeight + 10.0, m_box2_z);
		m_rotatingObject4.draw();
	glPopMatrix();

	/// DRAW ROTATING OBJECTS
	glPushMatrix();
		glTranslatef(0.0f, m_obstacleMaxHeight + 40.0, 0.0);

		glPushMatrix();
		glTranslatef(10.0f, 0.0f, 0.0f);
			glRotatef(-m_boxAngle * 2.0f, 0.0f, 1.0f, 0.0f);
			
			m_rotatingObject2.draw();

			glPushMatrix();
				glRotatef(-m_boxAngle * 3.0f, 0.0f, 1.0f, 0.0f);
				glTranslatef(4.0f, 0.0f, 0.0f);
				m_rotatingObject3.draw();
			glPopMatrix();
		glPopMatrix();
    
		glRotatef(m_boxAngle, 0.0f, 1.0f, 0.0f);
		glutSolidSphere(3.0, 64, 64);
		//m_rotatingObject1.draw();

	glPopMatrix();

	/// DRAW TEXTURE OBJECTS
	m_textureObject1.draw();
	m_textureObject2.draw();
	m_textureObject3.draw();

	/// DRAW SPLINE
	if (SHOW_SPLINE) {
		if (DISPLAY_LIST)
			glCallList(m_splineDL);
		else
			drawSpline();
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** drawSpline
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::drawSpline() {

	int stage = 0;
	int colorR = 255;
	int colorG = 0;
	int colorB = 0;

	glDisable(GL_LIGHTING);
	glLineWidth(5.0);
	glBegin(GL_LINE_STRIP);

		/// draw spline points
		for (unsigned int i = 0; i < splineVertices.size(); i++) {
		
			glColor3ub(colorR, colorG, colorB);

			/// draw spline point
			glVertex3fv(&splineVertices[i].x);

			if (stage == 0) {
				if (colorG == 255)
					stage = 1;
				else
					colorG++;
			} else if (stage == 1) {
				if (colorR == 0)
					stage = 2;
				else
					colorR--;
			} else if (stage == 2) {
				if (colorB == 255)
					stage = 3;
				else
					colorB++;
			} else if (stage == 3) {
				if (colorG == 0)
					stage = 4;
				else
					colorG--;
			} else if (stage == 4) {
				if (colorR == 255)
					stage = 5;
				else
					colorR++;
			} else if (stage == 5) {
				if (colorB == 0)
					stage = 0;
				else
					colorB--;
			}
		}

	glEnd();

	glEnable(GL_LIGHTING);
	glColor3ub(256, 128, 0);
	/// draw spline points
	for (unsigned int i = 0; i < splineControlVertices.size(); i++) {
		glPushMatrix();
			glTranslatef(splineControlVertices[i].x, splineControlVertices[i].y, splineControlVertices[i].z);
			glutSolidSphere(3.0, 64, 64);
		glPopMatrix();
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** createSpline
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::createSpline(int segments, int steps) {

	/// clear existing splines and control points
	splineVertices.clear();
	splineControlVertices.clear();

	/// random points / control vectors
	// random number generator
	std::tr1::mt19937 e;
	// set seed
	e.seed(g_systemClock->getSystemTime());

	// random numbers for points - set point boundry to some fraction of the world size
	std::tr1::uniform_real<float> rpx(-(m_worldWidth - m_obstacleBoundaryPadding) / 3, (m_worldWidth - m_obstacleBoundaryPadding) / 3);
	std::tr1::uniform_real<float> rpy(m_obstacleMaxHeight * 2, m_obstacleMaxHeight * 15.0f);
	std::tr1::uniform_real<float> rpz(-(m_worldHeight - m_obstacleBoundaryPadding) / 3, (m_worldHeight - m_obstacleBoundaryPadding) / 3);
	
	// random numbers for control vectors - large numbers generally result in smoother curves
	std::tr1::uniform_real<float> rvx(-(m_worldWidth - m_obstacleBoundaryPadding), (m_worldWidth - m_obstacleBoundaryPadding));
	std::tr1::uniform_real<float> rvy(m_obstacleMaxHeight * 5.0f, m_obstacleMaxHeight * 50.0f);
	std::tr1::uniform_real<float> rvz(-(m_worldHeight - m_obstacleBoundaryPadding), (m_worldHeight - m_obstacleBoundaryPadding));

	/// spline data
	float h1, h2, h3, h4, s;

	Vec3f P1, T1, P2, T2;
	Vec3f v1, v2, v3, v4, p;
	Vec3f lastPoint, lastControl, firstPoint, firstControl;

	/// generate [segments] splines
	for (unsigned int i = 0; i < segments; i++) {

		/// start point
		if (i == 0) {
			P1 = Vec3f(rpx(e), rpy(e), rpz(e));
			T1 = Vec3f(rvx(e), rvy(e), rvz(e));
			P2 = Vec3f(rpx(e), rpy(e), rpz(e));
			T2 = Vec3f(rvx(e), rvy(e), rvz(e));

			firstPoint = P1;
			firstControl = T1;

		/// interior points
		} else {
			P1 = lastPoint;
			T1 = lastControl;
			P2 = Vec3f(rpx(e), rpy(e), rpz(e));
			T2 = Vec3f(rvx(e), rvy(e), rvz(e));
		}

		/// ensure entire spline is continous and smooth
		lastPoint = P2;
		lastControl = T2;

		/// start point
		splineVertices.push_back(Vec3f(P1));

		/// push control point
		splineControlVertices.push_back(Vec3f(T1));

		/// generate interpolated points
		for (unsigned int t = 0; t < steps; t++) {

			s = (float)t / (float)steps;
			h1 =  (2 * pow(s, 3)) - (3 * pow(s, 2)) + 1;
			h2 = (-2 * pow(s, 3)) + (3 * pow(s, 2));
			h3 =  pow(s, 3) - (2 * pow(s, 2)) + s;
			h4 =  pow(s, 3) - pow(s, 2);    

			v1 = Vec3f(h1 * P1);
			v2 = Vec3f(h2 * P2);
			v3 = Vec3f(h3 * T1);
			v4 = Vec3f(h4 * T2);

			p = Vec3f(v1 + v2 + v3 + v4);

			/// interpolated point
			splineVertices.push_back(Vec3f(p));
		}
	}

	P1 = lastPoint;
	T1 = lastControl;
	P2 = firstPoint;
	T2 = firstControl;

	/// generate spline connecting first point to last point
	for (unsigned int t = 0; t < steps; t++) {

		s = (float)t / (float)steps;
		h1 =  (2 * pow(s, 3)) - (3 * pow(s, 2)) + 1;
		h2 = (-2 * pow(s, 3)) + (3 * pow(s, 2));
		h3 =  pow(s, 3) - (2 * pow(s, 2)) + s;
		h4 =  pow(s, 3) - pow(s, 2);    

		v1 = Vec3f(h1 * P1);
		v2 = Vec3f(h2 * P2);
		v3 = Vec3f(h3 * T1);
		v4 = Vec3f(h4 * T2);

		p = Vec3f(v1 + v2 + v3 + v4);

		/// interpolated point
		splineVertices.push_back(Vec3f(p));
	}

	/// generate spline display list
	glDeleteLists(m_splineDL, 1);
	glNewList(m_splineDL, GL_COMPILE);
		drawSpline();
	glEndList();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** rotateView
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::rotateView(float degree) {

	// increment camera rotation angle
	m_rotateAngle += degree;
	m_viewRotateAngle += degree;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** startNewGame
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::startNewGame() {

	createObstacles();

	/// move player to starting area
	setPlayerToStart();
	resetPlayer();

	/// restart timer
	g_controlScreen->resetTimer();

	/// reset properties, input and movement
	m_victory = false;
	m_movementSpeed = 0.0;
	m_movementVelocity = 0.0;
	m_rotationVelocity = 0.0;
	m_turningLeft = false;
	m_turningRight = false;

	/// create spline
	createSpline(m_splineSegments, m_splineSteps);

	cout << "START GAME > DONE" << endl;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** setPlayerToStart
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::setPlayerToStart() {

	/// position player model
	m_playerModel.m_xPos = -m_worldWidth / 2 + m_worldPlaneSquareSize / 2;
	m_playerModel.m_zPos = -m_worldHeight / 2 + m_worldPlaneSquareSize / 2;

	/// orient player model
	m_rotationAngle = DEG2RAD(45.0);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** checkVictoryConditions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::checkVictoryConditions(float x, float z) {

	/// check if player coordinates are within the square in the top left corner of the game world
	if (x > m_worldWidth / 2 - m_worldPlaneSquareSize && x < m_worldWidth / 2
		&& z > m_worldHeight / 2 - m_worldPlaneSquareSize && z < m_worldHeight / 2) {

		/// victory
		m_victory = true;
		m_movementVelocity = 0.0;
		m_rotationVelocity = 0.0;
		m_turningLeft = false;
		m_turningRight = false;

		/// set control screen victory status
		g_controlScreen->stopTimer();
		g_controlScreen->displayVictoryMessage();
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** shootProjectile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::shootProjectile(int type) {
	
	Vec3f* shootDirection;

	if (m_mouseLeftPressed) {
		shootDirection = &m_shootVector;
	} else {
		shootDirection = &m_movementVector;
	}

	/// shoot projectile
	switch (type) {

		/// straight
		case 0 :
			playerProjectiles[m_currentProjectileID]->animate(ANIMATION::ANIMATION_TYPES::TRANSLATE, 
																TWEEN::OUT_LINEAR, TWEEN::OUT_LINEAR, TWEEN::OUT_LINEAR,
																m_playerModel.m_xPos, 
																m_playerModel.m_yPos + 2.0f, 
																m_playerModel.m_zPos, 
																m_playerModel.m_xPos + shootDirection->x * m_projectileDistance, 
																m_playerModel.m_yPos + shootDirection->y * m_projectileDistance + 2.0f, 
																m_playerModel.m_zPos + shootDirection->z * m_projectileDistance, 
																m_projectileSpeed, m_projectileSpeed, m_projectileSpeed, false, -1, true);

			playerProjectiles[m_currentProjectileID]->animate(ANIMATION::ANIMATION_TYPES::SCALE, 
																TWEEN::OUT_EXPO, TWEEN::OUT_EXPO, TWEEN::OUT_EXPO,
																0.2f, 0.2f, 0.2f, 
																1.0f, 1.0f, 1.0f,
																m_projectileSpeed / 2.0f, m_projectileSpeed / 2.0f, m_projectileSpeed / 2.0f, false, -1, true);

			playerProjectiles[m_currentProjectileID]->animate(ANIMATION::ANIMATION_TYPES::SCALE, 
																TWEEN::LINEAR, TWEEN::LINEAR, TWEEN::LINEAR,
																1.0f, 1.0f, 1.0f, 
																0.0f, 0.0f, 0.0f,
																2.0, 2.0, 2.0, true, 0, false);
			break;

		/// strange 1
		case 1 :
			playerProjectiles[m_currentProjectileID]->animate(ANIMATION::ANIMATION_TYPES::TRANSLATE, 
																TWEEN::IN_CUBIC, TWEEN::IN_OUT_EXPO, TWEEN::IN_CUBIC,
																m_playerModel.m_xPos, 
																m_playerModel.m_yPos + 2.0f, 
																m_playerModel.m_zPos, 
																m_playerModel.m_xPos + shootDirection->x * m_projectileDistance, 
																m_playerModel.m_yPos + (shootDirection->y * m_projectileDistance) / 6.0f, 
																m_playerModel.m_zPos + shootDirection->z * m_projectileDistance, 
																m_projectileSpeed, m_projectileSpeed / 6.0f, m_projectileSpeed, false, -1, true);

			playerProjectiles[m_currentProjectileID]->animate(ANIMATION::ANIMATION_TYPES::SCALE, 
																TWEEN::OUT_EXPO, TWEEN::OUT_EXPO, TWEEN::OUT_EXPO,
																0.2f, 0.2f, 0.2f, 
																1.5f, 1.5f, 1.5f,
																m_projectileSpeed * .85f, m_projectileSpeed * .85f, m_projectileSpeed * .85f, false, -1, true);

			playerProjectiles[m_currentProjectileID]->animate(ANIMATION::ANIMATION_TYPES::SCALE, 
																TWEEN::LINEAR, TWEEN::LINEAR, TWEEN::LINEAR,
																1.0f, 1.0f, 1.0f, 
																0.0f, 0.0f, 0.0f,
																2.0, 2.0, 2.0, true, 1, false);
			break;


		/// strange 2
		case 2 :

			playerProjectiles[m_currentProjectileID]->animate(ANIMATION::ANIMATION_TYPES::TRANSLATE, 
																TWEEN::OUT_SINE, TWEEN::OUT_SINE, TWEEN::OUT_SINE,
																m_playerModel.m_xPos, 
																m_playerModel.m_yPos + 2.0f, 
																m_playerModel.m_zPos, 
																m_playerModel.m_xPos + shootDirection->x * 5.0f, 
																m_playerModel.m_yPos + (shootDirection->y * m_projectileDistance) / 4.0f, 
																m_playerModel.m_zPos + shootDirection->z * 5.0f, 
																m_projectileSpeed / 4.0f, m_projectileSpeed / 4.0f, m_projectileSpeed / 4.0f, false, -1, true);

			playerProjectiles[m_currentProjectileID]->animate(ANIMATION::ANIMATION_TYPES::TRANSLATE, 
																TWEEN::OUT_SINE, TWEEN::OUT_SINE, TWEEN::OUT_SINE,
																m_playerModel.m_xPos, 
																m_playerModel.m_yPos + 2.0f, 
																m_playerModel.m_zPos, 
																m_playerModel.m_xPos + shootDirection->x * m_projectileDistance, 
																m_playerModel.m_yPos + (shootDirection->y * m_projectileDistance) / 2.0f, 
																m_playerModel.m_zPos + shootDirection->z * m_projectileDistance, 
																m_projectileSpeed / 2.0f, m_projectileSpeed / 2.0f, m_projectileSpeed / 2.0f, true, 2, false);

			playerProjectiles[m_currentProjectileID]->animate(ANIMATION::ANIMATION_TYPES::SCALE, 
																TWEEN::OUT_EXPO, TWEEN::OUT_EXPO, TWEEN::OUT_EXPO,
																0.2f, 0.2f, 0.2f, 
																1.0f, 1.0f, 1.0f,
																m_projectileSpeed * .85f, m_projectileSpeed * .85f, m_projectileSpeed * .85f, false, -1, true);
			break;

		/// straight - long lasting
		case 3 :
			playerProjectiles[m_currentProjectileID]->animate(ANIMATION::ANIMATION_TYPES::TRANSLATE, 
																TWEEN::OUT_LINEAR, TWEEN::OUT_LINEAR, TWEEN::OUT_LINEAR,
																m_playerModel.m_xPos, 
																m_playerModel.m_yPos + 2.0f, 
																m_playerModel.m_zPos, 
																m_playerModel.m_xPos + shootDirection->x * m_projectileDistance, 
																m_playerModel.m_yPos + shootDirection->y * m_projectileDistance + 2.0f, 
																m_playerModel.m_zPos + shootDirection->z * m_projectileDistance, 
																m_projectileSpeed, m_projectileSpeed, m_projectileSpeed, false, -1, true);

			playerProjectiles[m_currentProjectileID]->animate(ANIMATION::ANIMATION_TYPES::SCALE, 
																TWEEN::OUT_EXPO, TWEEN::OUT_EXPO, TWEEN::OUT_EXPO,
																0.2f, 0.2f, 0.2f, 
																.5f, .5f, .5f,
																m_projectileSpeed / 2.0f, m_projectileSpeed / 2.0f, m_projectileSpeed / 2.0f, false, -1, true);
			break;
	}

	m_currentProjectileID++;
	if (m_currentProjectileID >= m_projectilePoolCount) {
		m_currentProjectileID = 0;
	}
}
/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** mouseMoved
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::mouseMoved(int x, int y) {

	float deltaMouse = 0.0f;

	/// left press - rotate
	if (m_mouseLeftPressed) {

		deltaMouse = m_mouseX - x;
		rotateView(m_cameraRotationIncrement * deltaMouse);

		/// left press - elevation
		deltaMouse =  y - m_mouseY;
		if (m_viewMode == 2 || m_viewMode == 3)
			m_cameraElevation += m_cameraElevationIncrement1 * deltaMouse;
		else
			m_viewCameraY += m_cameraElevationIncrement2 * deltaMouse;
	}

	/// right press - distance
	if (m_mouseRightPressed) {

		deltaMouse = y - m_mouseY;
		if (m_viewMode == 1) {
			m_viewCameraDistance += m_viewDistanceIncrement * deltaMouse;
		} else {

			if (CONTROL_MODE == 1) {
				deltaMouse = y - m_centerHeight;
			}

			m_cameraDistance += m_distanceIncrement * deltaMouse;
		}
	}

	/// flying mode
	if (CONTROL_MODE == 1 && !m_mouseLeftPressed) {

		if(x < m_centerWidth) {
			deltaMouse = m_centerWidth - x;
			m_playerHeading += -0.002f * deltaMouse;
		
		} else if (x > m_centerWidth) {
			deltaMouse = x - m_centerWidth;
			m_playerHeading += 0.002f * deltaMouse;
		}


		if (y < m_centerHeight) {
			deltaMouse = m_centerHeight - y;
			m_playerPitch += -0.002f * deltaMouse;
		
		} else if (y > m_centerHeight) {
			deltaMouse = y - m_centerHeight;
			m_playerPitch += 0.002f * deltaMouse;
		}

		if (MOUSE_WARP) {
			MOUSE_WARP = false;
		} else {
			MOUSE_WARP = true;
			glutWarpPointer(m_centerWidth, m_centerHeight);
		}
	}

	/// save mouse coordinates
	m_mouseX = x;
	m_mouseY = y;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** mousePressed
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::mousePressed(int mouseButton, int x, int y) {

	switch (mouseButton) {
	case 0:
		m_mouseLeftPressed = true;
		break;
	case 1:
		m_mouseRightPressed = true;
		break;
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** mouseReleased
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::mouseReleased(int mouseButton, int x, int y) {
	switch (mouseButton) {
	case 0:
		m_mouseLeftPressed = false;
		break;
	case 1:
		m_mouseRightPressed = false;
		break;
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** mouseWheelScrolled
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::mouseWheelScrolled(int delta) {

	if (delta < 0) {

		// set light distance
		if (m_mouseRightPressed) {
			m_lightPos[2] += m_lightDistanceIncrement;

		// set view properties
		} else {

			if (m_viewMode == 0) {
				m_orthoSize += m_orthoSizeIncrement;
			} else if (m_viewMode == 1) {
				m_viewCameraDistance += m_viewDistanceIncrement;
			} else {
				m_cameraDistance += m_distanceIncrement;
			}
		}

	} else if (delta > 0) {

		// set light distance
		if (m_mouseRightPressed) {
			m_lightPos[2] -= m_lightDistanceIncrement;

		// set view properties
		} else {
			
			if (m_viewMode == 0) {
				m_orthoSize -= m_orthoSizeIncrement;
			} else if (m_viewMode == 1) {
				m_viewCameraDistance -= m_viewDistanceIncrement;
			} else {
				m_cameraDistance -= m_distanceIncrement;
			}
		}
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** keyPressed
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::keyPressed(int key) {

	if (m_victory) return;

	switch (key) {

		// W/w
		case 87 :
		case 119 :
			if (m_navigationMode == 1 || m_viewMode == 2 || m_viewMode == 3) {
				m_movementDirection = 1;
				m_movementVelocity = m_movementDirection * m_accelerationSpeed;
			} else {
				m_viewHeightVelocity = m_viewHeightSpeed;
			}
			break;
		// ARROW UP
		case 293 :
			if (m_navigationMode == 0) 
				m_lightAngleXVelocity = -m_lightAngleXSpeed;
			else
				m_viewHeightVelocity = m_viewHeightSpeed;
			break;

		// S/s
		case 83 :
		case 115 :
			if (m_navigationMode == 1 || m_viewMode == 2 || m_viewMode == 3) {
				m_movementDirection = -1;
				m_movementVelocity = m_movementDirection * m_brakingSpeed;
			} else {
				m_viewHeightVelocity = -m_viewHeightSpeed;
			}
			break;
		// ARROW DOWN
		case 294 :
			if (m_navigationMode == 0) 
				m_lightAngleXVelocity = m_lightAngleXSpeed;
			else
				m_viewHeightVelocity = -m_viewHeightSpeed;
			break;

		// A/a
		case 65 :
		case 97 :
			if (m_navigationMode == 1 || m_viewMode == 2 || m_viewMode == 3) {
				if (!m_turningLeft) m_rotationVelocity = -m_rotationIncrement;
				m_turningLeft = true;
			} else {
				m_viewRotationVelocity = -m_viewRotationSpeed;
			}
			break;
		// ARROW LEFT
		case 291 :
			if (m_navigationMode == 0) {
				m_lightAngleYVelocity = -m_lightAngleYSpeed;
			} else {
				rotateView(m_cameraRotationIncrement * 8.0f);
				m_mouseLeftPressed = true;
			}
			break;

		// D/d
		case 68 :
		case 100 :
			if (m_navigationMode == 0 || m_viewMode == 2 || m_viewMode == 3) {
				if (!m_turningRight) m_rotationVelocity = m_rotationIncrement;
				m_turningRight = true;
			} else {
				m_viewRotationVelocity = m_viewRotationSpeed;
			}
			break;
		// ARROW RIGHT
		case 292 :
			if (m_navigationMode == 0) {
				m_lightAngleYVelocity = m_lightAngleYSpeed;
			} else {
				rotateView(-m_cameraRotationIncrement * 8.0f);
				m_mouseLeftPressed = true;
			}
			break;

		// z/Z
		case 122 :
		case 90 :
			if (m_viewMode == 0) {
				m_orthoSize += m_orthoSizeIncrement;
			} else if (m_viewMode == 1) {
				m_viewDistanceVelocity = m_viewDistanceSpeed;
			} else {
				m_verticalMovementVelocity = m_verticalMovementSpeed;
			}
			break;
		// x/X
		case 120 :
		case 88 :
			if (m_viewMode == 0) {
				m_orthoSize -= m_orthoSizeIncrement;
			} else if (m_viewMode == 1) {
				m_viewDistanceVelocity = -m_viewDistanceSpeed;
			} else {
				m_verticalMovementVelocity = -m_verticalMovementSpeed;
			}
			
			break;

		// NUM PAD UP
		case 56 :
			SHOW_NODE_ID++;
			cout << SHOW_NODE_ID << endl;
			break;
		// NUM PAD DOWN
		case 50 :
			SHOW_NODE_ID--;
			cout << SHOW_NODE_ID << endl;
			break;
		// NUM PAD LEFT
		case 52 :
			AABB_TREE_DEPTH--;
			break;
		// NUM PAD RIGHT
		case 54 :
			AABB_TREE_DEPTH++;
			break;

		// SPACE/F
		case 32 :
		case 102 :
			m_shooting = true;
			break;
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** keyReleased
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::keyReleased(int key) {

	switch (key) {
		// W
		case 87 :
		case 119 :
			m_movementVelocity = 0.0f;
			m_viewHeightVelocity = 0.0f;
			break;
		// ARROW UP
		case 293 :
			m_lightAngleXVelocity = 0.0f;
			m_viewHeightVelocity = 0.0f;
			break;

		// S
		case 83 :
		case 115 :
			m_movementVelocity = 0.0f;
			m_viewHeightVelocity = 0.0f;
			break;
		// ARROW DOWN
		case 294 :
			m_lightAngleXVelocity = 0.0f;
			m_viewHeightVelocity = 0.0f;
			break;

		// A
		case 65 :
		case 97 :
			if (m_turningLeft && !m_turningRight) m_rotationVelocity = 0.0;
			if (m_turningLeft && m_turningRight) m_rotationVelocity = m_rotationIncrement;
			m_turningLeft = false;
			m_viewRotationVelocity = 0.0f;

			break;
		// ARROW LEFT
		case 291 :
			m_lightAngleYVelocity = 0.0f;
			m_viewRotationVelocity = 0.0f;
			break;

		// D
		case 68 :
		case 100 :
			if (m_turningRight && !m_turningLeft) m_rotationVelocity = 0.0;
			if (m_turningRight && m_turningLeft) m_rotationVelocity = -m_rotationIncrement;
			m_turningRight = false;
			m_viewRotationVelocity = 0.0f;

			break;
		// ARROW RIGHT
		case 292 :
			m_lightAngleYVelocity = 0.0;
			m_viewRotationVelocity = 0.0f;
			break;

		// z/Z
		case 122 :
		case 90 :
			m_viewDistanceVelocity = 0.0f;
			m_verticalMovementVelocity = 0.0f;
			break;
		// x/X
		case 120 :
		case 88 :
			m_viewDistanceVelocity = 0.0f;
			m_verticalMovementVelocity = 0.0f;
			break;

		// SPACE/F
		case 32 :
		case 102 :
			m_shooting = false;
			break;
	}
}


void GameScreen::test() {

	AABBNode* node = NULL;
	vector<AABBNode*> nodes;
	nodes.push_back(modelObjects[4]->m_aabbTree);

	while (!nodes.empty()) {

		node = nodes.back();

		if (node->id == 163 || node->id == 299) 
			node->leaf = false;

		if (node->id == 73 || node->id == 138) {
			node->bbxmin = Vec3f(0.0f, 0.0f, 0.0f);
			node->bbxmax = Vec3f(0.0f, 0.0f, 0.0f);
		}

		nodes.pop_back();

		AABBNode* right = node->right;
		if (right != NULL) {
			nodes.push_back(right);
		}
		AABBNode* left = node->left;
		if (left != NULL) {
			nodes.push_back(left);
		}
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** setViewMode
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::setViewMode(int mode) {
	m_viewMode = mode;

	/// reset first person view
	if (m_viewMode == 2) {
		m_playerModel.m_yPos = 0.0f;
		m_cameraElevation = 2.6f;
		m_cameraDistance = 7.0f;
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** setViewMode
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::setNavigationMode(int mode) {
	m_navigationMode = mode;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** setAnimationSpeed
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::setAnimationSpeed(float speed) {
	m_animationSpeed = speed;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** regenerateDisplayLists
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::regenerateDisplayLists() {

	/// REGENERATE GAME OBJECT DL
	for (unsigned int i = 0; i < playerProjectiles.size(); ++i) {
		playerProjectiles[i]->regenerateDisplayList();
	}

	/// REGENERATE REQUIRED MODELS DL
	for (unsigned int i = 0; i < modelObjects.size(); ++i) {
		modelObjects[i]->regenerateDisplayList();
	}

	/// REGENERATE GAME OBSTACLES DL
	for (unsigned int i = 0; i < obstacleObjects.size(); ++i) {
		obstacleObjects[i]->regenerateDisplayList();
	}

	/// REGENERATE PRIMITIVE MODELS DL
	for (unsigned int i = 0; i < primitiveModels.size(); ++i) {
		primitiveModels[i]->regenerateDisplayList();
	}

	/// REGENERATE SPECIAL PRIMITIVES MODELS DL
	for (unsigned int i = 0; i < specialPrimitives.size(); ++i) {
		specialPrimitives[i]->regenerateDisplayList();
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** resetPlayer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void GameScreen::resetPlayer() {

	//setViewMode(2);
	m_playerModel.m_yPos = 0.0f;
	m_playerModel.orientationMatrix = NULL;
	m_movementVector = Vec3f(0.0f, 0.0f, 0.0f);
}