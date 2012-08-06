#ifndef MATH_3D_H
#define MATH_3D_H

#include <vector>
#include <system/Mesh.hpp>
#include <system/Typedefs.hpp>
#include <system/AABBTree.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                       Math3d
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

#define PI 3.14159265f
#define DEG2RAD(x) (x*PI)/180.0f

class Math3d {

private:

public:

	static int id_0;
	static int id_1;
	static int id_2;
	static int id_3;
	static int id_4;
	static int id_5;
	static int id_6;
	static int id_7;
	static int id_8;
	static int id_9;
	static int id_10;

	static bool verticesEqual(Vec3f& v0, Vec3f& v1);
	static void calculateNormal(Vec3f& result, const Vec3f point1, const Vec3f point2, const Vec3f point3);
	static void normalizeVector(Vec3f& v);

	/// normal calculations
	static void calculateFaceNormals(Mesh*);
	static void calculateModelFaceNormals(Mesh* mesh);
	static void calculateVertexNormals(vector<Vec3f>&, vector<Vec3f>&, vector<Vec3f>&, float, bool);
	static void calculateModelVertexNormals(Mesh*, float, bool);

	/// aabb tree calculations
	static void calculateBoundingBox(Vec3f&, Vec3f&, vector<Vec3f>&);
	static int findBestAABBTreeAxis(vector<Vec3f>&, Vec3f&);
	static int findLongestAABBTreeAxis(vector<Vec3f>&, Vec3f&);
	static AABBNode* buildAABBTree(vector<Vec3f>&, AABBTreeInfo&, int depth);
	static AABBNode* cloneAABBTree(AABBNode*);

	/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** crossProduct
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	static inline void crossProduct(Vec3f& product, const Vec3f v0, const Vec3f v1) {
		
		product.x = v0.y * v1.z - v1.y * v0.z;
		product.y = -v0.x * v1.z + v1.x * v0.z;
		product.z = v0.x * v1.y - v1.x * v0.y;
	}

	/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** intersects
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	static inline bool intersects(const Vec3f& v, const Vec3f& min, const Vec3f& max) {

		for (unsigned int i = 0; i < 3; i++) {
			if (v[i] < min[i] || v[i] > max[i]) return false;
		}
		return true;
	}

	/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** dotProduct
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	static inline float dotProduct(const Vec3f v0, const Vec3f v1) {
		
		return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
	}

};
#endif