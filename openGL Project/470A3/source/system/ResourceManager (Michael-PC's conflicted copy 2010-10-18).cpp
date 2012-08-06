/*
     File:	ResourceManager.cpp

  Authors:	Michael Martin
  Created:	Oct 17, 2010

  Purpose:	
	 Note:
*/

#include <GL/freeglut.h>
#include <system/ResourceManager.hpp>
#include <system/Globals.hpp>
#include <system/Bitmap.h>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                   ResourceManager
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** Constructor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
ResourceManager::ResourceManager() {
	
	/// initialize data
	m_textureFiles[0] = "../../Textures/grass_top.bmp";
	m_textureFiles[1] = "../../Textures/grass_side.bmp";
	m_textureFiles[2] = "../../Textures/asu_logo.bmp";
	m_textureFiles[3] = "../../Textures/bw.bmp";
	m_textureFiles[4] = "../../Textures/mickey.bmp";
	m_textureFiles[5] = "../../Textures/stone_top.bmp";

	/// load textures
	loadTextures();
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
		glDisable(GL_TEXTURE_2D);
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
	glDisable(GL_TEXTURE_2D);
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
				
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}