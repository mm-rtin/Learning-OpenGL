#ifndef AABB_TREE_H
#define AABB_TREE_H

#include <system/Typedefs.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                      AABB TREE
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

struct AABBNode {

	Vec3f bbxmin;
	Vec3f bbxmax;
	AABBNode* left;
	AABBNode* right;
	int depth;
	bool leaf;
	int id;
	int vertexCount;
};

struct AABBTreeInfo {

	int maxTreeDepth;		
	int leafVertexCount;		  
};

#endif