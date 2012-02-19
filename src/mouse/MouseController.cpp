
#include "WinAction.h"
#include "MouseController.h"
#include <iostream>
#include <time.h>
#include "appDelegate.h"
#include "strconv.h"


using namespace std;

void MouseController::doClickSelector()
{
	switch(MUI.mode)
	{
	case 0: //LCLick
		winaction::LeftClick();
		break;
	case 1: //2CLick
		winaction::DoubleClick();
		MUI.eyeMouseActionDone();
		break;
	case 2: //RCLick
		winaction::RightClick();
		MUI.eyeMouseActionDone();
		break;
	case 3: //DRAG
		if(inAction)
		{
			inAction=false;
			winaction::LeftUp();
			MUI.eyeMouseActionDone();
		}
		else
		{
			inAction=true;
			winaction::LeftDown();
		}
		break;
	case 4: //SCROLL
		if(inAction)
		{
			inAction=false;
			MUI.eyeMouseActionDone();
		}
		else
		{
			x_seed = x;
			y_seed = y;
			inAction=true;
			winaction::LeftClick();
		}
		break;
	}
}

//callbacks
void MouseController::eyeMove(int _x, int _y, onScreenPosition pos)
{
	x=_x;
	y=_y;
	
	if(isLocked)
	{		
		winaction::MoveMouse(_x,_y,scale_x,scale_y);
		wstate=mouse_locked;
		return;
	}

	switch(mode)
	{
	case idle:	
		if(pos==in)
		{
			if(!inAction){
				wstate=L"";
			}
			else{
				switch(MUI.mode)
				{
				case 3: //DRAG
					wstate=mouse_dragging;
					break;
				case 4: //SCROLL
					wstate=mouse_scrolling;
					break;
				}
			}
		}
		if(useErrorCompensator)
			whelp=mouse_help_1;
		else
			whelp=mouse_help_2;
		
		winaction::MoveMouse(x,y,scale_x,scale_y);
		if(MUI.mode==4 && inAction)
		{
			if(y-y_seed<-scrollThreshold)
				winaction::ScrollUp();
			else if(y-y_seed>scrollThreshold)
				winaction::ScrollDown();
		}
		break;
	case clickhelper:
		updateClickHelper();
		break;
	case errorcompensator:
		break;
	}
}
void MouseController::eyeClick(int _x, int _y, onScreenPosition pos)
{
	if(isLocked)
	{		
		if(MUI.bp_main.update(0,x,y))	isLocked=false;
		return;
	}
	switch(mode)
	{
	case idle:
		if(inAction)
		{
			doClickSelector();
		}
		else if(MUI.isMouseOver(_x,_y))
		{
			if(MUI.isMouseOverButton(_x,_y))
			{
				MUI.eyeMouseClicked(_x,_y);
				if(MUI.bp_main.getMode(0)==0) isLocked=false;
				else isLocked=true;
			}
			else{
				doClickSelector();
			}
		}
		else
		{
			doClickSelector();
		}
		break;
	case clickhelper:
		mode = idle;
		doClickHelper(_x,_y);
		break;
	case errorcompensator:
		break;
	}
}
void MouseController::eyeShortClick(int _x, int _y, onScreenPosition pos)
{
}
void MouseController::eyeLongClick(int _x, int _y, onScreenPosition pos)
{
	if(isLocked)
	{		
		return;
	}

	switch(mode)
	{
	case idle:
		//click helper
		if(useClickHelper && !inAction)
		{
			x_seed = _x;
			y_seed = _y;
			x_adj=0;
			y_adj=0;			
			mode = clickhelper;
			setClickHelper();
		}
		else
		{
			if(inAction)
			{
				doClickSelector();
			}
			else if(MUI.isMouseOver(_x,_y))
			{
				if(MUI.isMouseOverButton(_x,_y))
				{
					MUI.eyeMouseClicked(_x,_y);
				}
				else{
					doClickSelector();
				}
			}
			else
			{
				doClickSelector();
			}
		}
		break;
	case clickhelper:
		mode = idle;
		cancelClickHelper();
		if(useErrorCompensator && !inAction)
		{
			mode = errorcompensator;
			setErrorCompensate();
		}
		break;
	case errorcompensator:
		mode = idle;
		cancelErrorCompensate();
		break;
	}
}
void MouseController::eyeBlink(int _x, int _y)
{
}
void MouseController::eyeGesture(int _x, int _y)
{
}
void MouseController::eyeOutofScreen(int _x, int _y, onScreenPosition pos)
{
	if(isLocked)
	{		
		return;
	}
	switch(mode)
	{
	case idle:
		switch(pos)
		{
		case otop:
			wstate=mouse_out_top;
			break;
		case obottom:
			wstate=mouse_out_bottom;
			break;
		case oleft:
			wstate=mouse_out_left;
			break;
		case oright:
			wstate=mouse_out_right;
			break;
		}
		break;
	case clickhelper:
		break;
	case errorcompensator:
		break;
	}
}
void MouseController::eyeDwell(int _x, int _y, onScreenPosition pos,int dwellTime)
{
	if(isLocked)
	{		
		return;
	}
	switch(mode)
	{
	case idle:
		break;
	case clickhelper:
		break;
	case errorcompensator:
		mode = idle;
		x_new = _x;
		y_new = _y;
		doErrorCompensate();
		break;
	}
}
void MouseController::eyeClose(int _x, int _y, onScreenPosition pos,int state)
{
	if(isLocked)
	{		
		return;
	}
	switch(mode)
	{
	case idle:
		switch(state)
		{
		case 0:
			wstate=mouse_closed;
			break;
		case 1:
			wstate=mouse_closed_click;
			break;
		case 2:
			wstate=mouse_closed_long;
			break;
		}
		break;
	case clickhelper:
		switch(state)
		{
		case 0:
			wstate=mouse_click_helper;
			break;
		case 1:
			wstate=mouse_click_helper_ok;
			break;
		case 2:
			wstate=mouse_do_error_comp;
			break;
		}
		break;
	case errorcompensator:
		switch(state)
		{
		case 0:
			wstate=mouse_error_comp;
			break;
		case 2:
			wstate=mouse_cancel_error_comp;
			break;
		}
		break;
	}
}


