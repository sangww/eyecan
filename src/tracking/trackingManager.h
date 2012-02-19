#ifndef _TRACKSCENE_H
#define _TRACKSCENE_H


#include "ofMain.h"
#include "eyeTracker.h"
#include "ofxControlPanel.h"
#include "inputManager.h"
#include "buttonPanel.h"

class trackingManager {

	public:
		trackingManager();
		~trackingManager();
		
		void setup();
		void update();
		void draw();
		void drawInput(int xt, int yt, int wt, int ht);		
		
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		
		void setupGui();
		void updateGui();
	
		void trackEyes();
		
		// open video settings panel if using a camera
		void videoSettings();

		
		// returns the tracked pupil centroid
		ofPoint	getEyePoint();
		bool	bGotAnEyeThisFrame();
	
		//----- video sources... what are we tracking!	
		inputManager		IM;		
		ofxControlPanel		panel;
		eyeTracker			tracker;
		
		int					minBlob, maxBlob;
		float				threshold;
		bool				bFoundEye;
		ofPoint				eyePoint;

		//sang
		int x_seed,y_seed, original_mask_size;
		double scale;

		//button
		buttonToggle btt;
};

#endif
