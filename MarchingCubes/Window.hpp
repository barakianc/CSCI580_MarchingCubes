#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <Windows.h>
#include <GL\glew.h>
#include <string>

using namespace std;

class Window
{
private:
	static const int SCREEN_WIDTH_VIRTUAL = 1280;
	static const int SCREEN_HEIGHT_VIRTUAL = 720;

	int screenWidth;
	int screenHeight;
	
	bool fullscreen;

	string title;

public:
	
	HGLRC		hRC;		// possibly needs to be moved to renderer
	HDC			hDC;		// possibly needs to be moved to renderer
	HWND		hWnd;
	HINSTANCE	hInstance;
	WNDPROC		WndProc;


	Window(WNDPROC WndProc)
	{
		this->title = "";
		this->WndProc = WndProc;

		this->screenWidth = 1024;
		this->screenHeight = 576;

		this->hRC = NULL;
		this->hDC = NULL;
		this->hWnd = NULL;
		this->hInstance = NULL;

		this->fullscreen = false;
	}

	virtual ~Window()
	{
	}

	int getScreenWidth()
	{
		return screenWidth;
	}

	int getScreenHeight()
	{
		return screenHeight;
	}

	float getAspectRatio()
	{
		return (float)screenWidth / (float)screenHeight;
	}

	bool setFullscreen(bool isFullscreen)
	{
		if (fullscreen == isFullscreen)
			return true;

		return createWindow(title, screenWidth, screenHeight, 32, isFullscreen);
	}

	void resizeWindow(int width, int height)
	{
		if (height == 0)
			height = 1;

		glViewport(0,0, width,height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(90.0f, (float)width/(float)height, 0.1f, 500.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void killWindow()
	{
		if (fullscreen)
		{
			ChangeDisplaySettings(NULL, 0);
			ShowCursor(true);
		}

		if (hRC)
		{
			if (!wglMakeCurrent(NULL, NULL))
				MessageBox(NULL, "Release of DC and RC failed!", "Shutdown Error", MB_OK | MB_ICONINFORMATION);

			if (!wglDeleteContext(hRC))
				MessageBox(NULL, "Release rendering context failed!", "Shutdown Error", MB_OK | MB_ICONINFORMATION);

			hRC = NULL;
		}

		if (hDC && !ReleaseDC(hWnd, hDC))
		{
			MessageBox(NULL, "Release device context failed!", "Shutdown Error", MB_OK | MB_ICONINFORMATION);
			hDC = NULL;
		}

		if (hWnd && !DestroyWindow(hWnd))
		{
			MessageBox(NULL, "Could not release hWnd!", "Shutdown Error", MB_OK | MB_ICONINFORMATION);
			hWnd = NULL;
		}

		if (!UnregisterClass("OpenGL", hInstance))
		{
			MessageBox(NULL, "Could not unregister class!", "Shutdown Error", MB_OK | MB_ICONINFORMATION);
			hInstance = NULL;
		}
	}

	bool createWindow(string title, int width, int height, int bits, bool fullscreenFlag)
	{
		GLuint		PixelFormat;			
		WNDCLASS	wc;						
		DWORD		dwExStyle;				
		DWORD		dwStyle;				
		RECT		WindowRect;				
		WindowRect.left=(long)0;			
		WindowRect.right=(long)width;		
		WindowRect.top=(long)0;				
		WindowRect.bottom=(long)height;		

		fullscreen=fullscreenFlag;

		this->title = title;

		hInstance			= GetModuleHandle(NULL);				
		wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
		wc.lpfnWndProc		= (WNDPROC) WndProc;					
		wc.cbClsExtra		= 0;									
		wc.cbWndExtra		= 0;									
		wc.hInstance		= hInstance;							
		wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			
		wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			
		wc.hbrBackground	= NULL;									
		wc.lpszMenuName		= NULL;									
		wc.lpszClassName	= "OpenGL";								

		if (!RegisterClass(&wc))									
		{
			MessageBox(NULL,"Failed To Register The Window Class.","Error!",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;											
		}
	
		if (fullscreen)												
		{
			DEVMODE dmScreenSettings;								
			memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
			dmScreenSettings.dmSize=sizeof(dmScreenSettings);		
			dmScreenSettings.dmPelsWidth	= width;				
			dmScreenSettings.dmPelsHeight	= height;				
			dmScreenSettings.dmBitsPerPel	= bits;					
			dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

			if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
			{
				if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
				{
					fullscreen=false;
				}
				else
				{
					MessageBox(NULL,"Program Will Now Close.","Error!",MB_OK|MB_ICONSTOP);
					return false;
				}
			}
		}

		if (fullscreen)												
		{
			dwExStyle=WS_EX_APPWINDOW;								
			dwStyle=WS_POPUP;										
			ShowCursor(FALSE);										
		}
		else
		{
			dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			
			dwStyle=WS_OVERLAPPEDWINDOW;							
		}

		AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	

		// Create The Window
		if (!(hWnd=CreateWindowEx(	dwExStyle,						
									"OpenGL",						
									title.c_str(),							
									dwStyle |						
									WS_CLIPSIBLINGS | WS_CLIPCHILDREN,				
									0, 0,							
									WindowRect.right-WindowRect.left,
									WindowRect.bottom-WindowRect.top,
									NULL,							
									NULL,							
									hInstance,						
									NULL)))							
		{
			killWindow();
			MessageBox(NULL,"Window Creation Error.","Error!",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;
		}

		static	PIXELFORMATDESCRIPTOR pfd=
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,								
			PFD_DRAW_TO_WINDOW |			
			PFD_SUPPORT_OPENGL |			
			PFD_DOUBLEBUFFER,				
			PFD_TYPE_RGBA,					
			bits,							
			0, 0, 0, 0, 0, 0,				
			0,								
			0,								
			0,								
			0, 0, 0, 0,						
			16,								
			0,								
			0,								
			PFD_MAIN_PLANE,					
			0,								
			0, 0, 0							
		};
	
		if (!(hDC=GetDC(hWnd)))				
		{
			killWindow();					
			MessageBox(NULL,"Can't Create A GL Device Context.","Error!",MB_OK|MB_ICONEXCLAMATION);
			return false;
		}

		if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))
		{
			killWindow();
			MessageBox(NULL,"Can't Find A Suitable PixelFormat.","Error!",MB_OK|MB_ICONEXCLAMATION);
			return false;
		}

		if(!SetPixelFormat(hDC,PixelFormat,&pfd))
		{
			killWindow();
			MessageBox(NULL,"Can't Set The PixelFormat.","Error!",MB_OK|MB_ICONEXCLAMATION);
			return false;
		}

		if (!(hRC=wglCreateContext(hDC)))
		{
			killWindow();
			MessageBox(NULL,"Can't Create A GL Rendering Context.","Error!",MB_OK|MB_ICONEXCLAMATION);
			return false;
		}

		if(!wglMakeCurrent(hDC,hRC))
		{
			killWindow();
			MessageBox(NULL,"Can't Activate The GL Rendering Context.","Error!",MB_OK|MB_ICONEXCLAMATION);
			return false;
		}

		ShowWindow(hWnd,SW_SHOW);
		SetForegroundWindow(hWnd);
		SetFocus(hWnd);
		resizeWindow(width, height);

		return true;
	}
};

#endif