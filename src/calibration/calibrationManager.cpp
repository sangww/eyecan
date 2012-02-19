#include "calibrationManager.h"
#include "appDelegate.h"
#include "strconv.h"

//--------------------------------------------------------------
void calibrationManager::setup(){
	calibrationInfo.loadImage("images/calibrationInfo"+app->locale+".png");
	fontWS.loadFont("fonts/NanumGothic.otf",32);

	nDivisionsWidth = 4;
	nDivisionsHeight = 4;

	nPosition = 0;
	pos  = 0;

	inputEnergy = 0;

	bAutomatic = false;
	bPreAutomatic = false;
	bAmInAutodrive = false;

	startTime = ofGetElapsedTimef();
	preTimePerDot = 10.0;
	recordTimePerDot = 0.4;
	totalTimePerDot = preTimePerDot + recordTimePerDot;
	bInAutoRecording = false;
	autoPct = 0;

	calibrationRectangle.x = 0;
	calibrationRectangle.y = 0;
	screenWidth    = ::GetSystemMetrics( SM_CXSCREEN )-1; 
	screenHeight  = ::GetSystemMetrics( SM_CYSCREEN )-1; 
	calibrationRectangle.width = screenWidth;
	calibrationRectangle.height = screenHeight;

		
	//---- gaze
	panel.setup("Calibration", 660, 20, 280, 360);
	panel.addPanel("Setting", 1, false);
	panel.setWhichPanel("Mouse Setting");
	panel.addSlider("Smooth Mouse Movement", "AMOUNT_SMOOTHING", 0.97, 0.01, 1.0f, false);
	panel.addSlider("num divisions horiz", "N_DIV_W", nDivisionsWidth, 2, 15, true);
	panel.addSlider("num divisions vert", "N_DIV_H", nDivisionsHeight, 2, 15, true);
	panel.addSlider("(auto) time to pre-record", "PRE_RECORD_TIME", preTimePerDot, 0.1, 5, false);
	panel.addSlider("(auto) time for record", "RECORD_TIME", recordTimePerDot, 0.1, 1.5, false);
	
	panel.loadSettings("settings/calibrationSettings.xml");	
	
	nDivisionsWidth = panel.getValueI("N_DIV_W");
	nDivisionsHeight = panel.getValueI("N_DIV_H");;
	fitter.setup(nDivisionsWidth, nDivisionsHeight);
	
	smoothing = 1.0f;
	menuEnergy = 1;
	bPreAutomatic = false;

	//btt	
	bp.addButton("Back",660, 410,70,70,"images/home"+app->locale+".png");
	bp.addButton("Start",730,410,70,70,"images/play"+app->locale+".png");
	

	//strings
	ofxXmlSettings XML;
	XML.loadFile("strings/strings"+app->locale+".xml");
	calib_need_calibration=strconv(XML.getValue("calib_need_calibration","Need calibration"));
	calib_no_record=strconv(XML.getValue("calib_no_record","No calibration record"));
	calib_score_is=strconv(XML.getValue("calib_score_is","Your score is: "));
	calib_score_is_end=strconv(XML.getValue("calib_score_is_end"," /100"));
}


//--------------------------------------------------------------
void calibrationManager::start(){
	clear();
	fitter.setupDimension(nDivisionsWidth, nDivisionsHeight);
	bAutomatic = true;
	bAmInAutodrive = true;
	startTime = ofGetElapsedTimef();	
	fitter.startCalibration();	
}


//--------------------------------------------------------------
void calibrationManager::stop(){
	bAutomatic = false;
	bPreAutomatic = false;
	bAmInAutodrive = false;
	startTime = ofGetElapsedTimef();
	nPosition = 0;
	pos  = 0;
}



