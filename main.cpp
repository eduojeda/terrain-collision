#include "main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MSG	msg;
	
	/* Set up things */
	try {
		windowPtr = new OpenGLWindow("Peng!", WINDOW_W, WINDOW_H, WINDOW_BITS, WINDOW_FREQ);
	}
	catch(WindowException e) {
		MessageBox(NULL, e.getMessage(), "Cannot Create Window:", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	SetCursorPos(windowPtr->getWidth()/2, windowPtr->getHeight()/2);
	
	initialize();

	/* Main Event Loop*/
	while(!done) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {	// If there are messages for the application waiting
			if(msg.message == WM_QUIT) {
				done = true;
			}else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		
		DWORD tickCount = 0;
		DWORD lastTickCount = GetTickCount();
		
		//use QueryPerformanceCounter directly!
		
		if(active) {
			handleKeyboard();
			handleMouse();
			windowPtr->renderGLScene();
			// SHOULD CALL THIS WITH timeSetEvent()?
			// Maybe that will make it smoother
			tickCount = GetTickCount();
			worldPtr->simTimeStep(tickCount - lastTickCount);
			//worldPtr->simTimeStep(30);
			lastTickCount = tickCount;
		}
	}
	return(msg.wParam);
}

void handleKeyboard() {
float 	pitch = cameraPtr->getPitch()*M_PI/180.0,
		yaw = cameraPtr->getYaw()*M_PI/180.0,
		s = 0.5,  // sensibility
		p = 30.0; // ball-shot velocity
static bool leftMouseBtn;

	if(GetAsyncKeyState('A') & 0x8000)
		cameraPtr->move(-cos(yaw)*s, 0.0, sin(yaw)*s);
	if(GetAsyncKeyState('D') & 0x8000)
		cameraPtr->move(cos(yaw)*s, 0.0, -sin(yaw)*s);
	if(GetAsyncKeyState('W') & 0x8000)
		cameraPtr->move(-sin(yaw)*cos(pitch)*s, sin(pitch)*s, -cos(yaw)*cos(pitch)*s);
	if(GetAsyncKeyState('S') & 0x8000)
		cameraPtr->move(sin(yaw)*cos(pitch)*s, -sin(pitch)*s, cos(yaw)*cos(pitch)*s);
	if(GetAsyncKeyState('E') & 0x8000)
		cameraPtr->move(0.0, 0.3, 0.0);
	if(GetAsyncKeyState('C') & 0x8000)
		cameraPtr->move(0.0, -0.3, 0.0);
		
	if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		leftMouseBtn = true;
	if(!(GetAsyncKeyState(VK_LBUTTON) & 0x8000) && leftMouseBtn) {
		worldPtr->addMass(new Ball(Vector3D(cameraPtr->getPosition()),
											Vector3D(-sin(yaw)*cos(pitch)*p, sin(pitch)*p, -cos(yaw)*cos(pitch)*p)));
		leftMouseBtn = false;
	}
}

void handleMouse(void) {
	POINT mousePos;
	float delta;
	float centerX = windowPtr->getWidth() / 2.0;
	float centerY = windowPtr->getHeight() / 2.0;

	GetCursorPos(&mousePos);
	
	delta = centerX - mousePos.x;
	if(GetAsyncKeyState(VK_SHIFT) & 0x8000)
		cameraPtr->rotate(0.0, 0.0, 0.1 * delta);
	else
		cameraPtr->rotate(0.0, 0.1 * delta, 0.0);
	
	delta = centerY - mousePos.y;
	cameraPtr->rotate(0.1 * delta, 0.0, 0.0);

	SetCursorPos((int)centerX, (int)centerY);
}

void initialize() {
	glShadeModel(GL_FLAT);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Wireframe Mode
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	cameraPtr = new Camera(10.0, 20.0, 10.0, 0.0, -135.0, 0.0);
	worldPtr = new World();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message) {
		case WM_ACTIVATE:							// Watch For Window Activate Message
			if(!HIWORD(wParam)) {					// Check Minimization State
				active = true;
			}
			else {
				active = false;
			}
			return 0;
			break;

		case WM_SYSCOMMAND:							// Intercept System Commands
			switch(wParam) {
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;

		case WM_CLOSE:								// Did We Receive A Close Message?
			PostQuitMessage(0);
			return 0;
			break;

		case WM_SIZE:							   // Resize The OpenGL Window
			windowPtr->resizeGLScene(LOWORD(lParam),HIWORD(lParam)); // LoWord=Width, HiWord=Height
			return 0;
			break;

		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
				done = true;
			return 0;
			break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);		// Pass All Unhandled Messages To DefWindowProc
}
