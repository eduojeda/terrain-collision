#ifndef Camera_H
#define Camera_H

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "Entity.h"

class Camera: public Entity {
 public:
// Constructors
	Camera(float posX, float posY, float posZ, float rotX, float rotY, float rotZ);
	
// Methods
	void pointAt(Entity* what);
	void place();
	
	void draw() {}
	void update(float dt) {}
	void loadTextures() {}
};
#endif
