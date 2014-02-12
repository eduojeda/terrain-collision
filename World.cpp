#include "World.h"

GLfloat lightPosition[]= {-25.0f, 50.0f, -25.0f, 1.0f};
GLfloat lightAmbient[] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightSpecular[] = {0.0f, 0.0f, 0.0f, 1.0f};

World::World():	gravity_(0.0, -9.82, 0.0) {
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	terrain_ = new Terrain("./Data/Terrains/cat.tga", 32.0, 1.0);
	//terrain_ = new Terrain(TERRAIN_HOLES, 64, 64, 4.0, 1.0);
    //terrain_ = new Terrain(TERRAIN_ABYSS, 128, 128, 12.0, 1.0);
	addEntity(terrain_);

	for(int i=0; i < entitiesVector.size() ; i++) {
		entitiesVector.at(i)->loadTextures();
	}
}

void World::addEntity(Entity* entity) {
	entitiesVector.push_back(entity);
}

void World::addMass(Mass* mass) {
	entitiesVector.push_back(mass);
	massesVector.push_back(mass);
}

void World::render() {
	GLUquadricObj *q;
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPushMatrix();
		glTranslatef(lightPosition[0], lightPosition[1], lightPosition[2]);
		glColor3f(1.0, 1.0, 0.0);
		q = gluNewQuadric();
		gluQuadricDrawStyle(q, GLU_FILL);
		gluQuadricNormals(q, GLU_SMOOTH);
		gluSphere(q, 4.0, 60, 60);
		gluDeleteQuadric(q);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	for(int i=0; i < entitiesVector.size() ; i++) {
		entitiesVector.at(i)->draw();
	}
}

void World::simTimeStep(int milliseconds) {
	float maxDt = 0.05f;						// 50ms maximum timestep
	float dt = milliseconds / 1000.0;			// Convert ticks elapsed to seconds

	/* If the frame rate goes to low, dt may get too big and make the simulaton unstable.
		In that case we need to do more than one iteration to catch up */

	int numIterations = (int)(dt / maxDt) + 1;
	if(numIterations != 0)						// Avoid division by zero.
		dt /= numIterations;					// Recalculate dt

	Mass* mass;
	for(int i=0 ; i < numIterations ; i++) {
		for(int j=0 ; j < massesVector.size() ; j++) {
			mass = massesVector.at(j);
			mass->resetForce();
			mass->applyForce(gravity_ * mass->getMass());
			mass->simulate(dt);
			mass->checkTerrainCollision(terrain_);
		}

		/* USE LISTS NOT VECTORS?*/

		for(int j=0; j < entitiesVector.size() ; j++) {
			entitiesVector[j]->update(dt);
		}

/*		std::vector<Mass*>::iterator ent;
		for(ent = massesVector.begin(); ent != massesVector.end(); ent++ ) {
			if((*ent)->getPosition().getLength() > 100.0) {
				massesVector.erase(ent);
			}
		}*/
	}
}
