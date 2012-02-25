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



#include "WinAction.h"
#include "MouseUI.h"
#include "appDelegate.h"

#include <dinput.h>

MouseUI::MouseUI(){
}
MouseUI::~MouseUI(){	
}

void MouseUI::setup()
{	
	posX=posY=0;
	x_drag_start=-1; y_drag_start=-1;

	//mode init
	mode=1;
	hold=false;

	//gui init
	fontWS.loadFont("fonts/NanumGothic.otf",18);
	helpWS.loadFont("fonts/NanumGothic.otf",12);	

	bp_main.addButton("Lock", 270,85,70,70,"images/unlocked"+app->locale+".png");
	bp_main.addState(0,"Unlock", "images/locked"+app->locale+".png");
	bp_main.addButton("Next(0)", 340,85,70,70,"images/setting_mouse"+app->locale+".png");
	bp_main.addState(1,"Next(1)", "images/setting_mode"+app->locale+".png");
	bp_main.addState(1,"Next(2)", "images/setting_hotkey"+app->locale+".png");
	bp_main.addButton("Home", 410,85,70,70,"images/home"+app->locale+".png");


	for(int i=0;i<3;i++)
	{
		buttonPanel p;
		if(i==0) p.useTriState =true;
		bp.push_back(p);
	}
	//panel 0
	bp[0].addButton("",110,5,75,75,"images/mouse_click"+app->locale+".png");
	bp[0].addButton("",186,5,75,75,"images/mouse_2_click"+app->locale+".png");
	bp[0].addButton("",262,5,75,75,"images/mouse_R_click"+app->locale+".png");
	bp[0].addButton("",338,5,75,75,"images/mouse_drag"+app->locale+".png");
	bp[0].addButton("",414,5,75,75,"images/mouse_scroll"+app->locale+".png");
	bp[0].set(0,1);	
	//panel 1
	bp[1].addButton("Controller", 110,5,120,75,"images/pctrl"+app->locale+".png");
	bp[1].addButton("Config", 231,5,120,75,"images/camera_config"+app->locale+".png");
	bp[1].addButton("Calibration", 352,5,120,75,"images/calibration"+app->locale+".png");
	//panel 2
	bp[2].addButton("HK 1",110,5,75,75,"");
	bp[2].addButton("HK 2",186,5,75,75,"");
	bp[2].addButton("HK 3",262,5,75,75,"");
	bp[2].addButton("HK 4",338,5,75,75,"");
	bp[2].addButton("Reload\nSetting",414,5,75,75,"images/reload"+app->locale+".png");
}
void MouseUI::update(int x, int y)
{
	updateButtons(x,y);
}
void MouseUI::updateButtons(int x, int y)
{	
	if(bp_main.getMode(1) < bp.size())
	{
		bp[bp_main.getMode(1)].updateFocus(x,y);
	}
	bp_main.updateFocus(x,y);
}
void MouseUI::setupWindow()
{
	ofSetWindowPosition(posX, posY);
	ofSetWindowShape(490, 160);
}
void MouseUI::setupWindowPosition(int x, int y)
{
	posX=x;
	posY=y;
}



//to draw transparent rectangle
void rect (int left,int top,int right,int bottom,int bWidth,COLORREF bColor)
{
	HDC hdc = GetDC(NULL);
	HPEN hpen = CreatePen (0, bWidth, bColor);
	HPEN hpenOld = (HPEN)SelectObject (hdc, hpen);
	HBRUSH hbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hbrushOld = (HBRUSH)SelectObject (hdc, hbrush);
	::Rectangle (hdc, left, top, right, bottom);
	SelectObject (hdc, hbrushOld);
	SelectObject (hdc, hpenOld);
	DeleteObject (hbrush);
	DeleteObject (hpen);
	ReleaseDC (NULL, hdc);
}
void MouseUI::draw(wstring state, wstring help, bool originalPosition)
{	
	if(originalPosition) ofSetWindowPosition(posX, posY);
	ofSetColor(255,255,255);
	fontWS.drawString(state, 0,100);
	helpWS.drawString(help, 0,125);
	drawButtons();
}
void MouseUI::draw(wstring state, int x, int y, int size)
{	
	ofSetWindowPosition(x-size, y+size);
	ofSetColor(255,255,255);
	fontWS.drawString(state, 170,20);
}
void MouseUI::drawRect(int x, int y, int size){
	RECT rcScreen;
	GetClientRect (GetDesktopWindow(), &rcScreen);
	rect (x-size, y-size, x+size, y+size, 5, RGB(255,0,0));
}
void MouseUI::invalidate(){
	InvalidateRect (NULL, NULL, TRUE);
}
void MouseUI::drawButtons()
{
	if(bp_main.getMode(1) < bp.size())
	{
		bp[bp_main.getMode(1)].draw();
	}
	bp_main.draw();
}


