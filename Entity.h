#ifndef Entity_H
#define Entity_H

#include "Quaternion.h"
#include "Vector3D.h"

class Entity {
 protected:
// Data
	Vector3D position_;
	float pitch_, yaw_, roll_;
	Quaternion direction_;

	GLuint textures_[4];				// Max 4 textures per object

	GLfloat matAmbDiff_[4];
	GLfloat matSpecular_[4];
	GLfloat matShininess_[1];
	
 public:
// Constructors
	Entity() {};
	~Entity() {};
	
// Methods
	virtual void draw() = 0;
	virtual void update(float dt) = 0;
	virtual void loadTextures() = 0;
	
	void setPosition(float posX, float posY, float posZ);
	void setRotation(float rotX, float rotY, float rotZ);
	void move(float deltaX, float deltaY, float deltaZ);
	void rotate(float deltaX, float deltaY, float deltaZ);
	
	inline Vector3D getPosition() {return position_;}
	inline float getPitch() {return pitch_;}
	inline float getYaw() {return yaw_;}
	inline float getRoll() {return roll_;}
};

#endif
