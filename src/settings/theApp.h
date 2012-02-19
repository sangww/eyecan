#ifndef _THE_APP
#define _THE_APP


#include "ofMain.h"
#include "ofxThread.h"
#include "ofxControlPanel.h"



class theApp : public ofBaseApp {

	public:
		theApp();
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void resized(int w, int h);

		ofxControlPanel		panel;
};

#endif
