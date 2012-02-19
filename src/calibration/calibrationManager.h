#ifndef _CALIBMANGER
#define _CALIBMANGER


#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxControlPanel.h"
#include "ofxLeastSquares.h"
#include "calibrationFileSaver.h"
#include "pointFitter.h"
#include "ofTrueTypeFontWS.h"
#include "buttonPanel.h"

class calibrationManager {

	public:		
		void setup();
		void update();
		void draw();
	
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void keyPressed(int key);
	
		ofRectangle calibrationRectangle;
	
		void clear();
	
		double		cxfit[6];
		double		cyfit[6];	
		int			nDivisionsWidth;
		int			nDivisionsHeight;
		int			nPosition;
	
		float		xp, yp;
		int			pos;
	
		float		inputEnergy;
	
		bool		bPreAutomatic;	
		bool		bAutomatic;
		void		start();
		void		stop();		
	
		bool		bAmInAutodrive;
		float		startTime;
		float		preTimePerDot;
		float		postTimePerDot;
		float		recordTimePerDot;
		float		totalTimePerDot;
		bool		bInAutoRecording;
		bool		bReleasingAutoRecording;
		bool		bRemovePointsFarFromAverage;	
		float		autoPct;
		int			pt;
	
		ofImage		calibrationInfo;
		ofTrueTypeFontWS			fontWS;

		float	smoothing;
		float	menuEnergy;
	
		ofxControlPanel			panel;
		calibrationFileSaver calibFileSaver;
		pointFitter		fitter;

		int screenWidth, screenHeight;	
	
		int score;
		void caculateScore();

		//button
		buttonPanel bp;

		//wstring
		wstring calib_need_calibration, calib_no_record, calib_score_is, calib_score_is_end;
};

#endif
