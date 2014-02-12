#ifndef Ball_H
#define Ball_H

#include <gl\gl.h>
#include <gl\glu.h>
#include "Mass.h"

class Ball: public Mass {
 private:
// Data
	
 public:
// Constructors
	Ball(Vector3D pos, Vector3D vel);
	~Ball() {};

// Methods
	void draw();
	void loadTextures();
	void update(float dt) {}
};

#endif
