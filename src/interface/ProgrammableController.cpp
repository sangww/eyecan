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
#include "appDelegate.h"
#include "ProgrammableController.h"
#include <iostream>
#include <time.h>

#include <dinput.h>

using namespace std;

void ProgrammableController::doClickSelector()
{
	switch(mouseMode)
	{
	case 1: //LCLick
		winaction::LeftClick();
		break;
	case 2: //2CLick
		winaction::DoubleClick();
		break;
	case 3: //RCLick
		winaction::RightClick();
		break;
	case 4: //DRAG
		if(mouseInAction)
		{
			mouseInAction=false;
			winaction::LeftUp();
		}
		else
		{
			mouseInAction=true;
			winaction::LeftDown();
		}
		break;
	case 5: //SCROLL
		if(mouseInAction)
		{
			mouseInAction=false;
		}
		else
		{
			x_seed = x;
			y_seed = y;
			mouseInAction=true;
			winaction::LeftClick();
		}
		break;
	}
}

//callbacks
void ProgrammableController::eyeMove(int _x, int _y, onScreenPosition pos)
{
	x=_x;
	y=_y;

	if(!isStarted)
	{		
		winaction::MoveMouse(_x,_y,scale_x,scale_y);
		return;
	}

	f_list[AD_OPEN]->call();

	bool x_center, y_center;

	//direction
	x_center=y_center=false;
	if(_x<oneThirdHorizontal) f_list[AD_LEFT]->call();
	else if(_x>fScreenWidth-oneThirdHorizontal) f_list[AD_RIGHT]->call();
	else x_center=true;
	if(_y<oneThirdVertical) f_list[AD_TOP]->call();
	else if(_y>fScreenHeight-oneThirdVertical) f_list[AD_BOTTOM]->call();
	else y_center=true;
	if(x_center && y_center) f_list[AD_CENTER]->call();
	if(x_center) f_list[AD_CENTERHOR]->call();
	if(y_center) f_list[AD_CENTERVER]->call();
	
	//margin
	x_center=y_center=false;
	if(_x<marginHorizontal) f_list[AD_MARGINLEFT]->call();
	else if(_x>fScreenWidth-marginHorizontal) f_list[AD_MARGINRIGHT]->call();
	else x_center=true;
	if(_y<marginVertical) f_list[AD_MARGINTOP]->call();
	else if(_y>fScreenHeight-marginVertical) f_list[AD_MARGINBOTTOM]->call();
	else y_center=true;
	if(x_center && y_center) f_list[AD_MARGINCENTER]->call();
	if(x_center) f_list[AD_MARGINCENTERHOR]->call();
	if(y_center) f_list[AD_MARGINCENTERVER]->call();


	if(mouseMove)
	{
		if(mouseFps)
		{
			POINT cursorPos;
			GetCursorPos(&cursorPos);
			float x = cursorPos.x; 
			float y = cursorPos.y;
			
			if(_x>fScreenWidth-fpsMarginHorizontal) winaction::MoveMouse(x+fpsSensitivityX,y,scale_x,scale_y);
			else if(_x<fpsMarginHorizontal) winaction::MoveMouse(x-fpsSensitivityX,y,scale_x,scale_y);
			if(_y>fScreenHeight-fpsMarginVertical) winaction::MoveMouse(x,y+fpsSensitivityY,scale_x,scale_y);
			else if(_y<fpsMarginVertical) winaction::MoveMouse(x,y-fpsSensitivityY,scale_x,scale_y);
		}
		else
		{
			winaction::MoveMouse(_x,_y,scale_x,scale_y);
			if(mouseMode==5 && mouseInAction)
			{
				if(y-y_seed<-100)
					winaction::ScrollUp();
				else if(y-y_seed>100)
					winaction::ScrollDown();
			}
		}
	}

	//3x3 framework
	int ax,ay;
	
	if(_x<oneThirdHorizontal) ax=0;
	else if(_x>fScreenWidth-oneThirdHorizontal) ax=2;
	else ax=1;
	if(_y<oneThirdVertical) ay=0;
	else if(_y>fScreenHeight-oneThirdVertical) ay=2;
	else ay=1;

	int r = 1 + 3*ay+ax;

	if(r==track.front())
	{
		stayTime=0;
	}
	else
	{
		stayTime=0;
		track.push_front(r);
		if(track.size()>16) //limit total size
		{
			track.pop_back();
		}
	}
	/*
	cout<<"TRACK: ";
	list<int>::iterator iter = track.begin();
	for(int i=0;i<track.size();i++)
	{
		cout<<(*iter++)<<" ";
	}
	cout<<endl;
	*/
}
void ProgrammableController::eyeShortClick(int _x, int _y, onScreenPosition pos)
{
	if(!isStarted)
	{		
		return;
	}
	f_list[AD_SHORTCLICK]->call();
}
void ProgrammableController::eyeClick(int _x, int _y, onScreenPosition pos)
{
	if(!isStarted)
	{		
		winaction::LeftClick();
		return;
	}

	//btt update
	switch(PUI.bp.update(x,y))
	{
	case 0:
		app->changeMode();
		break;
	case 1:
		if(selIndex>0) selIndex--;
		break;
	case 2:
		if(selIndex<fn_list.size()-1) selIndex++;
		break;
	case 3:
		if(PUI.bp.getMode(3)==1)
		{
			resetFptrObject();
			interpretScript();
			isStarted=true;
		}
		else
		{
			isStarted=false;
		}
		break;
	default:		
		doClickSelector(); //TODO: diff actions
		f_list[AD_CLICK]->call();

	bool x_center, y_center;
				
	//just
	x_center=y_center=false;
	if(_x<oneThirdHorizontal) f_list[AD_CLICKLEFT]->call();
	else if(_x>fScreenWidth-oneThirdHorizontal) f_list[AD_CLICKRIGHT]->call();
	else x_center=true;
	if(_y<oneThirdVertical) f_list[AD_CLICKTOP]->call();
	else if(_y>fScreenHeight-oneThirdVertical) f_list[AD_CLICKBOTTOM]->call();
	else y_center=true;
	if(x_center && y_center) f_list[AD_CLICKCENTER]->call();

	//margin
	x_center=y_center=false;
	if(_x<marginHorizontal) f_list[AD_CLICKMARGINLEFT]->call();
	else if(_x>fScreenWidth-marginHorizontal) f_list[AD_CLICKMARGINRIGHT]->call();
	else x_center=true;
	if(_y<marginVertical) f_list[AD_CLICKMARGINTOP]->call();
	else if(_y>fScreenHeight-marginVertical) f_list[AD_CLICKMARGINBOTTOM]->call();
	else y_center=true;
	if(x_center && y_center) f_list[AD_CLICKMARGINCENTER]->call();
		

		switch(pos)
		{
		case otop:
			f_list[AD_CLICKOUTTOP]->call();
			break;
		case obottom:
			f_list[AD_CLICKOUTBOTTOM]->call();
			break;
		case oleft:
			f_list[AD_CLICKOUTLEFT]->call();
			break;
		case oright:
			f_list[AD_CLICKOUTRIGHT]->call();
			break;
		}
	}
}
void ProgrammableController::eyeLongClick(int _x, int _y, onScreenPosition pos)
{
	if(!isStarted)
	{		
		return;
	}
	f_list[AD_LONGCLICK]->call();

	if(pos!=in)
	{
		if(!isCenterAligning && mouseAutoCalibrate)
		{
			PUI.setMode(1);
			isCenterAligning=true;
			return;
		}
	}
	if(isCenterAligning)
	{
		PUI.setMode(0);
		isCenterAligning=false;
		PUI.bp.set(3,0);
		isStarted=false;
	}
}
void ProgrammableController::eyeBlink(int _x, int _y)
{
	if(!isStarted)
	{		
		return;
	}

	f_list[AD_BLINK]->call();

	bool x_center, y_center;

	//just
	x_center=y_center=false;
	if(_x<oneThirdHorizontal) f_list[AD_BLINKLEFT]->call();
	else if(_x>fScreenWidth-oneThirdHorizontal) f_list[AD_BLINKRIGHT]->call();
	else x_center=true;
	if(_y<oneThirdVertical) f_list[AD_BLINKTOP]->call();
	else if(_y>fScreenHeight-oneThirdVertical) f_list[AD_BLINKBOTTOM]->call();
	else y_center=true;
	if(x_center && y_center) f_list[AD_BLINKCENTER]->call();
				
	//margin
	x_center=y_center=false;
	if(_x<marginHorizontal) f_list[AD_BLINKMARGINLEFT]->call();
	else if(_x>fScreenWidth-marginHorizontal) f_list[AD_BLINKMARGINRIGHT]->call();
	else x_center=true;
	if(_y<marginVertical) f_list[AD_BLINKMARGINTOP]->call();
	else if(_y>fScreenHeight-marginVertical) f_list[AD_BLINKMARGINBOTTOM]->call();
	else y_center=true;
	if(x_center && y_center) f_list[AD_BLINKMARGINCENTER]->call();
		
}
void ProgrammableController::eyeGesture(int _x, int _y)
{
}
void ProgrammableController::eyeOutofScreen(int _x, int _y, onScreenPosition pos)
{
	if(!isStarted)
	{		
		return;
	}

	switch(pos)
	{
	case otop:
		f_list[AD_OUTTOP]->call();
		break;
	case obottom:
		f_list[AD_OUTBOTTOM]->call();
		break;
	case oleft:
		f_list[AD_OUTLEFT]->call();
		break;
	case oright:
		f_list[AD_OUTRIGHT]->call();
		break;
	}
}
void ProgrammableController::eyeDwell(int _x, int _y, onScreenPosition pos,int dwellTime)
{	
	if(!isStarted)
	{		
		return;
	}

	if(isCenterAligning)
	{
		PUI.setMode(0);
		am->adjustOffset(fScreenWidth/2-_x, fScreenHeight/2-_y);
		isCenterAligning=false;
	}
	else f_list[AD_DWELL]->call();
}
void ProgrammableController::eyeClose(int _x, int _y, onScreenPosition pos,int state)
{
	if(!isStarted)
	{		
		return;
	}
	f_list[AD_CLOSE]->call();
	
	bool x_center, y_center;
				
	//just
	x_center=y_center=false;
	if(_x<oneThirdHorizontal) f_list[AD_CLOSELEFT]->call();
	else if(_x>fScreenWidth-oneThirdHorizontal) f_list[AD_CLOSERIGHT]->call();
	else x_center=true;
	if(_y<oneThirdVertical) f_list[AD_CLOSETOP]->call();
	else if(_y>fScreenHeight-oneThirdVertical) f_list[AD_CLOSEBOTTOM]->call();
	else y_center=true;
	if(x_center && y_center) f_list[AD_CLOSECENTER]->call();

	//margin
	x_center=y_center=false;
	if(_x<marginHorizontal) f_list[AD_CLOSEMARGINLEFT]->call();
	else if(_x>fScreenWidth-marginHorizontal) f_list[AD_CLOSEMARGINRIGHT]->call();
	else x_center=true;
	if(_y<marginVertical) f_list[AD_CLOSEMARGINTOP]->call();
	else if(_y>fScreenHeight-marginVertical) f_list[AD_CLOSEMARGINBOTTOM]->call();
	else y_center=true;
	if(x_center && y_center) f_list[AD_CLOSEMARGINCENTER]->call();
}



