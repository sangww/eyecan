#include "inputManager.h"


//--------------------------------------------------------------
void inputManager::setup(){	
	ofxXmlSettings XML;
	XML.loadFile("settings/settings.xml");
	mode = XML.getValue("input:mode", 0);
		
	if (mode == INPUT_VIDEO){
		//cout << "input manager: loading video file" << endl;
		string movieToLoad = XML.getValue("input:videoFile", "");
		vidPlayer.loadMovie(movieToLoad);
		vidPlayer.play();
		width	= vidPlayer.width;
		height	= vidPlayer.height;
	}
	
	if (mode == INPUT_LIVE_VIDEO){
		//cout << "input manager: grabbing video" << endl;
		width = XML.getValue("input:videoGrabber:width", 640);
		height = XML.getValue("input:videoGrabber:height", 480);
		int device = XML.getValue("input:videoGrabber:deviceId", 0);
		
		if (device != 0){
			vidGrabber.setDeviceID(device);
		}
		
		vidGrabber.initGrabber(width, height, false);	// false = no texture. faster...
		width = vidGrabber.width;
		height = vidGrabber.height;		// in case we are different then what we request... ;)
	}
	
	if (width != 0 || height != 0){
		colorImg.setUseTexture(false);
		grayImage.setUseTexture(false);
		colorImg.allocate(width,height);
		grayImage.allocate(width,height);
	}

	bIsFrameNew = false;

}

//--------------------------------------------------------------
void inputManager::update(){
	
	
	
	if (mode == INPUT_LIVE_VIDEO){
		vidGrabber.grabFrame();
		bIsFrameNew = vidGrabber.isFrameNew();
	} else {
		vidPlayer.idleMovie();
		bIsFrameNew = vidPlayer.isFrameNew();
	}
	
	if (bIsFrameNew){

		if (mode == INPUT_LIVE_VIDEO){
			colorImg.setFromPixels(vidGrabber.getPixels(),width,height);
		} else {
			colorImg.setFromPixels(vidPlayer.getPixels(), width,height);
		}
		
        grayImage = colorImg;		// TODO: this color to gray conversion is *slow*, since it's using the CV cvt color, we can make this faster !
	}	
}

void inputManager::drawFrame(int x, int y, int width, int height){
	grayImage.draw(x,y,width, height);
}
