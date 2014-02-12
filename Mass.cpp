#include "Mass.h"

Mass::Mass() {
}

void Mass::applyForce(Vector3D f) {	
	force_ = force_ + f;
}

void Mass::resetForce() {
	force_.set(0.0, 0.0, 0.0);
}

void Mass::simulate(float dt) {
	velocity_ = velocity_ + force_*(invMass_*dt);
	position_ = position_ + velocity_*dt;
}

void Mass::checkTerrainCollision(Terrain* terrain) {
	float posX = position_.getX();
	float posZ = position_.getZ();

	if((posX >= 0.0) && (posX <= terrain->getSizeX()-1) &&
	   (posZ >= 0.0) && (posZ <= terrain->getSizeZ()-1)) {
	   	float terrainHeight = terrain->getHeight(posX, posZ);
		if(position_.getY()-0.5 <= terrainHeight) {
			Vector3D normal = terrain->getNormal(posX, posZ);
			position_.set(posX, terrainHeight+0.5, posZ);
			velocity_ = velocity_ - 2.0*(velocity_.dot(normal)) * normal;  // r = v - 2*(v.n) * n  <-- reflection
			// Pseudo friction/elasticity
			velocity_.set(velocity_.getX()*0.95,
						  velocity_.getY()*0.3,
						  velocity_.getZ()*0.95);
		}
	}
}
