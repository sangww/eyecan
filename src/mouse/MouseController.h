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


#ifndef _MOUSE_CTRL
#define _MOUSE_CTRL

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "MouseUI.h"
#include "ActionDecoder.h"

enum MouseMode{
	idle, clickhelper, errorcompensator,
};

class MouseController : ActionCallback {

	public:
		MouseController();
		~MouseController();

		bool inAction;
		bool isLocked;

		ActionDecoder *am;
		MouseMode mode;
		bool useClickHelper;
		bool useErrorCompensator;

		int x, y;
		int x_seed, y_seed, x_new, y_new, x_adj, y_adj;
		double fScreenWidth, fScreenHeight; 
		double scale_x,scale_y;
		int scrollThreshold;

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

		//selector
		void doClickSelector();

		//function definition
		int invalidate_cnt;
		void setErrorCompensate();
		void doErrorCompensate();
		void cancelErrorCompensate();
		void computeOffset();
		void setClickHelper();
		void updateClickHelper();
		void doClickHelper(int _x, int _y);
		void cancelClickHelper();


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

		//helper ui
		MouseUI	MUI;
		int szCompensateBox, szClickhelperBox;

		//msg
		wstring wstate;
		wstring whelp;

		//load settings
		void loadXMLSettings();

		//wstring
		wstring mouse_ready, mouse_help_f12,
				mouse_locked, mouse_dragging, mouse_scrolling, mouse_help_1, mouse_help_2,
				mouse_out_top, mouse_out_bottom, mouse_out_left, mouse_out_right, 
				mouse_closed, mouse_closed_click, mouse_closed_long, 
				mouse_click_helper, mouse_click_helper_ok, mouse_do_error_comp, mouse_error_comp, mouse_error_comp_ok, mouse_cancel_error_comp;
		wstring mouse_help_click_helper, mouse_help_error_comp;
};

#endif
