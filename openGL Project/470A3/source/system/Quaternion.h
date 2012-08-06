#ifndef QUATERNION_H
#define QUATERNION_H

#include <windows.h>
#include <math.h>
#include <GL/freeglut.h>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                      Quaternion
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

#define PI 3.141592653589793f

class Quaternion {

public:

	Quaternion();
	virtual ~Quaternion();

	Quaternion operator *(Quaternion q);
	void createMatrix(GLfloat *pMatrix);
	void createFromAxisAngle(GLfloat x, GLfloat y, GLfloat z, GLfloat degrees);

private:

	GLfloat m_x;
	GLfloat m_y;
	GLfloat m_z;
	GLfloat m_w;

};

#endif