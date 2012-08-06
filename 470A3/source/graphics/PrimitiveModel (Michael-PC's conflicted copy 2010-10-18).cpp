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
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Destructor - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
PrimitiveModel::~PrimitiveModel() {

	/// delete display list
	glDeleteLists(model1DL, 1);
	glDeleteLists(model2DL, 1);
	glDeleteLists(model3DL, 1);
	glDeleteLists(model4DL, 1);

	glDeleteLists(visualization1DL, 1);
	glDeleteLists(visualization2DL, 1);
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

	/// create display lists
	model1DL = glGenLists(1);
	model2DL = glGenLists(1);
	model3DL = glGenLists(1);
	model4DL = glGenLists(1);

	visualization1DL = glGenLists(1);
	visualization2DL = glGenLists(1);

	/// render model 1 (per face-textures)
	glNewList(model1DL, GL_COMPILE);
		renderModel(false, true);
	glEndList();

	/// render model 2 (per vertex-textures)
	glNewList(model2DL, GL_COMPILE);
		renderModel(true, true);
	glEndList();

	/// render model 3 (per face-no textures)
	glNewList(model3DL, GL_COMPILE);
		renderModel(false, false);
	glEndList();

	/// render model 4 (per vertex-no textures)
	glNewList(model4DL, GL_COMPILE);
		renderModel(true, false);
	glEndList();

	/// render visualizations 1 (per face)
	glNewList(visualization1DL, GL_COMPILE);
		renderVisualizations(false);
	glEndList();

	/// render visualizations 2 (per vertex)
	glNewList(visualization2DL, GL_COMPILE);
		renderVisualizations(true);
	glEndList();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** loadModel - load model from Mesh* data
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void PrimitiveModel::loadModel(Mesh* modelMesh, int texture1 = 0, int texture2 = 1) {

	/// delete existing model
	if (m_model)
		delete m_model;

	m_model = modelMesh;
	
	/// bounding box
	calculateBoundingBox(bbxMin, bbxMax);
	bbxCenter = (bbxMin+bbxMax)*0.5f;

	/// get textures
	m_texture1 = g_resourceManager->m_textures[texture1];
	m_texture2 = g_resourceManager->m_textures[texture2];
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** renderModel - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void PrimitiveModel::renderModel(bool perVertex, bool useTextures) {

	/// reference to array of vertices of primitive
	vector<Vec3f>& v = ModelObject::m_model->m_v;
	vector<Vec2f>& t = ModelObject::m_model->m_texc;
	vector<int>& tf = ModelObject::m_model->m_tex;

	/// draw primitive
	glBegin(GL_TRIANGLES);
		/// for each vertex
		for (int i = 0; i < v.size(); ++i) {
			
			// apply texture
			if (i % 3 == 0 && !tf.empty() && useTextures) {
				glEnable(GL_TEXTURE_2D);
				if (tf[i] == 0)
					glBindTexture(GL_TEXTURE_2D, m_texture1);
				else
					glBindTexture(GL_TEXTURE_2D, m_texture2);
			}

			// texture coordinates
			if (!t.empty() && useTextures)
				glTexCoord2f(t[i].x, t[i].y);

			// color
			if (i % 2 == 0) 
				glColor3f(m_color1.x, m_color1.y, m_color1.z);
			else
				glColor3f(m_color2.x + 0.2f, m_color2.y + 0.2f, m_color2.z + 0.2f);

			// set normal per face
			if (!perVertex) {
				glNormal3f(m_model->m_n[i].x, m_model->m_n[i].y, m_model->m_n[i].z);
			// set normal per vertex
			} else {
				glNormal3f(m_model->m_npv[i].x, m_model->m_npv[i].y, m_model->m_npv[i].z);
			}

			// draw vertex
			glVertex3f(v[i].x, v[i].y, v[i].z);
		}
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** renderVisualizations - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void PrimitiveModel::renderVisualizations(bool perVertex) {

	/// render visualizations
	glLineWidth(2.0f);
	glBegin(GL_LINES);

		vector<Vec3f>& v = ModelObject::m_model->m_v;

		/// per face properties
		float xSum = 0.0f;
		float ySum = 0.0f;
		float zSum = 0.0f;

		/// for each vertex
		for (int i = 0; i < v.size(); ++i) {

			/// per vertex normals
			if (perVertex) {

				Vec3f normal(m_model->m_npv[i].x / m_xScale * m_normalSize, m_model->m_npv[i].y / m_yScale * m_normalSize, m_model->m_npv[i].z / m_zScale * m_normalSize);
				glVertex3f(v[i].x, v[i].y, v[i].z);
				glVertex3f(v[i].x + normal.x, v[i].y + normal.y, v[i].z + normal.z);

			/// per face normals
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

					Vec3f normal(m_model->m_n[i].x / m_xScale * m_normalSize, m_model->m_n[i].y / m_yScale * m_normalSize, m_model->m_n[i].z / m_zScale * m_normalSize);
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

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** draw - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void PrimitiveModel::draw() {

	glPushMatrix();

		/// set transformations
		setTransformations();

		/// draw model
		// per vertex
		if (NORMALS_PER_VERTEX) {

			if (USE_TEXTURES) 
				glCallList(model2DL);
			else
				glCallList(model4DL);

		// per face
		} else {

			if (USE_TEXTURES) 
				glCallList(model1DL);
			else
				glCallList(model3DL);
		}

		/// draw visualizations
		if (VISUALIZATIONS_ON) {
			if (NORMALS_PER_VERTEX)
				glCallList(visualization2DL);
			else 
				glCallList(visualization1DL);
		}

	glPopMatrix();
}