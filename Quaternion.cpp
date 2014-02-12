#include "Quaternion.h"

Quaternion::Quaternion() {
	x_ = y_ = z_ = 0.0f;
	w_ = 1.0f;
}

Quaternion::Quaternion(float pitch, float yaw, float roll) {
	// Convert to radians
	pitch = (pitch / 180.0f) * M_PI;
	yaw = (yaw / 180.0f) * M_PI;
	roll = (roll / 180.0f) * M_PI;
	
	double	ex, ey, ez;		// temp half euler angles
	double	cr, cp, cy, sr, sp, sy, cpcy, spsy;		// temp vars in roll,pitch yaw

	ex = pitch / 2.0;	// convert to rads and half them
	ey = yaw / 2.0;
	ez = roll / 2.0;

	cr = cos(ex);
	cp = cos(ey);
	cy = cos(ez);

	sr = sin(ex);
	sp = sin(ey);
	sy = sin(ez);

	cpcy = cp * cy;
	spsy = sp * sy;

	w_ = float(cr * cpcy + sr * spsy);

	x_ = float(sr * cpcy - cr * spsy);
	y_ = float(cr * sp * cy + sr * cp * sy);
	z_ = float(cr * cp * sy - sr * sp * cy);
	
	/*Quaternion q;

	Quaternion qX(cos(pitch/2.0),	sin(pitch/2.0), 0.0, 0.0);
	Quaternion qY(cos(yaw/2.0),		0.0, sin(yaw/2.0), 0.0);
	Quaternion qZ(cos(roll/2.0),	0.0, 0.0, sin(roll/2.0));
	
	q = qX * qY * qZ;
	x_ = q.x_;
	y_ = q.y_;
	z_ = q.z_;
	w_ = q.w_;*/
	
	normalize();
}

Quaternion::Quaternion(float degrees, float axisX, float axisY, float axisZ) {
	// If the axis is a zero vector (meaning there is no rotation),
	// the quaternion should be set to the identity quaternion.
	if(axisX==axisY==axisZ==0.0) {
		Quaternion();
		return;
	}
	
	// Normalize axis
	float axisModule = sqrt((axisX*axisX) + (axisZ*axisZ) + (axisZ*axisZ));
	axisX /= axisModule;
	axisY /= axisModule;
	axisZ /= axisModule;
	
	// Convert the degrees to radians 
	float radians = (degrees / 180.0f) * M_PI;
	float scale = sin(radians / 2.0f);
		
	w_ = cos(radians / 2.0f);
	x_ = axisX * scale;
	y_ = axisY * scale;
	z_ = axisZ * scale;
	
	normalize();
}

void Quaternion::createMatrix(GLfloat *matrix) {
	// First row
	matrix[0] = 1.0f - 2.0f * ( y_ * y_ + z_ * z_ ); 
	matrix[1] = 2.0f * (x_ * y_ + z_ * w_);
	matrix[2] = 2.0f * (x_ * z_ - y_ * w_);
	matrix[3] = 0.0f;  
	// Second row
	matrix[4] = 2.0f * ( x_ * y_ - z_ * w_ );  
	matrix[5] = 1.0f - 2.0f * ( x_ * x_ + z_ * z_ ); 
	matrix[6] = 2.0f * (z_ * y_ + x_ * w_ );  
	matrix[7] = 0.0f;  
	// Third row
	matrix[8] = 2.0f * ( x_ * z_ + y_ * w_ );
	matrix[9] = 2.0f * ( y_ * z_ - x_ * w_ );
	matrix[10] = 1.0f - 2.0f * ( x_ * x_ + y_ * y_ );  
	matrix[11] = 0.0f;  
	// Fourth row
	matrix[12] = 0;
	matrix[13] = 0;
	matrix[14] = 0;
	matrix[15] = 1.0f;
	// Now matrix[] is a 4x4 homogeneous matrix that can be applied to an OpenGL Matrix
}

Quaternion Quaternion::operator *(const Quaternion q) {
	Quaternion r;

	r.w_ = w_*q.w_ - x_*q.x_ - y_*q.y_ - z_*q.z_;
	r.x_ = w_*q.x_ + x_*q.w_ + y_*q.z_ - z_*q.y_;
	r.y_ = w_*q.y_ + y_*q.w_ + z_*q.x_ - x_*q.z_;
	r.z_ = w_*q.z_ + z_*q.w_ + x_*q.y_ - y_*q.x_;
	
	return r;
}

Quaternion Quaternion::operator =(const Quaternion q) {
	x_ = q.x_;
	y_ = q.y_;
	z_ = q.z_;
	w_ = q.w_;
	return *this;
}

void Quaternion::normalize(void) {
	float aux = w_*w_ + x_*x_ + y_*y_ + z_*z_;
	if(fabs(aux - 1.0f) > 0.00001) {	// Don't normalize if we don't need to
		float magnitude = sqrt(aux);
		w_ /= magnitude;
		x_ /= magnitude;
		y_ /= magnitude;
		z_ /= magnitude;
	}
}

void Quaternion::conjugate(void) {
	x_ = -x_;
	y_ = -y_;
	z_ = -z_;
}

void Quaternion::getAxisAngle(Vector3D &v, float &angle) const
{
	double	temp_angle;		// temp angle
	double	scale;			// temp vars

	temp_angle = acos(w_);

	scale = (float)sqrt(x_*x_ + y_*y_ + z_*z_);

	if(scale == 0.0) {		// angle is 0 or 360 so just simply set axis to 0,0,1 with angle 0
		angle = 0.0f;
		v.set(0.0f, 0.0f, 1.0f);		// any axis will do
	}
	else {
		angle = (float)(temp_angle * 2.0);		// angle in radians

		v.set(float(x_ / scale), float(y_ / scale), float(z_ / scale));
		v.normalize();
	}

	return;
}
