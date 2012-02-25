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


#ifndef _PROGRAMMABLE_UI
#define _PROGRAMMABLE_UI


#include "ofMain.h"
#include "ofTrueTypeFontWS.h"
#include "buttonPanel.h"

class ProgrammableUI {

	public:
		ProgrammableUI();
		~ProgrammableUI();

		void setup();
		void update(int x, int y);
		void setupWindow();
		void setupWindowPosition(int x,int y);
		void setMode(int m);
		void draw(wstring state, int x, int y);
		void draw(wstring state, wstring help);
		void draw(string state, string help);
	
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseDragged(int x, int y, int button);	
		
	
		ofTrueTypeFontWS		fontWS, helpWS;

		int posX,posY; //window idle position
		float fScreenWidth; 
		float fScreenHeight; 
		int mode;
		int x_drag_start,y_drag_start;
		
		buttonPanel bp;
};

#endif
