#ifndef QUATERNION_H
#define QUATERNION_H

#include <gl\gl.h>
#include <math.h>
#include "Vector3D.h"

class Quaternion {
 private:
// Data
	float x_;
	float y_;
	float z_;
	float w_;
	
// Method
	void normalize();
	void conjugate();

 public:
// Constructors
	// Create identity quaternion
	Quaternion();
	// Create from Axis-Angle representation
	Quaternion(GLfloat degrees, GLfloat axisX, GLfloat axisY, GLfloat axisZ);
	// Create from Euler representation
	Quaternion(GLfloat pitch, GLfloat yaw, GLfloat roll);
	
// Methods
	Quaternion operator*(const Quaternion q);
	Quaternion operator=(const Quaternion q);
	void createMatrix(GLfloat* matrix);
	
	void getAxisAngle(Vector3D &v, float &angle) const;
};
#endif
