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


#ifndef _PROGRAMMABLE_CTRL
#define _PROGRAMMABLE_CTRL

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ProgrammableUI.h"
#include "ActionDecoder.h"
//#include "UtfConverter.h"

#include "fptrObject.h"

enum eyeaction
	{
		AD_OPEN,
		AD_BLINK,
		AD_SHORTCLICK,
		AD_CLICK,
		AD_LONGCLICK,
		AD_CLOSE,
		AD_DWELL,
		AD_OUTLEFT,
		AD_OUTRIGHT,
		AD_OUTTOP,
		AD_OUTBOTTOM,
		AD_CLICKOUTLEFT,
		AD_CLICKOUTRIGHT,
		AD_CLICKOUTTOP,
		AD_CLICKOUTBOTTOM,
		AD_MARGINLEFT,
		AD_MARGINRIGHT,
		AD_MARGINTOP,
		AD_MARGINBOTTOM,
		AD_MARGINCENTER,
		AD_MARGINCENTERHOR,
		AD_MARGINCENTERVER,
		AD_CLICKMARGINCENTER,
		AD_CLICKMARGINLEFT,
		AD_CLICKMARGINRIGHT,
		AD_CLICKMARGINTOP,
		AD_CLICKMARGINBOTTOM,
		AD_BLINKMARGINCENTER,
		AD_BLINKMARGINLEFT,
		AD_BLINKMARGINRIGHT,
		AD_BLINKMARGINTOP,
		AD_BLINKMARGINBOTTOM,
		AD_CLOSEMARGINCENTER,
		AD_CLOSEMARGINLEFT,
		AD_CLOSEMARGINRIGHT,
		AD_CLOSEMARGINTOP,
		AD_CLOSEMARGINBOTTOM,
		AD_LEFT,
		AD_RIGHT,
		AD_TOP,
		AD_BOTTOM,
		AD_CENTER,
		AD_CENTERHOR,
		AD_CENTERVER,
		AD_CLICKCENTER,
		AD_CLICKLEFT,
		AD_CLICKRIGHT,
		AD_CLICKTOP,
		AD_CLICKBOTTOM,
		AD_BLINKCENTER,
		AD_BLINKLEFT,
		AD_BLINKRIGHT,
		AD_BLINKTOP,
		AD_BLINKBOTTOM,
		AD_CLOSECENTER,
		AD_CLOSELEFT,
		AD_CLOSERIGHT,
		AD_CLOSETOP,
		AD_CLOSEBOTTOM,
		AD_NUMACTION
};

class ProgrammableController : ActionCallback {

	public:
		ProgrammableController();
		~ProgrammableController();

		ActionDecoder *am;
		
		int x, y;
		int x_seed, y_seed, x_new, y_new, x_adj, y_adj;
		double fScreenWidth, fScreenHeight; 
		double scale_x,scale_y;
		wstring wstate;

		//string list
		string pctrl_pause;

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
		void draw();
		void update();
		
		//config function
		int setup(ActionDecoder *_am);
		void setupWindow();
	
		//user interaction
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void keyPressed(int key);

		//helper ui
		ProgrammableUI	PUI;

		//input scripts
		vector<string> fn_list;
		int curIndex, selIndex;
		string fn_dir;
		void setScriptDir(string s);
		void reloadScriptDir();
		void setInputFile(string fn);

		//int margins
		int fpsMarginHorizontal;
		int fpsMarginVertical;
		int fpsSensitivityX, fpsSensitivityY;
		int marginHorizontal;
		int marginVertical;
		int oneThirdHorizontal;
		int oneThirdVertical;

		//PCtrl		
		void interpretScript();
		fptr getFunctionPtr(string s, int type=0, bool delay=false);
		WORD getKeyInputWORD(string s);		
		void resetFptrObject();
		void setFptrObject(string s, fptr f, DWORD d, bool b, bool r=false);
		fptrObject* getFptrObject(string s);
		void setupActionList();		
		
		//list of fptrobjs		
		vector<string> listAction, listMode;
		vector<fptrObject*> f_list;
		vector<fptrObject*> f_customList;
		vector<fptrObject*> f_repeatList;
		
		//mouse specific actions		
		void doClickSelector();
		bool mouseMove;
		bool mouseFps;
		int mouseMode;
		bool mouseAutoCalibrate;
		bool mouseInAction;
		bool isCenterAligning;
		bool isStarted;

		//3x3 framework		
		list<int> track;
		bool use3x3;
		int stayTime;

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