/*----------------------------------------------------------------------------------*/
// additional functions

//functions: error compensation
void MouseController::setErrorCompensate()
{
	//ui
	MUI.drawRect(x_seed,y_seed,szCompensateBox);
	wstate=mouse_error_comp;
	whelp=mouse_help_error_comp;

	//set mouse position to prev		
	winaction::MoveMouse(x_seed,y_seed,scale_x,scale_y);
}
void MouseController::doErrorCompensate()
{	
	MUI.invalidate();
	wstate=mouse_error_comp_ok;
	whelp=L"";
	MUI.draw(wstate,whelp,true);
	computeOffset ();
	return;
}
void MouseController::cancelErrorCompensate()
{	
	MUI.invalidate();
	MUI.draw(wstate,whelp,true);
	return;
}
void MouseController::computeOffset ()
{
	am->adjustOffset(x_seed-x_new, y_seed-y_new);
	cout<<"NEW OFFSET: "<<am->x_offset<<" , "<<am->y_offset<<endl;
}


//functions: click helper
void MouseController::setClickHelper()
{
	MUI.drawRect(x_seed,y_seed,szClickhelperBox);
	wstate=mouse_click_helper;
	whelp=mouse_help_click_helper;

	//set mouse position to prev		
	winaction::MoveMouse(x_seed,y_seed,scale_x,scale_y);
}
void MouseController::doClickHelper(int _x, int _y)
{	
	MUI.invalidate();
	MUI.draw(wstate,whelp,true);
	doClickSelector();
	MUI.eyeMouseClicked(_x,_y);
	x_adj=0;
	y_adj=0;		
	return;
}
void MouseController::updateClickHelper()
{	
	//x adjust
	if(x-x_seed-x_adj>3*szClickhelperBox) x_adj+=4;
	else if(x-x_seed-x_adj>2*szClickhelperBox) x_adj+=2;
	else if(x-x_seed-x_adj>szClickhelperBox) x_adj+=1;
	else if(x-x_seed-x_adj<-4*szClickhelperBox) x_adj-=4;
	else if(x-x_seed-x_adj<-2*szClickhelperBox) x_adj-=2;
	else if(x-x_seed-x_adj<-szClickhelperBox) x_adj-=1;
	//y adjust
	if(y-y_seed-y_adj>4*szClickhelperBox) y_adj+=4;
	else if(y-y_seed-y_adj>2*szClickhelperBox) y_adj+=2;
	else if(y-y_seed-y_adj>szClickhelperBox) y_adj+=1;
	else if(y-y_seed-y_adj<-4*szClickhelperBox) y_adj-=4;
	else if(y-y_seed-y_adj<-2*szClickhelperBox) y_adj-=2;
	else if(y-y_seed-y_adj<-szClickhelperBox) y_adj-=1;

	//max position
	if(x_adj>szClickhelperBox) x_adj=szClickhelperBox;
	else if(x_adj<-szClickhelperBox) x_adj=-szClickhelperBox;
	if(y_adj>szClickhelperBox) y_adj=szClickhelperBox;
	else if(y_adj<-szClickhelperBox) y_adj=-szClickhelperBox;

	//move mouse
	winaction::MoveMouse(x_seed+x_adj,y_seed+y_adj,scale_x,scale_y);
}
void MouseController::cancelClickHelper()
{	
	MUI.invalidate();
	MUI.draw(wstate,whelp,true);
	return;
}

