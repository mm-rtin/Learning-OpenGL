#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <system/Mesh.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                   ResourceManager
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

#define TEXTURE_COUNT 16
#define MODEL_COUNT 6

class ResourceManager {

private:

	/// data
	const char* m_textureFiles[TEXTURE_COUNT];
	const char* m_modelFiles[MODEL_COUNT];

	/// load methods
	void loadTextures();
	void loadModels();
	void loadModelTexture(Mesh*, std::string);

public:

	ResourceManager();
	~ResourceManager();

	/// texture objects
	GLuint m_textures[TEXTURE_COUNT];

	/// model meshes
	Mesh* m_modelMeshes[MODEL_COUNT];

	/// methods
	void setMinificationType(int);
	void setMagnificationType(int);
	void recalculateNormals(float, bool);

	
};
#endif