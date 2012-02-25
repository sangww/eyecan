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



#include "ProgrammableUI.h"
#include "appDelegate.h"
#include "strconv.h"


ProgrammableUI::ProgrammableUI(){
}
ProgrammableUI::~ProgrammableUI(){	
}

void ProgrammableUI::setup()
{	
	posX=posY=0;
	x_drag_start=-1; y_drag_start=-1;
	mode =0;
	
	fScreenWidth    = ::GetSystemMetrics( SM_CXSCREEN )-1; 
	fScreenHeight  = ::GetSystemMetrics( SM_CYSCREEN )-1; 
	
	//gui init
	fontWS.loadFont("fonts/NanumGothic.otf",14);
	helpWS.loadFont("fonts/NanumGothic.otf",11);
	
	bp.addButton("Back",7,64,70,70,"images/home"+app->locale+".png");
	bp.addButton("UP",80,64,70,70,"images/arrow_up"+app->locale+".png");
	bp.addButton("DOWN",150,64,70,70,"images/arrow_down"+app->locale+".png");
	bp.addButton("START",220,64,70,70,"images/play"+app->locale+".png");
	bp.addState(3,"STOP","images/stop"+app->locale+".png");
}
void ProgrammableUI::setupWindow()
{
	ofSetWindowPosition(posX, posY);
	ofSetWindowShape(300, 140);
}
void ProgrammableUI::setupWindowPosition(int x, int y)
{
	posX=x;
	posY=y;
}
void ProgrammableUI::setMode(int m)
{
	mode=m;
	switch(mode)
	{
	case 0:
		ofSetWindowPosition(posX, posY);
		ofSetWindowShape(300, 140);
		break;
	case 1:
		ofSetWindowPosition(fScreenWidth/2-100, fScreenHeight/2-100);
		ofSetWindowShape(200, 200);
		break;
	}
}

void ProgrammableUI::update(int x, int y)
{
	bp.updateFocus(x,y);
}


void ProgrammableUI::draw(wstring state, int x, int y)
{	
	posX=x;
	posY=y;
	ofSetWindowPosition(posX, posY);
	ofSetColor(255,255,255);
	if(mode==0){
		fontWS.drawString(state, 80,30);
	}
	else{
	}
	bp.draw();
}
void ProgrammableUI::draw(wstring state, wstring help)
{	
	ofSetColor(255,255,255);
	if(mode==0){
		fontWS.drawString(state, 80,30);
		helpWS.drawString(help, 80,50);
	}
	else{
	}
	bp.draw();
}
void ProgrammableUI::draw(string state,string help)
{	
	ofSetColor(255,255,255);
	if(mode==0){
		fontWS.drawString(strconv(state), 80,30);
		helpWS.drawString(strconv(help), 80,50);
	}
	else{
	}
	bp.draw();
}


//--------------------------------------------------------------
void ProgrammableUI::mouseDragged(int x, int y, int button){	
}
//--------------------------------------------------------------
void ProgrammableUI::mousePressed(int x, int y, int button){
	x_drag_start=x; y_drag_start=y;
}
//--------------------------------------------------------------
void ProgrammableUI::mouseReleased(int x, int y, int button){	
	if(x_drag_start>=0 && y_drag_start>=0)
	{
		posX+=x-x_drag_start; 
		posY+=y-y_drag_start;
		ofSetWindowPosition(posX, posY);
		x_drag_start=-1; y_drag_start=-1;
	}
}