/*----------------------------------------------------------------------------------*/



MouseController::MouseController(){

}

MouseController::~MouseController(){
	
}

int MouseController::setup(ActionDecoder *_am){
	//get system dimension
	fScreenWidth    = ::GetSystemMetrics( SM_CXSCREEN )-1; 
	fScreenHeight  = ::GetSystemMetrics( SM_CYSCREEN )-1; 	
	scale_x = (65535.0f/fScreenWidth);
	scale_y = (65535.0f/fScreenHeight);
	//setup ui
	MUI.setup();
	MUI.setupWindowPosition(fScreenWidth-600, fScreenHeight-200);

	//setup hotkey	
	for(int i=0; i<4;i++)
	{		
		fptrObject *fo = new fptrObject();
		MUI.f_list.push_back(fo);
	}

	loadXMLSettings();

	//state setting
	inAction=false;
	isLocked=false;
	mode = idle;
	x_adj=y_adj=0;	
	invalidate_cnt=0;

	//set callback
	am = _am;
	int id = am->setCallback(this);
	
	//strings
	ofxXmlSettings XML;
	XML.loadFile("strings/strings"+app->locale+".xml");
	mouse_ready=strconv(XML.getValue("mouse_ready","READY"));
	mouse_help_f12=strconv(XML.getValue("mouse_help_f12","Press F12 to start"));
	mouse_locked=strconv(XML.getValue("mouse_locked","Mouse Locked"));
	mouse_dragging=strconv(XML.getValue("mouse_dragging","Dragging..."));
	mouse_scrolling=strconv(XML.getValue("mouse_scrolling","Scrolling..."));
	mouse_help_1=strconv(XML.getValue("mouse_help_1","Close -> Click -> Adv. Click"));
	mouse_help_2=strconv(XML.getValue("mouse_help_2","Close -> Click"));
	mouse_out_top=strconv(XML.getValue("mouse_out_top","Out of Screen (TOP)"));
	mouse_out_bottom=strconv(XML.getValue("mouse_out_bottom","Out of Screen (BOTTOM)"));
	mouse_out_left=strconv(XML.getValue("mouse_out_left","Out of Screen (LEFT)"));
	mouse_out_right=strconv(XML.getValue("mouse_out_right","Out of Screen (RIGHT)"));
	mouse_closed=strconv(XML.getValue("mouse_closed","..."));
	mouse_closed_click=strconv(XML.getValue("mouse_closed_click","...CLICK..."));	
	mouse_closed_long=strconv(XML.getValue("mouse_closed_long","...LONG CLICK"));
	mouse_click_helper=strconv(XML.getValue("mouse_click_helper","<Adv. Click>"));
	mouse_click_helper_ok=strconv(XML.getValue("mouse_click_helper_ok","<Adv. Click> DONE"));
	mouse_help_click_helper=strconv(XML.getValue("mouse_help_click_helper","Look around to adjust mouse pos."));
	mouse_do_error_comp=strconv(XML.getValue("mouse_do_error_comp","START <Error Compensation>"));
	mouse_error_comp=strconv(XML.getValue("mouse_error_comp","<Error Compensation>"));
	mouse_error_comp_ok=strconv(XML.getValue("mouse_error_comp_ok","<Error Compensation> DONE"));
	mouse_cancel_error_comp=strconv(XML.getValue("mouse_cancel_error_comp","<Error Compensation> CANCEL"));
	mouse_help_error_comp=strconv(XML.getValue("mouse_help_error_comp","Stare at the pointer"));
		
	//UI setup
	wstate=mouse_ready;
	whelp=mouse_help_f12;

	//return handler id
	return id;
}
void MouseController::loadXMLSettings()
{
	ofxXmlSettings XML;
	XML.loadFile("settings/settings.xml");
	szCompensateBox = XML.getValue("mouse:compensationBox", 80);
	useErrorCompensator=XML.getValue("mouse:useCompensation",false);
	szClickhelperBox = XML.getValue("mouse:clickhelperBox", 120);
	useClickHelper=XML.getValue("mouse:useClickHelper",false);
	scrollThreshold=XML.getValue("mouse:scrollThreshold",200);
	MUI.interpretScript();
}

