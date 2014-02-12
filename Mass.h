#ifndef Mass_H
#define Mass_H

#include "Entity.h"
#include "Terrain.h"

class Mass: public Entity {
 protected:
// Data
	float mass_;
	float invMass_;
	Vector3D velocity_;
	Vector3D force_;
	
 public:
// Constructors
	Mass();
	~Mass() {};
	
// Methods
	void applyForce(Vector3D f);
	void resetForce();
	void simulate(float dt);
	void checkTerrainCollision(Terrain* terrain);
		
	inline float getMass() {return mass_;}
	inline Vector3D getVelocity() {return velocity_;}
};

#endif
