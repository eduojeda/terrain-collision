#include "OpenGLWindow.h"

int OpenGLWindow::renderGLScene() {
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cameraPtr->place();
	worldPtr->render();
	
	SwapBuffers(hDC_);
}

void OpenGLWindow::resizeGLScene(GLsizei width_, GLsizei height_) {
	if(height_ == 0) height_ = 1;				// Prevent A Divide By Zero
	glViewport(0, 0, width_, height_);
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (GLfloat)width_/(GLfloat)height_, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

OpenGLWindow::OpenGLWindow(char* title, int width, int height, int bits, int freq): hInstance_(GetModuleHandle(NULL)), width_(width), height_(height), title_(title), bits_(bits), freq_(freq) {
	try {
	  	if(MessageBox(NULL, "Would you like to run in Fullscreen Mode?", "Start Fullscreen?", MB_YESNO | MB_ICONQUESTION) == IDYES) {
			fullscreen_ = true;
			createGLWindow();
		}else {
	 		fullscreen_ = false;
	 		width_ = 800;
	 		height_ = 600;
	 		createGLWindow();
		}
	}
	catch(WindowException e) {
		killGLWindow();
		throw;
	}

	ShowWindow(hWnd_, SW_SHOW);
	SetForegroundWindow(hWnd_);				// Thread gets a slightly higher priority. Useful?
	SetFocus(hWnd_);
	resizeGLScene(width_, height_);
}

OpenGLWindow::~OpenGLWindow() {
	killGLWindow();
}

void OpenGLWindow::killGLWindow() {
	if(fullscreen_) {
		ChangeDisplaySettings(NULL, 0);		// Switch back to default display settings
		ShowCursor(true);
	}

	if(hRC_) {
		if(!wglMakeCurrent(NULL, NULL)) {
			MessageBox(NULL,  "Release of DC and RC failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC_)) {
			MessageBox(NULL, "Release of RC failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC_ = NULL;
	}

	if(hDC_ && !ReleaseDC(hWnd_, hDC_)) {
		MessageBox(NULL, "Release of DC failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC_ = NULL;
	}

	if(hWnd_ && !DestroyWindow(hWnd_)) {
		MessageBox(NULL, "Could not release hWnd_.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd_ = NULL;
	}

	if(!UnregisterClass("OpenGL", hInstance_)) {
		MessageBox(NULL, "Could not unregister class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance_ = NULL;
	}
}

void OpenGLWindow::createGLWindow() {
	GLuint pixelFormat;
	WNDCLASS wc;
	DWORD dwExStyle;
	DWORD dwStyle;
	RECT windowRect;
	windowRect.left=(long)0;
	windowRect.right=(long)width_;
	windowRect.top=(long)0;
	windowRect.bottom=(long)height_;

	/* Fill in the window class */
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC)WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance_;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// Don't need a menu
	wc.lpszClassName	= "OpenGL";

	if(!RegisterClass(&wc))
		throw WindowException("Could not register the window class.");

	if(fullscreen_) {
		DEVMODE dmScreenSettings;
		ZeroMemory(&dmScreenSettings, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(DEVMODE);
		dmScreenSettings.dmPelsWidth = width_;
		dmScreenSettings.dmPelsHeight = height_;
		dmScreenSettings.dmBitsPerPel = bits_;
		dmScreenSettings.dmDisplayFrequency = freq_;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

		if(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			// If the mode fails, offer two options.  Quit or use Windowed Mode.
			if(MessageBox(NULL, "The requested fullscreen mode is not supported by\nyour video card. Use Windowed Mode instead?", "OpenGL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
				fullscreen_ = false;
			else
				throw WindowException("No compatible display mode. Cannot proceed.");
		}
	}

	if(fullscreen_) {
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
	}else {
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}
	
	AdjustWindowRectEx(&windowRect, dwStyle, false, dwExStyle);		// Accounts for borders and menu

	/* Create the window */
	if(!(hWnd_ = CreateWindowEx(dwExStyle,
								"OpenGL",
								title_,
								dwStyle | WS_CLIPSIBLINGS |	WS_CLIPCHILDREN,
								0, 0,
								windowRect.right - windowRect.left,
								windowRect.bottom - windowRect.top,
								NULL, NULL,
								hInstance_,
								NULL)))
		throw WindowException("CreateWindowEx failed. Cannot proceed.");

	PIXELFORMATDESCRIPTOR pfd ;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize 		= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion 	= 1;
	pfd.dwFlags 	= PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType 	= PFD_TYPE_RGBA;
	pfd.cColorBits 	= bits_;
	pfd.cDepthBits 	= 16;

	if(!(hDC_ = GetDC(hWnd_)))
		throw WindowException("Could not get a Device Context.");

	if(!(pixelFormat = ChoosePixelFormat(hDC_, &pfd)))
		throw WindowException("Could not find a matching Pixel Format.");

	if(!SetPixelFormat(hDC_, pixelFormat, &pfd))
		throw WindowException("Could not set the Pixel Format.");

	if(!(hRC_ = wglCreateContext(hDC_)))
		throw WindowException("Could not create a GL Rendering Context.");

	if(!wglMakeCurrent(hDC_,hRC_))
		throw WindowException("Could not activate the GL Rendering Context.");
		
	ShowCursor(false);
}
