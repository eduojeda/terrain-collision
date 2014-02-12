#ifndef OpenGLWindow_H
#define OpenGLWindow_H

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <exception>
#include "Camera.h"
#include "World.h"

extern LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
extern Camera* 	cameraPtr;
extern World*	worldPtr;

class OpenGLWindow {
 private:
// Data
	HINSTANCE hInstance_;
	HDC		  hDC_;		// Private GDI Device Context
	HGLRC	  hRC_;		// Permanent Rendering Context
	HWND	  hWnd_;
	
	char*	title_;
	int		width_;
	int		height_;
	int		bits_;
	int		freq_;
	bool	fullscreen_;
	
// Methods
	void createGLWindow();
	void killGLWindow();

 public:
// Constructors
	OpenGLWindow(char* title, int width, int height, int bits, int freq);
	~OpenGLWindow();
	
// Methods
	int		renderGLScene();
	void	resizeGLScene(GLsizei width, GLsizei height);

	inline HDC getDeviceContext(){return hDC_;}
	inline int getWidth() {return width_;}
	inline int getHeight() {return height_;}
};

class WindowException: public std::exception {
 private:
	DWORD  _err;
	char*  _msg;
	
 public:
	WindowException(char* msg) : _msg(msg), _err(GetLastError()) {}
	DWORD getError(){ return _err; }
	char* getMessage(){ return _msg; }
};
#endif
