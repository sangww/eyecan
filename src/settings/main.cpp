/*
    eyeCan: a program that enables people to use PC with their eyes
    Copyright (C) 2012  Sang-won Leigh

    eyeCan is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    eyeCan is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#define _WIN32_WINNT 0x0500

#include "ofMain.h"
#include "theApp.h"
#include "ofAppGlutWindow.h"
#include <Windows.h>


//========================================================================
int main( )
{
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 380, 500, OF_WINDOW);			// <-------- setup the GL context

	//init window style
	/*
	m_hWnd = WindowFromDC(wglGetCurrentDC());  
	LONG style = ::GetWindowLong(m_hWnd, GWL_STYLE);  
	style &= ~WS_DLGFRAME& ~WS_CAPTION& ~WS_BORDER& ~WS_POPUP;  
	style &= ~WS_SIZEBOX  ;
	LONG exstyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);  
	exstyle &= ~WS_EX_DLGMODALFRAME;  
	::SetWindowLong(m_hWnd, GWL_STYLE, style);  
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, exstyle);  
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE);  
	*/
	ShowWindow(GetConsoleWindow(),SW_HIDE);
		
	// this kicks off the running of my app
	ofRunApp( new theApp());
}
