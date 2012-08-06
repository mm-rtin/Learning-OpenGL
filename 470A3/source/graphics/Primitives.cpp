/*
     File:  Primitives.cpp

   Author:  Michael Martin
  Created:  Sep 14, 2010
   Edited:  Sep 14, 2010

  Purpose:	
     Note:
*/

#include <vector>
#include <graphics/Primitives.hpp>
#include <system/Math3d.hpp>
#include <system/Globals.hpp>

/// use for icosahedron
#define X .525731112119133606 
#define Z .850650808352039932

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                        Primitives
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Default Constructor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Primitives::Primitives() {

	/// create default primitives
	m_cubeMesh = createCubeMesh();
	m_cylinderMesh = createCylinderMesh(20);
	m_icosahedron = createIcosahedron();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Destructor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Primitives::~Primitives() {

}


/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** createPlaneMesh
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Mesh* Primitives::createPlaneMesh(int width, int height, float squareSize) {

	int w = width / squareSize;
	int h = height / squareSize;
	int n = h + 1;

	float wStart = -width / 2;
	float hStart = -height / 2;

	// populate vertices
	Mesh* planeVertices = new Mesh;

	float xPos = wStart;
	float zPos = hStart;
	for (int i = 0; i <= w; i++) {
		for (int j = 0; j <= h; j++) {
			planeVertices->m_v.push_back(Vec3f(xPos * squareSize - wStart*squareSize - (width/2), 0, zPos * squareSize - hStart*squareSize - (height/2)));
			zPos++;
		}
		xPos++;
		zPos = hStart;
	}

	// create final planeMesh
	Mesh* planeMesh = new Mesh;
	vector<Vec3f>& v = planeVertices->m_v;
	Vec3f normal;
	Vec3f p1, p2, p3;

	for (int k = 0; k < v.size() - n; k++) {
				
		if ((k+1) % n == 0) continue;
		
		/// triangle 1
		// vertices
		p1 = Vec3f(v[k].x, 0, v[k].z);
		p2 = Vec3f(v[k + n + 1].x, 0, v[k + n + 1].z); 
		p3 = Vec3f(v[k + n].x, 0, v[k + n].z); 
		// add vertices to mesh
		planeMesh->m_v.push_back(p1);
		planeMesh->m_v.push_back(p2);
		planeMesh->m_v.push_back(p3);
		// texture coordinates
		planeMesh->m_texc.push_back(Vec2f(0.0f, 0.0f));
		planeMesh->m_texc.push_back(Vec2f(1.0f, 1.0f));
		planeMesh->m_texc.push_back(Vec2f(0.0f, 1.0f));

		/// triangle 2
		// vertices
		p1 = Vec3f(v[k].x, 0, v[k].z);
		p2 = Vec3f(v[k + 1].x, 0, v[k + 1].z); 
		p3 = Vec3f(v[k + n + 1].x, 0, v[k + n + 1].z); 
		// add vertices to mesh
		planeMesh->m_v.push_back(p1);
		planeMesh->m_v.push_back(p2);
		planeMesh->m_v.push_back(p3);
		// texture coordinates
		planeMesh->m_texc.push_back(Vec2f(0.0f, 0.0f));
		planeMesh->m_texc.push_back(Vec2f(1.0f, 0.0f));
		planeMesh->m_texc.push_back(Vec2f(1.0f, 1.0f));
	}

	/// calculate normals
	Math3d::calculateVertexNormals(planeMesh->m_npv, planeMesh->m_v, planeMesh->m_se, CREASE_ANGLE, NORMALS_PER_VERTEX_WEIGHTED);
	Math3d::calculateFaceNormals(planeMesh);

	/// assign new texture - for entire plane (top)
	planeMesh->m_tex.push_back(Vec3i(0, 0, planeMesh->m_v.size()));

	/// create default material
	Material* m = new Material();
	m->m_ambient = Color4f(0.8f, 0.8f, 0.8f, 1.0f);
	m->m_diffuse = Color4f(1.0f, 1.0f, 1.0f, 1.0f);
	m->m_specular = Color4f(0.0f, 0.0f, 0.0f, 1.0f);
	m->m_emission = Color4f(0.4f, 0.4f, 0.4f, 1.0f);
	m->m_shininess = 128;
	planeMesh->m_mtl.push_back(m);

	/// planes don't need aabb
	planeMesh->m_aabbTree = NULL;
	
	return planeMesh;
}


/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** createCubeMesh
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Mesh* Primitives::createCubeMesh() {

	Mesh* cubeMesh = new Mesh;
	Vec3f p1, p2, p3;
	Vec3f normal;

	/// assign new texture (top)
	cubeMesh->m_tex.push_back(Vec3i(0, 0, 0));

	/// top 1
	p1 = Vec3f( 1.0f, 1.0f,-1.0f);
	p2 = Vec3f(-1.0f, 1.0f,-1.0f); 
	p3 = Vec3f(1.0f, 1.0f, 1.0f); 
	cubeMesh->m_v.push_back(p1);
	cubeMesh->m_v.push_back(p2);
	cubeMesh->m_v.push_back(p3);
	// texture coordinates
	cubeMesh->m_texc.push_back(Vec2f(1.0f, 1.0f));
	cubeMesh->m_texc.push_back(Vec2f(0.0f, 1.0f));
	cubeMesh->m_texc.push_back(Vec2f(1.0f, 0.0f));

	/// top 2
	p1 = Vec3f(1.0f, 1.0f, 1.0f);
	p2 = Vec3f(-1.0f, 1.0f,-1.0f);
	p3 = Vec3f(-1.0f, 1.0f, 1.0f);
	cubeMesh->m_v.push_back(p1);
	cubeMesh->m_v.push_back(p2);
	cubeMesh->m_v.push_back(p3);
	// texture coordinates
	cubeMesh->m_texc.push_back(Vec2f(1.0f, 0.0f));
	cubeMesh->m_texc.push_back(Vec2f(0.0f, 1.0f));
	cubeMesh->m_texc.push_back(Vec2f(0.0f, 0.0f));

	/// bottom 1
	p1 = Vec3f( 1.0f, -1.0f,-1.0f);
	p2 = Vec3f(1.0f, -1.0f, 1.0f);
	p3 = Vec3f(-1.0f, -1.0f,-1.0f);
	cubeMesh->m_v.push_back(p1);
	cubeMesh->m_v.push_back(p2);
	cubeMesh->m_v.push_back(p3);
	// texture coordinates
	cubeMesh->m_texc.push_back(Vec2f(1.0f, 0.0f));
	cubeMesh->m_texc.push_back(Vec2f(1.0f, 1.0f));
	cubeMesh->m_texc.push_back(Vec2f(0.0f, 0.0f));

	/// bottom 2
	p1 = Vec3f( 1.0f, -1.0f, 1.0f);
	p2 = Vec3f(-1.0f, -1.0f, 1.0f);
	p3 = Vec3f(-1.0f, -1.0f,-1.0f);
	cubeMesh->m_v.push_back(p1);
	cubeMesh->m_v.push_back(p2);
	cubeMesh->m_v.push_back(p3);
	// texture coordinates
	cubeMesh->m_texc.push_back(Vec2f(1.0f, 1.0f));
	cubeMesh->m_texc.push_back(Vec2f(0.0f, 1.0f));
	cubeMesh->m_texc.push_back(Vec2f(0.0f, 0.0f));

	/// set end position of texture 1
	cubeMesh->m_tex[0].z = cubeMesh->m_v.size();

	/// assign new texture (side)
	cubeMesh->m_tex.push_back(Vec3i(1, cubeMesh->m_v.size(), 0));

	/// front 1
	p1 = Vec3f( 1.0f, 1.0f, 1.0f);
	p2 = Vec3f(-1.0f, 1.0f, 1.0f);
	p3 = Vec3f( 1.0f,-1.0f, 1.0f);	
	cubeMesh->m_v.push_back(p1);
	cubeMesh->m_v.push_back(p2);
	cubeMesh->m_v.push_back(p3);
	// texture coordinates
	cubeMesh->m_texc.push_back(Vec2f(1.0f, 1.0f));
	cubeMesh->m_texc.push_back(Vec2f(0.0f, 1.0f));
	cubeMesh->m_texc.push_back(Vec2f(1.0f, 0.0f));

	/// front 2
	p1 = Vec3f( 1.0f,-1.0f, 1.0f);	
	p2 = Vec3f(-1.0f, 1.0f, 1.0f);
	p3 = Vec3f(-1.0f,-1.0f, 1.0f);
	cubeMesh->m_v.push_back(p1);
	cubeMesh->m_v.push_back(p2);
	cubeMesh->m_v.push_back(p3);
	// texture coordinates
	cubeMesh->m_texc.push_back(Vec2f(1.0f, 0.0f));
	cubeMesh->m_texc.push_back(Vec2f(0.0f, 1.0f));
	cubeMesh->m_texc.push_back(Vec2f(0.0f, 0.0f));

	/// back 1
	p1 = Vec3f( 1.0f, 1.0f, -1.0f);
	p2 = Vec3f( 1.0f,-1.0f, -1.0f);	
	p3 = Vec3f(-1.0f, 1.0f, -1.0f);
	cubeMesh->m_v.push_back(p1);
	cubeMesh->m_v.push_back(p2);
	cubeMesh->m_v.push_back(p3);
	// texture coordinates
	cubeMesh->m_texc.push_back(Vec2f(0.0f, 1.0f));
	cubeMesh->m_texc.push_back(Vec2f(0.0f, 0.0f));
	cubeMesh->m_texc.push_back(Vec2f(1.0f, 1.0f));

	/// back 2
	p1 = Vec3f( 1.0f,-1.0f, -1.0f);	
	p2 = Vec3f(-1.0f,-1.0f, -1.0f);
	p3 = Vec3f(-1.0f, 1.0f, -1.0f);
	cubeMesh->m_v.push_back(p1);
	cubeMesh->m_v.push_back(p2);
	cubeMesh->m_v.push_back(p3);
	// texture coordinates
	cubeMesh->m_texc.push_back(Vec2f(0.0f, 0.0f));
	cubeMesh->m_texc.push_back(Vec2f(1.0f, 0.0f));
	cubeMesh->m_texc.push_back(Vec2f(1.0f, 1.0f));

	/// left 1
	p1 = Vec3f(-1.0f, 1.0f, 1.0f);
	p2 = Vec3f(-1.0f, 1.0f,-1.0f);
	p3 = Vec3f(-1.0f,-1.0f, 1.0f);
	cubeMesh->m_v.push_back(p1);
	cubeMesh->m_v.push_back(p2);
	cubeMesh->m_v.push_back(p3);
	// texture coordinates
	cubeMesh->m_texc.push_back(Vec2f(1.0f, 1.0f));
	cubeMesh->m_texc.push_back(Vec2f(0.0f, 1.0f));
	cubeMesh->m_texc.push_back(Vec2f(1.0f, 0.0f));

	/// left 2
	p1 = Vec3f(-1.0f,-1.0f, 1.0f);
	p2 = Vec3f(-1.0f, 1.0f,-1.0f);
	p3 = Vec3f(-1.0f,-1.0f,-1.0f);
	cubeMesh->m_v.push_back(p1);
	cubeMesh->m_v.push_back(p2);
	cubeMesh->m_v.push_back(p3);
	// texture coordinates
	cubeMesh->m_texc.push_back(Vec2f(1.0f, 0.0f));
	cubeMesh->m_texc.push_back(Vec2f(0.0f, 1.0f));
	cubeMesh->m_texc.push_back(Vec2f(0.0f, 0.0f));

	/// right 1
	p1 = Vec3f(1.0f, 1.0f, 1.0f);
	p2 = Vec3f(1.0f,-1.0f, 1.0f);
	p3 = Vec3f(1.0f, 1.0f,-1.0f);
	cubeMesh->m_v.push_back(p1);
	cubeMesh->m_v.push_back(p2);
	cubeMesh->m_v.push_back(p3);
	// texture coordinates
	cubeMesh->m_texc.push_back(Vec2f(0.0f, 1.0f));
	cubeMesh->m_texc.push_back(Vec2f(0.0f, 0.0f));
	cubeMesh->m_texc.push_back(Vec2f(1.0f, 1.0f));

	/// right 2
	p1 = Vec3f(1.0f,-1.0f, 1.0f);
	p2 = Vec3f(1.0f,-1.0f,-1.0f);
	p3 = Vec3f(1.0f, 1.0f,-1.0f);
	cubeMesh->m_v.push_back(p1);
	cubeMesh->m_v.push_back(p2);
	cubeMesh->m_v.push_back(p3);
	// texture coordinates
	cubeMesh->m_texc.push_back(Vec2f(0.0f, 0.0f));
	cubeMesh->m_texc.push_back(Vec2f(1.0f, 0.0f));
	cubeMesh->m_texc.push_back(Vec2f(1.0f, 1.0f));

	/// set end position of texture 2
	cubeMesh->m_tex[1].z = cubeMesh->m_v.size();

	/// calculate normals
	Math3d::calculateVertexNormals(cubeMesh->m_npv, cubeMesh->m_v, cubeMesh->m_se, CREASE_ANGLE, NORMALS_PER_VERTEX_WEIGHTED);
	Math3d::calculateFaceNormals(cubeMesh);

	/// create default material
	Material* m = new Material();
	m->m_ambient = Color4f(0.8f, 0.8f, 0.8f, 1.0f);
	m->m_diffuse = Color4f(1.0f, 1.0f, 1.0f, 1.0f);
	m->m_specular = Color4f(0.0f, 0.0f, 0.0f, 1.0f);
	m->m_emission = Color4f(0.4f, 0.4f, 0.4f, 1.0f);
	m->m_shininess = 128;
	cubeMesh->m_mtl.push_back(m);

	// generate AABB Tree
	AABBTreeInfo aabbTreeInfo;
	aabbTreeInfo.leafVertexCount = 3;
	aabbTreeInfo.maxTreeDepth = AABB_TREE_MAX_DEPTH;
	cubeMesh->m_aabbTree = Math3d::buildAABBTree(cubeMesh->m_v, aabbTreeInfo, 0);

	//cubeMesh->m_aabbTree = NULL;

	return cubeMesh;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** createCylinderMesh
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Mesh* Primitives::createCylinderMesh(int sides) {

	Mesh* cylinderMesh = new Mesh;

	float textureIncrement = 1.0f / float(sides);
	float textureCoordinate1 = 0.0f;
	float textureCoordinate2 = textureIncrement;
	float angle;

	// vector holds circle x/y coordinates
	vector<float> x;
	vector<float> y;
	x.reserve(sides);
	y.reserve(sides);

	for (int i = 0; i < sides; ++i)  {
		angle = 360 * i / (sides - 1);
		x[i] = sin(DEG2RAD(angle));
		y[i] = cos(DEG2RAD(angle));
	}

	// vertices
	Vec3f v0;
	Vec3f v1;
	Vec3f v2;
	Vec3f v3;

	Vec3f p1, p2, p3;
	// normal
	Vec3f normal;

	/// assign new texture 1 (top)
	cylinderMesh->m_tex.push_back(Vec3i(0, 0, 0));

	/// for each circle coordinate create top/bottom sections
	for (int j = 0; j < sides - 1; ++j) {
		
		v0 = Vec3<float>(x[j], y[j], -1);
		v1 = Vec3<float>(x[j + 1], y[j + 1], -1);
		v2 = Vec3<float>(x[j], y[j], 1);
		v3 = Vec3<float>(x[j + 1], y[j + 1], 1);
		
		// top
		p1 = Vec3f(0, 0, -1);
		p2 = Vec3f(v0.x, v0.y, -1);
		p3 = Vec3f(v1.x, v1.y, -1);
		cylinderMesh->m_v.push_back(p1);
		cylinderMesh->m_v.push_back(p2);
		cylinderMesh->m_v.push_back(p3);
		// texture coordinates
		cylinderMesh->m_texc.push_back(Vec2f(0.5f, 0.5f));
		cylinderMesh->m_texc.push_back(Vec2f(y[j] / 2.0f + 0.5f, x[j] / 2.0f + 0.5f));
		cylinderMesh->m_texc.push_back(Vec2f(y[j + 1] / 2.0f + 0.5f, x[j + 1] / 2.0f + 0.5f));

		// bottom
		p1 = Vec3f(0, 0, 1);
		p2 = Vec3f(v1.x, v1.y, 1);
		p3 = Vec3f(v0.x, v0.y, 1);
		cylinderMesh->m_v.push_back(p1);
		cylinderMesh->m_v.push_back(p2);
		cylinderMesh->m_v.push_back(p3);
		// texture coordinates
		cylinderMesh->m_texc.push_back(Vec2f(0.5f, 0.5f));
		cylinderMesh->m_texc.push_back(Vec2f(y[j + 1] / 2.0f + 0.5f, x[j + 1] / 2.0f + 0.5f));
		cylinderMesh->m_texc.push_back(Vec2f(y[j] / 2.0f + 0.5f, x[j] / 2.0f + 0.5f));

		textureCoordinate1 += textureIncrement;
		textureCoordinate2 += textureIncrement;
	}

	/// set end position of texture 1
	cylinderMesh->m_tex[0].z = cylinderMesh->m_v.size();

	textureIncrement = 1.0f / float(sides) * 10;

	// reset texture coordinates
	textureCoordinate1 = 0.0f;
	textureCoordinate2 = textureIncrement;


	/// assign new texture 2 (side)
	cylinderMesh->m_tex.push_back(Vec3i(1, cylinderMesh->m_v.size(), 0));

	/// for each circle coordinate create side wall sections
	for (int k = 0; k < sides - 1; ++k) {
		
		v0 = Vec3<float>(x[k], y[k], -1);
		v1 = Vec3<float>(x[k + 1], y[k + 1], -1);
		v2 = Vec3<float>(x[k], y[k], 1);
		v3 = Vec3<float>(x[k + 1], y[k + 1], 1);
		
		// side wall 1
		cylinderMesh->m_v.push_back(v0);
		cylinderMesh->m_v.push_back(v2);
		cylinderMesh->m_v.push_back(v1);
		// texture coordinates
		cylinderMesh->m_texc.push_back(Vec2f(textureCoordinate1, 0.0f));
		cylinderMesh->m_texc.push_back(Vec2f(textureCoordinate1, 1.0f));
		cylinderMesh->m_texc.push_back(Vec2f(textureCoordinate2, 0.0f));

		// side wall 2
		cylinderMesh->m_v.push_back(v2);
		cylinderMesh->m_v.push_back(v3);
		cylinderMesh->m_v.push_back(v1);
		// texture coordinates
		cylinderMesh->m_texc.push_back(Vec2f(textureCoordinate1, 1.0f));
		cylinderMesh->m_texc.push_back(Vec2f(textureCoordinate2, 1.0f));
		cylinderMesh->m_texc.push_back(Vec2f(textureCoordinate2, 0.0f));

		textureCoordinate1 += textureIncrement;
		textureCoordinate2 += textureIncrement;
	}

	/// set end position of texture 2
	cylinderMesh->m_tex[1].z = cylinderMesh->m_v.size();

	/// calculate normals
	Math3d::calculateVertexNormals(cylinderMesh->m_npv, cylinderMesh->m_v, cylinderMesh->m_se, CREASE_ANGLE, NORMALS_PER_VERTEX_WEIGHTED);
	Math3d::calculateFaceNormals(cylinderMesh);

	/// create default material
	Material* m = new Material();
	m->m_ambient = Color4f(0.8f, 0.8f, 0.8f, 1.0f);
	m->m_diffuse = Color4f(1.0f, 1.0f, 1.0f, 1.0f);
	m->m_specular = Color4f(0.0f, 0.0f, 0.0f, 1.0f);
	m->m_emission = Color4f(0.4f, 0.4f, 0.4f, 1.0f);
	m->m_shininess = 128;
	cylinderMesh->m_mtl.push_back(m);

	// generate AABB Tree
	AABBTreeInfo aabbTreeInfo;
	aabbTreeInfo.leafVertexCount = 3;
	aabbTreeInfo.maxTreeDepth = AABB_TREE_MAX_DEPTH;
	cylinderMesh->m_aabbTree = Math3d::buildAABBTree(cylinderMesh->m_v, aabbTreeInfo, 0);

	//cylinderMesh->m_aabbTree = NULL;
	
	return cylinderMesh;
}


/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** createIcosahedron
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Mesh* Primitives::createIcosahedron() {

	// X .525731112119133606 
	// Z .850650808352039932

	Vec3f p1, p2, p3;

	float vdata[12][3] = {    
	   {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},    
	   {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},    
	   {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0} 
	};
	int tindices[20][3] = { 
	   {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},    
	   {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},    
	   {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6}, 
	   {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };
	
	
	Mesh* icosahedronMesh = new Mesh;

	for (int i = 0; i < 20; i++) {
		p1 = Vec3f(vdata[tindices[i][0]][0], vdata[tindices[i][0]][1], vdata[tindices[i][0]][2]);
		p2 = Vec3f(vdata[tindices[i][1]][0], vdata[tindices[i][1]][1], vdata[tindices[i][1]][2]);
		p3 = Vec3f(vdata[tindices[i][2]][0], vdata[tindices[i][2]][1], vdata[tindices[i][2]][2]);

		icosahedronMesh->m_v.push_back(p1);
		icosahedronMesh->m_v.push_back(p3);
		icosahedronMesh->m_v.push_back(p2);
	}

	///** GENERATE DEFAULT TEXTURE COORDINATES
	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	int rowCount = 0;
	int div = 12;
	float xInc = 1.0f / (float)div;
	float yInc = 1.0f / (float)div;
	float tx = 0.0f;
	float ty = 0.0f;

	for (int i = 0; i < icosahedronMesh->m_v.size(); i++) {

		if (i % 3 == 0) {
			if (rowCount % 2 == 0)
				icosahedronMesh->m_texc.push_back(V2f(tx, ty));
			else
				icosahedronMesh->m_texc.push_back(V2f(tx, ty));

		} else if (i % 3 == 1) {
			if (rowCount % 2 == 0)
				icosahedronMesh->m_texc.push_back(V2f(tx + xInc, ty));
			else
				icosahedronMesh->m_texc.push_back(V2f(tx + xInc, ty + yInc));

		} else {
			if (rowCount % 2 == 0)
				icosahedronMesh->m_texc.push_back(V2f(tx + xInc, ty + yInc));
			else
				icosahedronMesh->m_texc.push_back(V2f(tx, ty + yInc));

			tx += xInc;
		}
		if (i % div == 0) {
			ty += yInc;
			rowCount++;
		}
	}

	icosahedronMesh->m_tex.push_back(Vec3i(0, 0, icosahedronMesh->m_v.size()));

	/// calculate normals
	Math3d::calculateVertexNormals(icosahedronMesh->m_npv, icosahedronMesh->m_v, icosahedronMesh->m_se, CREASE_ANGLE, NORMALS_PER_VERTEX_WEIGHTED);
	Math3d::calculateFaceNormals(icosahedronMesh);

	// generate AABB Tree
	AABBTreeInfo aabbTreeInfo;
	aabbTreeInfo.leafVertexCount = 3;
	aabbTreeInfo.maxTreeDepth = AABB_TREE_MAX_DEPTH;
	icosahedronMesh->m_aabbTree = Math3d::buildAABBTree(icosahedronMesh->m_v, aabbTreeInfo, 0);

	//icosahedronMesh->m_aabbTree = NULL;

	return icosahedronMesh;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** recalculateNormals
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Primitives::recalculateNormals(float creaseAngle, bool weighted) {

	// cube mesh
	Math3d::calculateVertexNormals(m_cubeMesh->m_npv, m_cubeMesh->m_v, m_cubeMesh->m_se, creaseAngle, weighted);
	// cylinder mesh
	Math3d::calculateVertexNormals(m_cylinderMesh->m_npv, m_cylinderMesh->m_v, m_cylinderMesh->m_se, creaseAngle, weighted);
	// icosahedron mesh
	Math3d::calculateVertexNormals(m_icosahedron->m_npv, m_icosahedron->m_v, m_icosahedron->m_se, creaseAngle, weighted);
}
