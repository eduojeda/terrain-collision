#include "Camera.h"

Quaternion camera_quat;

Camera::Camera(float posX, float posY, float posZ, float rotX, float rotY, float rotZ) {
	position_.set(posX, posY, posZ);
	pitch_ = rotX;
	yaw_ = rotY;
	roll_ =rotZ;
}

void Camera::pointAt(Entity* what) {	
	gluLookAt(	position_.getX(), position_.getY(), position_.getZ(),
				what->getPosition().getX(), what->getPosition().getY(), what->getPosition().getZ(),
				0.0, 1.0, 0.0);
}

void Camera::place() {		
	/*GLfloat matrix[16];
	Quaternion quaternion(-pitch_, -yaw_, -roll_);
	
	quaternion.createMatrix(matrix);
	glMultMatrixf(matrix);*/
	
	glRotatef(-pitch_, 1.0f, 0.0f, 0.0f);
	glRotatef(-yaw_, 0.0f, 1.0f, 0.0f);
	glRotatef(-roll_, 0.0f, 0.0f, 1.0f);
	
/*	Quaternion	key_quat(pitch_, yaw_, roll_),
				temp_quat;

	temp_quat = camera_quat;
	camera_quat = temp_quat * key_quat;

	Vector3D axis;
	float angle;

	camera_quat.getAxisAngle(axis, angle);

	float ax = axis.getX(),
		  ay = axis.getY(),
		  az = axis.getZ();
	angle = (angle / 180.0f) * M_PI;

	glRotatef(angle, ax, ay, az);	*/
	
	glTranslatef(-position_.getX(), -position_.getY(), -position_.getZ());
}
