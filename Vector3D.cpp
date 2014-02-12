#include "Vector3D.h"

Vector3D::Vector3D() {
	set(0.0, 0.0, 0.0);
}

Vector3D::Vector3D(float x, float y, float z) {
	set(x, y, z);
}

void Vector3D::draw(float x, float y, float z) {
	glPushMatrix();
		glTranslatef(x, y, z);
		glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(x_, y_, z_);
		glEnd();
		glPointSize(10.0);
		glBegin(GL_POINTS);
			glVertex3f(x_, y_, z_);
		glEnd();
	glPopMatrix();
}

void Vector3D::set(float x, float y, float z) {
	x_ = x;
	y_ = y;
	z_ = z;
}

float Vector3D::getLength() {
	return sqrt((x_*x_) + (y_*y_) + (z_*z_));
}

void Vector3D::setLength(float newLength) {
	float oldLength = getLength();

	x_ = (x_ / oldLength) * newLength;
	y_ = (y_ / oldLength) * newLength;
	z_ = (z_ / oldLength) * newLength;
}

Vector3D& Vector3D::operator=(const Vector3D &v) {
	x_ = v.x_;
	y_ = v.y_;
	z_ = v.z_;
	return(*this);
}

Vector3D Vector3D::operator+(const Vector3D &v) {
	return(Vector3D(x_ + v.x_,
					y_ + v.y_,
					z_ + v.z_));
}

Vector3D Vector3D::operator-(const Vector3D &v) {
	return(Vector3D(x_ - v.x_,
			y_ - v.y_,
			z_ - v.z_));
}

Vector3D Vector3D::operator*(const float scalar) {
	return(Vector3D(x_ * scalar,
					y_ * scalar,
					z_ * scalar));
}

Vector3D operator*(const float scalar, Vector3D &v) {
	return(Vector3D(v.getX() * scalar,
					v.getY() * scalar,
					v.getZ() * scalar));
}

Vector3D Vector3D::operator*(const Vector3D &v) {	// Cross product
	return(Vector3D(y_*v.z_ - z_*v.y_,
					z_*v.x_ - x_*v.z_,
					x_*v.y_ - y_*v.x_));
}

float Vector3D::dot(const Vector3D &v) {	// Dot product
	return(x_*v.x_ + y_*v.y_ + z_*v.z_);
}

void Vector3D::normalize(void) {
	float aux = x_*x_ + y_*y_ + z_*z_;
	if(fabs(aux - 1.0f) > 0.00001) {	// Don't normalize if we don't need to
		float magnitude = sqrt(aux);
		x_ /= magnitude;
		y_ /= magnitude;
		z_ /= magnitude;
	}
}