//--------------------------------------------------------------
void calibrationManager::update(){
	calibrationRectangle.x = 0;
	calibrationRectangle.y = 0;
	calibrationRectangle.width = ofGetWidth();
	calibrationRectangle.height = ofGetHeight();

	float widthPad = ofGetWidth() * 0.005f;
	float heightPad = ofGetHeight() * 0.005f;

	calibrationRectangle.x += widthPad;
	calibrationRectangle.y += heightPad;
	calibrationRectangle.width -= widthPad*2;
	calibrationRectangle.height -= heightPad*2;

	panel.update();

	nDivisionsWidth = panel.getValueI("N_DIV_W");
	nDivisionsHeight = panel.getValueI("N_DIV_H");;
	preTimePerDot = panel.getValueF("PRE_RECORD_TIME");;
	recordTimePerDot = panel.getValueF("RECORD_TIME");;
	smoothing = panel.getValueF("AMOUNT_SMOOTHING");
	postTimePerDot = recordTimePerDot;
	totalTimePerDot = preTimePerDot + recordTimePerDot + postTimePerDot;
	
	fitter.removeOutliers();


	if ((bAutomatic == true && bAmInAutodrive == true) || bPreAutomatic){
		menuEnergy = 0.94f * menuEnergy + 0.06f * 0.0f;
	} else {
		menuEnergy = 0.94f * menuEnergy + 0.06f * 1.0f;
	}

	// do the auto:
	if (bAutomatic == true && bAmInAutodrive == true){
		int nPts = nDivisionsWidth * nDivisionsHeight;
		float totalTime = totalTimePerDot * nPts;
		if (ofGetElapsedTimef() - startTime > totalTime){
			bAmInAutodrive = false;
			bInAutoRecording = false;
			bAutomatic = false;
			bPreAutomatic = false;

			fitter.calculate(calibrationRectangle);
			inputEnergy = 0;
			caculateScore();
			if (fitter.bBeenFit){
				fitter.saveCalibration();
			}
		} 
		else 
		{

			float diffTime = ofGetElapsedTimef() - startTime ;
			pt = (int)(diffTime / totalTimePerDot);
			float diffDotTime = diffTime - pt * totalTimePerDot;
			//cout << diffTime << " " << pt <<  " " << diffDotTime << endl;
			if (diffDotTime < preTimePerDot){
				autoPct = (diffDotTime / preTimePerDot);
				bReleasingAutoRecording = false;
				bInAutoRecording = false;
			}
			else if(diffDotTime < preTimePerDot+ recordTimePerDot){
				autoPct = (diffDotTime - preTimePerDot) / recordTimePerDot;
				bReleasingAutoRecording = false;
				bInAutoRecording = true;
			}
			else{
				autoPct = (diffDotTime - preTimePerDot - recordTimePerDot) / postTimePerDot;
				bReleasingAutoRecording = true;
				bInAutoRecording = false;
			}
			pos = pt;
		}
	}

	inputEnergy *= 0.98f;

	int xx = (pos % nDivisionsWidth);
	int yy = (pos / nDivisionsWidth);

	bool bEven = false;
	if (yy % 2 == 0)  bEven = true;
	
	xp = bEven ? calibrationRectangle.x + ((float)calibrationRectangle.width / (float)(nDivisionsWidth-1)) * xx :
				 calibrationRectangle.x + (calibrationRectangle.width - ((float)calibrationRectangle.width / (float)(nDivisionsWidth-1)) * xx);
	yp = calibrationRectangle.y + calibrationRectangle.height - ((float)calibrationRectangle.height / (float)(nDivisionsHeight-1)) * yy;
	fitter.update(pt, xp, yp);		// raw data is comming from testApp directly to fitter.
}


