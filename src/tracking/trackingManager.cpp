#include "trackingManager.h"
#include "appDelegate.h"


trackingManager::trackingManager(){
}
trackingManager::~trackingManager(){	
}

void trackingManager::setup()
{	
	IM.setup();	
	setupGui();	
	//--- set up tracking
	tracker.setup(IM.width, IM.height);
	minBlob		= 10*10;
	maxBlob		= 100*100;
	threshold	= 21;	
	bFoundEye = false;
	eyePoint.set(0,0,0);

	x_seed=-1;
	y_seed=-1;
	scale = IM.width/640.;

	//btt	
	btt.setup("Back",440, 420,70,70,"images/home"+app->locale+".png");
}

void trackingManager::update()
{
	IM.update();	
	//--- eye tracking (on new frames)	
	if (IM.bIsFrameNew){
		trackEyes();
	}	
	//--- gui
	panel.update();
	updateGui();	
}

ofPoint	trackingManager::getEyePoint(){
	return eyePoint;
}
bool trackingManager::bGotAnEyeThisFrame(){
	return bFoundEye;
}
void trackingManager::trackEyes()
{
	tracker.update(IM.grayImage, threshold, minBlob, maxBlob, 0.5f);
	bFoundEye	= tracker.bFoundOne;
	if(bFoundEye) //fix: click problem
		eyePoint	= tracker.getEyePoint();
}


void trackingManager::setupGui()
{	
	panel.setup("Camera", 520, 10, 240, 460);
	panel.addPanel("Camera Settings", 1, false);
	panel.addPanel("Advanced Settings", 1, false);	
	panel.addPanel("Optional", 1, false);
	
	//---- gaze
	panel.setWhichPanel("Camera Settings");
	panel.setWhichColumn(0);
	panel.addSlider("contrast ", "CONTRAST", 0.28f, 0.0, 1.0f, false);
	panel.addSlider("brightness ", "BRIGHTNESS", -0.02f, -1.0, 3.0f, false);
	panel.addSlider("gamma ", "GAMMA", 0.57f, 0.01, 3.0f, false);		
	panel.addSlider("threshold ", "THRESHOLD_GAZE", threshold, 0, 255, true);
    panel.addSlider("min blob","MIN_BLOB",10*10,0,5000,true);
    panel.addSlider("max blob","MAX_BLOB",100*100,0,50500,true);
	
	panel.setWhichPanel("Advanced Settings");
	panel.setWhichColumn(0);
	panel.addToggle("flip horizontal ", "B_RIGHT_FLIP_X", false);
	panel.addToggle("flip vertical ", "B_RIGHT_FLIP_Y", false);	
	panel.addToggle("use contrast / bri", "B_USE_CONTRAST", true);	
	panel.addToggle("use gamma ", "B_USE_GAMMA", true);
	panel.addToggle("use dilate", "B_USE_DILATE", true);
	panel.addSlider("dilate num ", "N_DILATIONS", 0, 0, 10, true);
	if (IM.mode == INPUT_VIDEO){
		// TODO: add theo's video playing things.... [zach]
	} else {
		panel.addToggle("load video settings", "VIDEO_SETTINGS", false);
	}
	
	panel.setWhichPanel("Optional");
	panel.setWhichColumn(0);		
	panel.addSlider("x position ", "EDGE_MASK_X", 320, 0, 640, true);
	panel.addSlider("y position ", "EDGE_MASK_Y", 240, 0, 640, true);
	panel.addSlider("inner radius ", "EDGE_MASK_INNER_RADIUS", 250, 0, 500, true);
	panel.addSlider("outer radius ", "EDGE_MASK_OUTER_RADIUS", 350, 0, 600, true);	
		
	panel.loadSettings("settings/trackingSettings.xml");	
}

