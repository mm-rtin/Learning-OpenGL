#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <Imath/ImathVec.h>
#include <string>

#include <system/Mesh.hpp>

class ModelLoader
{	
public:

	static Mesh* load(std::string);

private:

	static bool loadMtl(const char* file, vector<Material*>& mat_set);
};

#endif