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