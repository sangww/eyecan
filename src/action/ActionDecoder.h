#ifndef _ACTION_DECODER
#define _ACTION_DECODER

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include <list>
#include <vector>

enum onScreenPosition{
	in = 0,
	otop = 1,
	obottom = 2,
	oleft = 3,
	oright =4
};

class ActionCallback{
public:	
	virtual void eyeMove(int x,int y,onScreenPosition pos) =0;
	virtual void eyeClick(int x,int y,onScreenPosition pos) =0;
	virtual void eyeShortClick(int x,int y,onScreenPosition pos) =0;
	virtual void eyeLongClick(int x,int y,onScreenPosition pos) =0;
	virtual void eyeBlink(int x,int y) =0;
	virtual void eyeGesture(int x,int y) =0;
	virtual void eyeOutofScreen(int x,int y,onScreenPosition pos) =0;
	virtual void eyeDwell(int x,int y,onScreenPosition pos,int dwellTime) =0;
	virtual void eyeClose(int x,int y,onScreenPosition pos,int state) =0; //0-2: blink,click,long

	virtual void update()=0;
	virtual void draw()=0;
};

class ActionDecoder {

	public:
		ActionDecoder();
		~ActionDecoder();

		int idCurrentController;
		int idDefaultController;
		vector<ActionCallback*> cbList;
		ActionCallback* cb;
		int setCallback(ActionCallback *_cb);
		void setDefaultCallback(int cn);
		void changeCallback(int cc);
		void changeCallbackToDefault();
		
		int framerate;
		int	x, y;
		int x_offset,y_offset;
		int close_count, prev_state;	
		int thrShortClick, thrClick, thrLongClick, thrLongerClick;
		int stayRange, thrDwell, thrLongDwell;
		int thrOutDwell, thrOutLongClick; // here are many todos
		list<int> trackedPntsX,trackedPntsY;

		bool dispatchEvents;
				
		double fScreenWidth; 
		double fScreenHeight; 

		void Move (int i_x, int i_y , bool eyeclosed);
		void setup(int _framerate);
		void setOffset(int i_x, int i_y);
		void adjustOffset(int i_x, int i_y);
		void resetOffset();

		void toggleEventDispatch();

		void draw();
		void update();

		//load settings
		void loadXMLSettings();
};

#endif
