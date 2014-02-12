#include <windows.h>
#include "OpenGLWindow.h"
#include "Camera.h"
#include "World.h"

/* Function Declarations */
void handleKeyboard();
void handleMouse();
void initialize();
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/* Constants */
#define WINDOW_W	1680
#define WINDOW_H	1050
#define WINDOW_BITS	32
#define WINDOW_FREQ	85

/* Globals */ /*TRY TO REMOVE*/
bool			done = false;
bool			active = true;
OpenGLWindow*	windowPtr;
Camera*			cameraPtr;
World*			worldPtr;
