/*
     File:	PrimitiveModel.cpp

  Authors:	Michael Martin
  Created:	Sep 14, 2010

  Purpose:	
	 Note:
*/

#include <GL/freeglut.h>

#include <graphics/PrimitiveModel.hpp>
#include <system/Math3d.hpp>
#include <system/Globals.hpp>
#include <system/Bitmap.h>

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Constructor - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
PrimitiveModel::PrimitiveModel() : ModelObject() {
	
	/// default color
	m_color1 = Vec3f(1.0, 1.0, 1.0);
	m_color2 = Vec3f(0.8, 0.8, 0.8);

	m_aabbTree = NULL;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Destructor - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
PrimitiveModel::~PrimitiveModel() {

	/// delete display list
	glDeleteLists(m_renderDL, 1);
	glDeleteLists(m_visualizationDL, 1);
	glDeleteLists(m_aabbVisualizationDL, 1);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** initialize - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void PrimitiveModel::initialize() {

}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** activate
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void PrimitiveModel::activate() {
	
	transformAABBTree(m_aabbTree);

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
/** loadModel - load model from Mesh* data
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void PrimitiveModel::loadModel(Mesh* modelMesh, int texture1, int texture2) {

	/// delete existing model
	if (m_model)
		delete m_model;

	m_model = modelMesh;
	
	/// bounding box
	Math3d::calculateBoundingBox(bbxMin, bbxMax, m_model->m_v);
	bbxCenter = (bbxMin+bbxMax)*0.5f;

	/// get textures
	m_texture1 = g_resourceManager->m_textures[texture1];
	m_texture2 = g_resourceManager->m_textures[texture2];

	/// calculate normal size -> relative to bounding box size
	m_normalSize = (bbxMax - bbxMin).length() * 0.15;
	// cap normal size to maximum value
	if (m_normalSize > 1) 
		m_normalSize = 1;

	/// clone AABB Tree
	m_aabbTree = Math3d::cloneAABBTree(m_model->m_aabbTree);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** renderModel - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void PrimitiveModel::renderModel(bool perVertex, bool useTextures) {

	/// reference to array of vertices of primitive
	vector<Vec3f>& v = ModelObject::m_model->m_v;
	vector<Vec2f>& t = ModelObject::m_model->m_texc;
	vector<Vec3i>& tex = ModelObject::m_model->m_tex;

	vector<Vec3f>* n = &ModelObject::m_model->m_npv;

	bool hasMaterial = tex.size() == 0 ? false : true;

	/// for each texture/material
	unsigned int materialCount = hasMaterial ? tex.size() : 1;
	for (unsigned int i = 0; i < materialCount; ++i) {
		
		/// use textures
		if (useTextures) {
			glEnable(GL_TEXTURE_2D);
			/// use set texture
			if (hasMaterial) {
				// select texture face (top/side)
				if (tex[i].x == 0)
					glBindTexture(GL_TEXTURE_2D, m_texture1);
				else
					glBindTexture(GL_TEXTURE_2D, m_texture2);
			/// use default texture
			} else {
				glBindTexture(GL_TEXTURE_2D, g_resourceManager->m_textures[11]);
			}

		/// no texture
		} else {
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		glBegin(GL_TRIANGLES);
		
		// select vertex loop for defined/undefined materials
		int start = hasMaterial ? tex[i].y : 0;
		int end = hasMaterial ? tex[i].z : v.size();

		/// for each vertex
		for (unsigned int j = start; j < end; ++j) {
			
			// alternating color
			if (j % 2 == 0) 
				glColor3f(m_color1.x, m_color1.y, m_color1.z);
			else
				glColor3f(m_color2.x + 0.2f, m_color2.y + 0.2f, m_color2.z + 0.2f);

			// set normal per face
			if (!perVertex) {
				glNormal3f(m_model->m_n[j].x, m_model->m_n[j].y, m_model->m_n[j].z);
			// set normal per vertex
			} else {
				glNormal3f((*n)[j].x, (*n)[j].y, (*n)[j].z);
			}

			/// draw primitive
			// texture coordinates
			if (!t.empty() && useTextures)
				glTexCoord2f(t[j].x, t[j].y);

			// draw vertex
			glVertex3f(v[j].x, v[j].y, v[j].z);
		}
		glEnd();
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** renderVisualizations - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void PrimitiveModel::renderVisualizations(bool perVertex, bool sharpEdges, bool normalsOff) {

	/// references
	vector<Vec3f>& v = ModelObject::m_model->m_v;
	vector<Vec3f>* n = &ModelObject::m_model->m_npv;
	vector<Vec3f>* se = &ModelObject::m_model->m_se;

	glDisable(GL_LIGHTING);

	/// normals visualizations
	if (!normalsOff) {
		glLineWidth(2.0f);
		glBegin(GL_LINES);

			/// per face properties
			float xSum = 0.0f;
			float ySum = 0.0f;
			float zSum = 0.0f;

			/// for each vertex
			Vec3f normal;
			for (int i = 0; i < v.size(); ++i) {

				// per vertex normals
				if (perVertex) {
				
					normal = Vec3f((*n)[i].x, (*n)[i].y, (*n)[i].z);

					// scale normal
					normal *= m_normalSize;
					glVertex3f(v[i].x, v[i].y, v[i].z);
					glVertex3f(v[i].x + normal.x, v[i].y + normal.y, v[i].z + normal.z);

				// per face normals
				} else {

					// for every 3 vertrices (1 face)
					if ((i+1) % 3 == 0) {

						// get triangle center
						xSum += v[i].x;
						ySum += v[i].y;
						zSum += v[i].z;

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

						xSum += v[i].x;
						ySum += v[i].y;
						zSum += v[i].z;
					}
				}
			}
		glEnd();
	}

	/// sharp edge visualizations
	if (sharpEdges) {
		glLineWidth(4.0f);
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
/** draw - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void PrimitiveModel::draw() {

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

		/// set material
		Material* m;
		if (m_model->m_mtl.size() != 0 && !GLOBAL_MATERIAL) {
			m = m_model->m_mtl[0];

			GLfloat ambient[4] = {m->m_ambient.r, m->m_ambient.g, m->m_ambient.b, m->m_ambient.a};
			GLfloat diffuse[4] = {m->m_diffuse.r, m->m_diffuse.g, m->m_diffuse.b, m->m_diffuse.a};
			GLfloat specular[4] = {m->m_specular.r, m->m_specular.g, m->m_specular.b, m->m_specular.a};
			GLfloat emission[4] = {m->m_emission.r, m->m_emission.g, m->m_emission.b, m->m_emission.a};
		
			glPushAttrib(GL_LIGHTING_BIT);
				glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
				glMaterialf(GL_FRONT, GL_SHININESS, m->m_shininess);
				glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		}

		/// draw model
		if (DISPLAY_LIST) {
			glCallList(m_renderDL);
		// immediate mode
		} else {
			renderModel(NORMALS_PER_VERTEX, USE_TEXTURES);
		}

		if (m_model->m_mtl.size() != 0 && !GLOBAL_MATERIAL) {
			glPopAttrib();
		}

		/// draw visualizations
		if (DISPLAY_LIST) {
			if (VISUALIZATIONS_ON) {
				glCallList(m_visualizationDL);
			}
		// immediate mode
		} else {
			if (VISUALIZATIONS_ON)
				renderVisualizations(NORMALS_PER_VERTEX, SHARP_EDGES, NORMAL_VIS_OFF);
		}

	glPopMatrix();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** regenerateDisplayList - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void PrimitiveModel::regenerateDisplayList() {

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
void PrimitiveModel::setTransformations() {
	
	glTranslatef(m_xPos, m_yPos, m_zPos);
	glScalef(m_xScale, m_yScale, m_zScale);
	glRotatef(m_rotateAngle, m_xRotate, m_yRotate, m_zRotate);
	glRotatef(m_rotateAngle1, m_xRotate1, m_yRotate1, m_zRotate1);
}