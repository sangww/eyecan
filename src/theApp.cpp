#define _WIN32_WINNT 0x0500

#include "theApp.h"
#include "stdio.h"

#include <Windows.h>
#include <iostream>
//#include <CommDlg.h>

#include "winHandler.h"
#include "WinInterceptor.h"
#include "ofTrueTypeFontWS.h"
#include "strconv.h"

using namespace std;


//--------------------------------------------------------------
theApp::theApp(){
}                                                  

//--------------------------------------------------------------
void theApp::setup()
{	 
	loadXMLSettings();
	
	ofSetFrameRate((float)framerate);	
	ofSetVerticalSync(true); 
	screenWidth    = ::GetSystemMetrics( SM_CXSCREEN )-1; 
	screenHeight  = ::GetSystemMetrics( SM_CYSCREEN )-1;
	if(isOpaque){
		::SetWindowLong(m_hWnd, GWL_EXSTYLE, ::GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);  
 		SetLayeredWindowAttributes(m_hWnd, 0, 200, LWA_ALPHA);
	}
	
	//load resources
	fontWS.loadFont("fonts/NanumGothic.otf",24);

	//set some boolean states
	showConsole=false;
	if(showConsole)	ShowWindow(m_hCon,SW_SHOW);
	else			ShowWindow(m_hCon,SW_HIDE);
	simulateMouse=false;
	toggleKeyPressed = false;
	showPerf=true;
	testing=false;
	
	//set modes
	mode = MODE_TRACKING;
	TM.setup();
	CM.setup();
	AD.setup(framerate);
	
	//user controller registration 
	hMC = MC.setup(&AD);
	hPC = PC.setup(&AD);
	hTC = TC.setup(&AD);
	PC.setInputFile("scripts/default.xml");
	AD.changeCallback(hMC);
	AD.setDefaultCallback(hMC);
	
	//init eye tracker
	eyeSmoothed.set(0,0,0);	
	CM.fitter.loadCalibration();
	CM.caculateScore();

	//hook key event
	g_hKeyHook = SetWindowsHookEx( WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0 );		
	
	//if should automatically start mouse use
	if(autoStart) AD.toggleEventDispatch();
}

void theApp::loadXMLSettings()
{
	//load settings
	ofxXmlSettings XML;
	XML.loadFile("settings/settings.xml");

	framerate = XML.getValue("program:framerate", 60);
	isOpaque = XML.getValue("program:opaqueWindow", false);
	autoStart = XML.getValue("program:autoStart", false);

	bgr=XML.getValue("program:bgColor_r",100);
	bgg=XML.getValue("program:bgColor_g",100);
	bgb=XML.getValue("program:bgColor_b",100);

	locale = XML.getValue("program:locale","kor");
	if(locale.length()>0) locale="_"+locale;

	//aui
	sp_alarm.loadSound(XML.getValue("program:alarmTone","audio/alarm.wav"));
}
void theApp::reloadAllXMLSettings()
{
	cout<<"[RELOAD ALL SETTINGS]"<<endl;
	loadXMLSettings();
	AD.loadXMLSettings();
	MC.loadXMLSettings();
}

void theApp::playAlarmSound(DWORD c)
{
	sp_alarm.stop();
	sp_alarm.play();
}
void theApp::resetSound()
{
	for(int i=0;i<sp_list.size();i++)
	{
		delete sp_list[i];
	}
	sp_list.clear();
}
void theApp::addSound(string s)
{
	cout<<"add - "<<s<<endl;
	ofSoundPlayer* sp = new ofSoundPlayer();
	sp->loadSound(s);
	sp_list.push_back(sp);
}
void theApp::playSound(DWORD c)
{
	cout<<"Play - "<<c<<endl;
	sp_list[c]->stop();
	sp_list[c]->play();
}




