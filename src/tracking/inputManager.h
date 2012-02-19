#ifndef _INPUT_MANAGER
#define _INPUT_MANAGER

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxOpenCv.h"

enum {
	
	INPUT_LIVE_VIDEO, INPUT_VIDEO

};




class inputManager {

	public:

		void setup();
		void update();
		void drawFrame(int x, int y, int width, int height);

		int						mode;
	
		ofVideoGrabber			vidGrabber;
		ofVideoPlayer			vidPlayer;
		int						width,height;
		ofxCvColorImage			colorImg;
        ofxCvGrayscaleImage 	grayImage;
		
		bool					bIsFrameNew;			// on this update, is the frame new?
		
		/*
		bool bUseAuxCam;
		ofVideoGrabber			aux_vidGrabber;
		ofVideoPlayer			aux_vidPlayer;
		int						aux_width,aux_height;
		ofxCvColorImage			aux_colorImg;
        ofxCvGrayscaleImage 	aux_grayImage;
		*/
};

#endif
