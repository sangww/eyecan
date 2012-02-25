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
#include "buttonRect.h"

class buttonTriState : public buttonRect{
	
	public:
	
	buttonTriState(){
		mode=0;
	}

	void setup( string onName, float xIn, float yIn, float w, float h, string fn ){
		displayText = onName;			
		if(fn.length()>0)	icon.loadImage(fn);
		
		x		= xIn;
		y		= yIn;
		width	= w;
		height	= h;
	}
	void loadImage(string s){
		icon.loadImage(s);
	}
	bool update(float xIn, float yIn)
	{	
		if( inRect(xIn, yIn) ){
			mode=(++mode)%3;
			return true;
		}
		else{
			return false;
		}
	}	
	void update(){	
		mode=(++mode)%3;
	}	
	void set(int i){	
		mode=i%3;
	}	
	void reset(){
		mode=0;
	}	
	
	void draw(){
		ofFill();
				
		switch(mode)
		{
		case 0:
			if(active)	ofSetColor(150,150,255);	
			else ofSetColor(180, 180, 180);	
			break;
		case 1:
			ofSetColor(100., 200., 100.);
			break;
		case 2:
			ofSetColor(200., 100., 100.);
			break;
		default:
			break;
		}		
		ofRect(x, y, width, height); 

		ofNoFill();
		ofSetColor(30, 30, 30);
		ofRect(x, y, width, height); 

		float textWidth = 8.0f * displayText.length();
		float remainX = (width - textWidth)/2;

		float textHeight = 14.0f;
		float remainY = (height - textHeight)/2.0f + (textHeight/2.0f);
		
		ofSetColor(255,255,255); 
		icon.draw(x,y);
		//ofDrawBitmapString(displayText, x + remainX, y + remainY);
	}
		
	string displayText;
	ofImage icon;
	int mode; //0: none, 1: active. 2:hold	
};