ProgrammableController::ProgrammableController(){

}

ProgrammableController::~ProgrammableController(){
	
}

void ProgrammableController::setupWindow()
{
	isStarted=false;
	PUI.setupWindow();
}
int ProgrammableController::setup(ActionDecoder *_am)
{
	am = _am;
	int id = am->setCallback(this);
	
	fScreenWidth    = ::GetSystemMetrics( SM_CXSCREEN )-1; 
	fScreenHeight  = ::GetSystemMetrics( SM_CYSCREEN )-1; 
	
	scale_x = (65535.0f/fScreenWidth);
	scale_y = (65535.0f/fScreenHeight);

	marginHorizontal = fScreenWidth/5;
	marginVertical = fScreenHeight/5;
	oneThirdHorizontal = fScreenWidth/3;
	oneThirdVertical = fScreenHeight/3;

	x_adj=y_adj=0;			
	
	PUI.setup();
	PUI.setupWindowPosition(fScreenWidth-400, fScreenHeight-200);
	
	mouseMove=false;
	mouseFps=false;
	mouseMode=0;
	mouseAutoCalibrate=true;
	mouseInAction=false;
	isCenterAligning=false;
	isStarted=false;

	//setup fptrobject reserved //reset numlock
	setupActionList();
	resetFptrObject();	
	winaction::ResetNumLock();

	setScriptDir("scripts/");

	//load strings
	wstate=L"P.CTRLeigh";
	ofxXmlSettings XML;
	XML.loadFile("strings/strings"+app->locale+".xml");
	pctrl_pause=XML.getValue("pctrl_pause","PAUSED");

	return id;
}
void ProgrammableController::setScriptDir(string s)
{
	fn_dir =s;
	reloadScriptDir();
}
void ProgrammableController::reloadScriptDir()
{
	cout<<" [LOADING SCRIPT FILE LIST] "<<endl;
	string path = "data/"+ fn_dir;

	//reset
	fn_list.clear();
	curIndex=0;
	selIndex=0;

	//reload
	DIR *dir; 
	struct dirent *ent; 
	dir = opendir (path.c_str()); 
	if (dir != NULL) {  /* print all the files and directories within directory */ 		
		while ((ent = readdir (dir)) != NULL) {
			char s[256];
			sprintf (s, "%s%s", fn_dir.c_str(), ent->d_name); 
			fn_list.push_back(s);
		} 
		closedir (dir); 
	} else { /* could not open directory */ 		
		perror (""); 
		//return EXIT_FAILURE; 
	} 
}
void ProgrammableController::setInputFile(string fn)
{
	for(int i=0;i<fn_list.size();i++)
	{
		if(fn_list[i]==fn){
			selIndex=i;
			break;
		}
		else if(i==fn_list.size()-1)
		{			
			fn_list.push_back(fn);
		}
	}
	for(int i=0;i<fn_list.size();i++)
	{
		cout<<" --- "<<fn_list[i]<<endl;
	}
	resetFptrObject();
	interpretScript();
}
void ProgrammableController::interpretScript()
{
	ofxXmlSettings XML;
	XML.loadFile(fn_list[selIndex]); //need script intepreter 
	cout<<" [LOADING SCRIPT FILE] "<<endl;
	cout<<selIndex<<" "<<fn_list[selIndex]<<endl;
	curIndex=selIndex;

	marginHorizontal = fScreenWidth*XML.getValue("config:marginHorizontal",0.2);
	marginVertical = fScreenHeight*XML.getValue("config:marginVertical",0.2);
	fpsMarginHorizontal = fScreenWidth*XML.getValue("config:fpsMarginHorizontal",0.25);
	fpsMarginVertical = fScreenHeight*XML.getValue("config:fpsMarginVertical",0.25);
	fpsSensitivityX = XML.getValue("config:fpsSensitifityX",10);
	fpsSensitivityY = XML.getValue("config:fpsSensitifityY",7);

	int i=0;
	while(true){
		string s = XML.getValue("config:tone"+ofToString(i),"none");
		if(s=="none") break;
		else{
			app->addSound(s);
		}
		i++;
	}
	     
	for(int i=0;i<listMode.size();i++)
	{
		if(XML.tagExists(listMode[i]))
		{
			string s = XML.getValue(listMode[i], "none");
			cout<<listMode[i]<< " mode to: "<<s<<endl;

			if(listMode[i]=="action:mode"){
				if(s=="click")
				{
					mouseMode=1;
				}
				if(s=="2click")
				{
					mouseMode=2;
				}
				if(s=="Rclick")
				{
					mouseMode=3;
				}
				if(s=="drag")
				{
					mouseMode=4;
				}
				if(s=="scroll")
				{
					mouseMode=5;
				}
			}
			else if(listMode[i]=="action:move"){
				if(s=="on")
				{
					mouseMove=true;
				}
				else if(s=="fps")
				{
					mouseMove=true;
					mouseFps=true;
				}
			}
			else if(listMode[i]=="action:calibrate"){
				if(s=="off")
				{
					mouseAutoCalibrate=false;
				}
				else
				{
					mouseAutoCalibrate=true;
				}
			}
		}
	}
	for(int i=0;i<listAction.size();i++)
	{
		if(XML.tagExists(listAction[i]))
		{
			string s = XML.getValue(listAction[i], "none");
			cout<<listAction[i]<< " mapped to: "<<s<<endl;

			//generic mapping
			bool needToRepeat=false;
			vector<string> vs = split(s, '-'); //vector of string
			for(int j=0;j<vs.size();j++)
			{
				vector<string> vss = split(vs[j], ','); //vector of substring
				for(int k=0;k<vss.size();k++)
				{
					//get word and up/down parameter
					int type=0;
					bool repeat=false;
					bool delay=false;
					if(vss[k].front()=='@')
					{
						repeat=true;
						needToRepeat=true;
						vss[k]=vss[k].substr(1,vss[k].size());
					}
					else if(vss[k].front()=='~')
					{
						delay=true;
						vss[k]=vss[k].substr(1,vss[k].size());
					}

					if(vss[k].back()=='^')
					{	
						vss[k]=vss[k].substr(0,vss[k].size()-1);
						type=1;
					}
					else if(vss[k].back()=='_')
					{
						vss[k]=vss[k].substr(0,vss[k].size()-1);
						type=2;
					}
					WORD w=getKeyInputWORD(vss[k]);

					//get function ptr and set action
					if(w!=0x00)
					{
						fptr f=getFunctionPtr(vss[k],type,delay);
						if(k==vss.size()-1)
							setFptrObject(listAction[i],f,w,false,repeat);
						else
							setFptrObject(listAction[i],f,w,true,repeat);
					}
					else
					{
						fptr f=getFunctionPtr(vss[k],type,delay);
						if(k==vss.size()-1)
							setFptrObject(listAction[i],f,0,false,repeat);
						else
							setFptrObject(listAction[i],f,0,true,repeat);
					}
				}
			}
			if(needToRepeat) f_repeatList.push_back(getFptrObject(listAction[i]));
		}
	}
}
fptr ProgrammableController::getFunctionPtr(string s, int type, bool delay)
{
	if(s=="none")			return 0;
	else if(s=="click")
	{
		if(type==1)			return winaction::LeftUp;
		else if(type==2)	return winaction::LeftDown;
		else				return winaction::LeftClick;
	}
	else if(s=="right_click")
	{
		if(type==1)			return winaction::RightUp;
		else if(type==2)	return winaction::RightDown;
		else				return winaction::RightClick;
	}
	else if(s=="emergency")
	{
		return winaction::playAlarmSound;
	}
	else if(s.length()>4 && s.substr(0,4)=="play")
	{
		return winaction::playSound;
	}
	else
	{
		if(type==1)			return winaction::sendSKeyUp;
		else if(type==2)	return winaction::sendSKeyDown;
		else
		{
			if(delay)
				return winaction::sendSKeyDelay;
			else
				return winaction::sendSKey;
		}
	}
}
WORD ProgrammableController::getKeyInputWORD(string s)
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
	else if(s=="F12")
		return DIK_F12;
	else if(s.length()>4 && s.substr(0,4)=="play")
	{
		return ofToInt(s.substr(4,s.length()));
	}
	else
		return 0x00;
}
void ProgrammableController::setFptrObject(string s, fptr f, DWORD d, bool b, bool r)
{
	fptrObject* fo = getFptrObject(s);
	fo->attach(f,d,b,r);
}
void ProgrammableController::resetFptrObject()
{
	mouseMove=false;
	mouseFps=false;
	mouseMode=0;
	mouseAutoCalibrate=true;
	mouseInAction=false;
	isCenterAligning=false;
	
	for(int i=0;i<listAction.size();i++)
	{
		//if(listAction[i]=="action:mode" || listAction[i]=="action:move" || listAction[i]=="action:calibrate") continue;
		fptrObject* fo = getFptrObject(listAction[i]);
		fo->reset();
	}
	
	for(int i=0;i<f_customList.size();i++)
	{
		delete f_customList[i];
	}
	f_customList.clear();	
	
	//remove repeat list (ptr)
	f_repeatList.clear();

	//reset aui
	app->resetSound();
}
fptrObject* ProgrammableController::getFptrObject(string s)
{
	for(int i=0;i<listAction.size();i++)
	{
		if(s==listAction[i]){
			return f_list[i];
		}
	}
	return 0;
}
void ProgrammableController::setupActionList()
{
	//mode
	listMode.push_back("action:mode");
	listMode.push_back("action:move");
	listMode.push_back("action:calibrate");

	//action
	listAction.push_back("action:open");
	listAction.push_back("action:blink"); //mouse
	listAction.push_back("action:short_click");
	listAction.push_back("action:click");
	listAction.push_back("action:long_click");
	listAction.push_back("action:close");
	listAction.push_back("action:dwell");
	listAction.push_back("action:out_left"); //out
	listAction.push_back("action:out_right");
	listAction.push_back("action:out_top");
	listAction.push_back("action:out_bottom");
	listAction.push_back("action:click_out_left");
	listAction.push_back("action:click_out_right");
	listAction.push_back("action:click_out_top");
	listAction.push_back("action:click_out_bottom");
	listAction.push_back("action:margin_left");//margin
	listAction.push_back("action:margin_right");
	listAction.push_back("action:margin_top");
	listAction.push_back("action:margin_bottom");
	listAction.push_back("action:margin_center"); 
	listAction.push_back("action:margin_center_horizontal");
	listAction.push_back("action:margin_center_vertical");
	listAction.push_back("action:click_margin_center");
	listAction.push_back("action:click_margin_left");
	listAction.push_back("action:click_margin_right");
	listAction.push_back("action:click_margin_top");
	listAction.push_back("action:click_margin_bottom");	
	listAction.push_back("action:blink_margin_center");
	listAction.push_back("action:blink_margin_left");
	listAction.push_back("action:blink_margin_right");
	listAction.push_back("action:blink_margin_top");
	listAction.push_back("action:blink_margin_bottom");	
	listAction.push_back("action:close_margin_center");
	listAction.push_back("action:close_margin_left");
	listAction.push_back("action:close_margin_right");
	listAction.push_back("action:close_margin_top");
	listAction.push_back("action:close_margin_bottom");	
	listAction.push_back("action:left"); //direction
	listAction.push_back("action:right");
	listAction.push_back("action:top");
	listAction.push_back("action:bottom");
	listAction.push_back("action:center");
	listAction.push_back("action:center_horizontal");
	listAction.push_back("action:center_vertical");
	listAction.push_back("action:click_center");
	listAction.push_back("action:click_left");
	listAction.push_back("action:click_right");
	listAction.push_back("action:click_top");
	listAction.push_back("action:click_bottom");
	listAction.push_back("action:blink_center");
	listAction.push_back("action:blink_left");
	listAction.push_back("action:blink_right");
	listAction.push_back("action:blink_top");
	listAction.push_back("action:blink_bottom");
	listAction.push_back("action:close_center");
	listAction.push_back("action:close_left");
	listAction.push_back("action:close_right");
	listAction.push_back("action:close_top");
	listAction.push_back("action:close_bottom");	

	for(int i=0; i<listAction.size();i++)
	{		
		fptrObject *fo = new fptrObject();
		f_list.push_back(fo);
	}
}


