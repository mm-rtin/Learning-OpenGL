#ifndef PRIMITIVE_MODEL_H
#define PRIMITIVE_MODEL_H

#include <graphics/ModelObject.hpp>
#include <graphics/Primitives.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                    PrimitiveModel
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

class PrimitiveModel : public ModelObject {

private:

	/// texture
	GLuint m_texture1;
	GLuint m_texture2;

public:

	/// properties
	Vec3f m_color1;
	Vec3f m_color2;

	PrimitiveModel();
	~PrimitiveModel();

	virtual void initialize();
	virtual void loadModel(Mesh*, int, int);
	virtual void activate();

	virtual void draw();
	virtual void renderModel(bool, bool);
	virtual void renderVisualizations(bool, bool, bool);
	virtual void regenerateDisplayList();
	virtual void setTransformations();
};
#endif