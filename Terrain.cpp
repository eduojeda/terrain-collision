#include "Terrain.h"

void Terrain::update(float dt) {

}

void Terrain::draw() {
	GLfloat		vertexHeight;
	Vector3D	normal;

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbDiff_);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular_);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess_);
	glBindTexture(GL_TEXTURE_2D, textures_[0]);

	glDisable(GL_CULL_FACE);
	glPushMatrix();
		glTranslatef(position_.getX(), position_.getY(), position_.getZ());
		glScalef(gridScaleFactor_, heightScaleFactor_, gridScaleFactor_);
		for(int z = 0 ; z < sizeZ_-1 ; z++) {
			glBegin(GL_TRIANGLE_STRIP);
			for(int x = 0 ; x < sizeX_ ; x++) {
				vertexHeight = heightMap_[x + z * sizeX_];
				normal = normals_[x + z * sizeX_];
				glTexCoord2f(x/(float)(sizeX_-1), z/(float)(sizeZ_-1));
				glNormal3f(normal.getX(), normal.getY(), normal.getZ());
				glVertex3f(x, vertexHeight, z);

				vertexHeight = heightMap_[x + (z+1) * sizeX_];
				normal = normals_[x + (z+1) * sizeX_];
				glTexCoord2f(x/(float)(sizeX_-1), (z+1)/(float)(sizeZ_-1));
				glNormal3f(normal.getX(), normal.getY(), normal.getZ());
				glVertex3f(x, vertexHeight, (z+1));
			}
			glEnd();
		}
	glPopMatrix();
	glEnable(GL_CULL_FACE);

// POINT PLOTTING
/*	glDisable(GL_LIGHTING);
	glColor3f(0.3, 0.8, 0.3);
	glPointSize(2.0);
	for(int z = 0 ; z <= 100 ; z++) {
		glBegin(GL_POINTS);
		for(int x = 0 ; x <= 100 ; x++) {
			//glVertex3f(x, getHeight(x, z), z);
			normals_[x + z * sizeX_].draw(x, getHeight(x, z), z);
		}
		glEnd();
	}
	glEnable(GL_LIGHTING);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(8.0);
	for(float z = 0 ; z < getSizeZ()-1 ; z+=gridScaleFactor_) {
		glBegin(GL_POINTS);
		for(float x = 0 ; x < getSizeX()-1 ; x+=gridScaleFactor_) {
			glVertex3f(x, getHeight(x, z), z);
		}
		glEnd();
	}*/
}

void Terrain::loadTextures() {
	glGenTextures(1, &textures_[0]);

	try {
		TGAImage terrainTex("./Data/Textures/nautilus.tga"); // If this throws, terrainTex never gets used.
		glBindTexture(GL_TEXTURE_2D, textures_[0]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
						 (GLuint)terrainTex.getWidth(),
						 (GLuint)terrainTex.getHeight(),
						 GL_RGB, GL_UNSIGNED_BYTE,
						 terrainTex.getImageData());

	}catch(FileException e) {
		MessageBox(NULL, e.getMessage(), "Texture filename", MB_OK | MB_ICONEXCLAMATION);
	}catch(std::exception& e) {
		MessageBox(NULL, "Not enough memory.", "Memory", MB_OK | MB_ICONEXCLAMATION);
	}
}

Terrain::Terrain(char* filename, float heightScale,  float gridScale)
:	heightScaleFactor_(heightScale),
	gridScaleFactor_(gridScale)
{
	unsigned char* imageData;

	matAmbDiff_[0] = 0.5;
	matAmbDiff_[1] = 0.5;
	matAmbDiff_[2] = 0.5;
	matAmbDiff_[3] = 1.0;
	matSpecular_[0] = 1.0;
	matSpecular_[1] = 1.0;
	matSpecular_[2] = 1.0;
	matSpecular_[3] = 1.0;
	matShininess_[0] = 100.0;

	try {
		// Fill in the vertex heights array
		TGAImage image(filename);
		if(image.getBits() != 8) {
			throw FileException("Need a grayscale image for the height map.");
		}
		sizeX_ = image.getWidth();
		sizeZ_ = image.getHeight();
		imageData = (unsigned char*)image.getImageData();
		heightMap_ = new float[sizeX_ * sizeZ_];

		for(int i = 0 ; i < (sizeX_ * sizeZ_) ; i++) {
			heightMap_[i] = (float)imageData[i] / 256.0;
		}
	}
	catch(FileException e) {
		MessageBox(NULL, e.getMessage(), "Terrain Generation Error", MB_OK | MB_ICONEXCLAMATION);
	}
	catch(std::exception& e) {
		MessageBox(NULL, "Not enough memory.", "Memory", MB_OK | MB_ICONEXCLAMATION);
	}

	calculateNormals();
}