void ProgrammableController::update(){	
	PUI.update(x, y);

	//call key input repeaters
	for(int i=0;i<f_repeatList.size();i++)
		f_repeatList[i]->callRepeater();
}

void ProgrammableController::draw(){	
	if(isStarted)
		PUI.draw(fn_list[curIndex], fn_list[selIndex]);	
	else
		PUI.draw(pctrl_pause, fn_list[selIndex]);	
}


//--------------------------------------------------------------
void ProgrammableController::mouseDragged(int x, int y, int button){	
}
//--------------------------------------------------------------
void ProgrammableController::mousePressed(int x, int y, int button){
	PUI.mousePressed(x,y,button);
}
//--------------------------------------------------------------
void ProgrammableController::mouseReleased(int x, int y, int button){
	PUI.mouseReleased(x,y,button);	
	switch(PUI.bp.update(PUI.posX+x,PUI.posY+y))
	{
	case 0:
		app->changeMode();
		break;
	case 1:
		if(selIndex>0) selIndex--;
		break;
	case 2:
		if(selIndex<fn_list.size()-1) selIndex++;
		break;
	case 3:
		if(PUI.bp.getMode(3)==1)
		{
			resetFptrObject();
			interpretScript();
			isStarted=true;
		}
		else
		{
			isStarted=false;
		}
		break;
	}
}
//--------------------------------------------------------------
void ProgrammableController::keyPressed(int key){
}
