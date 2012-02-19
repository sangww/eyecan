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
