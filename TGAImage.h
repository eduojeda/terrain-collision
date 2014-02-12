#ifndef TGAImage_H
#define TGAImage_H

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <exception>

class TGAImage {
 private:
// Header Data
	unsigned char	identSize_;			// Size of ID field that follows 18 byte header (0 usually)
	unsigned char	colourMapType_;		// Type of colour map 0=none, 1=has palette
	unsigned char	imageType_;			// Type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed
	short int		colourMapStart_;	// First colour map entry in palette
	short int		colourMapLength_;	// Number of colours in palette
	unsigned char	colourMapBits_;		// Number of bits per palette entry 15,16,24,32
	short int		xStart_;			// Image x origin
	short int		yStart_;			// Image y origin
	short int		width_;				// Image width in pixels
	short int		height_;			// Image height in pixels
	unsigned char	bits_;				// Image bits per pixel 8,16,24,32
	unsigned char	descriptor_;		// Image descriptor bits (vh flip bits)

	unsigned char*	imageData_;			// Pointer to the array that will hold the image

// Methods
	void loadTGAHeader(FILE *file);
	void loadTGAData(FILE *file);

 public:
// Constructors
	TGAImage(char* filename);
	~TGAImage();
	
// Methods
	inline int getWidth() {return (int)width_;}
	inline int getHeight() {return (int)height_;}
	inline int getBits() {return (int)bits_;}
	inline void* getImageData() {return (void*)imageData_;}
};

class FileException: public std::exception {
 private:
	DWORD  _err;
	char*  _msg;
	
 public:
	FileException(char* msg) : _msg(msg), _err(GetLastError()) {}
	DWORD getError(){ return _err; }
	char* getMessage(){ return _msg; }
};
#endif