Terrain::Terrain(int terrainFunction, int sizeX, int sizeZ, float heightScale,  float gridScale): heightScaleFactor_(heightScale), gridScaleFactor_(gridScale), sizeX_(sizeX), sizeZ_(sizeZ) {
	matAmbDiff_[0] = 0.5;
	matAmbDiff_[1] = 0.5;
	matAmbDiff_[2] = 0.5;
	matAmbDiff_[3] = 1.0;
	matSpecular_[0] = 1.0;
	matSpecular_[1] = 1.0;
	matSpecular_[2] = 1.0;
	matSpecular_[3] = 1.0;
	matShininess_[0] = 100.0;

	try {
		heightMap_ = new float[sizeX_ * sizeZ_];
	}
	catch(std::exception& e) {
		MessageBox(NULL, "Not enough memory.", "Memory", MB_OK | MB_ICONEXCLAMATION);
	}

	switch(terrainFunction) {
		case TERRAIN_SINE:
			for(int i = 0 ; i < (sizeX_ * sizeZ_) ; i++) {
				heightMap_[i] = sin(i/1000.0);
			}
			break;

		case TERRAIN_ABYSS:
			for(int i = 0 ; i < sizeZ_ ; i++) {
				for(int j = 0 ; j < sizeX_ ; j++) {
					float x = (i - sizeX/2.0) / 10.0;
					float z = (j - sizeZ/2.0) / 10.0;
					heightMap_[i + j * sizeX_] = -1.0 / ((x*x) + (z*z));
				}
			}
			position_.set(0.0, -3.0, 0.0);
			break;

		case TERRAIN_HOLES:
			for(int i = 0 ; i < sizeZ_ ; i++) {
				for(int j = 0 ; j < sizeX_ ; j++) {
					float x = (i - sizeX/2.0) / 10.0;
					float z = (j - sizeZ/2.0) / 10.0;
					heightMap_[i + j * sizeX_] = pow((cos(x) + cos(z)),3);
				}
			}
			break;

		case TERRAIN_PLAIN:
			for(int i = 0 ; i < sizeZ_ ; i++) {
				for(int j = 0 ; j < sizeX_ ; j++) {
					heightMap_[i + j * sizeX_] = i/sizeZ_;
				}
			}
			break;

		case TERRAIN_SPHERE:
			break;

		case TERRAIN_BOWL:
			for(int i = 0 ; i < sizeZ_ ; i++) {
				for(int j = 0 ; j < sizeX_ ; j++) {
					float x = (i - sizeX/2.0) / 10.0;
					float z = (j - sizeZ/2.0) / 10.0;
					heightMap_[i + j * sizeX_] = sqrt(x*x + z*z);
				}
			}
			break;
	}
	calculateNormals();
}

float Terrain::getHeight(float x, float z) {
	float x1, y1, z1, A, B, C, D;

	// Get the plane equation for the terrain triangle x and z are in
	Vector3D n = getNormal(x, z);
	A = n.getX();
	B = n.getY();
	C = n.getZ();
	// Calculate D replacing a known point in the plane. (using p1 here)
	x1 = floor(x)+1.0;
	z1 = floor(z);
	y1 = heightMap_[int(x1+ z1 * sizeX_)] * heightScaleFactor_;
	D = -(A*x1 + B*y1 + C*z1);
	// Return the interpolated coordinate for Y
	return -(A*x + C*z+ D) / B;
}

Vector3D Terrain::getNormal(float x, float z) {
	struct Point {
		float x, y ,z;
	}p0, p1, p2, p3;
	Vector3D u, v, n;

	/*
	 * p0 o-----o-- p1		Points in the terrain triangle strip
	 *    |    /|
	 *    |  /  |  /
	 *    |/    |/
	 * p2 o-----o-- p3
	 *    |    /|
	 */

	p0.x = floor(x);
	p0.z = floor(z);
	p0.y = heightMap_[int(p0.x + p0.z * sizeX_)] * heightScaleFactor_;
	p1.x = floor(x)+1.0;
	p1.z = floor(z);
	p1.y = heightMap_[int(p1.x + p1.z * sizeX_)] * heightScaleFactor_;
	p2.x = floor(x);
	p2.z = floor(z)+1.0;
	p2.y = heightMap_[int(p2.x + p2.z * sizeX_)] * heightScaleFactor_;
	p3.x = floor(x)+1.0;
	p3.z = floor(z)+1.0;
	p3.y = heightMap_[int(p3.x + p3.z * sizeX_)] * heightScaleFactor_;

	if((z-floor(z)) < 1.0 - (x-floor(x))) {
		u.set(p2.x-p0.x, p2.y-p0.y, p2.z-p0.z);
		v.set(p1.x-p0.x, p1.y-p0.y, p1.z-p0.z);
		n = u * v;
	}else if((z-floor(z)) ==  1.0 - (x-floor(x))) {

	}else {
		u.set(p3.x-p2.x, p3.y-p2.y, p3.z-p2.z);
		v.set(p1.x-p2.x, p1.y-p2.y, p1.z-p2.z);
		n = u * v;
	}

	n.normalize();
	return n;
}

void Terrain::calculateNormals() {
/* NORMALS ARE NOT REALLY RIGHT */
/* SMOOTHING */
	try {
		normals_ = new Vector3D[sizeX_ * sizeZ_];
	}catch(std::exception& e) {
		MessageBox(NULL, "Not enough memory for normals.", "Memory", MB_OK | MB_ICONEXCLAMATION);
	}

	for(int z = 0 ; z < sizeZ_ ; z++) {
		for(int x = 0 ; x < sizeX_ ; x++) {
			normals_[x + z * sizeX_] = getNormal(x, z) * 1.0;
		}
	}
}

Terrain::~Terrain() {
	delete[] heightMap_;
}

float Terrain::getSizeX() {
	return sizeX_ * gridScaleFactor_;
}

float Terrain::getSizeZ() {
	return sizeZ_ * gridScaleFactor_;
}
