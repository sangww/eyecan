
#define _WIN32_WINNT 0x0500

#include "ofMain.h"
#include "theApp.h"
#include "ofAppGlutWindow.h"

#include <ShellAPI.h>
#include "winHandler.h"
#include "appDelegate.h"

//========================================================================
int main( )
{
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 780, 500, OF_WINDOW);			// <-------- setup the GL context

	//init window style
	m_hWnd = WindowFromDC(wglGetCurrentDC());  
	LONG style = ::GetWindowLong(m_hWnd, GWL_STYLE);  
	style &= ~WS_DLGFRAME& ~WS_CAPTION& ~WS_BORDER& ~WS_POPUP;  
	style &= ~WS_SIZEBOX  ;
	LONG exstyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);  
	exstyle &= ~WS_EX_DLGMODALFRAME;  
	::SetWindowLong(m_hWnd, GWL_STYLE, style);  
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, exstyle);  
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE);  

	//get Console
	m_hCon = GetConsoleWindow();
		
	// this kicks off the running of my app
	app = new theApp();
	ofRunApp( app);
}