void trackingManager::updateGui()
{	
	tracker.flip(  panel.getValueB("B_RIGHT_FLIP_X"),  panel.getValueB("B_RIGHT_FLIP_Y") );
	
	minBlob = panel.getValueI("MIN_BLOB");
	maxBlob = panel.getValueI("MAX_BLOB");
	
	threshold				= panel.getValueI("THRESHOLD_GAZE");	
	tracker.gamma			= panel.getValueF("GAMMA");
	tracker.bUseGamma		= panel.getValueB("B_USE_GAMMA");
	tracker.contrast		= panel.getValueF("CONTRAST");
	tracker.brightness		= panel.getValueF("BRIGHTNESS");
	tracker.bUseContrast	= panel.getValueB("B_USE_CONTRAST");
	tracker.nDilations		= panel.getValueI("N_DILATIONS");
	tracker.bUseDilate		= panel.getValueB("B_USE_DILATE");

	int oldx				= tracker.edgeMaskStartPos.x;
	int oldy				= tracker.edgeMaskStartPos.y;
	int oldir				= tracker.edgeMaskInnerRadius;
	int oldor				= tracker.edgeMaskOuterRadius;

	tracker.edgeMaskStartPos.x		= panel.getValueI("EDGE_MASK_X");
	tracker.edgeMaskStartPos.y		= panel.getValueI("EDGE_MASK_Y");
	tracker.edgeMaskInnerRadius	= panel.getValueI("EDGE_MASK_INNER_RADIUS");
	tracker.edgeMaskOuterRadius	= panel.getValueI("EDGE_MASK_OUTER_RADIUS");

	if (	oldx	!= tracker.edgeMaskStartPos.x  ||
			oldy	!= tracker.edgeMaskStartPos.y  ||
			oldir	!= tracker.edgeMaskInnerRadius ||
			oldor	!= tracker.edgeMaskOuterRadius	){		

			tracker.calculateEdgePixels();
		
	}

	if (IM.mode != INPUT_VIDEO){
		panel.setWhichPanel("Advanced Settings");
		if (panel.getValueB("VIDEO_SETTINGS") == true){
			
#ifdef TARGET_OSX
	// since macs fuck up bad fullscreen with video settings
			ofSetFullscreen(false);
#endif
			IM.vidGrabber.videoSettings();
			panel.setValueB("VIDEO_SETTINGS", false);
		}
	}	
}

void trackingManager::videoSettings(){	
	// TODO: fix this!! [zach]
	//if( !bUseVideoFiles ) ((ofVideoGrabber *)videoSource)->videoSettings();
}

void trackingManager::draw()
{	
	ofSetColor(255,255,255);
	tracker.grayImgPreModification.draw(10,10,160,120);		
	tracker.threshImg.draw(10,130,160,120);	
	tracker.contourFinder.draw(10,130,160,120);	
	tracker.grayImg.draw(180, 10, 320,240);	
	tracker.eyeTrackedEllipse.draw( 180,10,320, 240,IM.width,IM.height);

	panel.draw();
	btt.draw();
}
void trackingManager::drawInput(int xt, int yt, int wt, int ht){
	tracker.grayImg.draw(xt, yt, wt, ht);
}

//--------------------------------------------------------------
void trackingManager::mouseDragged(int x, int y, int button){	
	panel.mouseDragged(x, y, button);	
	if(y_seed>0)
	{		
		panel.setValueI("EDGE_MASK_INNER_RADIUS",max(0,original_mask_size+y-y_seed));
		panel.setValueI("EDGE_MASK_OUTER_RADIUS",(1+0.1*max(0,(x-x_seed)))*(original_mask_size+y-y_seed));
	}
}
//--------------------------------------------------------------
void trackingManager::mousePressed(int x, int y, int button){	
	panel.mousePressed(x, y, button);
	if(x>180 && x< 500 && y> 10 && y<250)
	{
		panel.setValueF("EDGE_MASK_X", scale*2.*(x-180));
		panel.setValueF("EDGE_MASK_Y", scale*2.*(y-10));
		x_seed=x;
		y_seed=y;
		original_mask_size=panel.getValueI("EDGE_MASK_INNER_RADIUS");
	}
	else
	{
		x_seed=-1;
		y_seed=-1;
	}
}
//--------------------------------------------------------------
void trackingManager::mouseReleased(int x, int y, int button)
{	
	panel.mouseReleased();	
	if(btt.inRect(x,y)) app->changeMode();
}
