#ifndef _THE_APP
#define _THE_APP


#include "ofMain.h"
#include "trackingManager.h"	
#include "calibrationManager.h"
#include "ActionDecoder.h"
#include "MouseController.h"
#include "ProgrammableController.h"
#include "TestController.h"
#include "ofxThread.h"


enum AppMode{	
	MODE_TRACKING,	MODE_CALIBRATING, MODE_MOUSE, MODE_PROGRAMMABLE, NUM
};


class theApp : public ofBaseApp {

	public:
		theApp();
		void setup();
		void update();
		void draw();

		void changeMode(int m = MODE_MOUSE);

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void resized(int w, int h);

		//----- main managers
		trackingManager			TM;
		calibrationManager		CM;
		ActionDecoder			AD;

		//control manager
		int hMC, hPC;
		MouseController			MC; // mouse control
		ProgrammableController	PC;	
		char szFileName[MAX_PATH];
		
		//text
		ofTrueTypeFontWS			fontWS;

		//------ drawing
		ofPoint eyeSmoothed;	
		int mode; 
		bool simulateMouse;
		bool toggleKeyPressed;
		int showConsole;
		bool showPerf;
		int screenWidth, screenHeight;
		int framerate;
		bool isOpaque;
		int bgr,bgg,bgb;
		bool autoStart;
		
		//audio ui
		void playAlarmSound(DWORD c);
		ofSoundPlayer sp_alarm;
		void playSound(DWORD c);
		void addSound(string s);
		void resetSound();
		vector<ofSoundPlayer*> sp_list;

		//load settings
		void loadXMLSettings();
		void reloadAllXMLSettings();
		string locale;

		//for testing purpose
		TestController TC;
		bool testing;
		int hTC;
};

#endif