void MouseController::update(){	
	MUI.update(x,y);
}

void MouseController::draw(){	
	invalidate_cnt++;
	if(invalidate_cnt==20)
	{
		switch(mode)
		{
		case clickhelper:
			MUI.drawRect(x_seed,y_seed,szClickhelperBox);
			break;
		case errorcompensator:
			MUI.drawRect(x_seed,y_seed,szCompensateBox);
			break;
		}
		invalidate_cnt=0;
	}
	MUI.draw(wstate,whelp,false);	
}

void MouseController::setupWindow()
{
	MUI.setupWindow();
}


//--------------------------------------------------------------
void MouseController::mouseDragged(int x, int y, int button){	
}
//--------------------------------------------------------------
void MouseController::mousePressed(int x, int y, int button){
	MUI.mousePressed(x,y,button);
}
//--------------------------------------------------------------
void MouseController::mouseReleased(int x, int y, int button){	
	MUI.mouseReleased(x,y,button);
		
	if(MUI.isMouseOver(MUI.posX+x,MUI.posY+y))
	{
		if(MUI.isMouseOverButton(MUI.posX+x,MUI.posY+y))
		{
			MUI.eyeMouseClicked(MUI.posX+x,MUI.posY+y);
		}
	}
	//set lock
	if(MUI.bp_main.getMode(0)==0) isLocked=false;
	else isLocked=true;
}
void MouseController::keyPressed(int key){
	if (key == OF_KEY_LEFT){
		am->adjustOffset(0, -4);
		cout<< "decreasing x offset"<<endl;
	}
	else if (key == OF_KEY_RIGHT){
		am->adjustOffset(0, 4);
		cout<< "increasing x offset"<<endl;
	}
	else if (key == OF_KEY_UP){
		am->adjustOffset(-4, 0);
		cout<< "decreasing y offset"<<endl;
	}
	else if (key == OF_KEY_DOWN){
		am->adjustOffset(4, 0);
		cout<< "increasing y offset"<<endl;
	}
}


