/*
    eyeCan: a program that enables people to use PC with their eyes
    Copyright (C) 2012  Sang-won Leigh

    eyeCan is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    eyeCan is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#define _WIN32_WINNT 0x0500

#include "theApp.h"
#include "stdio.h"

#include <Windows.h>
#include <iostream>

using namespace std;


//--------------------------------------------------------------
theApp::theApp(){
}                                                  

//--------------------------------------------------------------
void theApp::setup()
{	 
	panel.setup("eyeCan config", 10, 10, 360, 460);
	
	//panel setting
	panel.addPanel("Program", 1, false);
	panel.addPanel("Mouse", 1, false);
	panel.addPanel("Decoder", 1, false);
	panel.addPanel("Input", 1, false);

	//#1
	panel.setWhichPanel("Program");
	panel.addSlider("Frame Rate", "program:framerate", 60, 20, 60, true);
	panel.addToggle("Transparent Window", "program:opaqueWindow", false);
	panel.addToggle("Load External Programs", "program:loadExternal", false);
	panel.addSlider("Background Color (R)", "program:bgColor_r", 100, 0, 255, true);
	panel.addSlider("Background Color (G)", "program:bgColor_g", 125, 0, 255, true);
	panel.addSlider("Background Color (B)", "program:bgColor_b", 160, 0, 255, true);
	//TODO: alarmTone
	
	//#2
	panel.setWhichPanel("Mouse");
	panel.addSlider("Click Helper Box Size", "mouse:clickhelperBox", 120, 40, 200, true);
	panel.addToggle("Use Click Helper", "mouse:useClickHelper", false);
	panel.addSlider("Error Compensation Box Size", "mouse:compensationBox", 80, 20, 200, true);
	panel.addToggle("Use Error Compensation", "mouse:useCompensation", false);
	
	//#3
	panel.setWhichPanel("Decoder");
	panel.addSlider("Short Click Time (s)", "decoder:shortClickTime", 0.3, 0.05, 1.0, false);
	panel.addSlider("Click Time (s)", "decoder:clickTime", 0.5, 0.05, 1.0, false);
	panel.addSlider("Long Click Time (s)", "decoder:longClickTime", 1.5, 0.5, 3.0, false);
	panel.addSlider("Dwell Time (s)", "decoder:dwellTime", 3.0, 0.5, 6.0, false);
	panel.addSlider("Stay Boundary Size", "decoder:stayMotionRange", 50, 10, 200, true);
	
	//#4
	panel.setWhichPanel("Input");
	panel.addToggle("Input Mode (0:live video, 1:video file)", "input:mode", false);
	panel.addSlider("Input Device", "input:videoGrabber:deviceId", 1, 0, 5, true);
	panel.addSlider("Input Width", "input:videoGrabber:width", 640, 160, 640, true);
	panel.addSlider("Input Height", "input:videoGrabber:height", 480, 120, 480, true);

	//load xml
	panel.loadSettings("settings/Settings.xml");	
}




//--------------------------------------------------------------
void theApp::update()
{	
	panel.update();
	ofBackground(panel.getValueI("program:bgColor_r"),panel.getValueI("program:bgColor_g"),panel.getValueI("program:bgColor_b"));	
}


//--------------------------------------------------------------
void theApp::draw()
{	
	panel.draw();
}

//--------------------------------------------------------------
void theApp::keyPressed(int key){	
}
void theApp::keyReleased(int key){
}
void theApp::mouseMoved(int x, int y ){	
}
void theApp::mouseDragged(int x, int y, int button){
	panel.mouseDragged(x, y, button);	
}
void theApp::mousePressed(int x, int y, int button){	
	panel.mousePressed(x, y, button);
}
void theApp::mouseReleased(int x, int y, int button){	
	panel.mouseReleased();	
}
void theApp::resized(int w, int h){
}


