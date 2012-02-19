
#define _WIN32_WINNT 0x0500
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <WinUser.h>

#include "ActionDecoder.h"
#include <iostream>
#include <time.h>

void time_print(void)
{
	struct tm *current;
	time_t now;	
	time(&now);
	current = localtime(&now);
	printf("   %i:%i:%i\n", current->tm_hour, current->tm_min, current->tm_sec);
}
using namespace std;


int ActionDecoder::setCallback(ActionCallback *_cb)
{
	cbList.push_back(_cb);
	changeCallback(cbList.size()-1);
	return cbList.size()-1;
}
void ActionDecoder::setDefaultCallback(int cn)
{
	idDefaultController = cn;
}
void ActionDecoder::changeCallback(int cc)
{
	idCurrentController = cc;
	if(cc>=0)
		cb = cbList[cc];
}
void ActionDecoder::changeCallbackToDefault()
{
	idCurrentController = idDefaultController;
	if(idDefaultController>=0)
		cb = cbList[idDefaultController];
}

void ActionDecoder::setup(int _framerate){
	framerate = _framerate;
	loadXMLSettings();

	dispatchEvents=false;
	
	fScreenWidth    = ::GetSystemMetrics( SM_CXSCREEN )-1; 
	fScreenHeight  = ::GetSystemMetrics( SM_CYSCREEN )-1; 
	x_offset=0;
	y_offset=0;
}
void ActionDecoder::loadXMLSettings()
{
	//load settings
	ofxXmlSettings XML;
	XML.loadFile("settings/settings.xml");

	thrShortClick=(int)framerate*XML.getValue("decoder:shortClickTime",0.25);
	thrClick=(int)framerate*XML.getValue("decoder:clickTime",0.5);
	thrLongClick=(int)framerate*XML.getValue("decoder:longClickTime",1.5);
	thrDwell=(int)framerate*XML.getValue("decoder:dwellTime",3.5);
	stayRange = XML.getValue("decoder:stayMotionRange",50);
}


void ActionDecoder::toggleEventDispatch()
{
	dispatchEvents = !dispatchEvents;
}

void ActionDecoder::Move (int i_x, int i_y, bool eyeclosed)
{  
	if(idCurrentController<0) return;
	//if enabled
	if(!dispatchEvents) return;

	onScreenPosition pos=in;
		
	if(i_y+y_offset< 0) pos=otop;
	else if(i_y+y_offset> fScreenHeight) pos=obottom;
	else if(i_x+x_offset< 0) pos=oleft;
	else if(i_x+x_offset> fScreenWidth) pos=oright;

	if(eyeclosed)
	{
		if(prev_state == 0) //previously eye opened
		{
			prev_state = 1; //close!
			close_count =0;
		}

		if( close_count < 1024 )
		{
			close_count++; //don't want to overflow count value			
		}
		
		if( close_count > thrClick/2 && close_count < 5+ thrClick/2) //clear tracked pnts
		{
			if(close_count == thrClick/2+1) cout<<"RESET TRACKING!"<<endl;
			trackedPntsX.clear();
			trackedPntsY.clear();
		}
		if( close_count < thrClick )
		{
			cb->eyeClose(i_x+x_offset,i_y+y_offset,pos,0); //blink
		}
		else if( close_count < thrLongClick )
		{
			cb->eyeClose(i_x+x_offset,i_y+y_offset,pos,1);  //click
		}
		else
		{
			cb->eyeClose(i_x+x_offset,i_y+y_offset,pos,2); //long click
		}
	}
	else //eye is open
	{
		trackedPntsX.push_front(i_x+x_offset);
		trackedPntsY.push_front(i_y+y_offset);
		if(trackedPntsX.size()>1024) //limit total size
		{
			trackedPntsX.pop_back();
			trackedPntsY.pop_back();
		}
		
		//average computing and comparing
		if(trackedPntsX.size()>=thrDwell)
		{
			//find average
			int sumX=0,sumY=0,avgX,avgY;
			list<int>::iterator iterX = trackedPntsX.begin();
			list<int>::iterator iterY = trackedPntsY.begin();
			for(int i=0;i<thrDwell;i++)
			{
				sumX+=(*iterX++); 
				sumY+=(*iterY++);	
			}
			avgX=sumX/thrDwell;
			avgY=sumY/thrDwell;

			//find distance of each samples from average
			bool isDwell=true;
			iterX = trackedPntsX.begin();
			iterY = trackedPntsY.begin();
			for(int i=0;i<thrDwell;i++)
			{
				if(abs(avgX-(*iterX++))>stayRange && abs(avgX-(*iterX++))>stayRange)
				{
					isDwell = false;
					break;
				}
			}
			if(isDwell)
			{
				trackedPntsX.clear();
				trackedPntsY.clear();
				cout << "action: DWELL";
				time_print();
				cb->eyeDwell(i_x+x_offset,i_y+y_offset,pos,0);//dwell - TODO: dwell time computation
			}
		}
		
		//the eye is closed -> open
		if(prev_state == 1)
		{
			prev_state =0;

			if( close_count < thrShortClick )
			{
				cout << "action: BLINK";
				time_print();
				cb->eyeBlink(i_x+x_offset,i_y+y_offset);
			}
			else if( close_count <thrClick)
			{
				cout << "action: SHORTCLICK";
				time_print();
				cb->eyeShortClick(i_x+x_offset,i_y+y_offset,pos);
			}
			else if( close_count < thrLongClick )
			{				
				cout << "action: CLICK";
				time_print();
				cb->eyeClick(i_x+x_offset,i_y+y_offset,pos);
			}
			else
			{
				cout << "action: LONGCLICK";
				time_print();
				cb->eyeLongClick(i_x+x_offset,i_y+y_offset,pos);
			}
		}
		else //was already open
		{
			if(pos>in)
			{
				cout << "action: OutofScreen";
				time_print();
				cb->eyeOutofScreen(i_x+x_offset,i_y+y_offset,pos); //out screen
			}
		}
		
		//save it to memeber variable
		x=i_x+x_offset;
		y=i_y+y_offset;
		cb->eyeMove(x,y,pos); //TODO: by case?
	}
}

//adjust offset manually
void ActionDecoder::setOffset(int i_x, int i_y)
{
	x_offset=i_x;
	y_offset=i_y;
}
void ActionDecoder::adjustOffset(int i_x, int i_y)
{
	x_offset+=i_x;
	y_offset+=i_y;
}
void ActionDecoder::resetOffset()
{
	x_offset=0;
	y_offset=0;
}

ActionDecoder::ActionDecoder(){

}

ActionDecoder::~ActionDecoder(){
	
}

void ActionDecoder::draw()
{
	if(idCurrentController<0) return;
	cb->draw();
}
void ActionDecoder::update()
{
	if(idCurrentController<0 || !dispatchEvents) return;
	cb->update();
}