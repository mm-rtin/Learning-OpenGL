#ifndef MODEL_OBJECT_H
#define MODEL_OBJECT_H

#include <string>
#include <GL/freeglut.h>

#include <system/Mesh.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                      ModelObject
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

class ModelObject {

private:

public:

	/// display lists
	GLuint m_renderDL;
	GLuint m_visualizationDL;
	GLuint m_aabbVisualizationDL;

	AABBNode* m_aabbTree;

	/// object properties
	bool m_useTextures;
	float m_xPos;
	float m_yPos;
	float m_zPos;

	float m_xScale;
	float m_yScale;
	float m_zScale;

	float m_xRotate;
	float m_yRotate;
	float m_zRotate;
	float m_rotateAngle;

	float m_xRotate1;
	float m_yRotate1;
	float m_zRotate1;
	float m_rotateAngle1;

	float m_xRotate2;
	float m_yRotate2;
	float m_zRotate2;
	float m_rotateAngle2;

	float* orientationMatrix;

	/// visualization properties
	float m_normalSize;

	/// model
	Mesh* m_model;

	/// model parameters
	Vec3f bbxMin;
	Vec3f bbxMax;
	Vec3f bbxCenter;

	/// methods
	ModelObject();
	virtual ~ModelObject();
	
	virtual void initialize();

	virtual void loadModel(Mesh* mesh);
	virtual void activate();

	virtual void update();
	virtual void transformAABBTree(AABBNode*);
	virtual void transformMesh();
	virtual void renderModel(bool, bool);
	virtual void renderVisualizations(bool, bool, bool);
	virtual void renderAABBTree(AABBNode*, int, bool);
	virtual void draw();
	virtual void setTransformations();
	virtual void regenerateDisplayList();

	virtual bool checkCollision(Vec3f&);
};
#endif