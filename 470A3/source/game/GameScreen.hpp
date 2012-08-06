#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <vector>

#include <system/Screen.hpp>
#include <game/Obstacle.hpp>

#include <game/GameObject.hpp>
#include <graphics/ModelObject.hpp>
#include <graphics/Primitives.hpp>
#include <graphics/PrimitiveModel.hpp>
#include <system/Quaternion.h>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                    GameScreen
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

class GameScreen : public Screen {

private:

	/// screen properties
	int m_viewMode;
	int m_navigationMode;
	int m_initialWidth;
	int m_initialHeight;

	/// light properties
    GLfloat m_ambientLight[4];
    GLfloat m_diffuseLight[4];
    GLfloat m_specular[4];

	/// materials
	GLfloat m_no_mat[4];
	GLfloat m_mat_low_ambient[4];
	GLfloat m_mat_high_ambient[4];
	GLfloat m_mat_ambient_color[4];
	GLfloat m_mat_high_diffuse[4];
	GLfloat m_mat_low_diffuse[4];
	GLfloat m_mat_high_specular[4];
	GLfloat m_mat_low_specular[4];
	GLfloat m_mat_low_shininess[1];
	GLfloat m_mat_high_shininess[1];
	GLfloat m_mat_emission[4];

	/// light location/direction
	float m_lightDistanceIncrement;
	float m_lightAngleX;
	float m_lightAngleY;
	float m_lightAngleXSpeed;
	float m_lightAngleYSpeed;
	float m_lightAngleXVelocity;
	float m_lightAngleYVelocity;
	GLfloat	m_lightPos[4];
	GLfloat m_spotDir[3];

	/// world properties
	int m_worldWidth;
	int m_worldHeight;
	float m_groundPlanePosition;
	float m_worldPlaneSquareSize;
	float m_backgroundPlaneSquareSize;
	float m_requiredObjectsBoundaryPadding;
	float m_requiredObjectsScale;

	/// game properties
	bool m_victory;
	int m_numberOfObstacles;
	int m_obstacleBoundaryPadding;
	float m_obstacleMaxWidth;
	float m_obstacleMaxLength;
	float m_obstacleMaxHeight;
	float m_animationSpeed;

	/// player movement
	Vec3f m_movementVector;
	Vec3f m_flyingVector;
	float m_rotationAngle;
	float m_movementSpeed;
	float m_movementSpeedLimit;
	float m_rotationVelocity;
	float m_activeRotationVelocity;
	float m_movementVelocity;
	float m_verticalMovementSpeed;
	float m_verticalMovementVelocity;
	int m_movementDirection;
	bool m_turningLeft;
	bool m_turningRight;

	/// player model properties
	float orientationMatrix[16];
	float m_playerHeading;
	float m_playerPitch;
	float m_playerRoll;

	// increments
	float m_rotationIncrement;
	float m_accelerationSpeed;
	float m_brakingSpeed;

	/// view properties
	float m_rotateAngle;
	float m_viewX;
	float m_viewY;
	float m_viewZ;
	float m_viewYOffset;

	/// camera properties
	float m_cameraX;
	float m_cameraY;
	float m_cameraZ;
	float m_cameraDistance;
	float m_cameraElevation;
	// increments
	float m_cameraElevationIncrement1;
	float m_cameraElevationIncrement2;
	float m_cameraRotationIncrement;
	float m_distanceIncrement;

	/// overview view properties
	float m_viewRotateAngle;
	float m_viewCameraX;
	float m_viewCameraY;
	float m_viewCameraZ;
	float m_viewCameraLookY;
	float m_viewCameraDistance;
	// increments
	float m_viewRotationSpeed;
	float m_viewHeightSpeed;
	float m_viewRotationVelocity;
	float m_viewHeightVelocity;
	float m_viewDistanceIncrement;
	float m_viewDistanceSpeed;
	float m_viewDistanceVelocity;

	/// orthographic properties
	float m_orthoSize;
	float m_orthoSizeIncrement;

	/// mouse properties
	int m_mouseX;
	int m_mouseY;
	bool m_mouseLeftPressed;
	bool m_mouseRightPressed;

	/// rotating box properties
	float m_boxAngle;
	float m_boxRotationIncrement;

	/// rotating box (static) properties
	float m_box2Angle;
	float m_boxRotation2Increment;
	float m_box2_x;
	float m_box2_z;

	/// projectile properties
	Vec3f m_shootVector;
	int m_projectilePoolCount;
	int m_currentProjectileID;
	float m_projectileDistance;
	float m_projectileSpeed;
	float m_projectileFallRate;

	/// models
	ModelObject m_playerModel;

	/// game objects
	GameObject m_hazard1;
	GameObject m_hazard2;
	GameObject m_hazard3;
	GameObject m_hazard4;

	/// required models
	ModelObject m_carModel1;
	ModelObject m_carModel2;
	ModelObject m_alModel;
	ModelObject m_asuModel;
	ModelObject m_buildingModel;

	/// primitive models
	PrimitiveModel m_worldPlane;
	PrimitiveModel m_backgroundPlane;
	PrimitiveModel m_startPlane;
	PrimitiveModel m_endPlane;

	/// rotation objects
	PrimitiveModel m_rotatingObject1;
	PrimitiveModel m_rotatingObject2;
	PrimitiveModel m_rotatingObject3;
	PrimitiveModel m_rotatingObject4;

	/// texture objects
	PrimitiveModel m_textureObject1;
	PrimitiveModel m_textureObject2;
	PrimitiveModel m_textureObject3;

	/// spline properties
	GLuint m_splineDL;
	int m_splineSegments;
	int m_splineSteps;

	/// shooting properties
	bool m_shooting;
	float m_lastFired;
	float m_firingRate;

	/// spline movement properties
	Vec3f m_splineDirection;
	unsigned int m_currentSplineIndex;

	/// array of game objects
	std::vector<GameObject*> playerProjectiles;
	/// array of required models
	std::vector<ModelObject*> modelObjects;
	/// array of primitive models
	std::vector<PrimitiveModel*> primitiveModels;
	/// array of obstacle objects
	std::vector<Obstacle*> obstacleObjects;
	/// array of special primitives
	std::vector<PrimitiveModel*> specialPrimitives;

	/// array of spline path vertices
	std::vector<Vec3f> splineVertices;
	std::vector<Vec3f> splineControlVertices;

public:

	GameScreen();
	~GameScreen();

	/// screen methods
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

	/// lighting methods
	void setupLighting();
	void updateLighting();
	void changeGlobalMaterial(int);

	/// view/camera methods
	void rotateView(float);
	void updateView();
	void setViewMode(int);
	void setNavigationMode(int);

	/// world methods
	void createEnvironmentObjects();
	void createObstacles();

	/// spline methods
	void createSpline(int, int);
	void drawSpline();

	/// game methods
	void startNewGame();
	void checkVictoryConditions(float, float);

	/// player methods
	void setPlayerToStart();
	void updateMovementVector();

	/// misc methods
	void setAnimationSpeed(float);
	void regenerateDisplayLists();
	void shootProjectile(int);
	void resetPlayer();

	/// object load mehods
	void loadModels();
	void loadGameObjects();

	void test();
};

#endif