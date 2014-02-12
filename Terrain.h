#ifndef Terrain_H
#define Terrain_H

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <math.h>
#include <exception>
#include "Entity.h"
#include "TGAImage.h"

/* Constants */
#define TERRAIN_SINE		0
#define TERRAIN_ABYSS		1
#define TERRAIN_HOLES		2
#define TERRAIN_PLAIN		3
#define TERRAIN_SPHERE		-1
#define TERRAIN_BOWL		4

class Terrain: public Entity {
 private:
// Data
	float heightScaleFactor_;
	float gridScaleFactor_;
	int sizeX_;
	int sizeZ_;
	float* heightMap_;
	Vector3D* normals_;
	//float* colors_;
	
// Methods
	void calculateNormals();
	
 public:
// Constructors
	Terrain(char* filename, float heightScale,  float gridScale);
	Terrain(int terrainFunction, int sizeX, int sizeZ, float heightScale,  float gridScale);
	~Terrain();
	
// Methods
	void draw();
	void update(float dt);
	void loadTextures();
	float getHeight(float x, float z);
	Vector3D getNormal(float x, float z);
	float getSizeX();
	float getSizeZ();
};
#endif
