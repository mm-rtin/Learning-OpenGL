/*
     File:	ResourceManager.cpp

  Authors:	Michael Martin
  Created:	Oct 17, 2010

  Purpose:	
	 Note:
*/

#include <GL/freeglut.h>
#include <system/ResourceManager.hpp>
#include <system/ModelLoader.hpp>
#include <system/Globals.hpp>
#include <system/Bitmap.h>
#include <system/Math3d.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                   ResourceManager
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Constructor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
ResourceManager::ResourceManager() {
	
	/// initialize texture files
	// organic
	m_textureFiles[0] = "../Textures/grass_top.bmp";
	m_textureFiles[1] = "../Textures/grass_side.bmp";
	m_textureFiles[2] = "../Textures/dirt.bmp";
	m_textureFiles[3] = "../Textures/tree_top.bmp";
	m_textureFiles[4] = "../Textures/tree.bmp";	
	m_textureFiles[5] = "../Textures/cactus_top.bmp";
	m_textureFiles[6] = "../Textures/cactus.bmp";
	// minerals
	m_textureFiles[7] = "../Textures/stone.bmp";
	m_textureFiles[8] = "../Textures/lava.bmp";
	m_textureFiles[9] = "../Textures/diamond.bmp";
	// high res
	m_textureFiles[10] = "../Textures/stone_top.bmp";
	m_textureFiles[11] = "../Textures/blue_marble.bmp";
	m_textureFiles[12] = "../Textures/wood_panel.bmp";
	// required
	m_textureFiles[13] = "../../Textures/asu_logo.bmp";
	m_textureFiles[14] = "../../Textures/bw.bmp";
	m_textureFiles[15] = "../../Textures/mickey.bmp";
	
	/// initialize model files
	// required
	m_modelFiles[0] = "../../Models/Required/cars01_obj/Car_01.obj";
	m_modelFiles[1] = "../../Models/Required/cars09_obj/Car_09.obj";
	m_modelFiles[2] = "../../Models/Required/cars14_obj/Car_14.obj";
	m_modelFiles[3] = "../../Models/Required/Building.obj";
	m_modelFiles[4] = "../../Models/Required/al.obj";
	m_modelFiles[5] = "../../Models/Required/ASU-02.obj";
	
	/// load textures and models
	loadTextures();
	loadModels();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Destructor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
ResourceManager::~ResourceManager() {

}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** setMinificationType
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ResourceManager::setMinificationType(int type) {

	/// for each texture set TexParemeters
    for (GLint i = 0; i < TEXTURE_COUNT; i++) {

		/// bind each texture to set parameters
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);

		switch (type) {
			// simple
			case 0:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				break;
			// smooth
			case 1:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				break;
		}
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** setMagnificationType
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ResourceManager::setMagnificationType(int type) {

	/// for each texture set TexParemeters
    for (GLint i = 0; i < TEXTURE_COUNT; i++) {

		/// bind each texture to set parameters
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);

		switch (type) {
			case 0:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			case 1:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
		}
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** loadTextures
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ResourceManager::loadTextures() {

	/// texture objects
	BITMAPINFO* BitmapInfo;
	GLubyte* BitmapBits;

	///** load textures
	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	glGenTextures(TEXTURE_COUNT, m_textures);

	for (GLint i = 0; i < TEXTURE_COUNT; i++) {

		BitmapBits = LoadDIBitmap(m_textureFiles[i], &BitmapInfo);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);              // set to 4 for bitmap file

		/// build mipmaps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 
						  GL_RGB,                           // internal format
						  BitmapInfo->bmiHeader.biWidth,    // width
						  BitmapInfo->bmiHeader.biHeight,   // height
						  GL_BGR_EXT,                       // data format
						  GL_UNSIGNED_BYTE,                 // element per pixel
						  BitmapBits);                      // data

		/// texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		free(BitmapBits);
		free(BitmapInfo);
				
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}

	cout << "LOAD TEXTURES > DONE" << endl;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** loadModels
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ResourceManager::loadModels() {

	/// current model mesh
	Mesh* modelMesh;

	/// load all models
	for (unsigned int i = 0; i < MODEL_COUNT; i++) {

		// load model
		modelMesh = ModelLoader::load(m_modelFiles[i]);

		// load model texture
		int idx = string(m_modelFiles[i]).find_last_of('/');
		loadModelTexture(modelMesh, string(m_modelFiles[i]).substr(0, idx + 1));

		m_modelMeshes[i] = modelMesh;
	}

	cout << "LOAD MODELS > DONE" << endl;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** loadModelTexture
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ResourceManager::loadModelTexture(Mesh* mesh, std::string path) {

	for (unsigned int i = 0; i < mesh->m_mtl.size(); i++) {

		if (!mesh->m_mtl[i]->m_texture.empty()) {
			BITMAPINFO* BitmapInfo;
			GLubyte* BitmapBits = LoadDIBitmap((path + mesh->m_mtl[i]->m_texture).c_str(), &BitmapInfo);

			if (BitmapBits) {
				glGenTextures(1, &mesh->m_mtl[i]->m_id);

				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, mesh->m_mtl[i]->m_id);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // set to 4 for bitmap file

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				glTexImage2D(GL_TEXTURE_2D,			
					0,								// mipmap level
					GL_RGB,							// internal format
					BitmapInfo->bmiHeader.biWidth,	// width
					BitmapInfo->bmiHeader.biHeight,	// height
					0,								// border
					GL_BGR_EXT,						// data format  (NOTE: Bitmap's format is BGR!!!!)
					GL_UNSIGNED_BYTE,				// element per pixel
					BitmapBits);					// data
				free(BitmapBits);
				free(BitmapInfo);
				
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			glDisable(GL_TEXTURE_2D);
		}
	}
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** recalculateNormals
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ResourceManager::recalculateNormals(float creaseAngle, bool weighted) {

	/// recalculate normals for all models
	for (unsigned int i = 0; i < MODEL_COUNT; i++) {

		// calculate vertex normals
		Math3d::calculateModelVertexNormals(m_modelMeshes[i], creaseAngle, weighted);
	}
}