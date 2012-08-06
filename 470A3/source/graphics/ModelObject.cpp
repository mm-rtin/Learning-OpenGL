/*
     File:	ModelObject.cpp

  Authors:	Michael Martin
  Created:	Sep 12, 2010

  Purpose:	
	 Note:
*/

#include <GL/freeglut.h>

#include <system/Globals.hpp>
#include <system/ModelLoader.hpp>
#include <graphics/ModelObject.hpp>
#include <system/Bitmap.h>
#include <system/Math3d.hpp>
#include <ImathMatrix.h>

#include <iostream>
#include <fstream>

#define DEG2RAD(x) (x*PI)/180.0f

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Constructor - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
ModelObject::ModelObject() {

	/// initialize properties
	m_useTextures = true;
	m_xPos = 0.0f;
	m_yPos = 0.0f;
	m_zPos = 0.0f;

	m_xScale = 1.0f;
	m_yScale = 1.0f;
	m_zScale = 1.0f;

	m_xRotate = 0.0f;
	m_yRotate = 1.0f;
	m_zRotate = 0.0f;
	m_rotateAngle = 0.0f;

	m_xRotate1 = 0.0f;
	m_yRotate1 = 1.0f;
	m_zRotate1 = 0.0f;
	m_rotateAngle1 = 0.0f;

	m_xRotate2 = 0.0f;
	m_yRotate2 = 0.0f;
	m_zRotate2 = 1.0f;
	m_rotateAngle2 = 0.0f;

	/// visualization properties
	m_normalSize = 0.5f;

	orientationMatrix = 0;

	m_aabbTree = NULL;
	m_model = 0;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Destructor - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
ModelObject::~ModelObject() {

	/// delete display list
	glDeleteLists(m_renderDL, 1);
	glDeleteLists(m_visualizationDL, 1);
	glDeleteLists(m_aabbVisualizationDL, 1);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** initialize - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ModelObject::initialize() {

	
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** activate
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ModelObject::activate() {
	
	if (g_resourceManager->m_modelMeshes[0] != m_model) {
		// generate AABB Tree
		AABBTreeInfo aabbTreeInfo;
		aabbTreeInfo.leafVertexCount = 3;
		aabbTreeInfo.maxTreeDepth = AABB_TREE_MAX_DEPTH;
		m_aabbTree = Math3d::buildAABBTree(m_model->m_v, aabbTreeInfo, 0);

		transformAABBTree(m_aabbTree);
	} else {
		m_aabbTree = NULL;
	}

	/// create display lists
	m_renderDL = glGenLists(1);
	m_visualizationDL = glGenLists(1);
	m_aabbVisualizationDL = glGenLists(1);

	/// render DL
	glNewList(m_renderDL, GL_COMPILE);
		renderModel(NORMALS_PER_VERTEX, USE_TEXTURES);
	glEndList();

	/// visualization DL
	glNewList(m_visualizationDL, GL_COMPILE);
		renderVisualizations(NORMALS_PER_VERTEX, SHARP_EDGES, NORMAL_VIS_OFF);
	glEndList();

	/// aabb visualization DL
	glNewList(m_aabbVisualizationDL, GL_COMPILE);
		renderAABBTree(m_aabbTree, AABB_TREE_DEPTH, VISUALIZE_LEAF);
	glEndList();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** loadModel - load model from file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ModelObject::loadModel(Mesh* mesh) {

	/// delete existing model
	if (m_model) {
		for	(unsigned int i = 0; i < m_model->m_mtl.size(); i++)
			if (m_model->m_mtl[i]->m_id > 0)
				glDeleteTextures(1, &m_model->m_mtl[i]->m_id);
		delete m_model;
	}

	// set model mesh
	m_model = mesh;

	// set aabbTree
	m_aabbTree = m_model->m_aabbTree;

	// transform mesh - corrects static model rotation for AABB calculations
	transformMesh();

	Math3d::calculateBoundingBox(bbxMin, bbxMax, m_model->m_v);
	bbxCenter = (bbxMin+bbxMax)*0.5f;

	// calculate face and vertex normals for model
	Math3d::calculateModelFaceNormals(m_model);
	Math3d::calculateModelVertexNormals(m_model, CREASE_ANGLE, NORMALS_PER_VERTEX_WEIGHTED);

	/// calculate normal size -> relative to bounding box size
	m_normalSize = (bbxMax - bbxMin).length() * 0.08;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** update - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ModelObject::update() {


}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
transformMesh
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ModelObject::transformMesh() {

	/// create transformation matrix
	float m[4][4];

	Matrix44<float> transformMatrix(m);
	transformMatrix.makeIdentity();

	transformMatrix.rotate(Vec3f(DEG2RAD(m_rotateAngle1) * m_xRotate1, DEG2RAD(m_rotateAngle1) * m_yRotate1, DEG2RAD(m_rotateAngle1) * m_zRotate1));

	/// mesh references
	vector<Vec3f>& vo = m_model->m_vo;
	vector<Vec3f>& v = m_model->m_v;

	/// apply transformMatrix to mesh
	for (int i = 0; i < vo.size(); i++) {

		Vec3f* originalVertex = &vo[i];
		Vec3f* transformedVertex = &v[i];

		*transformedVertex = *originalVertex * transformMatrix;
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** transformAABBTree - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ModelObject::transformAABBTree(AABBNode* aabbTree) {

	if (aabbTree != NULL) {

		/// create transformation matrix for aabb tree
		float m[4][4];

		Matrix44<float> transformMatrix(m);
		transformMatrix.makeIdentity();
		transformMatrix.translate(Vec3f(m_xPos, m_yPos, m_zPos));
		transformMatrix.scale(Vec3f(m_xScale, m_yScale, m_zScale));

		/// traverse tree
		AABBNode* node = NULL;
		vector<AABBNode*> nodes;
		nodes.push_back(aabbTree);

		while (!nodes.empty()) {

			/// get node
			node = nodes.back();

			/// transform AABB
			node->bbxmin *= transformMatrix;
			node->bbxmax *= transformMatrix;

			/// pop node
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

	regenerateDisplayList();
}


/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** checkCollision - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool ModelObject::checkCollision(Vec3f& v) {

	/// data reference
	AABBNode* aabbTree = m_aabbTree;

	if (m_aabbTree != NULL) {

		/// data
		bool intersects = false;

		/// check if point intersects root node
		if (!Math3d::intersects(v, aabbTree->bbxmin, aabbTree->bbxmax)) {
			return false;
		}

		/// traverse tree
		AABBNode* node = NULL;
		vector<AABBNode*> nodes;
		nodes.push_back(aabbTree);

		while (!nodes.empty()) {

			/// get node
			node = nodes.back();

			/// check intersection with AABB
			intersects = Math3d::intersects(v, node->bbxmin, node->bbxmax);

			if (intersects && node->leaf) {
				return true;
			}

			/// pop node
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

	return false;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** renderModel - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ModelObject::renderModel(bool perVertex, bool useTextures) {

	/// data references
	vector<int>& vi = m_model->m_vi;
	vector<int>& ni = m_model->m_ni;
	vector<int>& ti = m_model->m_ti;
	vector<Vec3f>& v = m_model->m_v;
	vector<Vec3f>& n = m_model->m_npv;
	vector<Vec2f>& t = m_model->m_texc;
	vector<int>& mtlvi = m_model->m_mtlvi;
	vector<int>& mtli = m_model->m_mtli;
	vector<Material*> mtl = m_model->m_mtl;

	///** DRAW MODEL
	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/// materials
	for (unsigned int i = 0; i < mtlvi.size() + 1; i++) {

		Material* m = i == 0 ? mtl[0] : mtl[mtli[i - 1]];

		// apply textures
		if (useTextures) {

			glEnable(GL_TEXTURE_2D);
			if (m->m_id > 0)
				glBindTexture(GL_TEXTURE_2D, m->m_id);
			else
				glBindTexture(GL_TEXTURE_2D, g_resourceManager->m_textures[12]);

		// no textures
		} else {
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		glBegin(GL_TRIANGLES);

			unsigned int start;
			unsigned int end;

			// set start
			if (i == 0)
				start = 0;
			else
				mtlvi[i - 1];
			// set end
			if (i == mtlvi.size()) 
				end = vi.size();
			else
				end = mtlvi[i];
				 
			/// vertex indices
			for (unsigned int j = start; j < end; j++) {

				// texture coordinate
				if (!t.empty() && !ti.empty() && useTextures) {
					glTexCoord2fv(&t[ti[j]].x);
				}
				
				// set normal per face
				if (!perVertex && !m_model->m_n.empty()) {
					glNormal3f(m_model->m_n[j].x, m_model->m_n[j].y, m_model->m_n[j].z);
					
				// set normal per vertex
				} else if (!n.empty()) {
					glNormal3fv(&n[j].x);
				}

				// set color
				if (useTextures) {
					glColor3f(1.0f, 1.0f, 1.0f);
				} else {
					Vec3f offset = (v[vi[j]] - bbxCenter);;
					glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
				}
				
				// draw vertex
				glVertex3fv(&v[vi[j]].x);
			}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** renderVisualizations - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ModelObject::renderVisualizations(bool perVertex, bool sharpEdges, bool normalsOff) {
	
	/// data references
	vector<Vec3f>& v = m_model->m_v;
	vector<int>& vi = m_model->m_vi;
	vector<Vec3f>* n = &m_model->m_npv;
	vector<Vec3f>* se = &m_model->m_se;

	glDisable(GL_LIGHTING);

	/// draw normal visualization
	if (!normalsOff) {
		glLineWidth(1.0f);
		glColor3ub(206, 229, 255);
		glBegin(GL_LINES);
		
			/// per face properties
			float xSum = 0.0f;
			float ySum = 0.0f;
			float zSum = 0.0f;

			/// for each vertex
			Vec3f normal;
			for (int i = 0; i < vi.size(); ++i) {

				// per vertex normals
				if (perVertex && !(*n).empty()) {

					// get normal
					normal = Vec3f((*n)[i].x, (*n)[i].y, (*n)[i].z);
				
					// scale normal
					normal *= m_normalSize;

					glVertex3f(v[vi[i]].x, v[vi[i]].y, v[vi[i]].z);
					glVertex3f(v[vi[i]].x + normal.x, v[vi[i]].y + normal.y, v[vi[i]].z + normal.z);

				// per face normals
				} else if (!m_model->m_n.empty()) {

					// for every 3 vertices (1 face)
					if ((i+1) % 3 == 0) {

						// get triangle center
						xSum += v[vi[i]].x;
						ySum += v[vi[i]].y;
						zSum += v[vi[i]].z;

						xSum /= 3;
						ySum /= 3;
						zSum /= 3;

						normal = Vec3f(m_model->m_n[i].x, m_model->m_n[i].y, m_model->m_n[i].z);

						// scale normal
						normal *= m_normalSize;

						glVertex3f(xSum, ySum, zSum);
						glVertex3f(xSum + normal.x, ySum + normal.y, zSum + normal.z);

						// reset center
						xSum = 0.0f;
						ySum = 0.0f;
						zSum = 0.0f;

					} else {

						xSum += v[vi[i]].x;
						ySum += v[vi[i]].y;
						zSum += v[vi[i]].z;
					}
				}
			}

		glEnd();
	}

	/// sharp edge visualizations
	if (sharpEdges) {
		glLineWidth(2.0f);
		glColor3ub(255, 156, 0);
		glBegin(GL_LINES);

			// draw sharp edges
			for (int j = 0; j < (*se).size(); ++j) {
				glVertex3f((*se)[j].x, (*se)[j].y, (*se)[j].z);
			}

		glEnd();
	}

	glLineWidth(1.0f);
	glEnable(GL_LIGHTING);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** renderAABBTree - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ModelObject::renderAABBTree(AABBNode* aabbTree, int depth, bool leaf) {

	glDisable(GL_LIGHTING);

	// disable depth showing for leaf
	if (leaf)
		depth = -99;

	/// AABB Tree visualization
	if (aabbTree != NULL) {

		/// traverse tree
		AABBNode* node = NULL;
		vector<AABBNode*> nodes;
		nodes.push_back(aabbTree);

		/// data
		Vec3f min;
		Vec3f max;

		glLineWidth(2.0f);

		glBegin(GL_LINES);
		while (!nodes.empty()) {

			node = nodes.back();

			/// get AABB
			min = node->bbxmin;
			max = node->bbxmax;

			nodes.pop_back();

			AABBNode* right = node->right;
			if (right != NULL) {
				nodes.push_back(right);
			}
			AABBNode* left = node->left;
			if (left != NULL) {
				nodes.push_back(left);
			}

			///** DRAW BOUNDING BOX
			///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			/// set color
			switch (node->depth) {

				case 0 :
					glColor3ub(0, 0, 0);
					break;
				case 1 :
					glColor3ub(255, 0, 0);
					break;
				case 2 :
					glColor3ub(255, 200, 0);
					break;
				case 3 :
					glColor3ub(200, 255, 0);
					break;
				case 4 :
					glColor3ub(36, 255, 0);
					break;
				case 5 :
					glColor3ub(0, 255, 162);
					break;
				case 6 :
					glColor3ub(0, 228, 255);
					break;
				case 7 :
					glColor3ub(252, 0, 255);
					break;
				case 8 :
					glColor3ub(128, 255, 255);
					break;
				case 9 :
					glColor3ub(255, 0, 128);
					break;
				default :
					glColor3ub(255, 255, 255);
			}

			
			if (((node->depth == depth || depth == -1 || (node->leaf && leaf)) && SHOW_NODE_ID < 0) || (SHOW_NODE_ID == node->id && node->depth == depth)) {

				/// front
				glVertex3f(min.x, min.y, min.z);
				glVertex3f(max.x, min.y, min.z);

				glVertex3f(max.x, min.y, min.z);
				glVertex3f(max.x, max.y, min.z);

				glVertex3f(max.x, max.y, min.z);
				glVertex3f(min.x, max.y, min.z);

				glVertex3f(min.x, max.y, min.z);
				glVertex3f(min.x, min.y, min.z);

				/// back
				glVertex3f(max.x, min.y, max.z);
				glVertex3f(max.x, max.y, max.z);

				glVertex3f(max.x, max.y, max.z);
				glVertex3f(min.x, max.y, max.z);

				glVertex3f(min.x, max.y, max.z);
				glVertex3f(min.x, min.y, max.z);

				glVertex3f(min.x, min.y, max.z);
				glVertex3f(max.x, min.y, max.z);

				/// right
				glVertex3f(max.x, min.y, min.z);
				glVertex3f(max.x, min.y, max.z);

				glVertex3f(max.x, min.y, max.z);
				glVertex3f(max.x, max.y, max.z);

				glVertex3f(max.x, max.y, max.z);
				glVertex3f(max.x, max.y, min.z);

				glVertex3f(max.x, max.y, min.z);
				glVertex3f(max.x, min.y, min.z);

				/// left
				glVertex3f(min.x, min.y, min.z);
				glVertex3f(min.x, min.y, max.z);

				glVertex3f(min.x, min.y, max.z);
				glVertex3f(min.x, max.y, max.z);

				glVertex3f(min.x, max.y, max.z);
				glVertex3f(min.x, max.y, min.z);

				glVertex3f(min.x, max.y, min.z);
				glVertex3f(min.x, min.y, min.z);

				/// top
				glVertex3f(min.x, max.y, min.z);
				glVertex3f(max.x, max.y, min.z);

				glVertex3f(max.x, max.y, min.z);
				glVertex3f(max.x, max.y, max.z);

				glVertex3f(max.x, max.y, max.z);
				glVertex3f(min.x, max.y, max.z);

				glVertex3f(min.x, max.y, max.z);
				glVertex3f(min.x, max.y, min.z);

				/// bottom
				glVertex3f(min.x, min.y, min.z);
				glVertex3f(max.x, min.y, min.z);

				glVertex3f(max.x, min.y, min.z);
				glVertex3f(max.x, min.y, max.z);

				glVertex3f(max.x, min.y, max.z);
				glVertex3f(min.x, min.y, max.z);

				glVertex3f(min.x, min.y, max.z);
				glVertex3f(min.x, min.y, min.z);

			}
		}
		glEnd();
	}

	glEnable(GL_LIGHTING);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** draw - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ModelObject::draw() {

	glPushMatrix();

	    /// VISUALIZE AABB Tree - 
		/// Don't apply transformations since the actual AABB tree vectors represent the actual model in space
		if (VISUALIZE_AABB_TREE) {
			if (DISPLAY_LIST) {
				glCallList(m_aabbVisualizationDL);
			} else {
				renderAABBTree(m_aabbTree, AABB_TREE_DEPTH, VISUALIZE_LEAF);
			}
		}

		/// set transformations
		setTransformations();

		/// draw model
		if (DISPLAY_LIST) {
			glCallList(m_renderDL);
		} else {
			renderModel(NORMALS_PER_VERTEX, USE_TEXTURES);
		}

		/// draw visualizations
		if (VISUALIZATIONS_ON) {
			if (DISPLAY_LIST) {
				glCallList(m_visualizationDL);
			} else {
				renderVisualizations(NORMALS_PER_VERTEX, SHARP_EDGES, NORMAL_VIS_OFF);
			}
		}

	glPopMatrix();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** regenerateDisplayList - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ModelObject::regenerateDisplayList() {

	/// delete display list
	glDeleteLists(m_renderDL, 1);
	glDeleteLists(m_visualizationDL, 1);
	glDeleteLists(m_aabbVisualizationDL, 1);

	/// aabb visualization DL
	glNewList(m_aabbVisualizationDL, GL_COMPILE);
		renderAABBTree(m_aabbTree, AABB_TREE_DEPTH, VISUALIZE_LEAF);
	glEndList();

	/// regenerate render model display list
	glNewList(m_renderDL, GL_COMPILE);
		renderModel(NORMALS_PER_VERTEX, USE_TEXTURES);
	glEndList();

	/// regenerate render model display list
	glNewList(m_visualizationDL, GL_COMPILE);
		renderVisualizations(NORMALS_PER_VERTEX, SHARP_EDGES, NORMAL_VIS_OFF);
	glEndList();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** setTransformations - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ModelObject::setTransformations() {
	
	// translation
	glTranslatef(m_xPos, m_yPos, m_zPos);
	
	// 3D orientation matrix
	if (orientationMatrix != NULL) {

		/// scale - why is the orientationMatrix "inside out"?
		glScalef(m_xScale, m_yScale, m_zScale);
		glScalef(-m_xScale, -m_yScale, -m_zScale);
		glMultMatrixf(orientationMatrix);
		glRotatef(m_rotateAngle, m_xRotate, m_yRotate, m_zRotate);

	// static model transformation
	} else {

		glScalef(m_xScale, m_yScale, m_zScale);
		glRotatef(m_rotateAngle, m_xRotate, m_yRotate, m_zRotate);
	}
}