#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "Vector3D.h"
#include "Entity.h"
#include "Mass.h"
#include "Terrain.h"
#include "Ball.h"

class World {
 private:
// Data
	std::vector<Entity*> entitiesVector;
	std::vector<Mass*> massesVector;
	Vector3D gravity_;
	Terrain* terrain_;

 public:
// Constructors
	World();
	
// Methods
	void render();
	void simTimeStep(int milliseconds);
	void addEntity(Entity* entity);
	void addMass(Mass* mass);
};

#endif
