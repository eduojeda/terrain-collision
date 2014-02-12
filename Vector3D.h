#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <gl\gl.h>
#include <gl\glu.h>
#include <math.h>

class Vector3D {
 private:
// Data
	float x_;
	float y_;
	float z_;
	
 public:
// Constructors
	Vector3D();
	Vector3D(float x, float y, float z);
	
// Methods
	void draw(float x, float y, float z);
	void set(float x, float y, float z);
	inline float getX() {return x_;}
	inline float getY() {return y_;}
	inline float getZ() {return z_;}
	float getLength();
	void setLength(float newLength);
	void normalize(void);

	Vector3D&	operator=(const Vector3D &v);
	Vector3D	operator+(const Vector3D &v);
	Vector3D	operator-(const Vector3D &v);
	Vector3D	operator*(const float scalar);
	Vector3D	operator*(const Vector3D &v);	// Cross product
	float		dot(const Vector3D &v);			// Dot product
};

// Non-member operators
Vector3D operator*(const float scalar, Vector3D &v);
#endif
