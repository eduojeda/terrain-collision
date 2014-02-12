#include "Entity.h"

void Entity::setPosition(float posX, float posY, float posZ) {
	position_.set(posX, posY, posZ);
}

void Entity::move(float deltaX, float deltaY, float deltaZ) {
	Vector3D deltaV(deltaX, deltaY, deltaZ);
	
	position_ = position_ + deltaV;
}

void Entity::setRotation(float rotX, float rotY, float rotZ) {
	pitch_	= rotX;
	yaw_	= rotY;
	roll_	= rotZ;
}

void Entity::rotate(float deltaX, float deltaY, float deltaZ) {
	pitch_	+= deltaX;
	yaw_	+= deltaY;
	roll_	+= deltaZ;
}
