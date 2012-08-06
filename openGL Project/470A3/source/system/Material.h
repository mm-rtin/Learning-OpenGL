#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>
#include <Imath/ImathVec.h>

using namespace Imath;
using namespace std;

struct Color4f {
	float r, g, b, a;

	Color4f() {
		r = g = b = a = 0;
	}

	Color4f(float x, float y, float z, float w) {
		r = x;
		g = y;
		b = z;
		a = w;
	}
};

class Material {

public:
	string m_name;
	Color4f m_ambient;
	Color4f m_diffuse;
	Color4f m_specular;
	Color4f m_emission;
	float m_shininess;
	string m_texture;
	unsigned int m_id;

	/// constructor
	Material() {
		m_name = "";
		m_ambient = Color4f(0.2f, 0.2f, 0.2f, 1.0f);
		m_diffuse = Color4f(0.8f, 0.8f, 0.8f, 1.0f);
		m_specular = Color4f(0.0f, 0.0f, 0.0f, 1.0f);
		m_emission = Color4f(0.0f, 0.0f, 0.0f, 1.0f);
		m_shininess = 0;
		m_texture = "";
		m_id = 0;
	}
};

#endif