//--------------------------------------------------------------
void MouseUI::mouseDragged(int x, int y, int button){
	
}
//--------------------------------------------------------------
void MouseUI::mousePressed(int x, int y, int button){
	x_drag_start=x; y_drag_start=y;
}
//--------------------------------------------------------------
void MouseUI::mouseReleased(int x, int y, int button){	
	if(x_drag_start>=0 && y_drag_start>=0)
	{
		posX+=x-x_drag_start; 
		posY+=y-y_drag_start;
		ofSetWindowPosition(posX, posY);
		x_drag_start=-1; y_drag_start=-1;
	}
}




//gui interaction
bool MouseUI::isMouseOver(int x, int y)
{
	if (x>=posX && x<=posX+ofGetWidth() && y>=posY && y<=posY+ofGetHeight())
		return true;
	return false;
}
bool MouseUI::isMouseOverButton(int x, int y)
{	
	if(bp_main.getMode(1) < bp.size())
	{
		if(bp[bp_main.getMode(1)].inRect(x,y)>-1) return true;
	}
	if(bp_main.inRect(x,y)>-1) return true;
	return false;
}


//Mouse Click Mode Control
void MouseUI::eyeMouseClicked(int x, int y )
{
	if(bp_main.getMode(1)==0)
	{
		for(int i=0;i<bp[0].num;i++)
		{
			if(bp[0].update(i,x,y))
			{
				if(i==0)
				{
					resetMouseButtons(0);
					resetMouseMode();
				}
				else
				{
					if(bp[0].triStateButtons[i].mode==2)
					{
						hold=true;
					}
					else if(bp[0].triStateButtons[i].mode==1)
					{
						mode=i;
						resetMouseButtons(i);
					}
					else{
						resetMouseMode();
					}
				}
			}
		}
	}
	else if(bp_main.getMode(1) == 1)
	{
		switch(bp[1].inRect(x,y))
		{
		case 0:
			app->changeMode(3);
			break;
		case 1:
			app->changeMode(0);
			break;
		case 2:
			app->changeMode(1);
			break;
		}
	}
	else if(bp_main.getMode(1) == 2)
	{
		switch(bp[2].inRect(x,y))
		{
		case 0:
			f_list[0]->call();
			break;
		case 1:
			f_list[1]->call();
			break;
		case 2:
			f_list[2]->call();
			break;
		case 3:
			f_list[3]->call();
			break;
		case 4:
			app->reloadAllXMLSettings();
			break;
		}
	}
	switch(bp_main.update(x,y))
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		bp_main.reset(1);
		break;
	}
}
void MouseUI::eyeMouseActionDone()
{
	if(!hold)
	{
		resetMouseMode();
	}
}
void MouseUI::resetMouseButtons(int except)
{
	hold=false;
	for(int i=0;i<bp[0].num;i++)
	{
		if(i!=except) bp[0].reset(i);
	}
}
void MouseUI::resetMouseMode()
{
	mode=0;
	resetMouseButtons(0);
	bp[0].set(0,1);
}



