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



#ifndef _TEST_CTRL
#define _MOUSE_CTRL

#include "ofMain.h"
#include "ActionDecoder.h"
#include "ofTrueTypeFontWS.h"

class TestController : ActionCallback {

	public:
		TestController();
		~TestController();
		
		ActionDecoder *am;

		//callbacks		
		void eyeMove(int _x, int _y, onScreenPosition pos);
		void eyeShortClick(int _x, int _y, onScreenPosition pos);
		void eyeClick(int _x, int _y, onScreenPosition pos);
		void eyeLongClick(int _x, int _y, onScreenPosition pos);
		void eyeBlink(int _x, int _y);
		void eyeGesture(int _x, int _y);
		void eyeOutofScreen(int _x, int _y, onScreenPosition pos);
		void eyeDwell(int _x, int _y, onScreenPosition pos,int dwellTime);
		void eyeClose(int _x, int _y, onScreenPosition pos,int state);
		
		//config function
		int setup(ActionDecoder *_am);
		void update();
		void draw();
		void setupWindow();
	
		//user interaction
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void keyPressed(int key);

		//msg
		wstring wstate;
		ofTrueTypeFontWS fontWS;

		//load settings
		void loadXMLSettings();
};

#endif