//--------------------------------------------------------------
void theApp::update()
{	
	//update
	ofBackground(bgr,bgg,bgb);	
	TM.update();
	CM.update();

	//intercept global toggle key (f12)
	//cout<<_key_code<<" "<<_key_state<<"--"<<endl;
	if(_key_code ==119)
	{
		if(toggleKeyPressed && _key_state==257)
		{
			toggleKeyPressed = false;
			AD.toggleEventDispatch();
		}
		else if((!toggleKeyPressed) && _key_state==256)
		{
			toggleKeyPressed = true;
		}
	}

	// record some points if we are in auto mode
	if (CM.bAutomatic == true && CM.bAmInAutodrive == true && CM.bInAutoRecording)
	{		
		if (TM.bGotAnEyeThisFrame()){	
			ofPoint trackedEye = TM.getEyePoint();
			CM.fitter.registerCalibrationInput(trackedEye.x,trackedEye.y);
			CM.inputEnergy = 1;
		}		
	}		
	
	if(simulateMouse) //eye simulation
	{	
		//printf("winmouse: %d, x:%d, y:%d\n", _mouse_code, _mouse_x, _mouse_y);
		bool eyeFound = true;
		if(_mouse_code==512)
		{
			eyeFound = true; //need to be changed but ok now
		}
		else if(_mouse_code==519)
		{
			eyeFound = false;
		}
		AD.Move (_mouse_x, _mouse_y, !eyeFound);	
		AD.update();
	}
	else if (CM.fitter.bBeenFit || CM.bPreAutomatic){ //real tracking
		if(TM.bFoundEye)
		{
			ofPoint trackedEye = TM.getEyePoint();
			ofPoint screenPoint = CM.fitter.getCalibratedPoint(trackedEye.x, trackedEye.y);
			eyeSmoothed.x = CM.smoothing * eyeSmoothed.x + (1-CM.smoothing) * screenPoint.x;
			eyeSmoothed.y = CM.smoothing * eyeSmoothed.y + (1-CM.smoothing) * screenPoint.y;
		}
		AD.Move (eyeSmoothed.x, eyeSmoothed.y, !TM.bFoundEye);
		AD.update();
	}
}


//--------------------------------------------------------------
void theApp::draw()
{	
	ofSetColor(255, 255, 255);	
		
	//draw framerate
	if(showPerf)
		ofDrawBitmapString("FrameRate: " + ofToString(ofGetFrameRate(), 5), 10, ofGetHeight() - 5);

	if(testing) return;

	switch(mode){ 
		case MODE_TRACKING:
			TM.draw();
			break;
		case MODE_CALIBRATING:
			CM.draw();
			break;
		case MODE_MOUSE://change to AD.draw?
			AD.draw();
			break;
		case MODE_PROGRAMMABLE:
			AD.draw();
			break;
	}
	
	// draw a green dot to see how good the tracking is:
	if (CM.fitter.bBeenFit){ 
		ofSetColor(0,255,0,120);
		ofFill();
		ofCircle(eyeSmoothed.x-ofGetWindowPositionX(), eyeSmoothed.y-ofGetWindowPositionY(), 20);
	}	

	// draw a assist screen
	if (mode == MODE_MOUSE) {
		ofSetColor(255, 255, 255, 120);
		TM.drawInput(5 , 5, 100, 75);	
	}	
	else if (mode == MODE_PROGRAMMABLE) {
		ofSetColor(255, 255, 255, 120);
		TM.drawInput(5 , 5, 72, 54);	
	}	
	else if (mode == MODE_CALIBRATING) {
		if (CM.fitter.bBeenFit)
		{
			ofSetColor(255, 255, 255, 120);
			TM.drawInput(0, 0, TM.IM.width, TM.IM.height);	
		}
	}
}

