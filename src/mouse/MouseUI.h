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



#ifndef _MOUSE_UI
#define _MOUSE_UI

#include "ofMain.h"
#include "ofTrueTypeFontWS.h"
#include "buttonPanel.h"
#include <vector>
#include "fptrObject.h"

class MouseUI {

	public:
		MouseUI();
		~MouseUI();

		void update(int x, int y);
		void draw(wstring state, wstring help, bool originalPosition=false);
		void draw(wstring state, int x, int y, int size);
		void drawRect(int x, int y, int size);
		void resetMouseButtons(int except);
		void resetMouseMode();
		void drawButtons();
		void updateButtons(int x, int y);
		void invalidate();
	
		//mouse event
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseDragged(int x, int y, int button);

		bool isMouseOver(int x, int y);
		bool isMouseOverButton(int x, int y);
		void eyeMouseClicked(int x, int y );
		void eyeMouseActionDone();
	
		//setup
		void setup();
		void setupWindow();
		void setupWindowPosition(int x,int y);
	
		//gui
		ofTrueTypeFontWS		fontWS, helpWS;
		
		int mode;
		bool hold;
		int posX,posY; //window idle position
		int x_drag_start,y_drag_start;

		vector<buttonPanel> bp;
		buttonPanel bp_main;
		
		//hotkey framework		
		vector<fptrObject*> f_list;
		void interpretScript();
		WORD getKeyInputWORD(string s);		

		//string tokenizer
		vector<string> &split(const string &s, char delim, vector<string> &elems) { 
			stringstream ss(s); 
			string item; 
			while(getline(ss, item, delim)) { 
				elems.push_back(item); 
			} 
			return elems; 
		}  
		vector<string> split(const string &s, char delim) { 
			vector<string> elems; 
			return split(s, delim, elems); 
		}
};

#endif
