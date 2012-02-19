
#pragma once 
#include "ofMain.h"
#include "buttonTriState.h"
#include "buttonToggle.h"

class buttonPanel : public ofRectangle{
	
	public:
	
	buttonPanel(){
		num=0;
		useTriState=false;
	}
	
	bool inRect(int i, int x, int y)
	{
		if(useTriState){
			if(triStateButtons[i].inRect(x,y)) 
				return true;
		}
		else{
			if(buttons[i].inRect(x,y)) 
				return true;
		}
		return false;
	}
	int inRect(int x, int y)
	{
		if(useTriState){
			for (int i=0; i<num; i++)
			{
				if(triStateButtons[i].inRect(x,y)) 
					return i;
			}
		}
		else{
			for (int i=0; i<num; i++)
			{
				if(buttons[i].inRect(x,y)) 
					return i;
			}
		}
		return -1;
	}
	void addButton(string title, int xx, int yy, int ww, int hh, string fn)
	{
		if(useTriState){
			buttonTriState bt;	
			bt.setup(title, xx,yy,ww,hh, fn);
			triStateButtons.push_back(bt);
			num++;
		}
		else{
			buttonToggle bt;	
			bt.setup(title, xx,yy,ww,hh, fn);
			buttons.push_back(bt);
			num++;
		}
	}
	void addState(int i,string title, string fn)
	{
		if(!useTriState)
		{
			buttons[i].addState(title, fn);
		}
	}
	int getMode(int i){
		if(useTriState)
		{
			return triStateButtons[i].mode;
		}
		else
		{
			return buttons[i].mode;
		}
	}

	void setup( float xIn, float yIn, float w, float h ){
		x		= xIn;
		y		= yIn;
		width	= w;
		height	= h;
	}
	void set(int i, int j)
	{
		if(useTriState)
			triStateButtons[i].set(j);
		else
			buttons[i].set(j);
	}	
	void reset(int i)
	{
		if(useTriState)
			triStateButtons[i].reset();
		else
			buttons[i].reset();
	}	
	void reset()
	{
	}	
	
	bool update(int i, int x, int y)
	{	
		if(useTriState)
		{
			return triStateButtons[i].update(x,y);
		}
		else
		{
			return buttons[i].update(x,y);
		}
	}	
	int update(int x, int y)
	{
		if(useTriState){
			for (int i=0; i<num; i++)
			{
				if(triStateButtons[i].update(x,y)) 
					return i;
			}
		}
		else{
			for (int i=0; i<num; i++)
			{
				if(buttons[i].update(x,y)) 
					return i;
			}
		}
		return -1;
	}
	void updateFocus(int x, int y)
	{	
		if(useTriState)
		{
			for (int i=0; i<num; i++)
			{
				if(!triStateButtons[i].active &&triStateButtons[i].inRect(x,y)){
					triStateButtons[i].active = true;
				}
				else if(triStateButtons[i].active && !triStateButtons[i].inRect(x,y)){
					triStateButtons[i].active = false;
				}
			}
		}
		else
		{
			for (int i=0; i<num; i++)
			{
				if(!buttons[i].active &&buttons[i].inRect(x,y)){
					buttons[i].active = true;
				}
				else if(buttons[i].active && !buttons[i].inRect(x,y)){
					buttons[i].active = false;
				}
			}
		}
	}	
	void draw(){
		if(useTriState)
		{
			for(int i=0;i<num;i++)
				triStateButtons[i].draw();
		}
		else
		{
			for(int i=0;i<num;i++)
				buttons[i].draw();
		}
	}
	
	vector<buttonToggle> buttons;
	vector<buttonTriState> triStateButtons;
	int num;
	bool useTriState;
};