//hot key
void MouseUI::interpretScript()
{
	ofxXmlSettings XML;
	XML.loadFile("settings/hotkeySettings.xml"); //need script intepreter 
	cout<<" [LOADING HOTKEY FILE] "<<endl;
	     
	for(int i=0;i<4;i++)
	{
		f_list[i]->reset(); //reset

		string tag="hotkey"+ofToString(i);
		if(XML.tagExists(tag))
		{
			string s = XML.getValue(tag, "none");
			cout<<tag<< " mapped to: "<<s<<endl;
			bp[2].buttons[i].state_title[0]="";

			//generic mapping
			vector<string> vs = split(s, '+'); //vector of string
			for(int j=0;j<vs.size();j++)
			{
				bp[2].buttons[i].state_title[0]+=vs[j]+"\n";
				WORD w=getKeyInputWORD(vs[j]);

				//get function ptr and set action
				if(w!=0x00)
				{
					f_list[i]->attach(winaction::sendSKeyDown,w,true,false);
				}
				else
				{
					f_list[i]->attach(winaction::sendSKeyDown,0,true,false);
				}
			}
			for(int j=vs.size()-1;j>=0;j--)
			{
				WORD w=getKeyInputWORD(vs[j]);

				//get function ptr and set action
				if(w!=0x00)
				{
					if(j==0)	f_list[i]->attach(winaction::sendSKeyUp,w,false,false);
					else	f_list[i]->attach(winaction::sendSKeyUp,w,true,false);
				}
				else
				{
					if(j==0)	f_list[i]->attach(winaction::sendSKeyUp,0,false,false);
					else	f_list[i]->attach(winaction::sendSKeyUp,0,true,false);
				}
			}
		}
	}
}
WORD MouseUI::getKeyInputWORD(string s)
{
	if(s.length()==1)
	{
		UINT  virtualKey = VkKeyScan( s.at(0) );
		DWORD scanCode   = MapVirtualKey( virtualKey, 0 );
		return scanCode;
	}
	else if(s=="enter")
		return DIK_RETURN;
	else if(s=="space")
		return DIK_SPACE;
	else if(s=="left")
		return DIK_LEFTARROW;
	else if(s=="right")
		return DIK_RIGHTARROW;
	else if(s=="up")
		return DIK_UPARROW;
	else if(s=="down")
		return DIK_DOWNARROW;
	else if(s=="pageup")
		return DIK_PGUP;
	else if(s=="pagedown")
		return DIK_PGDN;
	else if(s=="insert")
		return DIK_INSERT;
	else if(s=="delete")
		return DIK_DELETE;
	else if(s=="pause")
		return DIK_PAUSE;
	else if(s=="scrolllock")
		return DIK_SCROLL;
	else if(s=="backspace")
		return DIK_BACKSPACE;
	else if(s=="leftctrl" || s=="ctrl")
		return DIK_LCONTROL;
	else if(s=="rightctrl")
		return DIK_RCONTROL;
	else if(s=="leftalt" || s=="alt")
		return DIK_LALT;
	else if(s=="rightalt")
		return DIK_RALT;
	else if(s=="tab")
		return DIK_TAB;
	else if(s=="capslock")
		return DIK_CAPSLOCK;
	else if(s=="leftshift" || s=="shift")
		return DIK_LSHIFT;
	else if(s=="rightshift")
		return DIK_RSHIFT;
	else if(s=="leftwin" || s=="win")
		return DIK_LWIN;
	else if(s=="rightwin")
		return DIK_RWIN;
	else if(s=="leftmenu")
		return DIK_LMENU;
	else if(s=="rightmenu" || s=="menu")
		return DIK_RMENU;
	else if(s=="esc")
		return DIK_ESCAPE;
	else if(s=="F1")
		return DIK_F3;
	else if(s=="F2")
		return DIK_F2;
	else if(s=="F3")
		return DIK_F3;
	else if(s=="F4")
		return DIK_F4;
	else if(s=="F5")
		return DIK_F5;
	else if(s=="F6")
		return DIK_F6;
	else if(s=="F7")
		return DIK_F7;
	else if(s=="F8")
		return DIK_F8;
	else if(s=="F9")
		return DIK_F9;
	else if(s=="F10")
		return DIK_F10;
	else if(s=="F11")
		return DIK_F11;
	else
		return 0x00;
}