//--------------------------------------------------------------
void calibrationManager::draw(){
	// draw a light grid:
	if (!bPreAutomatic || !bAutomatic) panel.draw();
	
	ofEnableAlphaBlending();
	ofSetColor(70, 70, 70, 255 - 255 *  menuEnergy);
	ofFill();
	ofRect(0,0,ofGetWidth(), ofGetHeight());
	
	ofSetColor(255, 255, 255, 160);
	for (int i = 0; i < nDivisionsWidth; i++){
		float xLine = calibrationRectangle.x + ((float)calibrationRectangle.width / (float)(nDivisionsWidth-1)) * i;
		ofLine(xLine, calibrationRectangle.y,xLine, calibrationRectangle.y + calibrationRectangle.height);
	}
	for (int i = 0; i < nDivisionsHeight; i++){
		float yLine = calibrationRectangle.y + calibrationRectangle.height - ((float)calibrationRectangle.height / (float)(nDivisionsHeight-1)) * i;
		ofLine(calibrationRectangle.x,yLine, calibrationRectangle.x + calibrationRectangle.width,yLine);
	}

	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	if (fitter.bBeenFit != true){
		ofEnableAlphaBlending();

		float rad = 30;
		glLineWidth(4);
		ofSetColor(255, 255, 255, 100);
		ofLine (xp, yp-rad, xp, yp+rad);
		ofLine (xp-rad, yp, xp+rad, yp);
		ofSetColor(255, 255, 255, 255);
		ofFill();

		if (bAutomatic == true && bAmInAutodrive == true && bInAutoRecording){
		} 
		else {
			ofCircle(xp, yp, rad*0.33);
		}

		// TODO: do some animation stuff here:
		if (bAutomatic == true && bAmInAutodrive == true)
		{
			if (bInAutoRecording)
			{
				ofSetColor(255, 0, 0, 200);
				ofCircle(xp, yp, 26);
				ofSetColor(255, 255,255);
				ofCircle(xp, yp, 5);
			} 
			else if(!bReleasingAutoRecording)
			{
				ofSetColor(255, 255, 255, 150);
				ofCircle(xp, yp, 200 - 200* autoPct);
			}
			else{
				ofSetColor(0, 255, 0, 200);
				ofCircle(xp, yp, 26);
				ofSetColor(255, 255,255);
				ofCircle(xp, yp, 5);
			} 
		}

		glLineWidth(1);
		ofFill();
		if (!bAutomatic){
			if (inputEnergy > 0)
			{
				ofSetColor(255, 0, 127, 60*inputEnergy);
				ofCircle(xp, yp, rad * 3.5 );

				ofSetColor(255, 0, 127, 150*inputEnergy);
				ofCircle(xp, yp, rad );
			}
		}
	}
	//-----------------------------------------------------------------------
	
	// draw calibration score
	if (fitter.bBeenFit)
	{			
		for(int i=0; i<fitter.eyePoints.size();i++)
		{
			if(i%3==0)	ofSetColor(0,255,0,120);
			else if(i%3==1) ofSetColor(0,0,255,120);
			else ofSetColor(255,0,0,120);
			ofCircle(fitter.eyePoints[i].x, fitter.eyePoints[i].y, 4);
		}
	}
	//-----------------------------------------------------------------------
	if (!bAutomatic && !bPreAutomatic){
		ofSetColor(255, 255, 255, 0);
		bp.draw();
		if (!fitter.bBeenFit)	{
			ofSetColor(255, 0, 0, 255);
			fontWS.drawString(calib_need_calibration, 350,530);
		} else {
			ofSetColor(0, 0, 0, 255);
			if(fitter.eyePoints.size()==0){
				fontWS.drawString(calib_no_record,350,530);
			}
			else{
				fontWS.drawString(calib_score_is+strconv(ofToString(score))+calib_score_is_end,350,530);
			}
		}
	}

	ofSetColor(255, 255, 255);
	if (bPreAutomatic){
		calibrationInfo.draw(100,380);
	}
	fitter.draw();
}

void calibrationManager::clear(){
	pos = 0;
	fitter.clear();
}

//--------------------------------------------------------------

void calibrationManager::caculateScore(){
	if( fitter.eyePoints.size() >0 )
	{				
		cout<<"[CALIBRATION SCORE]"<<endl;
		int i=0;
		ofPoint sp, cp, pp;
		vector<ofPoint> vp;
		vector<double> scores;
		score = 0;
		double sum=0.;
				
		while(i<fitter.eyePoints.size())
		{
			cp = fitter.screenPoints[i];
			if(i==0 || (sp.x==cp.x && sp.y==cp.y))
			{
				vp.push_back(fitter.eyePoints[i]);
			}
			else
			{
				sp=cp;
				double mx =0., my=0., vx= 0., vy=0.;
				for(int j=0;j<vp.size();j++)
				{
					mx+=vp[j].x;
					my+=vp[j].x;
				}
				mx=mx/(double)vp.size();
				my=my/(double)vp.size();
				for(int j=0;j<vp.size();j++)
				{
					vx+=(vp[j].x-mx)*(vp[j].x-mx);
					vy+=(vp[j].x-my)*(vp[j].x-my);
				}
				vx=vx/(double)vp.size();
				vy=vy/(double)vp.size();					

				scores.push_back(sqrt(vx+vy));
				sum+=sqrt(vx+vy);

				cout<<"var of pp:  "<<pp.x<<" "<<pp.y<<" = "<<vx<<" "<<vy<<" std_dev: "<<sqrt(vx+vy)<<endl;
				vp.clear();
			}
			pp = fitter.screenPoints[i];
			i++;
		}
		score=(100-0.6*sum);
		if(score<0) score=0;
	}
}

//--------------------------------------------------------------
void calibrationManager::mouseDragged(int x, int y, int button){
	panel.mouseDragged(x,y,button);
}

//--------------------------------------------------------------
void calibrationManager::mousePressed(int x, int y, int button){
	panel.mousePressed(x,y,button);
}

//--------------------------------------------------------------
void calibrationManager::mouseReleased(int x, int y, int button){
	panel.mouseReleased();
	if (bPreAutomatic == true){
		bPreAutomatic = false;
		start();
	}

	switch(bp.inRect(x,y))
	{
	case 0:
		app->changeMode();
		break;
	case 1:
		if (bPreAutomatic == false && !bAmInAutodrive) { 
			bPreAutomatic = true;
		}
		break;
	}
}

//--------------------------------------------------------------
void calibrationManager::keyPressed(int key){
	if (key == 'x'){
		clear();
		stop();
	}
}


