#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <system/Mesh.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                     Primitives
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

class Primitives {

private: 

public:

	/// primitive meshes
	Mesh* m_cubeMesh;
	Mesh* m_cylinderMesh;
	Mesh* m_icosahedron;

	/// methods
	Primitives();
	~Primitives();

	Mesh* createPlaneMesh(int, int, float);
	Mesh* createCubeMesh();
	Mesh* createCylinderMesh(int);
	Mesh* createIcosahedron();

	void recalculateNormals(float creaseAngle, bool weighted);
};
#endif