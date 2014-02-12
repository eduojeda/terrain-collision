#include "Ball.h"

Ball::Ball(Vector3D pos, Vector3D vel) {
	position_ = pos;
	velocity_ = vel;
	mass_ = 50.0;
	invMass_ = 1/mass_;
	
	matAmbDiff_[0] = 0.5;
	matAmbDiff_[1] = 0.0;
	matAmbDiff_[2] = 0.0;
	matAmbDiff_[3] = 1.0;
	matSpecular_[0] = 1.0;
	matSpecular_[1] = 1.0;
	matSpecular_[2] = 1.0;
	matSpecular_[3] = 1.0;
	matShininess_[0] = 100.0;
}

void Ball::draw() {
	GLUquadricObj *q;

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbDiff_);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular_);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess_);

	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
		glTranslatef(position_.getX(), position_.getY(), position_.getZ());
		q = gluNewQuadric();
		gluQuadricDrawStyle(q, GLU_FILL);
		gluQuadricNormals(q, GLU_SMOOTH);
		gluSphere(q, 0.5, 60, 60);
		gluDeleteQuadric(q);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

void Ball::loadTextures() {
	/*try {
		TGAImage terrainTex("./Textures/terrainTex.tga"); // If this throws, terrainTex never gets used.
		glGenTextures(1, &textures_[0]);
		//glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, (GLuint)terrainTex.getWidth(), (GLuint)terrainTex.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, terrainTex.getImageData());
	}catch(FileException e) {
		MessageBox(NULL, e.getMessage(), "Texture filename", MB_OK | MB_ICONEXCLAMATION);
	}catch(std::exception& e) {
		MessageBox(NULL, "Not enough memory.", "Memory", MB_OK | MB_ICONEXCLAMATION);
	}*/
}
