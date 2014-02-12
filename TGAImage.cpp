#include "TGAImage.h"

TGAImage::TGAImage(char* filename) {
	FILE* file = fopen(filename, "rb");
	if(file == NULL)
		throw FileException("File does not exist.");

	try {
		loadTGAHeader(file);
		if(	(identSize_ != 0) |		// Only true color uncompressed RGB
			(colourMapType_ != 0) |	// or Grayscale images supported.
			(imageType_ < 2) |
			(imageType_ > 3)) {
			throw FileException("TGA image type not supported.");
		}
		loadTGAData(file);
	}catch(...) {
		throw;
	}

	fclose(file);
}

TGAImage::~TGAImage() {
	delete[] imageData_;
}

void TGAImage::loadTGAHeader(FILE *file) {
	fread(&identSize_, sizeof(unsigned char), 1, file);
	fread(&colourMapType_, sizeof(unsigned char), 1, file);
	fread(&imageType_, sizeof(unsigned char), 1, file);
	fread(&colourMapStart_, sizeof(short int), 1, file);
	fread(&colourMapLength_, sizeof(short int), 1, file);
	fread(&colourMapBits_, sizeof(unsigned char), 1, file);
	fread(&xStart_, sizeof(short int), 1, file);
	fread(&yStart_, sizeof(short int), 1, file);
	fread(&width_, sizeof(short int), 1, file);
	fread(&height_, sizeof(short int), 1, file);
	fread(&bits_, sizeof(unsigned char), 1, file);
	fread(&descriptor_, sizeof(unsigned char), 1, file);
}

void TGAImage::loadTGAData(FILE *file) {
	unsigned char aux;
	int bytesPerPixel	= bits_ / 8,						// Components per pixel
		total 			= width_ * height_ * bytesPerPixel;	// Total number of bytes

	try {
		imageData_ = new unsigned char[total];
	}catch(std::exception& e) {
		MessageBox(NULL, "Ran out of memory.", "Memory", MB_OK | MB_ICONEXCLAMATION);
		throw;
	}
	if(fread(imageData_, sizeof(unsigned char), total, file) != total) {
		throw FileException("Error reading file data.");
	}

	if(bytesPerPixel >= 3) {						// TGA stores data as BGR(A) so we'll have
		for(int i=0; i<total; i+=bytesPerPixel) {	// to swap R and B if image is RGB
			aux = imageData_[i];
			imageData_[i] = imageData_[i+2];
			imageData_[i+2] = aux;
		}
	}
}
