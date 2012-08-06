/*
     File:	Math3d.cpp

  Authors:	Michael Martin
  Created:	Oct 03, 2010

  Purpose:	
	 Note:
*/

#include <system/Math3d.hpp>

#include <iostream>
#include <fstream>


/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                       Math3d
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

#define MIN2(a,b) (((a) < (b))?(a):(b))
#define MAX2(a,b) (((a) > (b))?(a):(b))


	int Math3d::id_0 = 0;
	int Math3d::id_1 = 0;
	int Math3d::id_2 = 0;
	int Math3d::id_3 = 0;
	int Math3d::id_4 = 0;
	int Math3d::id_5 = 0;
	int Math3d::id_6 = 0;
	int Math3d::id_7 = 0;
	int Math3d::id_8 = 0;
	int Math3d::id_9 = 0;
	int Math3d::id_10 = 0;


/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** verticesEqual
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool Math3d::verticesEqual(Vec3f& v0, Vec3f& v1) {

	return v0.equalWithAbsError(v1, .001);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** calculateNormal
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Math3d::calculateNormal(Vec3f& normal, const Vec3f p1, const Vec3f p2, const Vec3f p3) {
	
	/// temp vectors
	Vec3f v0;
	Vec3f v1;	

	/// get two vectors from 3 points - counter-clockwise winding
	// vector 1
	v0.x = p2.x - p1.x;
	v0.y = p2.y - p1.y;
	v0.z = p2.z - p1.z;
	// vector 2
	v1.x = p3.x - p1.x;
	v1.y = p3.y - p1.y;
	v1.z = p3.z - p1.z;

	// cross product of two vectors returns normal vector
	Math3d::crossProduct(normal, v0, v1);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** normalizeVector
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Math3d::normalizeVector(Vec3f& v) {

	float length = sqrt(v.x * v.x + v.y*v.y + v.z*v.z);

    v.x /= length;
    v.y /= length;
    v.z /= length;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** calculateFaceNormals
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Math3d::calculateFaceNormals(Mesh* mesh) {

	/// clear existing normals
	mesh->m_n.clear();

	// normal
	Vec3f normal;

	/// for every 3rd vertex in m_v : calculate normal for the ith, i+1, and i+2 vertices (1 face)
	for (int i = 0; i < mesh->m_v.size(); i+=3) {

		// calculate normal
		Math3d::calculateNormal(normal, mesh->m_v[i], mesh->m_v[i + 1], mesh->m_v[i + 2]);
		Math3d::normalizeVector(normal);

		// push the same normal 3 times to fill the normal vector
		mesh->m_n.push_back(normal);
		mesh->m_n.push_back(normal);
		mesh->m_n.push_back(normal);
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** calculateModelFaceNormals
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Math3d::calculateModelFaceNormals(Mesh* mesh) {

	/// data references for convenience purpose
	vector<int>& vi = mesh->m_vi;
	vector<Vec3f>& v = mesh->m_v;
	vector<Vec3f>& n = mesh->m_n;

	/// clear existing normals
	n.clear();

	/// for each vertex (vertex indices)
	Vec3f v1, v2, v3;      // base vertices
	Vec3f normal;          // normal
	for (int i = 0; i < vi.size(); i+=3) {

		// get vertices of face where vertex (i) is part of
		v1 = v[vi[i]];
		v2 = v[vi[i + 1]];
		v3 = v[vi[i + 2]];

		// calculate normal
		Math3d::calculateNormal(normal, v1, v2, v3);
		Math3d::normalizeVector(normal);

		// push the same normal 3 times to fill the normal vector
		n.push_back(normal);
		n.push_back(normal);
		n.push_back(normal);
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** calculateVertexNormals
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Math3d::calculateVertexNormals(vector<Vec3f>& n, vector<Vec3f>& v, vector<Vec3f>& se, float creaseAngle, bool weighted) {

	/// intialize creaseAngle
	float cosineAngle = cos(DEG2RAD(creaseAngle));

	/// clear existing normals
	n.clear();
	/// clear existing sharp edges
	se.clear();

	// counters
	int i = 0;
	int j = 0;

	///** GET SHARED VERTICES
	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	// 2D vector
	vector<std::vector<int>> sharedVertices;

	/// for each vertex
	for (i = 0; i < v.size(); ++i) {

		vector<int> row;
		/// find shared vertices
		for (j = 0; j < v.size(); ++j) {
			if (Math3d::verticesEqual(v[i], v[j]) && i != j) {
				/// push shared vertex (j) to (i vertex) shared list
				row.push_back(j);
			}
		}
		sharedVertices.push_back(row);
	}

	///** GET PER VERTEX NORMALS
	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	// averaged flag
	bool averaged = false;

	// normal - if weighted is used: will not be normalized
	Vec3f normal0(0.0f, 0.0f, 0.0f);
	Vec3f normal1(0.0f, 0.0f, 0.0f);
	// normalized version for dotProduct calculation
	Vec3f normal0N;
	Vec3f normal1N;

	/// for each vertex
	for (i = 0; i < v.size(); ++i) {

		Vec3f v1, v2, v3;  // base vertice
		averaged = false;

		// normal of the vertex
		Vec3f vertexNormal(0.0f, 0.0f, 0.0f);

		// get vertices of face where vertex (i) is part of
		int offset = i % 3;
		v1 = v[i - offset];
		v2 = v[i - offset + 1];
		v3 = v[i - offset + 2];

		// get base vertex normal
		Math3d::calculateNormal(normal0, v1, v2, v3);

		/// if NOT weighted -> normalize
		if (!weighted) {
			Math3d::normalizeVector(normal0);
		}

		/// for each shared vertex
		for (j = 0; j < sharedVertices[i].size(); ++j) {
		
			Vec3f p1, p2, p3;  // shared triangle vertices
			// get triangle points (shared triangle)
			int offset = sharedVertices[i][j] % 3;
			p1 = v[sharedVertices[i][j] - offset];
			p2 = v[sharedVertices[i][j] - offset + 1];
			p3 = v[sharedVertices[i][j] - offset + 2];

			// calculate normal for shared triangle
			Math3d::calculateNormal(normal1, p1, p2, p3);

			/// if NOT weighted
			if (!weighted) {
				Math3d::normalizeVector(normal1);
			}

			// create normalized versions of normal0 and normal1 for dotProduct calculation which only work on vectors of unit length
			normal0N = Vec3f(normal0.x, normal0.y, normal0.z);
			normal1N = Vec3f(normal1.x, normal1.y, normal1.z);
			Math3d::normalizeVector(normal0N);
			Math3d::normalizeVector(normal1N);

			// average normal if angle between faces is less than m_creaseAngle
			if (Math3d::dotProduct(normal0N, normal1N) > cosineAngle || creaseAngle == 360.0f) {

				// set average normal for current vertex i
				vertexNormal.x += normal1.x;
				vertexNormal.y += normal1.y;
				vertexNormal.z += normal1.z;
				averaged = true;

			/// sharp edge between these faces
			} else {

				vector<Vec3f> sharpVertices;

				/// get 2 vertices that are common of the 2 faces (defines a shard edge)
				// check v1
				if (verticesEqual(v1, p1) || verticesEqual(v1, p2) || verticesEqual(v1, p3)) {
					sharpVertices.push_back(Vec3f(v1.x, v1.y, v1.z));
				}
				// check v2
				if (verticesEqual(v2, p1) || verticesEqual(v2, p2) || verticesEqual(v2, p3)) {
					sharpVertices.push_back(Vec3f(v2.x, v2.y, v2.z));
				}
				// check v3
				if (verticesEqual(v3, p1) || verticesEqual(v3, p2) || verticesEqual(v3, p3)) {
					sharpVertices.push_back(Vec3f(v3.x, v3.y, v3.z));
				}

				// add found sharp vertices to sharp edges vector (se) if exactly 2 vertices found
				if (sharpVertices.size() == 2) {
					se.push_back(sharpVertices[0]);
					se.push_back(sharpVertices[1]);
				}
			}
		}

		/// set vertex normal
		if (averaged) {
			Math3d::normalizeVector(vertexNormal);
			n.push_back(vertexNormal);
		} else {
			Math3d::normalizeVector(normal0);
			n.push_back(normal0);
		}
	}
}


/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** calculateModelVertexNormals
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Math3d::calculateModelVertexNormals(Mesh* mesh, float creaseAngle, bool weighted) {

	/// intialize creaseAngle
	float cosineAngle = cos(DEG2RAD(creaseAngle));

	/// data references for convenience purpose
	vector<int>& vi = mesh->m_vi;
	vector<Vec3f>& v = mesh->m_v;
	vector<Vec3f>& n = mesh->m_npv;
	vector<Vec3f>& se = mesh->m_se;

	/// clear existing normals
	n.clear();
	/// clear existing sharp edges
	se.clear();

	
	///** GET SHARED VERTICES
	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	// 2D vector
	vector<std::vector<int>> sharedVertices;
	
	// counters
	unsigned int i = 0;
	unsigned int j = 0;

	/// for each vertex (vertex indices)
	for (i = 0; i < vi.size(); i++) {
		
		vector<int> row;
		/// find shared vertices
		for (j = 0; j < vi.size(); j++) {
			if (vi[i] == vi[j]) {
				/// push shared vertex index (j) to (i vertex) shared list
				row.push_back(j);
			}
		}
		sharedVertices.push_back(row);
	}


	///** GET PER VERTEX NORMALS
	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	// averaged flag
	bool averaged = false;

	// normal - if weighted is used: will not be normalized
	Vec3f normal0(0.0f, 0.0f, 0.0f);
	Vec3f normal1(0.0f, 0.0f, 0.0f);
	// normalized version for dotProduct calculation
	Vec3f normal0N;
	Vec3f normal1N;

	/// for each vertex (vertex indices)
	for (i = 0; i < vi.size(); i++) {

		Vec3f v1, v2, v3;  // base vertice
		averaged = false;

		// normal of the vertex
		Vec3f vertexNormal(0.0f, 0.0f, 0.0f);

		// get vertices of face where vertex (i) is part of
		int offset = i % 3;
		v1 = v[vi[i - offset]];
		v2 = v[vi[i - offset + 1]];
		v3 = v[vi[i - offset + 2]];

		// get base vertex normal
		Math3d::calculateNormal(normal0, v1, v2, v3);

		/// if NOT weighted -> normalize
		if (!weighted) {
			Math3d::normalizeVector(normal0);
		}

		/// for each shared vertex
		for (j = 0; j < sharedVertices[i].size(); ++j) {
		
			Vec3f p1, p2, p3;  // shared triangle vertices
			// get triangle points (shared triangle)
			int offset = sharedVertices[i][j] % 3;
			p1 = v[vi[sharedVertices[i][j] - offset]];
			p2 = v[vi[sharedVertices[i][j] - offset + 1]];
			p3 = v[vi[sharedVertices[i][j] - offset + 2]];

			// calculate normal for shared triangle
			Math3d::calculateNormal(normal1, p1, p2, p3);

			/// if NOT weighted
			if (!weighted) {
				Math3d::normalizeVector(normal1);
			}

			//std::cout << Math3d::dotProduct(normal0, normal1) << " " << float(cosineAngle) << " " << (Math3d::dotProduct(normal0, normal1) > cosineAngle) << endl;

			// create normalized versions of normal0 and normal1 for dotProduct calculation which only work on vectors of unit length
			normal0N = Vec3f(normal0.x, normal0.y, normal0.z);
			normal1N = Vec3f(normal1.x, normal1.y, normal1.z);
			Math3d::normalizeVector(normal0N);
			Math3d::normalizeVector(normal1N);

			// average normal if angle between faces is less than m_creaseAngle
			if (Math3d::dotProduct(normal0N, normal1N) > cosineAngle || creaseAngle == 360.0f) {

				// set average normal for current vertex i
				vertexNormal.x += normal1.x;
				vertexNormal.y += normal1.y;
				vertexNormal.z += normal1.z;
				averaged = true;

			/// sharp edge between these faces
			} else {

				vector<Vec3f> sharpVertices;

				/// get 2 vertices that are common of the 2 faces (defines a shard edge)
				// check v1
				if (verticesEqual(v1, p1) || verticesEqual(v1, p2) || verticesEqual(v1, p3)) {
					sharpVertices.push_back(Vec3f(v1.x, v1.y, v1.z));
				}
				// check v2
				if (verticesEqual(v2, p1) || verticesEqual(v2, p2) || verticesEqual(v2, p3)) {
					sharpVertices.push_back(Vec3f(v2.x, v2.y, v2.z));
				}
				// check v3
				if (verticesEqual(v3, p1) || verticesEqual(v3, p2) || verticesEqual(v3, p3)) {
					sharpVertices.push_back(Vec3f(v3.x, v3.y, v3.z));
				}

				// add found sharp vertices to sharp edges vector (se) if exactly 2 vertices found
				if (sharpVertices.size() == 2) {
					se.push_back(sharpVertices[0]);
					se.push_back(sharpVertices[1]);
				}
			}
		}

		/// set vertex normal
		if (averaged) {
			Math3d::normalizeVector(vertexNormal);
			n.push_back(vertexNormal);
		} else {
			Math3d::normalizeVector(normal0);
			n.push_back(normal0);
		}
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** findLongestAABBTreeAxis
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int Math3d::findLongestAABBTreeAxis(vector<Vec3f>& v, Vec3f& bbxCenter) {

	/// axis loop (0 = x, 1 = y, 2 = z)
	int axis = 0;
	/// length of each axis
	float axisValues[3];
	/// values
	float min;
	float max;

	/// for each axis (x, y, z)
	for (axis = 0; axis < 3; axis++) {

		min = 0.0f;
		max = 0.0f;

		/// for each vertex
		for (int i = 0; i < v.size(); i++) {

			switch (axis) {
				case 0 : 
					if (v[i].x < min)
						min = v[i].x;
					else if (v[i].x > max)
						max = v[i].x;
					break;

				case 1 :
					if (v[i].y < min)
						min = v[i].y;
					else if (v[i].y > max)
						max = v[i].y;
					break;

				case 2:
					if (v[i].z < min)
						min = v[i].z;
					else if (v[i].z > max)
						max = v[i].z;
					break;
			}
		}
		
		axisValues[axis] = max - min;
	}

	/// find the longest axis
	int index = 0;
	int result = axisValues[0];
	for (int i = 1; i < 3; i++) {
		if (axisValues[i] > result) {
			result = axisValues[i];
			index = i;
		}
	}  

	return index;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** findBestAABBTreeAxis
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int Math3d::findBestAABBTreeAxis(vector<Vec3f>& v, Vec3f& bbxCenter) {

	/// axis loop (0 = x, 1 = y, 2 = z)
	int axis = 0;
	/// number of triangles on the left/right side
	int left = 0;
	int right = 0;
	/// the difference between the number of left and right triangles for each axis
	int axisValues[3];
	float triangleCenter[3];

	/// for each axis (x, y, z)
	for (axis = 0; axis < 3; axis++) {

		left = 0;
		right = 0;

		/// for each triangle
		for (int i = 0; i < v.size(); i+=3) {
					
				triangleCenter[0] = (v[i].x + v[i + 1].x + v[i + 2].x) / 3.0f;
				triangleCenter[1] = (v[i].y + v[i + 1].y + v[i + 2].y) / 3.0f;
				triangleCenter[2] = (v[i].z + v[i + 1].z + v[i + 2].z) / 3.0f;  

				/// triangle falls on left side of model for axis
				if (triangleCenter[axis] <= bbxCenter[axis])
					left++;
				/// triangle falls on right side of model for axis
				else
					right++;
		}

		/// save the difference between left and right triangle counts for each axis
		axisValues[axis] = abs(left - right);
	}

	/// set first axis (x) as current lowest difference between left and right
	int index = 0;
	int result = axisValues[0];

	/// find axis which has the lowest difference in the number of left and right triangles
	for (int i = 1; i < 3; i++) {
		if (axisValues[i] < result) {
			result = axisValues[i];
			index = i;
		}
	}  

	return index;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** buildAABBTree
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
AABBNode* Math3d::buildAABBTree(vector<Vec3f>& v, AABBTreeInfo &treeInfo, int depth) {

	/// number of vertices
	int vertexCount = v.size();

	/// create AABBNode
	AABBNode* aabbNode = new AABBNode();
	aabbNode->leaf = false;
	aabbNode->depth = depth;
	aabbNode->bbxmax = Vec3f(0.0f, 0.0f, 0.0f);
	aabbNode->bbxmin = Vec3f(0.0f, 0.0f, 0.0f);
	aabbNode->vertexCount = vertexCount;

	/// debugging > id creation
	switch (depth) {
		case 0:
			id_0 = 0;
			id_1 = 0;
			id_2 = 0;
			id_3 = 0;
			id_4 = 0;
			id_5 = 0;
			id_6 = 0;
			id_7 = 0;
			id_8 = 0;
			id_9 = 0;
			id_10 = 0;
			aabbNode->id = 0;
			break;
		case 1:
			aabbNode->id = id_1;
			id_1++;
			break;
		case 2:
			aabbNode->id = id_2;
			id_2++;
			break;
		case 3:
			id_3++;
			aabbNode->id = id_3;
			break;
		case 4:
			aabbNode->id = id_4;
			id_4++;
			break;
		case 5:
			aabbNode->id = id_5;
			id_5++;
			break;
		case 6:
			id_6++;
			aabbNode->id = id_6;
			break;
		case 7:
			aabbNode->id = id_7;
			id_7++;
			break;
		case 8:
			id_8++;
			aabbNode->id = id_8;
			break;
		case 9:
			aabbNode->id = id_9;
			id_9++;
			break;
		case 10:
			aabbNode->id = id_10;
			id_10++;
			break;
	}

	/// get the AABB for the current AABB tree node - assign to aabb node
	calculateBoundingBox(aabbNode->bbxmin, aabbNode->bbxmax, v);

	if (vertexCount > treeInfo.leafVertexCount && depth < treeInfo.maxTreeDepth) {

		/// get center
		Vec3f bbxCenter = (aabbNode->bbxmin + aabbNode->bbxmax) / 2.0f;

        /// find the best axis to split the mesh
		int axis = findBestAABBTreeAxis(v, bbxCenter);

		/// left and ride side vertex vectors
		vector<Vec3f> leftSide;
		vector<Vec3f> rightSide;  

		/// split mesh
		for (int i = 0; i < vertexCount; i+=3) {

			if (i + 1 < vertexCount && i + 2 < vertexCount) {

				float triangleCenter[3];
				triangleCenter[0] = (v[i].x + v[i + 1].x + v[i + 2].x) / 3.0f;
				triangleCenter[1] = (v[i].y + v[i + 1].y + v[i + 2].y) / 3.0f;
				triangleCenter[2] = (v[i].z + v[i + 1].z + v[i + 2].z) / 3.0f;

				/// triangle to left
				if (triangleCenter[axis] <= bbxCenter[axis]) {
					leftSide.push_back(v[i]);
					leftSide.push_back(v[i + 1]);
					leftSide.push_back(v[i + 2]);  

				/// triangle to right
				} else {
					rightSide.push_back(v[i]);
					rightSide.push_back(v[i + 1]);
					rightSide.push_back(v[i + 2]);  
				}  
			}
		}

		/// build left child nodes
		if (leftSide.size() > 0) {
			aabbNode->left = buildAABBTree(leftSide, treeInfo, depth + 1);
		} else {
			aabbNode->left = NULL;
		}

		/// build right child nodes
		if (rightSide.size() > 0) {
			aabbNode->right = buildAABBTree(rightSide, treeInfo, depth + 1);
		} else {
			aabbNode->right = NULL;
		}


	/// leaf node
	} else {

		aabbNode->leaf = true;
		aabbNode->left = NULL;
		aabbNode->right = NULL;
	}

	return aabbNode;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** cloneAABBTree
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
AABBNode* Math3d::cloneAABBTree(AABBNode* node) {

	if (node == NULL)
		return NULL;

	/// create AABBNode
	AABBNode* aabbNode = new AABBNode();

	/// copy
	aabbNode->bbxmax = Vec3f(node->bbxmax.x, node->bbxmax.y, node->bbxmax.z);
	aabbNode->bbxmin = Vec3f(node->bbxmin.x, node->bbxmin.y, node->bbxmin.z);
	aabbNode->depth = node->depth;
	aabbNode->leaf = node->leaf;

	if (node->left != NULL) {
		aabbNode->left = cloneAABBTree(node->left);
	} else {
		aabbNode->left = NULL;
	}

	if (node->right != NULL) {
		aabbNode->right = cloneAABBTree(node->right);
	} else {
		aabbNode->right = NULL;
	}

	return aabbNode;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** calculateBoundingBox - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Math3d::calculateBoundingBox(Vec3f& bbxmin, Vec3f& bbxmax, vector<Vec3f>& v) {
	
	if(v.empty() || v.size() == 0) {
		return;
	}	

	bbxmax = bbxmin = v[0];

	for(unsigned int i = 1; i < v.size(); i++) {	
		bbxmin.x = MIN2(bbxmin.x, v[i].x);
		bbxmin.y = MIN2(bbxmin.y, v[i].y);
		bbxmin.z = MIN2(bbxmin.z, v[i].z);

		bbxmax.x = MAX2(bbxmax.x, v[i].x);
		bbxmax.y = MAX2(bbxmax.y, v[i].y);
		bbxmax.z = MAX2(bbxmax.z, v[i].z);
	}
}