void theApp::changeMode(int m)
{
	if(testing) return;
	mode = m;
	mode %= NUM; // number of modes;
	switch(mode)
	{
	case MODE_TRACKING:
		AD.changeCallback(hMC);
		ofSetWindowPosition(0, 0);
		ofSetWindowShape(780, 500);
		break;
	case MODE_CALIBRATING:
		AD.changeCallback(hMC);
		ofSetWindowPosition(0, 0);
		ofSetWindowShape(screenWidth, screenHeight);
		break;
	case MODE_MOUSE:
		AD.changeCallback(hMC);
		MC.setupWindow();
		break;
	case MODE_PROGRAMMABLE:		
		AD.changeCallback(hPC);
		PC.setupWindow();
		break;
	}
}
//--------------------------------------------------------------
void theApp::keyPressed(int key){	
	switch (key)
	{			
		case	OF_KEY_RETURN:
			changeMode(mode+1);
			break;	 
		case	'1':
			changeMode(MODE_TRACKING);
			break;	 
		case	'2':
			changeMode(MODE_CALIBRATING);
			break;	 
		case	'3':
			changeMode(MODE_MOUSE);
			break;	 
		case	'4':
			changeMode(MODE_PROGRAMMABLE);
			break;	 
		case 'i':
		case 'I':
			if(simulateMouse)  UnhookWindowsHookEx(g_hMouseHook);
			else g_hMouseHook = SetWindowsHookEx( WH_MOUSE_LL, LowLevelMouseProc, GetModuleHandle(NULL), 0 );
			simulateMouse = !simulateMouse;
			break;
		case 'r':
		case 'R':
			AD.resetOffset(); //TODO: when we do calibration
			break;
		case 'o':
		case 'O':    
			/*
			TCHAR _curDirectoryPath[256];
			GetCurrentDirectoryA(256, _curDirectoryPath);		
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = 0;
			if(GetOpenFileName(&ofn)) {
				cout<<szFileName;
				PC.setInputFile(szFileName);
			}
			SetCurrentDirectoryA(_curDirectoryPath);
			break;
			*/
		case 'c':
		case 'C':
			showConsole=!showConsole;
			if(showConsole)
				ShowWindow(m_hCon,SW_SHOW);
			else
				ShowWindow(m_hCon,SW_HIDE);			
			break;
		case 'f':
		case 'F':
			showPerf=!showPerf;
			break;
		case 't':
		case 'T':
			testing=!testing;
			if(testing) TC.setupWindow();
			changeMode(mode);
			break;
	}	

	//send to some controllers
	if (mode == MODE_CALIBRATING){
		CM.keyPressed(key);
	}
	else if (mode == MODE_MOUSE){
		MC.keyPressed(key);
	}
}

//--------------------------------------------------------------
void theApp::keyReleased(int key){
}
void theApp::mouseMoved(int x, int y ){	
}
void theApp::mouseDragged(int x, int y, int button)
{	
	if (mode == MODE_MOUSE)				MC.mouseDragged(x, y, button);
	if (mode == MODE_PROGRAMMABLE)		PC.mouseDragged(x, y, button);
	if (mode == MODE_TRACKING)			TM.mouseDragged(x, y, button);
	if (mode == MODE_CALIBRATING)		CM.mouseDragged(x, y, button);
}
void theApp::mousePressed(int x, int y, int button)
{	
	if (mode == MODE_MOUSE)				MC.mousePressed(x, y, button);
	if (mode == MODE_PROGRAMMABLE)		PC.mousePressed(x, y, button);
	if (mode == MODE_TRACKING)			TM.mousePressed(x, y, button);
	if (mode == MODE_CALIBRATING)		CM.mousePressed(x, y, button);
}
void theApp::mouseReleased(int x, int y, int button)
{	
	if (mode == MODE_MOUSE)				MC.mouseReleased(x,y,button);
	if (mode == MODE_PROGRAMMABLE)		PC.mouseReleased(x,y,button);
	if (mode == MODE_TRACKING)			TM.mouseReleased(x,y,button);
	if (mode == MODE_CALIBRATING)		CM.mouseReleased(x,y,button);
}
void theApp::resized(int w, int h){
}


