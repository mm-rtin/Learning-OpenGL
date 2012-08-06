#ifndef MESH_H
#define MESH_H

#include <system/Typedefs.hpp>
#include <system/Material.h>
#include <system/AABBTree.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                          Mesh
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

class Mesh {

private:

public:

	/// data
	vector<Vec3f> m_v;        // vertices
	vector<Vec3f> m_vo;       // vertices - original
	vector<Vec3f> m_n;	      // normal per face
	vector<Vec3f> m_npv;      // normal per vertex
	vector<Vec2f> m_texc;     // texture coordinates

	vector<int> m_vi;	      // vertex indices
	vector<int> m_ni;	      // normal indices
	vector<int> m_ti;	      // texture coordinates indices

	vector<int> m_mtlvi;      // vertex indices that begin with a new material
	vector<int> m_mtli;	      // material indices

	vector<Material*> m_mtl;  // materials
	vector<Vec3i> m_tex;      // texture to apply per set of vertices (vec3f.y to vec3f.z)

	/// custom data
	vector<Vec3f> m_se;       // vertices of a sharp edge (every set of 2 vertices defines 1 sharp edge visualization)
	AABBNode* m_aabbTree;     // AABB Tree root node
};

#endif